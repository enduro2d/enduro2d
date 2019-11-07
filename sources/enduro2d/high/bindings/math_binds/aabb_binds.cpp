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
    void bind_aabb_t(const str& name, sol::state& l) {
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

            sol::meta_function::to_string, [](const aabb<T>& v){
                return strings::rformat("%0", v);
            },

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
}

namespace e2d::bindings::math
{
    void bind_aabb(sol::state& l) {
        bind_aabb_t<f32>("b3f", l);
    }
}
