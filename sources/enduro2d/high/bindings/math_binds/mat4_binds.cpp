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
    void bind_mat4_t(const str& name, sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        .new_usertype<mat4<T>>(name,
            sol::constructors<
                mat4<T>(),
                mat4<T>(const vec4<T>&, const vec4<T>&, const vec4<T>&, const vec4<T>&)
            >(),

            "zero", &mat4<T>::zero,
            "identity", &mat4<T>::identity,

            "rows", &mat4<T>::rows,

            sol::meta_function::equal_to, sol::resolve<bool(const mat4<T>&, const mat4<T>&)>(::operator==),
            sol::meta_function::unary_minus, sol::resolve<mat4<T>(const mat4<T>&)>(::operator-),

            sol::meta_function::addition, sol::overload(
                sol::resolve<mat4<T>(T, const mat4<T>&)>(::operator+),
                sol::resolve<mat4<T>(const mat4<T>&, T)>(::operator+),
                sol::resolve<mat4<T>(const mat4<T>&, const mat4<T>&)>(::operator+)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<mat4<T>(T, const mat4<T>&)>(::operator*),
                sol::resolve<mat4<T>(const mat4<T>&, T)>(::operator*),
                sol::resolve<mat4<T>(const mat4<T>&, const mat4<T>&)>(::operator*)),

            "make_scale_matrix4", sol::overload(
                sol::resolve<mat4<T>(T,T,T)>(&math::make_scale_matrix4),
                sol::resolve<mat4<T>(const vec4<T>&)>(&math::make_scale_matrix4),
                sol::resolve<mat4<T>(const vec3<T>&)>(&math::make_scale_matrix4),
                sol::resolve<mat4<T>(const vec2<T>&, T)>(&math::make_scale_matrix4)),

            "make_translation_matrix4", sol::overload(
                sol::resolve<mat4<T>(T,T,T)>(&math::make_translation_matrix4),
                sol::resolve<mat4<T>(const vec4<T>&)>(&math::make_translation_matrix4),
                sol::resolve<mat4<T>(const vec3<T>&)>(&math::make_translation_matrix4),
                sol::resolve<mat4<T>(const vec2<T>&, T)>(&math::make_translation_matrix4)),

            "make_rotation_matrix4", sol::overload(
                sol::resolve<mat4<T>(const deg<T>&,T,T,T)>(&math::make_rotation_matrix4),
                sol::resolve<mat4<T>(const deg<T>&,const vec4<T>&)>(&math::make_rotation_matrix4),
                sol::resolve<mat4<T>(const deg<T>&,const vec3<T>&)>(&math::make_rotation_matrix4),
                sol::resolve<mat4<T>(const deg<T>&,const vec2<T>&,T)>(&math::make_rotation_matrix4),
                sol::resolve<mat4<T>(const rad<T>&,T,T,T)>(&math::make_rotation_matrix4),
                sol::resolve<mat4<T>(const rad<T>&,const vec4<T>&)>(&math::make_rotation_matrix4),
                sol::resolve<mat4<T>(const rad<T>&,const vec3<T>&)>(&math::make_rotation_matrix4),
                sol::resolve<mat4<T>(const rad<T>&,const vec2<T>&,T)>(&math::make_rotation_matrix4),
                sol::resolve<mat4<T>(const quat<T>&)>(&math::make_rotation_matrix4)),

            "approximately", [](const mat4<T>& l, const mat4<T>& r){ return math::approximately(l,r); },

            "inversed", [](const mat4<T>& m){ return math::inversed(m); },
            "transposed", [](const mat4<T>& m){ return math::transposed(m); },

            "contains_nan", sol::resolve<bool(const mat4<T>&)>(&math::contains_nan));
    }
}

namespace e2d::bindings::math
{
    void bind_mat4(sol::state& l) {
        bind_mat4_t<f32>("m4f", l);
    }
}