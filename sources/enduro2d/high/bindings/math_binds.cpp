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

            "zero", &vec2<T>::zero,
            "unit", &vec2<T>::unit,
            "unit_x", &vec2<T>::unit_x,
            "unit_y", &vec2<T>::unit_y,

            "x", &vec2<T>::x,
            "y", &vec2<T>::y,

            sol::meta_function::equal_to, sol::resolve<bool(const vec2<T>&, const vec2<T>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const vec2<T>&, const vec2<T>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const vec2<T>&, const vec2<T>&)>(::operator<=),

            sol::meta_function::unary_minus, sol::resolve<vec2<T>(const vec2<T>&)>(::operator-),

            sol::meta_function::addition, sol::overload(
                sol::resolve<vec2<T>(T, const vec2<T>&)>(::operator+),
                sol::resolve<vec2<T>(const vec2<T>&, T)>(::operator+),
                sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(::operator+)),

            sol::meta_function::subtraction, sol::overload(
                sol::resolve<vec2<T>(T, const vec2<T>&)>(::operator-),
                sol::resolve<vec2<T>(const vec2<T>&, T)>(::operator-),
                sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(::operator-)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<vec2<T>(T, const vec2<T>&)>(::operator*),
                sol::resolve<vec2<T>(const vec2<T>&, T)>(::operator*),
                sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(::operator*),
                sol::resolve<vec2<T>(const vec2<T>&, const mat2<T>&)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<vec2<T>(T, const vec2<T>&)>(::operator/),
                sol::resolve<vec2<T>(const vec2<T>&, T)>(::operator/),
                sol::resolve<vec2<T>(const vec2<T>&, const vec2<T>&)>(::operator/)),

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

            "zero", &vec3<T>::zero,
            "unit", &vec3<T>::unit,
            "unit_x", &vec3<T>::unit_x,
            "unit_y", &vec3<T>::unit_y,
            "unit_z", &vec3<T>::unit_z,

            "x", &vec3<T>::x,
            "y", &vec3<T>::y,
            "z", &vec3<T>::z,

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
                sol::resolve<vec3<T>(const vec3<T>&, const mat3<T>&)>(::operator*),
                sol::resolve<vec3<T>(const vec3<T>&, const quat<T>&)>(::operator*)),

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

            "lerp", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, T)>(&math::lerp),
            "lerp", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, const vec4<T>&)>(&math::lerp),
            "inverse_lerp", sol::resolve<vec4<T>(const vec4<T>&, const vec4<T>&, const vec4<T>&)>(&math::inverse_lerp),

            "contains_nan", sol::resolve<bool(const vec4<T>&)>(&math::contains_nan));
    }

    template < typename T >
    void bind_quat(const str& name, sol::state& l) {
        l.new_usertype<quat<T>>(name,
            sol::constructors<
                quat<T>(),
                quat<T>(quat<T>),
                quat<T>(T,T,T,T),
                quat<T>(vec4<T>)>(),

            "x", &quat<T>::x,
            "y", &quat<T>::y,
            "z", &quat<T>::z,
            "w", &quat<T>::w,

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

            "make_quat_from_axis_angle", sol::overload(
                sol::resolve<quat<T>(const deg<T>&, const vec3<T>&)>(&math::make_quat_from_axis_angle),
                sol::resolve<quat<T>(const rad<T>&, const vec3<T>&)>(&math::make_quat_from_axis_angle)),

            "make_quat_from_euler_angles", sol::overload(
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

    template < typename T, typename Tag >
    void bind_unit(const str& name, sol::state& l) {
        l.new_usertype<unit<T,Tag>>(name,
            sol::constructors<
                unit<T,Tag>(),
                unit<T,Tag>(T),
                unit<T,Tag>(unit<T,Tag>)>(),

            "value", &unit<T,Tag>::value,

            sol::meta_function::equal_to, sol::resolve<bool(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator<=),

            sol::meta_function::unary_minus, sol::resolve<unit<T,Tag>(const unit<T,Tag>&)>(::operator-),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<unit<T,Tag>(T, const unit<T,Tag>&)>(::operator*),
                sol::resolve<unit<T,Tag>(const unit<T,Tag>&, T)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<unit<T,Tag>(T, const unit<T,Tag>&)>(::operator/),
                sol::resolve<unit<T,Tag>(const unit<T,Tag>&, T)>(::operator/)),

            sol::meta_function::addition, sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator+),
            sol::meta_function::subtraction, sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator-),

            "approximately", [](const unit<T,Tag>& l, const unit<T,Tag>& r){ return math::approximately(l,r); },

            "minimized", sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&)>(&math::minimized),
            "maximized", sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&)>(&math::maximized),
            "clamped", sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&, const unit<T,Tag>&)>(&math::clamped),
            "saturated", sol::resolve<unit<T,Tag>(const unit<T,Tag>&)>(&math::saturated),

            "contains_nan", sol::resolve<bool(const unit<T,Tag>&)>(&math::contains_nan));
    }

    template < typename T >
    void bind_mat2(const str& name, sol::state& l) {
        l.new_usertype<mat2<T>>(name,
            sol::constructors<
                mat2<T>(),
                mat2<T>(const vec2<T>&, const vec2<T>&)
            >(),

            "zero", &mat2<T>::zero,
            "identity", &mat2<T>::identity,

            "rows", &mat2<T>::rows,

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

            "make_scale_matrix2", sol::overload(
                sol::resolve<mat2<T>(T,T)>(&math::make_scale_matrix2),
                sol::resolve<mat2<T>(const vec4<T>&)>(&math::make_scale_matrix2),
                sol::resolve<mat2<T>(const vec3<T>&)>(&math::make_scale_matrix2),
                sol::resolve<mat2<T>(const vec2<T>&)>(&math::make_scale_matrix2)),

            "make_rotation_matrix2", sol::overload(
                sol::resolve<mat2<T>(const deg<T>&)>(&math::make_rotation_matrix2),
                sol::resolve<mat2<T>(const rad<T>&)>(&math::make_rotation_matrix2)),

            "approximately", [](const mat2<T>& l, const mat2<T>& r){ return math::approximately(l,r); },

            "inversed", [](const mat2<T>& m){ return math::inversed(m); },
            "transposed", [](const mat2<T>& m){ return math::transposed(m); },

            "contains_nan", sol::resolve<bool(const mat2<T>&)>(&math::contains_nan));
    }

    template < typename T >
    void bind_mat3(const str& name, sol::state& l) {
        l.new_usertype<mat3<T>>(name,
            sol::constructors<
                mat3<T>(),
                mat3<T>(const vec3<T>&, const vec3<T>&, const vec3<T>&)
            >(),

            "zero", &mat3<T>::zero,
            "identity", &mat3<T>::identity,

            "rows", &mat3<T>::rows,

            sol::meta_function::equal_to, sol::resolve<bool(const mat3<T>&, const mat3<T>&)>(::operator==),
            sol::meta_function::unary_minus, sol::resolve<mat3<T>(const mat3<T>&)>(::operator-),

            sol::meta_function::addition, sol::overload(
                sol::resolve<mat3<T>(T, const mat3<T>&)>(::operator+),
                sol::resolve<mat3<T>(const mat3<T>&, T)>(::operator+),
                sol::resolve<mat3<T>(const mat3<T>&, const mat3<T>&)>(::operator+)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<mat3<T>(T, const mat3<T>&)>(::operator*),
                sol::resolve<mat3<T>(const mat3<T>&, T)>(::operator*),
                sol::resolve<mat3<T>(const mat3<T>&, const mat3<T>&)>(::operator*)),

            "make_scale_matrix3", sol::overload(
                sol::resolve<mat3<T>(T,T,T)>(&math::make_scale_matrix3),
                sol::resolve<mat3<T>(const vec4<T>&)>(&math::make_scale_matrix3),
                sol::resolve<mat3<T>(const vec3<T>&)>(&math::make_scale_matrix3),
                sol::resolve<mat3<T>(const vec2<T>&, T)>(&math::make_scale_matrix3)),

            "make_rotation_matrix3", sol::overload(
                sol::resolve<mat3<T>(const deg<T>&,T,T,T)>(&math::make_rotation_matrix3),
                sol::resolve<mat3<T>(const deg<T>&,const vec4<T>&)>(&math::make_rotation_matrix3),
                sol::resolve<mat3<T>(const deg<T>&,const vec3<T>&)>(&math::make_rotation_matrix3),
                sol::resolve<mat3<T>(const deg<T>&,const vec2<T>&,T)>(&math::make_rotation_matrix3),
                sol::resolve<mat3<T>(const rad<T>&,T,T,T)>(&math::make_rotation_matrix3),
                sol::resolve<mat3<T>(const rad<T>&,const vec4<T>&)>(&math::make_rotation_matrix3),
                sol::resolve<mat3<T>(const rad<T>&,const vec3<T>&)>(&math::make_rotation_matrix3),
                sol::resolve<mat3<T>(const rad<T>&,const vec2<T>&,T)>(&math::make_rotation_matrix3),
                sol::resolve<mat3<T>(const quat<T>&)>(&math::make_rotation_matrix3)),

            "approximately", [](const mat3<T>& l, const mat3<T>& r){ return math::approximately(l,r); },

            "inversed", [](const mat3<T>& m){ return math::inversed(m); },
            "transposed", [](const mat3<T>& m){ return math::transposed(m); },

            "contains_nan", sol::resolve<bool(const mat3<T>&)>(&math::contains_nan));
    }

    template < typename T >
    void bind_mat4(const str& name, sol::state& l) {
        l.new_usertype<mat4<T>>(name,
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

    template < typename T >
    void bind_rect(const str& name, sol::state& l) {
        l.new_usertype<rect<T>>(name,
            sol::constructors<
                rect<T>(),
                rect<T>(rect<T>),
                rect<T>(T,T),
                rect<T>(T,T,T,T),
                rect<T>(vec2<T>),
                rect<T>(vec2<T>,vec2<T>)>(),

            "zero", &rect<T>::zero,
            "unit", &rect<T>::unit,

            "position", &rect<T>::position,
            "size", &rect<T>::size,

            sol::meta_function::equal_to, sol::resolve<bool(const rect<T>&, const rect<T>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const rect<T>&, const rect<T>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const rect<T>&, const rect<T>&)>(::operator<=),

            sol::meta_function::addition, sol::overload(
                sol::resolve<rect<T>(const rect<T>&, T)>(::operator+),
                sol::resolve<rect<T>(const rect<T>&, const vec2<T>&)>(::operator+)),

            sol::meta_function::subtraction, sol::overload(
                sol::resolve<rect<T>(const rect<T>&, T)>(::operator-),
                sol::resolve<rect<T>(const rect<T>&, const vec2<T>&)>(::operator-)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<rect<T>(const rect<T>&, T)>(::operator*),
                sol::resolve<rect<T>(const rect<T>&, const vec2<T>&)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<rect<T>(const rect<T>&, T)>(::operator/),
                sol::resolve<rect<T>(const rect<T>&, const vec2<T>&)>(::operator/)),

            "make_minmax_rect", sol::overload(
                sol::resolve<rect<T>(T,T,T,T)>(&math::make_minmax_rect),
                sol::resolve<rect<T>(const vec2<T>&,const vec2<T>&)>(&math::make_minmax_rect),
                sol::resolve<rect<T>(const rect<T>&)>(&math::make_minmax_rect)),

            "approximately", [](const rect<T>& l, const rect<T>& r){ return math::approximately(l,r); },

            "minimum", sol::resolve<vec2<T>(const rect<T>&)>(&math::minimum),
            "maximum", sol::resolve<vec2<T>(const rect<T>&)>(&math::maximum),

            "area", sol::resolve<T(const rect<T>&)>(&math::area),
            "abs_area", sol::resolve<T(const rect<T>&)>(&math::abs_area),

            "merged", sol::resolve<rect<T>(const rect<T>&,const rect<T>&)>(&math::merged),
            "inside", sol::resolve<bool(const rect<T>&,const vec2<T>&)>(&math::inside),
            "overlaps", sol::resolve<bool(const rect<T>&,const rect<T>&)>(&math::overlaps),

            "normalized_to_point", sol::resolve<vec2<T>(const rect<T>&,const vec2<T>&)>(&math::normalized_to_point),
            "point_to_normalized", sol::resolve<vec2<T>(const rect<T>&,const vec2<T>&)>(&math::point_to_normalized),

            "contains_nan", sol::resolve<bool(const rect<T>&)>(&math::contains_nan));
    }

    template < typename T >
    void bind_aabb(const str& name, sol::state& l) {
        l.new_usertype<aabb<T>>(name,
            sol::constructors<
                aabb<T>(),
                aabb<T>(aabb<T>),
                aabb<T>(T,T,T),
                aabb<T>(T,T,T,T,T,T),
                aabb<T>(vec3<T>),
                aabb<T>(vec3<T>,vec3<T>)>(),

            "zero", &aabb<T>::zero,
            "unit", &aabb<T>::unit,

            "position", &aabb<T>::position,
            "size", &aabb<T>::size,

            sol::meta_function::equal_to, sol::resolve<bool(const aabb<T>&, const aabb<T>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const aabb<T>&, const aabb<T>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const aabb<T>&, const aabb<T>&)>(::operator<=),

            sol::meta_function::addition, sol::overload(
                sol::resolve<aabb<T>(const aabb<T>&, T)>(::operator+),
                sol::resolve<aabb<T>(const aabb<T>&, const vec3<T>&)>(::operator+)),

            sol::meta_function::subtraction, sol::overload(
                sol::resolve<aabb<T>(const aabb<T>&, T)>(::operator-),
                sol::resolve<aabb<T>(const aabb<T>&, const vec3<T>&)>(::operator-)),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<aabb<T>(const aabb<T>&, T)>(::operator*),
                sol::resolve<aabb<T>(const aabb<T>&, const vec3<T>&)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<aabb<T>(const aabb<T>&, T)>(::operator/),
                sol::resolve<aabb<T>(const aabb<T>&, const vec3<T>&)>(::operator/)),

            "make_minmax_rect", sol::overload(
                sol::resolve<aabb<T>(T,T,T,T,T,T)>(&math::make_minmax_aabb),
                sol::resolve<aabb<T>(const vec3<T>&,const vec3<T>&)>(&math::make_minmax_aabb),
                sol::resolve<aabb<T>(const aabb<T>&)>(&math::make_minmax_aabb)),

            "approximately", [](const aabb<T>& l, const aabb<T>& r){ return math::approximately(l,r); },

            "minimum", sol::resolve<vec3<T>(const aabb<T>&)>(&math::minimum),
            "maximum", sol::resolve<vec3<T>(const aabb<T>&)>(&math::maximum),

            "volume", sol::resolve<T(const aabb<T>&)>(&math::volume),
            "abs_volume", sol::resolve<T(const aabb<T>&)>(&math::abs_volume),

            "merged", sol::resolve<aabb<T>(const aabb<T>&,const aabb<T>&)>(&math::merged),
            "inside", sol::resolve<bool(const aabb<T>&,const vec3<T>&)>(&math::inside),
            "overlaps", sol::resolve<bool(const aabb<T>&,const aabb<T>&)>(&math::overlaps),

            "normalized_to_point", sol::resolve<vec3<T>(const aabb<T>&,const vec3<T>&)>(&math::normalized_to_point),
            "point_to_normalized", sol::resolve<vec3<T>(const aabb<T>&,const vec3<T>&)>(&math::point_to_normalized),

            "contains_nan", sol::resolve<bool(const aabb<T>&)>(&math::contains_nan));
    }

    template < typename T >
    void bind_trs2(const str& name, sol::state& l) {
        l.new_usertype<trs2<T>>(name,
            sol::constructors<
                trs2<T>(),
                trs2<T>(trs2<T>),
                trs2<T>(vec2<T>,deg<T>,vec2<T>),
                trs2<T>(vec2<T>,rad<T>,vec2<T>)>(),

            "zero", &trs2<T>::zero,
            "unit", &trs2<T>::identity,

            "translation", &trs2<T>::translation,
            "rotation", &trs2<T>::rotation,
            "scale", &trs2<T>::scale,

            sol::meta_function::equal_to, sol::resolve<bool(const trs2<T>&, const trs2<T>&)>(::operator==),

            "make_translation_trs2", sol::resolve<trs2<T>(const vec2<T>&)>(&math::make_translation_trs2),
            "make_rotation_trs2", sol::overload(
                sol::resolve<trs2<T>(const deg<T>&)>(&math::make_rotation_trs2),
                sol::resolve<trs2<T>(const rad<T>&)>(&math::make_rotation_trs2)),
            "make_scale_trs2", sol::resolve<trs2<T>(const vec2<T>&)>(&math::make_scale_trs2),

            "approximately", [](const trs2<T>& l, const trs2<T>& r){ return math::approximately(l,r); },
            "contains_nan", sol::resolve<bool(const trs2<T>&)>(&math::contains_nan));
    }

    template < typename T >
    void bind_trs3(const str& name, sol::state& l) {
        l.new_usertype<trs3<T>>(name,
            sol::constructors<
                trs3<T>(),
                trs3<T>(trs3<T>),
                trs3<T>(vec3<T>,quat<T>,vec3<T>)>(),

            "zero", &trs3<T>::zero,
            "unit", &trs3<T>::identity,

            "translation", &trs3<T>::translation,
            "rotation", &trs3<T>::rotation,
            "scale", &trs3<T>::scale,

            sol::meta_function::equal_to, sol::resolve<bool(const trs3<T>&, const trs3<T>&)>(::operator==),

            "make_translation_trs3", sol::resolve<trs3<T>(const vec3<T>&)>(&math::make_translation_trs3),
            "make_rotation_trs3", sol::resolve<trs3<T>(const quat<T>&)>(&math::make_rotation_trs3),
            "make_scale_trs3", sol::resolve<trs3<T>(const vec3<T>&)>(&math::make_scale_trs3),

            "approximately", [](const trs3<T>& l, const trs3<T>& r){ return math::approximately(l,r); },
            "contains_nan", sol::resolve<bool(const trs3<T>&)>(&math::contains_nan));
    }
}

namespace e2d::bindings
{
    void bind_math(sol::state& l) {
        bind_vec2<f32>("v2f", l);
        bind_vec3<f32>("v3f", l);
        bind_vec4<f32>("v4f", l);
        bind_quat<f32>("q4f", l);
        bind_unit<f32, deg_tag>("degf", l);
        bind_unit<f32, rad_tag>("radf", l);
        bind_mat2<f32>("m2f", l);
        bind_mat3<f32>("m3f", l);
        bind_mat4<f32>("m4f", l);
        bind_rect<f32>("b2f", l);
        bind_aabb<f32>("b3f", l);
        bind_trs2<f32>("t2f", l);
        bind_trs3<f32>("t3f", l);
    }
}
