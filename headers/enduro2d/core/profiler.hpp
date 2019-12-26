/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class profiler final : public module<profiler> {
    public:
        struct begin_scope_info {
            str name;
            std::thread::id tid;
            std::chrono::steady_clock::time_point tp;
        };

        struct end_scope_info {
            std::thread::id tid;
            std::chrono::steady_clock::time_point tp;
        };

        struct thread_event_info {
            str name;
            std::thread::id tid;
            std::chrono::steady_clock::time_point tp;
        };

        struct global_event_info {
            str name;
            std::thread::id tid;
            std::chrono::steady_clock::time_point tp;
        };

        using event_info = std::variant<
            begin_scope_info,
            end_scope_info,
            thread_event_info,
            global_event_info>;
    public:
        class sink : private e2d::noncopyable {
        public:
            virtual ~sink() noexcept = default;
            virtual void on_event(const event_info& event) noexcept = 0;
        };
        using sink_uptr = std::unique_ptr<sink>;
    public:
        class scope final : private e2d::noncopyable {
        public:
            scope(profiler& profiler, str name);
            ~scope() noexcept;
        private:
            profiler& profiler_;
        };
    public:
        profiler(deferrer& d);
        ~profiler() noexcept final;

        void begin_scope(str name) noexcept;
        void end_scope() noexcept;

        void thread_event(str name) noexcept;
        void global_event(str name) noexcept;
    public:
        struct recording_info {
            vector<event_info> events;
        };

        template < typename Rep, typename Period >
        stdex::promise<recording_info> record_for(
            const std::chrono::duration<Rep, Period>& timeout_duration);

        template < typename Clock, typename Duration >
        stdex::promise<recording_info> record_until(
            const std::chrono::time_point<Clock, Duration>& timeout_time);
    public:
        template < typename T, typename... Args >
        T& register_sink(Args&&... args);
        sink& register_sink(sink_uptr sink);
        void unregister_sink(const sink& sink) noexcept;
    private:
        deferrer& deferrer_;
        vector<sink_uptr> sinks_;
        std::recursive_mutex rmutex_;
    };
}

#define E2D_PROFILER_SCOPE(name)\
    auto E2D_PP_CAT(e2d_generated_profiler_scope_, __LINE__) =\
        ::e2d::profiler::scope(the<profiler>(), name)

namespace e2d
{
    template < typename Rep, typename Period >
    stdex::promise<profiler::recording_info> profiler::record_for(
        const std::chrono::duration<Rep, Period>& timeout_duration)
    {
        return record_until(std::chrono::steady_clock::now() + timeout_duration);
    }

    template < typename Clock, typename Duration >
    stdex::promise<profiler::recording_info> profiler::record_until(
        const std::chrono::time_point<Clock, Duration>& timeout_time)
    {
        using promise_t = stdex::promise<recording_info>;
        using time_point_t = std::chrono::time_point<Clock, Duration>;

        class temp_sink final : public sink {
        public:
            temp_sink(time_point_t time_point)
            : time_point_(time_point) {}

            void on_event(const event_info& event) noexcept final {
                try {
                    info_.events.push_back(event);

                    if ( std::holds_alternative<begin_scope_info>(event) ) {
                        ++depth_;
                    } else if ( std::holds_alternative<end_scope_info>(event) ) {
                        E2D_ASSERT(depth_);
                        --depth_;
                    }

                    if ( !depth_ && time_point_ <= Clock::now() ) {
                        promise_.resolve(std::move(info_));
                    }
                } catch (...) {
                    promise_.reject(std::current_exception());
                }
            }

            promise_t& promise() noexcept {
                return promise_;
            }

            const promise_t& promise() const noexcept {
                return promise_;
            }
        private:
            promise_t promise_;
            recording_info info_;
            std::size_t depth_{0u};
            time_point_t time_point_;
        };

        temp_sink& s = register_sink<temp_sink>(timeout_time);
        return s.promise().then([&s, this](auto&& info){
            return deferrer_.do_in_main_thread([&s, this](auto&& info){
                unregister_sink(s);
                return std::forward<decltype(info)>(info);
            }, std::forward<decltype(info)>(info));
        });
    }

    template < typename T, typename... Args >
    T& profiler::register_sink(Args&&... args) {
        return static_cast<T&>(
            register_sink(std::make_unique<T>(std::forward<Args>(args)...)));
    }
}
