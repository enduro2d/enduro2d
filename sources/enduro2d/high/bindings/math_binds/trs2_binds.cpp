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
    void bind_trs2_t(const str& name, sol::state& l) {
        l.new_usertype<trs2<T>>(name,
            sol::constructors<
                trs2<T>(),
                trs2<T>(trs2<T>),
                trs2<T>(vec2<T>,deg<T>,vec2<T>),
                trs2<T>(vec2<T>,rad<T>,vec2<T>)>(),

            "zero", &trs2<T>::zero,
            "identity", &trs2<T>::identity,

            "translation", &trs2<T>::translation,
            "rotation", &trs2<T>::rotation,
            "scale", &trs2<T>::scale,

            sol::meta_function::to_string, [](const trs2<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const trs2<T>&, const trs2<T>&)>(::operator==),

            "make_translation", sol::resolve<trs2<T>(const vec2<T>&)>(&math::make_translation_trs2),
            "make_rotation", sol::overload(
                sol::resolve<trs2<T>(const deg<T>&)>(&math::make_rotation_trs2),
                sol::resolve<trs2<T>(const rad<T>&)>(&math::make_rotation_trs2)),
            "make_scale", sol::resolve<trs2<T>(const vec2<T>&)>(&math::make_scale_trs2),

            "approximately", [](const trs2<T>& l, const trs2<T>& r){ return math::approximately(l,r); },
            "contains_nan", sol::resolve<bool(const trs2<T>&)>(&math::contains_nan));
    }
}

namespace e2d::bindings::math
{
    void bind_trs2(sol::state& l) {
        bind_trs2_t<f32>("t2f", l);
    }
}
