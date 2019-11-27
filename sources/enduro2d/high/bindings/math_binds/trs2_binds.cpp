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
            sol::no_constructor,

            "zero", &trs2<T>::zero,
            "identity", &trs2<T>::identity,

            "translation", &trs2<T>::translation,
            "rotation", sol::property(
                [](const trs2<T>& t){ return t.rotation.value; },
                [](trs2<T>& t, T v){ t.rotation.value = v; }),
            "scale", &trs2<T>::scale,

            sol::meta_function::to_string, [](const trs2<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const trs2<T>&, const trs2<T>&)>(::operator==),

            "make_translation", [](const vec2<T>& t) -> trs2<T> {
                return math::make_translation_trs2(t);
            },

            "make_rotation", [](T angle) -> trs2<T> {
                return math::make_rotation_trs2(make_rad(angle));
            },

            "make_scale", [](const vec2<T>& s) -> trs2<T> {
                return math::make_scale_trs2(s);
            },

            "approximately", [](const trs2<T>& l, const trs2<T>& r) -> bool {
                return math::approximately(l,r);
            },

            "contains_nan", [](const trs2<T>& t) -> bool {
                return math::contains_nan(t);
            }
        );
    }
}

namespace e2d::bindings::math
{
    void bind_trs2(sol::state& l) {
        bind_trs2_t<f32>("t2f", l);
    }
}
