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
#include <chrono>
#include <memory>
#include <vector>
#include <utility>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <condition_variable>

#include "promise.hpp"

namespace scheduler_hpp
{
    using namespace promise_hpp;

    enum class scheduler_priority {
        lowest,
        below_normal,
        normal,
        above_normal,
        highest
    };

    enum class scheduler_processing_status {
        done,
        timeout,
        cancelled
    };

    class scheduler_cancelled_exception : public std::runtime_error {
    public:
        scheduler_cancelled_exception()
        : std::runtime_error("scheduler has stopped working") {}
    };

    class scheduler final : private detail::noncopyable {
    public:
        scheduler();
        ~scheduler() noexcept;

        using processing_result_t = std::pair<
            scheduler_processing_status,
            std::size_t>;

        template < typename F, typename... Args >
        using schedule_invoke_result_t = invoke_hpp::invoke_result_t<
            std::decay_t<F>,
            std::decay_t<Args>...>;

        template < typename F, typename... Args
                 , typename R = schedule_invoke_result_t<F, Args...> >
        promise<R> schedule(F&& f, Args&&... args);

        template < typename F, typename... Args
                 , typename R = schedule_invoke_result_t<F, Args...> >
        promise<R> schedule(scheduler_priority scheduler_priority, F&& f, Args&&... args);

        processing_result_t process_one_task() noexcept;
        processing_result_t process_all_tasks() noexcept;

        template < typename Rep, typename Period >
        processing_result_t process_tasks_for(
            const std::chrono::duration<Rep, Period>& timeout_duration) noexcept;

        template < typename Clock, typename Duration >
        processing_result_t process_tasks_until(
            const std::chrono::time_point<Clock, Duration>& timeout_time) noexcept;
    private:
        class task;
        using task_ptr = std::unique_ptr<task>;
        template < typename R, typename F, typename... Args >
        class concrete_task;
    private:
        void push_task_(scheduler_priority scheduler_priority, task_ptr task);
        task_ptr pop_task_() noexcept;
        void shutdown_() noexcept;
        void process_task_(std::unique_lock<std::mutex> lock) noexcept;
    private:
        std::vector<std::pair<scheduler_priority, task_ptr>> tasks_;
        std::atomic<bool> cancelled_{false};
        std::atomic<std::size_t> active_task_count_{0};
        mutable std::mutex tasks_mutex_;
        mutable std::condition_variable cond_var_;
    };

    class scheduler::task : private noncopyable {
    public:
        virtual ~task() noexcept = default;
        virtual void run() noexcept = 0;
        virtual void cancel() noexcept = 0;
    };

    template < typename R, typename F, typename... Args >
    class scheduler::concrete_task : public task {
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
    class scheduler::concrete_task<void, F, Args...> : public task {
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

namespace scheduler_hpp
{
    inline scheduler::scheduler() = default;

    inline scheduler::~scheduler() noexcept {
        shutdown_();
    }

    template < typename F, typename... Args, typename R >
    promise<R> scheduler::schedule(F&& f, Args&&... args) {
        return schedule(
            scheduler_priority::normal,
            std::forward<F>(f),
            std::forward<Args>(args)...);
    }

    template < typename F, typename... Args, typename R >
    promise<R> scheduler::schedule(scheduler_priority priority, F&& f, Args&&... args) {
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

    inline scheduler::processing_result_t scheduler::process_one_task() noexcept {
        std::unique_lock<std::mutex> lock(tasks_mutex_);
        if ( cancelled_ ) {
            return std::make_pair(scheduler_processing_status::cancelled, 0u);
        }
        if ( tasks_.empty() ) {
            return std::make_pair(scheduler_processing_status::done, 0u);
        }
        process_task_(std::move(lock));
        return std::make_pair(scheduler_processing_status::done, 1u);
    }

    inline scheduler::processing_result_t scheduler::process_all_tasks() noexcept {
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
                ? scheduler_processing_status::cancelled
                : scheduler_processing_status::done,
            processed_tasks);
    }

    template < typename Rep, typename Period >
    scheduler::processing_result_t scheduler::process_tasks_for(
        const std::chrono::duration<Rep, Period>& timeout_duration) noexcept
    {
        return process_tasks_until(
            std::chrono::steady_clock::now() + timeout_duration);
    }

    template < typename Clock, typename Duration >
    scheduler::processing_result_t scheduler::process_tasks_until(
        const std::chrono::time_point<Clock, Duration>& timeout_time) noexcept
    {
        std::size_t processed_tasks = 0;
        while ( !cancelled_ && active_task_count_ ) {
            if ( !(Clock::now() < timeout_time) ) {
                return std::make_pair(
                    scheduler_processing_status::timeout,
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
                ? scheduler_processing_status::cancelled
                : scheduler_processing_status::done,
            processed_tasks);
    }

    inline void scheduler::push_task_(scheduler_priority priority, task_ptr task) {
        tasks_.emplace_back(priority, std::move(task));
        std::push_heap(tasks_.begin(), tasks_.end());
        ++active_task_count_;
        cond_var_.notify_one();
    }

    inline scheduler::task_ptr scheduler::pop_task_() noexcept {
        if ( !tasks_.empty() ) {
            std::pop_heap(tasks_.begin(), tasks_.end());
            task_ptr task = std::move(tasks_.back().second);
            tasks_.pop_back();
            return task;
        }
        return nullptr;
    }

    inline void scheduler::shutdown_() noexcept {
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

    inline void scheduler::process_task_(std::unique_lock<std::mutex> lock) noexcept {
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

namespace scheduler_hpp
{
    //
    // concrete_task<R, F, Args...>
    //

    template < typename R, typename F, typename... Args >
    template < typename U >
    scheduler::concrete_task<R, F, Args...>::concrete_task(U&& u, std::tuple<Args...>&& args)
    : f_(std::forward<U>(u))
    , args_(std::move(args)) {}

    template < typename R, typename F, typename... Args >
    void scheduler::concrete_task<R, F, Args...>::run() noexcept {
        try {
            R value = invoke_hpp::apply(std::move(f_), std::move(args_));
            promise_.resolve(std::move(value));
        } catch (...) {
            promise_.reject(std::current_exception());
        }
    }

    template < typename R, typename F, typename... Args >
    void scheduler::concrete_task<R, F, Args...>::cancel() noexcept {
        promise_.reject(scheduler_cancelled_exception());
    }

    template < typename R, typename F, typename... Args >
    promise<R> scheduler::concrete_task<R, F, Args...>::future() noexcept {
        return promise_;
    }

    //
    // concrete_task<void, F, Args...>
    //

    template < typename F, typename... Args >
    template < typename U >
    scheduler::concrete_task<void, F, Args...>::concrete_task(U&& u, std::tuple<Args...>&& args)
    : f_(std::forward<U>(u))
    , args_(std::move(args)) {}

    template < typename F, typename... Args >
    void scheduler::concrete_task<void, F, Args...>::run() noexcept {
        try {
            invoke_hpp::apply(std::move(f_), std::move(args_));
            promise_.resolve();
        } catch (...) {
            promise_.reject(std::current_exception());
        }
    }

    template < typename F, typename... Args >
    void scheduler::concrete_task<void, F, Args...>::cancel() noexcept {
        promise_.reject(scheduler_cancelled_exception());
    }

    template < typename F, typename... Args >
    promise<void> scheduler::concrete_task<void, F, Args...>::future() noexcept {
        return promise_;
    }
}
