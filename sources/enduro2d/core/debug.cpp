/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/debug.hpp>

namespace
{
    using namespace e2d;

    const char* level_to_cstr(debug::level l) noexcept {
        #define DEFINE_CASE(x) case debug::level::x: return #x
        switch ( l ) {
            DEFINE_CASE(trace);
            DEFINE_CASE(warning);
            DEFINE_CASE(error);
            DEFINE_CASE(fatal);
            default:
                E2D_ASSERT_MSG(false, "unexpected level");
                return "unknown";
        }
        #undef DEFINE_CASE
    }

    str log_text_format(debug::level lvl, str_view text) {
        return strings::rformat(
            "[%0](%1) -> %2\n",
            level_to_cstr(lvl), time::now_ms(), text);
    }
}

namespace e2d
{
    debug::sink& debug::register_sink(sink_uptr sink) {
        return register_sink_ex(level::trace, std::move(sink));
    }

    void debug::unregister_sink(const sink& sink) noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        sinks_.erase(std::remove_if(
            sinks_.begin(), sinks_.end(),
            [&sink](const std::pair<level, sink_uptr>& p){
                return p.second.get() == &sink;
            }), sinks_.end());
    }

    debug::sink& debug::register_sink_ex(level min_lvl, sink_uptr sink) {
        E2D_ASSERT(sink);
        std::lock_guard<std::mutex> guard(mutex_);
        sinks_.push_back(std::make_pair(min_lvl, std::move(sink)));
        return *sinks_.back().second;
    }

    debug& debug::set_min_level(level lvl) noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        min_level_ = lvl;
        return *this;
    }

    debug::level debug::min_level() const noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        return min_level_;
    }

    //
    // debug_stream_sink
    //

    debug_stream_sink::debug_stream_sink(output_stream_uptr stream)
    : stream_(std::move(stream)) {}

    debug_stream_sink::debug_stream_sink(output_stream_uptr stream, debug::level flush_level)
    : stream_(std::move(stream))
    , flush_level_(flush_level) {}

    bool debug_stream_sink::on_message(debug::level lvl, str_view text) noexcept {
        try {
            return stream_ && output_sequence(*stream_)
                .write_all(log_text_format(lvl, text))
                .flush_if(lvl >= flush_level_)
                .success();
        } catch (...) {
            return false;
        }
    }

    //
    // debug_console_sink
    //

    bool debug_console_sink::on_message(debug::level lvl, str_view text) noexcept {
        try {
            const str log_text = log_text_format(lvl, text);
            const std::ptrdiff_t rprintf = std::printf("%s", log_text.c_str());
            return rprintf >= 0
                && math::numeric_cast<std::size_t>(rprintf) == log_text.length();
        } catch (...) {
            return false;
        }
    }
}
