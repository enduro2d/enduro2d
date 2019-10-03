/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "bindings.hpp"

namespace
{
    using namespace e2d;

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

            "approximately", [](const color& l, const color& r){ return math::approximately(l,r); },

            "minimum", sol::resolve<f32(const color&)>(&math::minimum),
            "maximum", sol::resolve<f32(const color&)>(&math::maximum),

            "minimized", sol::resolve<color(const color&, const color&)>(&math::minimized),
            "maximized", sol::resolve<color(const color&, const color&)>(&math::maximized),
            "clamped", sol::resolve<color(const color&, const color&, const color&)>(&math::clamped),

            "contains_nan", sol::resolve<bool(const color&)>(&math::contains_nan),

            "pack_color", sol::resolve<u32(const color&)>(&colors::pack_color),
            "unpack_color", sol::resolve<color(u32)>(&colors::unpack_color));
    }

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

            "approximately", [](const color32& l, const color32& r){ return math::approximately(l,r); },

            "minimum", sol::resolve<u8(const color32&)>(&math::minimum),
            "maximum", sol::resolve<u8(const color32&)>(&math::maximum),

            "minimized", sol::resolve<color32(const color32&, const color32&)>(&math::minimized),
            "maximized", sol::resolve<color32(const color32&, const color32&)>(&math::maximized),
            "clamped", sol::resolve<color32(const color32&, const color32&, const color32&)>(&math::clamped),

            "contains_nan", sol::resolve<bool(const color32&)>(&math::contains_nan),

            "pack_color", sol::resolve<u32(const color32&)>(&colors::pack_color32),
            "unpack_color", sol::resolve<color32(u32)>(&colors::unpack_color32));
    }
}

namespace e2d::bindings
{
    void bind_utils(sol::state& l) {
        bind_color(l);
        bind_color32(l);
    }
}
