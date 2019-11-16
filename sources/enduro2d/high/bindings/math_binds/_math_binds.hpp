/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../bindings.hpp"

namespace e2d::bindings::math
{
    void bind_vec2(sol::state& l);
    void bind_vec3(sol::state& l);
    void bind_vec4(sol::state& l);

    void bind_mat2(sol::state& l);
    void bind_mat3(sol::state& l);
    void bind_mat4(sol::state& l);

    void bind_quat(sol::state& l);

    void bind_rect(sol::state& l);
    void bind_aabb(sol::state& l);

    void bind_trs2(sol::state& l);
    void bind_trs3(sol::state& l);
}

namespace e2d::bindings
{
    inline void bind_math(sol::state& l) {
        math::bind_vec2(l);
        math::bind_vec3(l);
        math::bind_vec4(l);

        math::bind_mat2(l);
        math::bind_mat3(l);
        math::bind_mat4(l);

        math::bind_quat(l);

        math::bind_rect(l);
        math::bind_aabb(l);

        math::bind_trs2(l);
        math::bind_trs3(l);
    }
}
