/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math_binds.hpp"

namespace
{
    using namespace e2d;

    template < typename T >
    void bind_vec3_t(const str& name, sol::state& l) {
        l.new_usertype<vec3<T>>(name,
            sol::constructors<
                vec3<T>(),
                vec3<T>(T),
                vec3<T>(T,T,T),
                vec3<T>(vec2<T>,T),
                vec3<T>(vec3<T>),
                vec3<T>(vec4<T>)>(),

            "zero", &vec3<T>::zero,
            "unit", &vec3<T>::unit,
            "unit_x", &vec3<T>::unit_x,
            "unit_y", &vec3<T>::unit_y,
            "unit_z", &vec3<T>::unit_z,

            "x", &vec3<T>::x,
            "y", &vec3<T>::y,
            "z", &vec3<T>::z,

            sol::meta_function::to_string, [](const vec3<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const vec3<T>&, const vec3<T>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const vec3<T>&, const vec3<T>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const vec3<T>&, const vec3<T>&)>(::operator<=),

            sol::meta_function::unary_minus, sol::resolve<vec3<T>(const vec3<T>&)>(::operator-),

            sol::meta_function::addition, sol::overload(
                sol::resolve<vec3<T>(T, const vec3<T>&)>(::operator+),
                sol::resolve<vec3<T>(const vec3<T>&, T)>(::operator+),
                sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(::operator+)),

            sol::meta_function::subtraction, sol::overload(
                sol::resolve<vec3<T>(T, const vec3<T>&)>(::operator-),
                sol::resolve<vec3<T>(const vec3<T>&, T)>(::operator-),
                sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(::operator-)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<vec3<T>(T, const vec3<T>&)>(::operator*),
                sol::resolve<vec3<T>(const vec3<T>&, T)>(::operator*),
                sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(::operator*),
                sol::resolve<vec3<T>(const vec3<T>&, const mat3<T>&)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<vec3<T>(T, const vec3<T>&)>(::operator/),
                sol::resolve<vec3<T>(const vec3<T>&, T)>(::operator/),
                sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(::operator/)),

            "approximately", [](const vec3<T>& l, const vec3<T>& r){ return math::approximately(l,r); },

            "dot", sol::resolve<T(const vec3<T>&, const vec3<T>&)>(&math::dot),
            "abs_dot", sol::resolve<T(const vec3<T>&, const vec3<T>&)>(&math::abs_dot),

            "cross", sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(&math::cross),

            "length_squared", sol::resolve<T(const vec3<T>&)>(&math::length_squared),
            "length", sol::resolve<T(const vec3<T>&)>(&math::length),

            "distance_squared", sol::resolve<math::make_distance_t<T>(const vec3<T>&, const vec3<T>&)>(&math::distance_squared),
            "distance", sol::resolve<T(const vec3<T>&, const vec3<T>&)>(&math::distance),

            "normalized", [](const vec3<T>& v){ return math::normalized(v); },

            "minimum", sol::resolve<T(const vec3<T>&)>(&math::minimum),
            "maximum", sol::resolve<T(const vec3<T>&)>(&math::maximum),

            "minimized", sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(&math::minimized),
            "maximized", sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(&math::maximized),
            "clamped", sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&, const vec3<T>&)>(&math::clamped),
            "saturated", sol::resolve<vec3<T>(const vec3<T>&)>(&math::saturated),

            "lerp", sol::overload(
                sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&, T)>(&math::lerp),
                sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&, const vec3<T>&)>(&math::lerp)),
            "inverse_lerp", sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&, const vec3<T>&)>(&math::inverse_lerp));
    }
}

namespace e2d::bindings::math
{
    void bind_vec3(sol::state& l) {
        bind_vec3_t<f32>("v3f", l);
    }
}
