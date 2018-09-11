/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/core/debug.hpp>

namespace
{
    using namespace e2d;

    const char* level_to_str(debug::level lvl) noexcept {
        switch ( lvl ) {
            case debug::level::trace:   return "trace";
            case debug::level::warning: return "warning";
            case debug::level::error:   return "error";
            case debug::level::fatal:   return "fatal";
            default:
                E2D_ASSERT(false);
                return "unknown";
        }
    }

    str log_text_format(debug::level lvl, str_view text) {
        return strings::rformat(
            "[%0](%1) -> %2\n",
            level_to_str(lvl), time::now_ms(), text);
    }
}

namespace e2d
{
    //
    // debug
    //

    debug::debug() {
    }

    debug::~debug() {
    }

    void debug::set_min_level(level lvl) noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        min_level_ = lvl;
    }

    debug::level debug::min_level() const noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        return min_level_;
    }

    //
    // debug_file_sink
    //

    debug_file_sink::debug_file_sink(str_view path)
    : path_(path) {
        make_write_file(path, false);
    }

    bool debug_file_sink::on_message(debug::level lvl, str_view text) noexcept {
        try {
            auto file = make_write_file(path_, true);
            str log_text = log_text_format(lvl, text);
            return file && output_sequence(*file)
                .write(log_text.c_str(), log_text.length())
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
            str log_text = log_text_format(lvl, text);
            const std::ptrdiff_t rprintf = std::printf("%s", log_text.c_str());
            return rprintf >= 0
                && math::numeric_cast<std::size_t>(rprintf) == log_text.length();
        } catch (...) {
            return false;
        }
    }
}
