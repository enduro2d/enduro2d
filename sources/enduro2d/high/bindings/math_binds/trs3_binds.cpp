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
    void bind_trs3_t(const str& name, sol::state& l) {
        l.new_usertype<trs3<T>>(name,
            sol::no_constructor,

            "zero", &trs3<T>::zero,
            "identity", &trs3<T>::identity,

            "translation", &trs3<T>::translation,
            "rotation", &trs3<T>::rotation,
            "scale", &trs3<T>::scale,

            sol::meta_function::to_string, [](const trs3<T>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const trs3<T>&, const trs3<T>&)>(::operator==),

            "make_translation", [](const vec3<T>& t) -> trs3<T> {
                return math::make_translation_trs3(t);
            },

            "make_rotation", [](const quat<T>& q) -> trs3<T> {
                return math::make_rotation_trs3(q);
            },

            "make_scale", [](const vec3<T>& s) -> trs3<T> {
                return math::make_scale_trs3(s);
            },

            "approximately", [](const trs3<T>& l, const trs3<T>& r) -> bool {
                return math::approximately(l,r);
            },

            "contains_nan", [](const trs3<T>& t) -> bool {
                return math::contains_nan(t);
            }
        );
    }
}

namespace e2d::bindings::math
{
    void bind_trs3(sol::state& l) {
        bind_trs3_t<f32>("t3f", l);
    }
}
