/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math_binds.hpp"

namespace
{
    using namespace e2d;

    template < typename T >
    void bind_vec4_t(const str& name, sol::state& l) {
        l.new_usertype<vec4<T>>(name,
            sol::constructors<
                vec4<T>(),
                vec4<T>(T),
                vec4<T>(T,T,T,T),
                vec4<T>(vec2<T>,T,T),
                vec4<T>(vec3<T>,T),
                vec4<T>(vec4<T>)>(),

            "zero", &vec4<T>::zero,
            "unit", &vec4<T>::unit,
            "unit_x", &vec4<T>::unit_x,
            "unit_y", &vec4<T>::unit_y,
            "unit_z", &vec4<T>::unit_z,
            "unit_w", &vec4<T>::unit_w,

            "x", &vec4<T>::x,
            "y", &vec4<T>::y,
            "z", &vec4<T>::z,
            "w", &vec4<T>::w,

            sol::meta_function::to_string, [](const vec4<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const vec4<T>&, const vec4<T>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const vec4<T>&, const vec4<T>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const vec4<T>&, const vec4<T>&)>(::operator<=),

            sol::meta_function::unary_minus, sol::resolve<vec4<T>(const vec4<T>&)>(::operator-),

            sol::meta_function::addition, sol::overload(
                sol::resolve<vec4<T>(T, const vec4<T>&)>(::operator+),
                sol::resolve<vec4<T>(const vec4<T>&, T)>(::operator+),
                sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(::operator+)),

            sol::meta_function::subtraction, sol::overload(
                sol::resolve<vec4<T>(T, const vec4<T>&)>(::operator-),
                sol::resolve<vec4<T>(const vec4<T>&, T)>(::operator-),
                sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(::operator-)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<vec4<T>(T, const vec4<T>&)>(::operator*),
                sol::resolve<vec4<T>(const vec4<T>&, T)>(::operator*),
                sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(::operator*),
                sol::resolve<vec4<T>(const vec4<T>&, const mat4<T>&)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<vec4<T>(T, const vec4<T>&)>(::operator/),
                sol::resolve<vec4<T>(const vec4<T>&, T)>(::operator/),
                sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(::operator/)),

            "approximately", [](const vec4<T>& l, const vec4<T>& r){ return math::approximately(l,r); },

            "dot", sol::resolve<T(const vec4<T>&, const vec4<T>&)>(&math::dot),
            "abs_dot", sol::resolve<T(const vec4<T>&, const vec4<T>&)>(&math::abs_dot),

            "length_squared", sol::resolve<T(const vec4<T>&)>(&math::length_squared),
            "length", sol::resolve<T(const vec4<T>&)>(&math::length),

            "distance_squared", sol::resolve<math::make_distance_t<T>(const vec4<T>&, const vec4<T>&)>(&math::distance_squared),
            "distance", sol::resolve<T(const vec4<T>&, const vec4<T>&)>(&math::distance),

            "normalized", [](const vec4<T>& v){ return math::normalized(v); },

            "minimum", sol::resolve<T(const vec4<T>&)>(&math::minimum),
            "maximum", sol::resolve<T(const vec4<T>&)>(&math::maximum),

            "minimized", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(&math::minimized),
            "maximized", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(&math::maximized),
            "clamped", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, const vec4<T>&)>(&math::clamped),
            "saturated", sol::resolve<vec4<T>(const vec4<T>&)>(&math::saturated),

            "lerp", sol::overload(
                sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, T)>(&math::lerp),
                sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, const vec4<T>&)>(&math::lerp)),
            "inverse_lerp", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, const vec4<T>&)>(&math::inverse_lerp));
    }
}

namespace e2d::bindings::math
{
    void bind_vec4(sol::state& l) {
        bind_vec4_t<f32>("v4f", l);
    }
}
