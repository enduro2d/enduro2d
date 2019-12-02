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
            u64 ts = 0;
            u64 tid = 0;
        };

        struct end_scope_info {
            u64 ts = 0;
            u64 tid = 0;
        };

        struct thread_event_info {
            str name;
            u64 ts = 0;
            u64 tid = 0;
        };

        struct global_event_info {
            str name;
            u64 ts = 0;
            u64 tid = 0;
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
        profiler();
        ~profiler() noexcept final;

        void begin_scope(str name) noexcept;
        void end_scope() noexcept;

        void thread_event(str name) noexcept;
        void global_event(str name) noexcept;
    public:
        template < typename T, typename... Args >
        T& register_sink(Args&&... args);
        sink& register_sink(sink_uptr sink);
        void unregister_sink(const sink& sink) noexcept;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}

#define E2D_PROFILER_SCOPE(name)\
    auto E2D_PP_CAT(e2d_generated_profiler_scope_, __LINE__) = ::e2d::profiler::scope(the<profiler>(), name)

namespace e2d
{
    template < typename T, typename... Args >
    T& profiler::register_sink(Args&&... args) {
        return static_cast<T&>(
            register_sink(std::make_unique<T>(std::forward<Args>(args)...)));
    }
}
