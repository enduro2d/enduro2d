/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_core.hpp"

#include "deferrer.hpp"

namespace e2d
{
    class profiler final : public module<profiler> {
    public:
        using args_t = flat_map<str, str>;

        struct begin_scope_info {
            str name;
            args_t args;
            std::thread::id tid;
            std::chrono::microseconds tp;
        };

        struct end_scope_info {
            std::thread::id tid;
            std::chrono::microseconds tp;
        };

        struct thread_event_info {
            str name;
            args_t args;
            std::thread::id tid;
            std::chrono::microseconds tp;
        };

        struct global_event_info {
            str name;
            args_t args;
            std::thread::id tid;
            std::chrono::microseconds tp;
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
        class auto_scope final : private e2d::noncopyable {
        public:
            auto_scope(profiler* profiler, str name);
            auto_scope(profiler* profiler, str name, args_t args);
            ~auto_scope() noexcept;
        private:
            profiler* profiler_ = nullptr;
        };
    public:
        profiler(deferrer& d);
        ~profiler() noexcept final;

        void begin_scope(str name) noexcept;
        void begin_scope(str name, args_t args) noexcept;
        void end_scope() noexcept;

        void thread_event(str name) noexcept;
        void thread_event(str name, args_t args) noexcept;

        void global_event(str name) noexcept;
        void global_event(str name, args_t args) noexcept;
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
        std::size_t depth_{0u};
        vector<sink_uptr> sinks_;
        std::recursive_mutex rmutex_;
    };
}

#define E2D_PROFILER_SCOPE(name)\
    auto E2D_PP_CAT(e2d_generated_profiler_auto_scope_, __LINE__) =\
        ::e2d::profiler::auto_scope(\
            modules::is_initialized<profiler>() ? &the<profiler>() : nullptr,\
            name);

#define E2D_PROFILER_SCOPE_EX(name, ...)\
    auto E2D_PP_CAT(e2d_generated_profiler_auto_scope_, __LINE__) =\
        ::e2d::profiler::auto_scope(\
            modules::is_initialized<profiler>() ? &the<profiler>() : nullptr,\
            name,\
            __VA_ARGS__);

#define E2D_PROFILER_THREAD_EVENT(name)\
    if ( modules::is_initialized<profiler>() ) {\
        the<profiler>().thread_event(name);\
    }

#define E2D_PROFILER_THREAD_EVENT_EX(name, ...)\
    if ( modules::is_initialized<profiler>() ) {\
        the<profiler>().thread_event(name, __VA_ARGS___);\
    }

#define E2D_PROFILER_GLOBAL_EVENT(name)\
    if ( modules::is_initialized<profiler>() ) {\
        the<profiler>().global_event(name);\
    }

#define E2D_PROFILER_GLOBAL_EVENT_EX(name, ...)\
    if ( modules::is_initialized<profiler>() ) {\
        the<profiler>().global_event(name, __VA_ARGS__);\
    }

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
            temp_sink(std::size_t depth, time_point_t time_point)
            : depth_(depth)
            , time_point_(time_point) {}

            void on_event(const event_info& event) noexcept final {
                const bool skip = info_.events.empty() && depth_;

                if ( std::holds_alternative<begin_scope_info>(event) ) {
                    ++depth_;
                } else if ( std::holds_alternative<end_scope_info>(event) ) {
                    E2D_ASSERT(depth_);
                    --depth_;
                }

                try {
                    if ( !skip ) {
                        info_.events.push_back(event);
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

        temp_sink& s = register_sink<temp_sink>(depth_, timeout_time);
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

namespace e2d::profilers
{
    bool try_save_recording_info(
        const profiler::recording_info& src,
        buffer& dst) noexcept;

    bool try_save_recording_info(
        const profiler::recording_info& src,
        const output_stream_uptr& dst) noexcept;
}
