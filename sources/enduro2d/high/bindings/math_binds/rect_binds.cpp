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
    void bind_rect_t(const str& name, sol::state& l) {
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

            sol::meta_function::to_string, [](const rect<T>& v){
                return strings::rformat("%0", v);
            },

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

            "make_minmax", sol::overload(
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
            "point_to_normalized", sol::resolve<vec2<T>(const rect<T>&,const vec2<T>&)>(&math::point_to_normalized));
    }
}

namespace e2d::bindings::math
{
    void bind_rect(sol::state& l) {
        bind_rect_t<f32>("b2f", l);
    }
}
