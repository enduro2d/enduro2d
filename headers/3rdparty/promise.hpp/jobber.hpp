/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/promise.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cstdint>
#include <cassert>

#include <tuple>
#include <mutex>
#include <atomic>
#include <thread>
#include <chrono>
#include <memory>
#include <vector>
#include <utility>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <condition_variable>

#include "promise.hpp"

namespace jobber_hpp
{
    using namespace promise_hpp;

    enum class jobber_priority {
        lowest,
        below_normal,
        normal,
        above_normal,
        highest
    };

    enum class jobber_wait_status {
        no_timeout,
        cancelled,
        timeout
    };

    class jobber_cancelled_exception : public std::runtime_error {
    public:
        jobber_cancelled_exception()
        : std::runtime_error("jobber has stopped working") {}
    };

    class jobber final : private detail::noncopyable {
    public:
        explicit jobber(std::size_t threads);
        ~jobber() noexcept;

        using active_wait_result_t = std::pair<
            jobber_wait_status,
            std::size_t>;

        template < typename F, typename... Args >
        using async_invoke_result_t = invoke_hpp::invoke_result_t<
            std::decay_t<F>,
            std::decay_t<Args>...>;

        template < typename F, typename... Args
                 , typename R = async_invoke_result_t<F, Args...> >
        promise<R> async(F&& f, Args&&... args);

        template < typename F, typename... Args
                 , typename R = async_invoke_result_t<F, Args...> >
        promise<R> async(jobber_priority priority, F&& f, Args&&... args);

        void pause() noexcept;
        void resume() noexcept;
        bool is_paused() const noexcept;

        jobber_wait_status wait_all() const noexcept;
        active_wait_result_t active_wait_all() noexcept;
        active_wait_result_t active_wait_one() noexcept;

        template < typename Rep, typename Period >
        jobber_wait_status wait_all_for(
            const std::chrono::duration<Rep, Period>& timeout_duration) const;

        template < typename Clock, typename Duration >
        jobber_wait_status wait_all_until(
            const std::chrono::time_point<Clock, Duration>& timeout_time) const;

        template < typename Rep, typename Period >
        active_wait_result_t active_wait_all_for(
            const std::chrono::duration<Rep, Period>& timeout_duration);

        template < typename Clock, typename Duration >
        active_wait_result_t active_wait_all_until(
            const std::chrono::time_point<Clock, Duration>& timeout_time);
    private:
        class task;
        using task_ptr = std::unique_ptr<task>;
        template < typename R, typename F, typename... Args >
        class concrete_task;
    private:
        void push_task_(jobber_priority priority, task_ptr task);
        task_ptr pop_task_() noexcept;
        void shutdown_() noexcept;
        void worker_main_() noexcept;
        void process_task_(std::unique_lock<std::mutex> lock) noexcept;
    private:
        std::vector<std::thread> threads_;
        std::vector<std::pair<jobber_priority, task_ptr>> tasks_;
        std::atomic<bool> paused_{false};
        std::atomic<bool> cancelled_{false};
        std::atomic<std::size_t> active_task_count_{0};
        mutable std::mutex tasks_mutex_;
        mutable std::condition_variable cond_var_;
    };

    class jobber::task : private noncopyable {
    public:
        virtual ~task() noexcept = default;
        virtual void run() noexcept = 0;
        virtual void cancel() noexcept = 0;
    };

    template < typename R, typename F, typename... Args >
    class jobber::concrete_task : public task {
        F f_;
        std::tuple<Args...> args_;
        promise<R> promise_;
    public:
        template < typename U >
        concrete_task(U&& u, std::tuple<Args...>&& args);
        void run() noexcept final;
        void cancel() noexcept final;
        promise<R> future() noexcept;
    };

    template < typename F, typename... Args >
    class jobber::concrete_task<void, F, Args...> : public task {
        F f_;
        std::tuple<Args...> args_;
        promise<void> promise_;
    public:
        template < typename U >
        concrete_task(U&& u, std::tuple<Args...>&& args);
        void run() noexcept final;
        void cancel() noexcept final;
        promise<void> future() noexcept;
    };
}

namespace jobber_hpp
{
    inline jobber::jobber(std::size_t threads) {
        try {
            threads_.resize(threads);
            for ( std::thread& thread : threads_ ) {
                thread = std::thread(&jobber::worker_main_, this);
            }
        } catch (...) {
            shutdown_();
            throw;
        }
    }

    inline jobber::~jobber() noexcept {
        shutdown_();
    }

    template < typename F, typename... Args, typename R >
    promise<R> jobber::async(F&& f, Args&&... args) {
        return async(
            jobber_priority::normal,
            std::forward<F>(f),
            std::forward<Args>(args)...);
    }

    template < typename F, typename... Args, typename R >
    promise<R> jobber::async(jobber_priority priority, F&& f, Args&&... args) {
        using task_t = concrete_task<
            R,
            std::decay_t<F>,
            std::decay_t<Args>...>;
        std::unique_ptr<task_t> task = std::make_unique<task_t>(
            std::forward<F>(f),
            std::make_tuple(std::forward<Args>(args)...));
        promise<R> future = task->future();
        std::lock_guard<std::mutex> guard(tasks_mutex_);
        push_task_(priority, std::move(task));
        return future;
    }

    inline void jobber::pause() noexcept {
        std::lock_guard<std::mutex> guard(tasks_mutex_);
        paused_.store(true);
        cond_var_.notify_all();
    }

    inline void jobber::resume() noexcept {
        std::lock_guard<std::mutex> guard(tasks_mutex_);
        paused_.store(false);
        cond_var_.notify_all();
    }

    inline bool jobber::is_paused() const noexcept {
        return paused_;
    }

    inline jobber_wait_status jobber::wait_all() const noexcept {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        cond_var_.wait(lock, [this](){
            return cancelled_ || !active_task_count_;
        });
        return cancelled_
            ? jobber_wait_status::cancelled
            : jobber_wait_status::no_timeout;
    }

    inline jobber::active_wait_result_t jobber::active_wait_all() noexcept {
        std::size_t processed_tasks = 0;
        while ( !cancelled_ && active_task_count_ ) {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            cond_var_.wait(lock, [this](){
                return cancelled_ || !active_task_count_ || !tasks_.empty();
            });
            if ( !tasks_.empty() ) {
                process_task_(std::move(lock));
                ++processed_tasks;
            }
        }
        return std::make_pair(
            cancelled_
                ? jobber_wait_status::cancelled
                : jobber_wait_status::no_timeout,
            processed_tasks);
    }

    inline jobber::active_wait_result_t jobber::active_wait_one() noexcept {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        if ( cancelled_ ) {
            return std::make_pair(jobber_wait_status::cancelled, 0u);
        }
        if ( tasks_.empty() ) {
            return std::make_pair(jobber_wait_status::no_timeout, 0u);
        }
        process_task_(std::move(lock));
        return std::make_pair(jobber_wait_status::no_timeout, 1u);
    }

    template < typename Rep, typename Period >
    jobber_wait_status jobber::wait_all_for(
        const std::chrono::duration<Rep, Period>& timeout_duration) const
    {
        return wait_all_until(
            std::chrono::steady_clock::now() + timeout_duration);
    }

    template < typename Clock, typename Duration >
    jobber_wait_status jobber::wait_all_until(
        const std::chrono::time_point<Clock, Duration>& timeout_time) const
    {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        return cond_var_.wait_until(lock, timeout_time, [this](){
            return cancelled_ || !active_task_count_;
        })  ? jobber_wait_status::no_timeout
            : jobber_wait_status::timeout;
    }

    template < typename Rep, typename Period >
    jobber::active_wait_result_t jobber::active_wait_all_for(
        const std::chrono::duration<Rep, Period>& timeout_duration)
    {
        return active_wait_all_until(
            std::chrono::steady_clock::now() + timeout_duration);
    }

    template < typename Clock, typename Duration >
    jobber::active_wait_result_t jobber::active_wait_all_until(
        const std::chrono::time_point<Clock, Duration>& timeout_time)
    {
        std::size_t processed_tasks = 0;
        while ( !cancelled_ && active_task_count_ ) {
            if ( !(Clock::now() < timeout_time) ) {
                return std::make_pair(
                    jobber_wait_status::timeout,
                    processed_tasks);
            }
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            cond_var_.wait_until(lock, timeout_time, [this](){
                return cancelled_ || !active_task_count_ || !tasks_.empty();
            });
            if ( !tasks_.empty() ) {
                process_task_(std::move(lock));
                ++processed_tasks;
            }
        }
        return std::make_pair(
            cancelled_
                ? jobber_wait_status::cancelled
                : jobber_wait_status::no_timeout,
            processed_tasks);
    }

    inline void jobber::push_task_(jobber_priority priority, task_ptr task) {
        tasks_.emplace_back(priority, std::move(task));
        std::push_heap(tasks_.begin(), tasks_.end());
        ++active_task_count_;
        cond_var_.notify_one();
    }

    inline jobber::task_ptr jobber::pop_task_() noexcept {
        if ( !tasks_.empty() ) {
            std::pop_heap(tasks_.begin(), tasks_.end());
            task_ptr task = std::move(tasks_.back().second);
            tasks_.pop_back();
            return task;
        }
        return nullptr;
    }

    inline void jobber::shutdown_() noexcept {
        {
            std::lock_guard<std::mutex> guard(tasks_mutex_);
            while ( !tasks_.empty() ) {
                task_ptr task = pop_task_();
                if ( task ) {
                    task->cancel();
                    --active_task_count_;
                }
            }
            cancelled_.store(true);
            cond_var_.notify_all();
        }
        for ( std::thread& thread : threads_ ) {
            if ( thread.joinable() ) {
                thread.join();
            }
        }
    }

    inline void jobber::worker_main_() noexcept {
        while ( true ) {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            cond_var_.wait(lock, [this](){
                return cancelled_ || (!paused_ && !tasks_.empty());
            });
            if ( cancelled_ ) {
                break;
            }
            process_task_(std::move(lock));
        }
    }

    inline void jobber::process_task_(std::unique_lock<std::mutex> lock) noexcept {
        assert(lock.owns_lock());
        task_ptr task = pop_task_();
        if ( task ) {
            lock.unlock();
            task->run();
            lock.lock();
            --active_task_count_;
            cond_var_.notify_all();
        }
    }
}

namespace jobber_hpp
{
    //
    // concrete_task<R, F, Args...>
    //

    template < typename R, typename F, typename... Args >
    template < typename U >
    jobber::concrete_task<R, F, Args...>::concrete_task(U&& u, std::tuple<Args...>&& args)
    : f_(std::forward<U>(u))
    , args_(std::move(args)) {}

    template < typename R, typename F, typename... Args >
    void jobber::concrete_task<R, F, Args...>::run() noexcept {
        try {
            R value = invoke_hpp::apply(std::move(f_), std::move(args_));
            promise_.resolve(std::move(value));
        } catch (...) {
            promise_.reject(std::current_exception());
        }
    }

    template < typename R, typename F, typename... Args >
    void jobber::concrete_task<R, F, Args...>::cancel() noexcept {
        promise_.reject(jobber_cancelled_exception());
    }

    template < typename R, typename F, typename... Args >
    promise<R> jobber::concrete_task<R, F, Args...>::future() noexcept {
        return promise_;
    }

    //
    // concrete_task<void, F, Args...>
    //

    template < typename F, typename... Args >
    template < typename U >
    jobber::concrete_task<void, F, Args...>::concrete_task(U&& u, std::tuple<Args...>&& args)
    : f_(std::forward<U>(u))
    , args_(std::move(args)) {}

    template < typename F, typename... Args >
    void jobber::concrete_task<void, F, Args...>::run() noexcept {
        try {
            invoke_hpp::apply(std::move(f_), std::move(args_));
            promise_.resolve();
        } catch (...) {
            promise_.reject(std::current_exception());
        }
    }

    template < typename F, typename... Args >
    void jobber::concrete_task<void, F, Args...>::cancel() noexcept {
        promise_.reject(jobber_cancelled_exception());
    }

    template < typename F, typename... Args >
    promise<void> jobber::concrete_task<void, F, Args...>::future() noexcept {
        return promise_;
    }
}
