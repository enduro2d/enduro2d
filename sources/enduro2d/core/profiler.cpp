/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/profiler.hpp>

namespace
{
    using namespace e2d;

    str recording_info_to_json(const profiler::recording_info& src) {
        str dst("{\"traceEvents\":[\n");
        for ( std::size_t i = 0; i < src.events.size(); ++i ) {
            std::visit(utils::overloaded {
                [&dst](const profiler::begin_scope_info& e){
                    char buf[512] = {'\0'};
                    strings::format(buf, std::size(buf),
                        "  {\"name\":\"%0\", \"ph\":\"B\", \"ts\":%1, \"tid\":%2, \"pid\":0}",
                        e.name,
                        e.tp.count(),
                        std::hash<std::thread::id>()(e.tid));
                    dst += buf;
                },
                [&dst](const profiler::end_scope_info& e){
                    char buf[512] = {'\0'};
                    strings::format(buf, std::size(buf),
                        "  {\"ph\":\"E\", \"ts\":%0, \"tid\":%1, \"pid\":0}",
                        e.tp.count(),
                        std::hash<std::thread::id>()(e.tid));
                    dst += buf;
                },
                [&dst](const profiler::thread_event_info& e){
                    char buf[512] = {'\0'};
                    strings::format(buf, std::size(buf),
                        "  {\"name\":\"%0\", \"ph\":\"i\", \"s\":\"t\", \"ts\":%1, \"tid\":%2, \"pid\":0}",
                        e.name,
                        e.tp.count(),
                        std::hash<std::thread::id>()(e.tid));
                    dst += buf;
                },
                [&dst](const profiler::global_event_info& e){
                    char buf[512] = {'\0'};
                    strings::format(buf, std::size(buf),
                        "  {\"name\":\"%0\", \"ph\":\"i\", \"s\":\"g\", \"ts\":%1, \"tid\":%2, \"pid\":0}",
                        e.name,
                        e.tp.count(),
                        std::hash<std::thread::id>()(e.tid));
                    dst += buf;
                }
            }, src.events[i]);

            if ( i < src.events.size() - 1u ) {
                dst += ",\n";
            }
        }
        dst += "\n]}";
        return dst;
    }
}

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
        ++depth_;

        begin_scope_info event{
            std::move(name),
            std::this_thread::get_id(),
            time::to_chrono(time::now_us())};
        
        std::lock_guard guard(rmutex_);
        for ( const auto& sink : sinks_ ) {
            if ( sink ) {
                sink->on_event(event);
            }
        }
    }

    void profiler::end_scope() noexcept {
        E2D_ASSERT(depth_);
        --depth_;

        end_scope_info event{
            std::this_thread::get_id(),
            time::to_chrono(time::now_us())};
        
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
            time::to_chrono(time::now_us())};
        
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
            time::to_chrono(time::now_us())};
        
        std::lock_guard guard(rmutex_);
        for ( const auto& sink : sinks_ ) {
            if ( sink ) {
                sink->on_event(event);
            }
        }
    }
}

namespace e2d::profilers
{
    bool try_save_recording_info(
        const profiler::recording_info& src,
        buffer& dst) noexcept
    {
        try {
            str json = recording_info_to_json(src);
            dst.assign(json.data(), json.size());
            return true;
        } catch (...) {
            return false;
        }
    }
    
    bool try_save_recording_info(
        const profiler::recording_info& src,
        const output_stream_uptr& dst) noexcept
    {
        buffer file_data;
        return try_save_recording_info(src, file_data)
            && streams::try_write_tail(file_data, dst);
    }
}
