/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/luasol.hpp>

#include "bindings/bindings.hpp"

namespace e2d
{
    luasol::luasol() {
        lua_.open_libraries(
            sol::lib::base,
            sol::lib::package,
            sol::lib::coroutine,
            sol::lib::string,
            sol::lib::math,
            sol::lib::table,
            sol::lib::utf8);
        bindings::bind_math(lua_);
        bindings::bind_utils(lua_);
        bindings::bind_core(lua_);
        bindings::bind_high(lua_);
    }

    luasol::~luasol() noexcept {
    }

    sol::state& luasol::lua() noexcept {
        return lua_;
    }

    const sol::state& luasol::lua() const noexcept {
        return lua_;
    }
}
