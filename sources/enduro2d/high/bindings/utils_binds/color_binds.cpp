/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils_binds.hpp"

using namespace e2d;

namespace e2d::bindings::utils
{
    void bind_color(sol::state& l) {
        l.new_usertype<color>("color",
            sol::constructors<
                color(),
                color(color),
                color(color32),
                color(vec4<f32>),
                color(vec3<f32>, f32),
                color(f32,f32,f32,f32)>(),

            "clear", &color::clear,
            "black", &color::black,
            "white", &color::white,
            "red", &color::red,
            "green", &color::green,
            "blue", &color::blue,
            "yellow", &color::yellow,
            "magenta", &color::magenta,
            "cyan", &color::cyan,

            "r", &color::r,
            "g", &color::g,
            "b", &color::b,
            "a", &color::a,

            sol::meta_function::equal_to, sol::resolve<bool(const color&, const color&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const color&, const color&)>(::operator<),

            sol::meta_function::addition, sol::overload(
                sol::resolve<color(f32, const color&)>(::operator+),
                sol::resolve<color(color, f32)>(::operator+),
                sol::resolve<color(color, const color&)>(::operator+)),

            sol::meta_function::subtraction, sol::overload(
                sol::resolve<color(f32, const color&)>(::operator-),
                sol::resolve<color(color, f32)>(::operator-),
                sol::resolve<color(color, const color&)>(::operator-)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<color(f32, const color&)>(::operator*),
                sol::resolve<color(color, f32)>(::operator*),
                sol::resolve<color(color, const color&)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<color(f32, const color&)>(::operator/),
                sol::resolve<color(color, f32)>(::operator/),
                sol::resolve<color(color, const color&)>(::operator/)),

            "approximately", sol::resolve<bool(const color&, const color&)>(&math::approximately),

            "minimum", sol::resolve<f32(const color&)>(&math::minimum),
            "maximum", sol::resolve<f32(const color&)>(&math::maximum),

            "minimized", sol::resolve<color(const color&, const color&)>(&math::minimized),
            "maximized", sol::resolve<color(const color&, const color&)>(&math::maximized),
            "clamped", sol::resolve<color(const color&, const color&, const color&)>(&math::clamped),

            "contains_nan", sol::resolve<bool(const color&)>(&math::contains_nan));
    }
}
