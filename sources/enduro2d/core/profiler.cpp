/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/profiler.hpp>

namespace e2d
{
    profiler::scope::scope(profiler& profiler, str name)
    : profiler_(profiler) {
        profiler_.begin_scope(std::move(name));
    }

    profiler::scope::~scope() noexcept {
        profiler_.end_scope();
    }
}

namespace e2d
{
    class profiler::internal_state final : private e2d::noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        sink& register_sink(sink_uptr sink) {
            E2D_ASSERT(sink);
            std::lock_guard<std::recursive_mutex> guard(rmutex_);
            sinks_.push_back(std::move(sink));
            return *sinks_.back();
        }

        void unregister_sink(const sink& sink) noexcept {
            std::lock_guard<std::recursive_mutex> guard(rmutex_);
            for ( auto iter = sinks_.begin(); iter != sinks_.end(); ) {
                if ( iter->get() == &sink ) {
                    iter = sinks_.erase(iter);
                } else {
                    ++iter;
                }
            }
        }

        void emit_event(event_info event) noexcept {
            std::lock_guard<std::recursive_mutex> guard(rmutex_);
            for ( const auto& sink : sinks_ ) {
                if ( sink ) {
                    sink->on_event(event);
                }
            }
        }
    private:
        vector<sink_uptr> sinks_;
        std::recursive_mutex rmutex_;
    };
}

namespace e2d
{
    profiler::profiler()
    : state_(new internal_state()) {}
    profiler::~profiler() noexcept = default;

    profiler::sink& profiler::register_sink(sink_uptr sink) {
        return state_->register_sink(std::move(sink));
    }

    void profiler::unregister_sink(const sink& sink) noexcept {
        state_->unregister_sink(sink);
    }

    void profiler::begin_scope(str name) noexcept {
        state_->emit_event(begin_scope_info{
            std::move(name),
            math::numeric_cast<u64>(time::now_us().value),
            std::hash<std::thread::id>()(std::this_thread::get_id())});
    }

    void profiler::end_scope() noexcept {
        state_->emit_event(end_scope_info{
            math::numeric_cast<u64>(time::now_us().value),
            std::hash<std::thread::id>()(std::this_thread::get_id())});
    }

    void profiler::thread_event(str name) noexcept {
        state_->emit_event(thread_event_info{
            std::move(name),
            math::numeric_cast<u64>(time::now_us().value),
            std::hash<std::thread::id>()(std::this_thread::get_id())});
    }

    void profiler::global_event(str name) noexcept {
        state_->emit_event(global_event_info{
            std::move(name),
            math::numeric_cast<u64>(time::now_us().value),
            std::hash<std::thread::id>()(std::this_thread::get_id())});
    }
}
