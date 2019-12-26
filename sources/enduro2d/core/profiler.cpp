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
    profiler::profiler(deferrer& d)
    : deferrer_(d) {}
    profiler::~profiler() noexcept = default;

    profiler::sink& profiler::register_sink(sink_uptr sink) {
        E2D_ASSERT(sink);
        std::lock_guard guard(rmutex_);
        sinks_.push_back(std::move(sink));
        return *sinks_.back();
    }

    void profiler::unregister_sink(const sink& sink) noexcept {
        std::lock_guard guard(rmutex_);
        for ( auto iter = sinks_.begin(); iter != sinks_.end(); ) {
            if ( iter->get() == &sink ) {
                iter = sinks_.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    void profiler::begin_scope(str name) noexcept {
        begin_scope_info event{
            std::move(name),
            std::this_thread::get_id(),
            std::chrono::steady_clock::now()};
        
        std::lock_guard guard(rmutex_);
        for ( const auto& sink : sinks_ ) {
            if ( sink ) {
                sink->on_event(event);
            }
        }
    }

    void profiler::end_scope() noexcept {
        end_scope_info event{
            std::this_thread::get_id(),
            std::chrono::steady_clock::now()};
        
        std::lock_guard guard(rmutex_);
        for ( const auto& sink : sinks_ ) {
            if ( sink ) {
                sink->on_event(event);
            }
        }
    }

    void profiler::thread_event(str name) noexcept {
        thread_event_info event{
            std::move(name),
            std::this_thread::get_id(),
            std::chrono::steady_clock::now()};
        
        std::lock_guard guard(rmutex_);
        for ( const auto& sink : sinks_ ) {
            if ( sink ) {
                sink->on_event(event);
            }
        }
    }

    void profiler::global_event(str name) noexcept {
        global_event_info event{
            std::move(name),
            std::this_thread::get_id(),
            std::chrono::steady_clock::now()};
        
        std::lock_guard guard(rmutex_);
        for ( const auto& sink : sinks_ ) {
            if ( sink ) {
                sink->on_event(event);
            }
        }
    }
}
