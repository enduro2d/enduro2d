/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class debug final : public module<debug> {
    public:
        enum class level : u8 {
            trace,
            warning,
            error,
            fatal
        };
        class sink : private e2d::noncopyable {
        public:
            virtual ~sink() noexcept = default;
            virtual bool on_message(level lvl, str_view text) noexcept = 0;
        };
        using sink_uptr = std::unique_ptr<sink>;
    public:
        debug() = default;
        ~debug() noexcept final = default;

        template < typename T, typename... Args >
        T& register_sink(Args&&... args);
        sink& register_sink(sink_uptr sink);
        void unregister_sink(const sink& sink) noexcept;

        template < typename T, typename... Args >
        T& register_sink_ex(level min_lvl, Args&&... args);
        sink& register_sink_ex(level min_lvl, sink_uptr sink);

        debug& set_min_level(level lvl) noexcept;
        level min_level() const noexcept;

        template < typename... Args >
        debug& log(level lvl, str_view fmt, Args&&... args) noexcept;

        template < typename... Args >
        debug& trace(str_view fmt, Args&&... args) noexcept;

        template < typename... Args >
        debug& warning(str_view fmt, Args&&... args) noexcept;

        template < typename... Args >
        debug& error(str_view fmt, Args&&... args) noexcept;

        template < typename... Args >
        debug& fatal(str_view fmt, Args&&... args) noexcept;
    private:
        mutable std::mutex mutex_;
        vector<std::pair<level, sink_uptr>> sinks_;
        level min_level_ = level::trace;
    };

    class debug_stream_sink final : public debug::sink {
    public:
        debug_stream_sink(output_stream_uptr stream);
        debug_stream_sink(output_stream_uptr stream, debug::level flush_level);
        bool on_message(debug::level lvl, str_view text) noexcept final;
    private:
        output_stream_uptr stream_;
        debug::level flush_level_ = debug::level::warning;
    };

    class debug_console_sink final : public debug::sink {
    public:
        bool on_message(debug::level lvl, str_view text) noexcept final;
    };
}

namespace e2d
{
    template < typename T, typename... Args >
    T& debug::register_sink(Args&&... args) {
        return register_sink_ex<T>(
            level::trace,
            std::forward<Args>(args)...);
    }

    template < typename T, typename... Args >
    T& debug::register_sink_ex(level min_lvl, Args&&... args) {
        return static_cast<T&>(register_sink_ex(
            min_lvl,
            std::make_unique<T>(std::forward<Args>(args)...)));
    }

    template < typename... Args >
    debug& debug::log(level lvl, str_view fmt, Args&&... args) noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        if ( lvl >= min_level_ && !sinks_.empty() ) {
            str formatted_text;
            try {
                formatted_text = strings::rformat(
                    fmt, std::forward<Args>(args)...);
            } catch (...) {
                E2D_ASSERT_MSG(false, "DEBUG: ignored log formatting exception");
                return *this;
            }
            for ( const auto& [slvl, sink] : sinks_ ) {
                if ( lvl >= slvl && sink ) {
                    bool success = sink->on_message(lvl, formatted_text);
                    E2D_UNUSED(success);
                    E2D_ASSERT_MSG(success, "DEBUG: ignored failed log sink call");
                }
            }
        }
        return *this;
    }

    template < typename... Args >
    debug& debug::trace(str_view fmt, Args&&... args) noexcept {
        return log(level::trace, fmt, std::forward<Args>(args)...);
    }

    template < typename... Args >
    debug& debug::warning(str_view fmt, Args&&... args) noexcept {
        return log(level::warning, fmt, std::forward<Args>(args)...);
    }

    template < typename... Args >
    debug& debug::error(str_view fmt, Args&&... args) noexcept {
        return log(level::error, fmt, std::forward<Args>(args)...);
    }

    template < typename... Args >
    debug& debug::fatal(str_view fmt, Args&&... args) noexcept {
        return log(level::fatal, fmt, std::forward<Args>(args)...);
    }
}
