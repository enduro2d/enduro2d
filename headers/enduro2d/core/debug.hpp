/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
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
        debug();
        ~debug();

        template < typename T, typename... Args >
        T& add_sink(Args&&... args);
        sink& add_sink(sink_uptr sink);

        template < typename T, typename... Args >
        T& add_sink_ex(level min_lvl, Args&&... args);
        sink& add_sink_ex(level min_lvl, sink_uptr sink);

        void set_min_level(level lvl) noexcept;
        level min_level() const noexcept;

        template < typename... Args >
        debug& log(level lvl, str_view fmt, Args&&... args);

        template < typename... Args >
        debug& trace(str_view fmt, Args&&... args);

        template < typename... Args >
        debug& warning(str_view fmt, Args&&... args);

        template < typename... Args >
        debug& error(str_view fmt, Args&&... args);

        template < typename... Args >
        debug& fatal(str_view fmt, Args&&... args);
    private:
        mutable std::mutex mutex_;
        level min_level_ = level::trace;
        vector<std::pair<level, sink_uptr>> sinks_;
    };

    class debug_file_sink final : public debug::sink {
    public:
        debug_file_sink(str_view path);
        bool on_message(debug::level lvl, str_view text) noexcept final;
    private:
        str path_;
    };

    class debug_console_sink final : public debug::sink {
    public:
        bool on_message(debug::level lvl, str_view text) noexcept final;
    };
}

namespace e2d
{
    template < typename T, typename... Args >
    T& debug::add_sink(Args&&... args) {
        return add_sink_ex<T>(level::trace, std::forward<Args>(args)...);
    }

    inline debug::sink& debug::add_sink(sink_uptr sink) {
        return add_sink_ex(level::trace, std::move(sink));
    }

    template < typename T, typename... Args >
    T& debug::add_sink_ex(level min_lvl, Args&&... args) {
        return static_cast<T&>(add_sink_ex(
            min_lvl,
            std::make_unique<T>(std::forward<Args>(args)...)));
    }

    inline debug::sink& debug::add_sink_ex(level min_lvl, sink_uptr sink) {
        E2D_ASSERT(sink);
        std::lock_guard<std::mutex> guard(mutex_);
        sinks_.push_back(std::make_pair(min_lvl, std::move(sink)));
        return *sinks_.back().second;
    }

    template < typename... Args >
    debug& debug::log(level lvl, str_view fmt, Args&&... args) {
        std::lock_guard<std::mutex> guard(mutex_);
        if ( lvl >= min_level_ && !sinks_.empty() ) {
            str text = strings::rformat(fmt, std::forward<Args>(args)...);
            for ( const auto& pair : sinks_ ) {
                if ( lvl >= pair.first && pair.second ) {
                    pair.second->on_message(lvl, text);
                }
            }
        }
        return *this;
    }

    template < typename... Args >
    debug& debug::trace(str_view fmt, Args&&... args) {
        return log(level::trace, fmt, std::forward<Args>(args)...);
    }

    template < typename... Args >
    debug& debug::warning(str_view fmt, Args&&... args) {
        return log(level::warning, fmt, std::forward<Args>(args)...);
    }

    template < typename... Args >
    debug& debug::error(str_view fmt, Args&&... args) {
        return log(level::error, fmt, std::forward<Args>(args)...);
    }

    template < typename... Args >
    debug& debug::fatal(str_view fmt, Args&&... args) {
        return log(level::fatal, fmt, std::forward<Args>(args)...);
    }
}
