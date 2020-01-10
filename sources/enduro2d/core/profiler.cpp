/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/profiler.hpp>

#include <enduro2d/core/engine.hpp>
#include <enduro2d/core/vfs.hpp>

namespace
{
    using namespace e2d;

    str recording_args_to_json(const profiler::args_t& args) {
        str dst = "{";
        for ( auto iter = args.begin(); iter != args.end(); ) {
            dst += strings::rformat("\"%0\":\"%1\"", iter->first, iter->second);
            if ( ++iter != args.end() ) {
                dst += ",";
            }
        }
        dst += "}";
        return dst;
    }

    str recording_info_to_json(const profiler::recording_info& src) {
        str dst("{\"traceEvents\":[\n");

        dst += strings::rformat(
            "  {\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":%0,\"pid\":0,\"args\":{\"name\":\"%1\"}},\n",
            std::hash<std::thread::id>()(the<engine>().main_thread()),
            "Main");

        for ( std::thread::id tid : the<vfs>().worker().thread_ids() ) {
            dst += strings::rformat(
                "  {\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":%0,\"pid\":0,\"args\":{\"name\":\"%1\"}},\n",
                std::hash<std::thread::id>()(tid),
                "VFS");
        }

        for ( std::thread::id tid : the<deferrer>().worker().thread_ids() ) {
            dst += strings::rformat(
                "  {\"name\":\"thread_name\",\"ph\":\"M\",\"tid\":%0,\"pid\":0,\"args\":{\"name\":\"%1\"}},\n",
                std::hash<std::thread::id>()(tid),
                "Worker");
        }

        for ( auto iter = src.events.begin(); iter != src.events.end(); ) {
            std::visit(utils::overloaded {
                [&dst](const profiler::begin_scope_info& e){
                    char buf[512] = {'\0'};
                    strings::format(buf, std::size(buf),
                        "  {\"name\":\"%0\",\"ph\":\"B\",\"ts\":%1,\"tid\":%2,\"pid\":0,\"cat\":\"\",\"args\":%3}",
                        e.name,
                        e.tp.count(),
                        std::hash<std::thread::id>()(e.tid),
                        recording_args_to_json(e.args));
                    dst += buf;
                },
                [&dst](const profiler::end_scope_info& e){
                    char buf[512] = {'\0'};
                    strings::format(buf, std::size(buf),
                        "  {\"ph\":\"E\",\"ts\":%0,\"tid\":%1,\"pid\":0,\"cat\":\"\"}",
                        e.tp.count(),
                        std::hash<std::thread::id>()(e.tid));
                    dst += buf;
                },
                [&dst](const profiler::thread_event_info& e){
                    char buf[512] = {'\0'};
                    strings::format(buf, std::size(buf),
                        "  {\"name\":\"%0\",\"ph\":\"i\",\"s\":\"t\",\"ts\":%1,\"tid\":%2,\"pid\":0,\"cat\":\"\",\"args\":%3}",
                        e.name,
                        e.tp.count(),
                        std::hash<std::thread::id>()(e.tid),
                        recording_args_to_json(e.args));
                    dst += buf;
                },
                [&dst](const profiler::global_event_info& e){
                    char buf[512] = {'\0'};
                    strings::format(buf, std::size(buf),
                        "  {\"name\":\"%0\",\"ph\":\"i\",\"s\":\"g\",\"ts\":%1,\"tid\":%2,\"pid\":0,\"cat\":\"\",\"args\":%3}",
                        e.name,
                        e.tp.count(),
                        std::hash<std::thread::id>()(e.tid),
                        recording_args_to_json(e.args));
                    dst += buf;
                }
            }, *iter);

            if ( ++iter != src.events.end() ) {
                dst += ",\n";
            }
        }

        dst += "\n]}";
        return dst;
    }
}

namespace e2d
{
    profiler::auto_scope::auto_scope(profiler* profiler, str name)
    : profiler_(profiler) {
        if ( profiler_ ) {
            profiler_->begin_scope(std::move(name));
        }
    }

    profiler::auto_scope::auto_scope(profiler* profiler, str name, args_t args)
    : profiler_(profiler) {
        if ( profiler_ ) {
            profiler_->begin_scope(std::move(name), std::move(args));
        }
    }

    profiler::auto_scope::~auto_scope() noexcept {
        if ( profiler_ ) {
            profiler_->end_scope();
        }
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
        return begin_scope(std::move(name), {});
    }

    void profiler::begin_scope(str name, args_t args) noexcept {
        ++depth_;

        begin_scope_info event{
            std::move(name),
            std::move(args),
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
        return thread_event(std::move(name), {});
    }

    void profiler::thread_event(str name, args_t args) noexcept {
        thread_event_info event{
            std::move(name),
            std::move(args),
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
        return global_event(std::move(name), {});
    }

    void profiler::global_event(str name, args_t args) noexcept {
        global_event_info event{
            std::move(name),
            std::move(args),
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
