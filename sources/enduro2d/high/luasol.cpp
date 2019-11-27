/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/luasol.hpp>

#include "bindings/math_binds/_math_binds.hpp"
#include "bindings/utils_binds/_utils_binds.hpp"
#include "bindings/core_binds/_core_binds.hpp"
#include "bindings/high_binds/_high_binds.hpp"

namespace e2d
{
    luasol::luasol() {
        state_.open_libraries(
            sol::lib::base,
            //sol::lib::package,
            sol::lib::coroutine,
            sol::lib::string,
            //sol::lib::os,
            sol::lib::math,
            sol::lib::table,
            //sol::lib::debug,
            //sol::lib::bit32
            //sol::lib::io
            sol::lib::utf8);

        bindings::bind_math(state_);
        bindings::bind_utils(state_);
        bindings::bind_core(state_);
        bindings::bind_high(state_);
    }

    void luasol::collect_garbage() {
        E2D_ASSERT(is_in_main_thread());
        state_.collect_garbage();
    }

    std::optional<script> luasol::load_script(buffer_view src) {
        E2D_ASSERT(is_in_main_thread());

        sol::load_result result = state_.load_buffer(
            reinterpret_cast<const char*>(src.data()),
            src.size());

        if ( !result.valid() ) {
            sol::error err = result;
            the<debug>().error("LUASOL: Failed to load script buffer:\n"
                "--> Info: %0",
                err.what());
            return std::nullopt;
        }

        return script(sol::protected_function(result));
    }

    std::optional<script> luasol::load_script(const input_stream_uptr& src) {
        E2D_ASSERT(is_in_main_thread());

        buffer file_data;
        if ( !streams::try_read_tail(file_data, src) ) {
            the<debug>().error("LUASOL: Failed to read script stream");
            return std::nullopt;
        }

        return load_script(file_data);
    }
}
