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
    void bind_ray2_t(const str& name, sol::state& l) {
        l.new_usertype<ray2<T>>(name,
            sol::constructors<
                ray2<T>(),
                ray2<T>(ray2<T>),
                ray2<T>(T,T),
                ray2<T>(T,T,T,T),
                ray2<T>(vec2<T>),
                ray2<T>(vec2<T>,vec2<T>)>(),

            "zero", &ray2<T>::zero,
            "unit_x", &ray2<T>::unit_x,
            "unit_y", &ray2<T>::unit_y,

            "origin", &ray2<T>::origin,
            "direction", &ray2<T>::direction,

            sol::meta_function::to_string, [](const ray2<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const ray2<T>&, const ray2<T>&)>(::operator==),

            sol::meta_function::addition, sol::overload(
                sol::resolve<ray2<T>(const ray2<T>&, T)>(::operator+),
                sol::resolve<ray2<T>(const ray2<T>&, const vec2<T>&)>(::operator+)),

            sol::meta_function::subtraction, sol::overload(
                sol::resolve<ray2<T>(const ray2<T>&, T)>(::operator-),
                sol::resolve<ray2<T>(const ray2<T>&, const vec2<T>&)>(::operator-)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<ray2<T>(const ray2<T>&, T)>(::operator*),
                sol::resolve<ray2<T>(const ray2<T>&, const vec2<T>&)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<ray2<T>(const ray2<T>&, T)>(::operator/),
                sol::resolve<ray2<T>(const ray2<T>&, const vec2<T>&)>(::operator/)),

            "approximately", [](const ray2<T>& l, const ray2<T>& r){ return math::approximately(l,r); },

            "contains_nan", sol::resolve<bool(const ray2<T>&)>(&math::contains_nan));
    }
}

namespace e2d::bindings::math
{
    void bind_ray2(sol::state& l) {
        bind_ray2_t<f32>("r2f", l);
    }
}
