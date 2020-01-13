/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils_binds.hpp"

using namespace e2d;

namespace e2d::bindings::utils
{
    void bind_color32(sol::state& l) {
        l.new_usertype<color32>("color32",
            sol::constructors<
                color32(),
                color32(color),
                color32(color32),
                color32(vec4<u8>),
                color32(vec3<u8>, u8),
                color32(u8,u8,u8,u8)>(),

            "clear", &color32::clear,
            "black", &color32::black,
            "white", &color32::white,
            "red", &color32::red,
            "green", &color32::green,
            "blue", &color32::blue,
            "yellow", &color32::yellow,
            "magenta", &color32::magenta,
            "cyan", &color32::cyan,

            "r", &color32::r,
            "g", &color32::g,
            "b", &color32::b,
            "a", &color32::a,

            sol::meta_function::equal_to, sol::resolve<bool(const color32&, const color32&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const color32&, const color32&)>(::operator<),

            sol::meta_function::addition, sol::overload(
                sol::resolve<color32(u8, const color32&)>(::operator+),
                sol::resolve<color32(color32, u8)>(::operator+),
                sol::resolve<color32(color32, const color32&)>(::operator+)),

            sol::meta_function::subtraction, sol::overload(
                sol::resolve<color32(u8, const color32&)>(::operator-),
                sol::resolve<color32(color32, u8)>(::operator-),
                sol::resolve<color32(color32, const color32&)>(::operator-)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<color32(u8, const color32&)>(::operator*),
                sol::resolve<color32(color32, u8)>(::operator*),
                sol::resolve<color32(color32, const color32&)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<color32(u8, const color32&)>(::operator/),
                sol::resolve<color32(color32, u8)>(::operator/),
                sol::resolve<color32(color32, const color32&)>(::operator/)),

            "approximately", sol::resolve<bool(const color32&, const color32&)>(&math::approximately),

            "minimum", sol::resolve<u8(const color32&)>(&math::minimum),
            "maximum", sol::resolve<u8(const color32&)>(&math::maximum),

            "minimized", sol::resolve<color32(const color32&, const color32&)>(&math::minimized),
            "maximized", sol::resolve<color32(const color32&, const color32&)>(&math::maximized),
            "clamped", sol::resolve<color32(const color32&, const color32&, const color32&)>(&math::clamped));
    }
}
