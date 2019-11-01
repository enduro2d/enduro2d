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
        l["e2d"].get_or_create<sol::table>()
        .new_usertype<trs3<T>>(name,
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

namespace e2d::bindings::math
{
    void bind_trs3(sol::state& l) {
        bind_trs3_t<f32>("t3f", l);
    }
}
