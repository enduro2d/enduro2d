/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "bindings.hpp"

namespace
{
    using namespace e2d;

    template < typename T >
    void bind_vec2(const str& name, sol::state& l) {
        l.new_usertype<vec2<T>>(name,
            sol::constructors<
                vec2<T>(),
                vec2<T>(T),
                vec2<T>(T,T),
                vec2<T>(vec2<T>),
                vec2<T>(vec3<T>),
                vec2<T>(vec4<T>)>(),

            "x", &vec2<T>::x,
            "y", &vec2<T>::y,

            sol::meta_function::equal_to, sol::resolve<bool(const vec2<T>&, const vec2<T>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const vec2<T>&, const vec2<T>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const vec2<T>&, const vec2<T>&)>(::operator<=),

            sol::meta_function::unary_minus, sol::resolve<vec2<T>(const vec2<T>&)>(::operator-),
            sol::meta_function::addition, sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(::operator+),
            sol::meta_function::subtraction, sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(::operator-),
            sol::meta_function::multiplication, sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(::operator*),
            sol::meta_function::division, sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(::operator/),

            "approximately", [](const vec2<T>& l, const vec2<T>& r){ return math::approximately(l,r); },

            "dot", sol::resolve<T(const vec2<T>&, const vec2<T>&)>(&math::dot),
            "abs_dot", sol::resolve<T(const vec2<T>&, const vec2<T>&)>(&math::abs_dot),

            "length_squared", sol::resolve<T(const vec2<T>&)>(&math::length_squared),
            "length", sol::resolve<T(const vec2<T>&)>(&math::length),

            "distance_squared", sol::resolve<math::make_distance_t<T>(const vec2<T>&, const vec2<T>&)>(&math::distance_squared),
            "distance", sol::resolve<T(const vec2<T>&, const vec2<T>&)>(&math::distance),

            "normalized", [](const vec2<T>& v){ return math::normalized(v); },

            "minimum", sol::resolve<T(const vec2<T>&)>(&math::minimum),
            "maximum", sol::resolve<T(const vec2<T>&)>(&math::maximum),

            "minimized", sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(&math::minimized),
            "maximized", sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(&math::maximized),
            "clamped", sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&, const vec2<T>&)>(&math::clamped),
            "saturated", sol::resolve<vec2<T>(const vec2<T>&)>(&math::saturated),

            "lerp", sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&, T)>(&math::lerp),
            "lerp", sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&, const vec2<T>&)>(&math::lerp),
            "inverse_lerp", sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&, const vec2<T>&)>(&math::inverse_lerp),

            "contains_nan", sol::resolve<bool(const vec2<T>&)>(&math::contains_nan));
    }

    template < typename T >
    void bind_vec3(const str& name, sol::state& l) {
        l.new_usertype<vec3<T>>(name,
            sol::constructors<
                vec3<T>(),
                vec3<T>(T),
                vec3<T>(T,T,T),
                vec3<T>(vec2<T>,T),
                vec3<T>(vec3<T>),
                vec3<T>(vec4<T>)>(),

            "x", &vec3<T>::x,
            "y", &vec3<T>::y,
            "z", &vec3<T>::z,

            sol::meta_function::equal_to, sol::resolve<bool(const vec3<T>&, const vec3<T>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const vec3<T>&, const vec3<T>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const vec3<T>&, const vec3<T>&)>(::operator<=),

            sol::meta_function::unary_minus, sol::resolve<vec3<T>(const vec3<T>&)>(::operator-),
            sol::meta_function::addition, sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(::operator+),
            sol::meta_function::subtraction, sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(::operator-),
            sol::meta_function::multiplication, sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(::operator*),
            sol::meta_function::division, sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&)>(::operator/),

            "approximately", [](const vec3<T>& l, const vec3<T>& r){ return math::approximately(l,r); },

            "dot", sol::resolve<T(const vec3<T>&, const vec3<T>&)>(&math::dot),
            "abs_dot", sol::resolve<T(const vec3<T>&, const vec3<T>&)>(&math::abs_dot),

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

            "lerp", sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&, T)>(&math::lerp),
            "lerp", sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&, const vec3<T>&)>(&math::lerp),
            "inverse_lerp", sol::resolve<vec3<T>(const vec3<T>&, const vec3<T>&, const vec3<T>&)>(&math::inverse_lerp),

            "contains_nan", sol::resolve<bool(const vec3<T>&)>(&math::contains_nan));
    }

    template < typename T >
    void bind_vec4(const str& name, sol::state& l) {
        l.new_usertype<vec4<T>>(name,
            sol::constructors<
                vec4<T>(),
                vec4<T>(T),
                vec4<T>(T,T,T,T),
                vec4<T>(vec2<T>,T,T),
                vec4<T>(vec3<T>,T),
                vec4<T>(vec4<T>)>(),

            "x", &vec4<T>::x,
            "y", &vec4<T>::y,
            "z", &vec4<T>::z,
            "w", &vec4<T>::w,

            sol::meta_function::equal_to, sol::resolve<bool(const vec4<T>&, const vec4<T>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const vec4<T>&, const vec4<T>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const vec4<T>&, const vec4<T>&)>(::operator<=),

            sol::meta_function::unary_minus, sol::resolve<vec4<T>(const vec4<T>&)>(::operator-),
            sol::meta_function::addition, sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(::operator+),
            sol::meta_function::subtraction, sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(::operator-),
            sol::meta_function::multiplication, sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(::operator*),
            sol::meta_function::division, sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&)>(::operator/),

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

            "lerp", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, T)>(&math::lerp),
            "lerp", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, const vec4<T>&)>(&math::lerp),
            "inverse_lerp", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, const vec4<T>&)>(&math::inverse_lerp),

            "contains_nan", sol::resolve<bool(const vec4<T>&)>(&math::contains_nan));
    }
}

namespace e2d::bindings
{
    void bind_math(sol::state& l) {
        bind_vec2<f32>("v2f", l);
        bind_vec3<f32>("v3f", l);
        bind_vec4<f32>("v4f", l);
    }
}
