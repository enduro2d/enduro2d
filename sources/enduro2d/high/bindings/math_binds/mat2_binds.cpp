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
    void bind_mat2_t(const str& name, sol::state& l) {
        l.new_usertype<mat2<T>>(name,
            sol::constructors<
                mat2<T>(),
                mat2<T>(const vec2<T>&, const vec2<T>&)
            >(),

            "zero", &mat2<T>::zero,
            "identity", &mat2<T>::identity,

            "rows", &mat2<T>::rows,

            sol::meta_function::to_string, [](const mat2<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const mat2<T>&, const mat2<T>&)>(::operator==),
            sol::meta_function::unary_minus, sol::resolve<mat2<T>(const mat2<T>&)>(::operator-),

            sol::meta_function::addition, sol::overload(
                sol::resolve<mat2<T>(T, const mat2<T>&)>(::operator+),
                sol::resolve<mat2<T>(const mat2<T>&, T)>(::operator+),
                sol::resolve<mat2<T>(const mat2<T>&, const mat2<T>&)>(::operator+)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<mat2<T>(T, const mat2<T>&)>(::operator*),
                sol::resolve<mat2<T>(const mat2<T>&, T)>(::operator*),
                sol::resolve<mat2<T>(const mat2<T>&, const mat2<T>&)>(::operator*)),

            "make_scale", sol::overload(
                sol::resolve<mat2<T>(T,T)>(&math::make_scale_matrix2),
                sol::resolve<mat2<T>(const vec4<T>&)>(&math::make_scale_matrix2),
                sol::resolve<mat2<T>(const vec3<T>&)>(&math::make_scale_matrix2),
                sol::resolve<mat2<T>(const vec2<T>&)>(&math::make_scale_matrix2)),

            "make_rotation", sol::overload(
                sol::resolve<mat2<T>(const deg<T>&)>(&math::make_rotation_matrix2),
                sol::resolve<mat2<T>(const rad<T>&)>(&math::make_rotation_matrix2)),

            "approximately", [](const mat2<T>& l, const mat2<T>& r){ return math::approximately(l,r); },

            "inversed", [](const mat2<T>& m){ return math::inversed(m); },
            "transposed", [](const mat2<T>& m){ return math::transposed(m); },

            "contains_nan", sol::resolve<bool(const mat2<T>&)>(&math::contains_nan));
    }
}

namespace e2d::bindings::math
{
    void bind_mat2(sol::state& l) {
        bind_mat2_t<f32>("m2f", l);
    }
}
