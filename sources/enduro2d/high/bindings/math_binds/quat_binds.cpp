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
    void bind_quat_t(const str& name, sol::state& l) {
        l.new_usertype<quat<T>>(name,
            sol::constructors<
                quat<T>(),
                quat<T>(quat<T>),
                quat<T>(T,T,T,T),
                quat<T>(vec4<T>)>(),

            "zero", &quat<T>::zero,
            "identity", &quat<T>::identity,

            "x", &quat<T>::x,
            "y", &quat<T>::y,
            "z", &quat<T>::z,
            "w", &quat<T>::w,

            sol::meta_function::to_string, [](const quat<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const quat<T>&, const quat<T>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const quat<T>&, const quat<T>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const quat<T>&, const quat<T>&)>(::operator<=),

            sol::meta_function::addition, sol::overload(
                sol::resolve<quat<T>(T, const quat<T>&)>(::operator+),
                sol::resolve<quat<T>(const quat<T>&, T)>(::operator+),
                sol::resolve<quat<T>(const quat<T>&, const quat<T>&)>(::operator+)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<quat<T>(T, const quat<T>&)>(::operator*),
                sol::resolve<quat<T>(const quat<T>&, T)>(::operator*),
                sol::resolve<quat<T>(const quat<T>&, const quat<T>&)>(::operator*)),

            "make_from_axis_angle", sol::overload(
                sol::resolve<quat<T>(const deg<T>&, const vec3<T>&)>(&math::make_quat_from_axis_angle),
                sol::resolve<quat<T>(const rad<T>&, const vec3<T>&)>(&math::make_quat_from_axis_angle)),

            "make_from_euler_angles", sol::overload(
                sol::resolve<quat<T>(const deg<T>&, const deg<T>&, const deg<T>&)>(&math::make_quat_from_euler_angles),
                sol::resolve<quat<T>(const rad<T>&, const rad<T>&, const rad<T>&)>(&math::make_quat_from_euler_angles)),

            "approximately", [](const quat<T>& l, const quat<T>& r){ return math::approximately(l,r); },

            "dot", sol::resolve<T(const quat<T>&, const quat<T>&)>(&math::dot),
            "abs_dot", sol::resolve<T(const quat<T>&, const quat<T>&)>(&math::abs_dot),

            "length_squared", sol::resolve<T(const quat<T>&)>(&math::length_squared),
            "length", sol::resolve<T(const quat<T>&)>(&math::length),

            "normalized", [](const quat<T>& v){ return math::normalized(v); },

            "lerp", sol::resolve<quat<T>(const quat<T>&, const quat<T>&, T)>(&math::lerp),

            "inversed", [](const quat<T>& v){ return math::inversed(v); },
            "conjugated", [](const quat<T>& v){ return math::conjugated(v); },

            "contains_nan", sol::resolve<bool(const quat<T>&)>(&math::contains_nan));
    }
}

namespace e2d::bindings::math
{
    void bind_quat(sol::state& l) {
        bind_quat_t<f32>("q4f", l);
    }
}
