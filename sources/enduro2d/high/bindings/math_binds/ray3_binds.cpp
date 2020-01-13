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
    void bind_ray3_t(const str& name, sol::state& l) {
        l.new_usertype<ray3<T>>(name,
            sol::constructors<
                ray3<T>(),
                ray3<T>(ray3<T>),
                ray3<T>(T,T,T),
                ray3<T>(T,T,T,T,T,T),
                ray3<T>(vec3<T>),
                ray3<T>(vec3<T>,vec3<T>)>(),

            "zero", &ray3<T>::zero,
            "unit", &ray3<T>::unit_x,
            "unit_y", &ray3<T>::unit_y,
            "unit_z", &ray3<T>::unit_z,

            "origin", &ray3<T>::origin,
            "direction", &ray3<T>::direction,

            sol::meta_function::to_string, [](const ray3<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const ray3<T>&, const ray3<T>&)>(::operator==),

            sol::meta_function::addition, sol::overload(
                sol::resolve<ray3<T>(const ray3<T>&, T)>(::operator+),
                sol::resolve<ray3<T>(const ray3<T>&, const vec3<T>&)>(::operator+)),

            sol::meta_function::subtraction, sol::overload(
                sol::resolve<ray3<T>(const ray3<T>&, T)>(::operator-),
                sol::resolve<ray3<T>(const ray3<T>&, const vec3<T>&)>(::operator-)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<ray3<T>(const ray3<T>&, T)>(::operator*),
                sol::resolve<ray3<T>(const ray3<T>&, const vec3<T>&)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<ray3<T>(const ray3<T>&, T)>(::operator/),
                sol::resolve<ray3<T>(const ray3<T>&, const vec3<T>&)>(::operator/)),

            "approximately", [](const ray3<T>& l, const ray3<T>& r){ return math::approximately(l,r); });
    }
}

namespace e2d::bindings::math
{
    void bind_ray3(sol::state& l) {
        bind_ray3_t<f32>("r3f", l);
    }
}
