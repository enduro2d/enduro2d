/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_utils.hpp"
#include "time.hpp"

namespace e2d
{
    class jobber final : private noncopyable {
    public:
        enum class priority {
            lowest,
            below_normal,
            normal,
            above_normal,
            highest
        };
    public:
        explicit jobber(u32 threads);
        ~jobber() noexcept;

        template < typename F, typename... Args >
        using async_invoke_result_t = stdex::invoke_result_t<
            std::decay_t<F>,
            std::decay_t<Args>...>;

        template < typename F, typename... Args
                 , typename R = async_invoke_result_t<F, Args...> >
        std::future<R> async(F&& f, Args&&... args);

        template < typename F, typename... Args
                 , typename R = async_invoke_result_t<F, Args...> >
        std::future<R> async(priority priority, F&& f, Args&&... args);

        void pause() noexcept;
        void resume() noexcept;
        bool is_paused() const noexcept;

        void wait_all() const noexcept;
        void active_wait_all() noexcept;

        template < typename T, typename TimeTag >
        void wait_for(const unit<T, TimeTag>& time_for) const noexcept;

        template < typename T, typename TimeTag >
        void wait_until(const unit<T, TimeTag>& time_until) const noexcept;

        template < typename T, typename TimeTag >
        void active_wait_for(const unit<T, TimeTag>& time_for) noexcept;

        template < typename T, typename TimeTag >
        void active_wait_until(const unit<T, TimeTag>& time_until) noexcept;
    private:
        class task;
        using task_ptr = std::unique_ptr<task>;
        template < typename R, typename F, typename... Args >
        class concrete_task;
    private:
        void push_task_(priority priority, task_ptr task);
        task_ptr pop_task_() noexcept;
        void shutdown_() noexcept;
        void worker_main_() noexcept;
        void process_task_(std::unique_lock<std::mutex> lock) noexcept;
    private:
        vector<std::thread> threads_;
        vector<std::pair<priority, task_ptr>> tasks_;
        std::mutex tasks_mutex_;
        std::condition_variable cond_var_;
        std::atomic<bool> paused_{false};
        std::atomic<bool> cancelled_{false};
        std::atomic<size_t> active_task_count_{0};
    };

    class jobber::task : private noncopyable {
    public:
        virtual ~task() noexcept = default;
        virtual void run() noexcept = 0;
    };

    template < typename R, typename F, typename... Args >
    class jobber::concrete_task : public task {
        F f_;
        std::tuple<Args...> args_;
        std::promise<R> promise_;
    public:
        template < typename U >
        concrete_task(U&& u, std::tuple<Args...>&& args);
        void run() noexcept final;
        std::future<R> future() noexcept;
    };

    template < typename F, typename... Args >
    class jobber::concrete_task<void, F, Args...> : public task {
        F f_;
        std::tuple<Args...> args_;
        std::promise<void> promise_;
    public:
        template < typename U >
        concrete_task(U&& u, std::tuple<Args...>&& args);
        void run() noexcept final;
        std::future<void> future() noexcept;
    };
}

namespace e2d
{
    //
    // async
    //

    template < typename F, typename... Args, typename R >
    std::future<R> jobber::async(F&& f, Args&&... args) {
        return async(
            priority::normal,
            std::forward<F>(f),
            std::forward<Args>(args)...);
    }

    template < typename F, typename... Args, typename R >
    std::future<R> jobber::async(priority priority, F&& f, Args&&... args) {
        using task_t = concrete_task<
            R,
            std::decay_t<F>,
            std::decay_t<Args>...>;
        std::unique_ptr<task_t> task = std::make_unique<task_t>(
            std::forward<F>(f),
            std::make_tuple(std::forward<Args>(args)...));
        std::future<R> future = task->future();
        std::lock_guard<std::mutex> guard(tasks_mutex_);
        push_task_(priority, std::move(task));
        return future;
    }

    //
    // wait
    //

    template < typename T, typename TimeTag >
    void jobber::wait_for(const unit<T, TimeTag>& time_for) const noexcept {
        if ( time_for.value > T(0) ) {
            wait_until(
                time::now<TimeTag, u64>() +
                time_for.template cast_to<u64>());
        }
    }

    template < typename T, typename TimeTag >
    void jobber::wait_until(const unit<T, TimeTag>& time_until) const noexcept {
        while ( active_task_count_ ) {
            const auto time_now = time::now<TimeTag, u64>();
            if ( time_now >= time_until.template cast_to<u64>() ) {
                break;
            }
            std::this_thread::yield();
        }
    }

    //
    // active_wait
    //

    template < typename T, typename TimeTag >
    void jobber::active_wait_for(const unit<T, TimeTag>& time_for) noexcept {
        if ( time_for.value > T(0) ) {
            active_wait_until(
                time::now<TimeTag, u64>() +
                time_for.template cast_to<u64>());
        }
    }

    template < typename T, typename TimeTag >
    void jobber::active_wait_until(const unit<T, TimeTag>& time_until) noexcept {
        while ( active_task_count_ ) {
            const auto time_now = time::now<TimeTag, u64>();
            if ( time_now >= time_until.template cast_to<u64>() ) {
                break;
            }
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            process_task_(std::move(lock));
        }
    }

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
            R value = stdex::apply(std::move(f_), std::move(args_));
            promise_.set_value(std::move(value));
        } catch (...) {
            promise_.set_exception(std::current_exception());
        }
    }

    template < typename R, typename F, typename... Args >
    std::future<R> jobber::concrete_task<R, F, Args...>::future() noexcept {
        return promise_.get_future();
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
            stdex::apply(std::move(f_), std::move(args_));
            promise_.set_value();
        } catch (...) {
            promise_.set_exception(std::current_exception());
        }
    }

    template < typename F, typename... Args >
    std::future<void> jobber::concrete_task<void, F, Args...>::future() noexcept {
        return promise_.get_future();
    }
}
