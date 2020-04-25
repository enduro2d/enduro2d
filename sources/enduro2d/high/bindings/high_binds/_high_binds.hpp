/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../bindings.hpp"

namespace e2d::bindings::high
{
    void bind_editor(sol::state& l);
    void bind_library(sol::state& l);
    void bind_luasol(sol::state& l);
    void bind_world(sol::state& l);

    void bind_node(sol::state& l);
    void bind_gobject(sol::state& l);

    void bind_actor(sol::state& l);
    void bind_behaviour(sol::state& l);
    void bind_button(sol::state& l);
    void bind_camera(sol::state& l);
    void bind_colliders(sol::state& l);
    void bind_flipbook_player(sol::state& l);
    void bind_handle(sol::state& l);
    void bind_label(sol::state& l);
    void bind_layout(sol::state& l);
    void bind_model_renderer(sol::state& l);
    void bind_named(sol::state& l);
    void bind_renderer(sol::state& l);
    void bind_scene(sol::state& l);
    void bind_slider(sol::state& l);
    void bind_spine_player(sol::state& l);
    void bind_sprite_renderer(sol::state& l);
    void bind_toggle_group(sol::state& l);
    void bind_toggle(sol::state& l);
    void bind_touchable(sol::state& l);
    void bind_widget(sol::state& l);
    void bind_wsprite(sol::state& l);
}

namespace e2d::bindings
{
    inline void bind_high(sol::state& l) {
        high::bind_editor(l);
        high::bind_library(l);
        high::bind_luasol(l);
        high::bind_world(l);

        high::bind_node(l);
        high::bind_gobject(l);

        high::bind_actor(l);
        high::bind_behaviour(l);
        high::bind_button(l);
        high::bind_camera(l);
        high::bind_colliders(l);
        high::bind_flipbook_player(l);
        high::bind_handle(l);
        high::bind_label(l);
        high::bind_layout(l);
        high::bind_model_renderer(l);
        high::bind_named(l);
        high::bind_renderer(l);
        high::bind_scene(l);
        high::bind_slider(l);
        high::bind_spine_player(l);
        high::bind_sprite_renderer(l);
        high::bind_toggle_group(l);
        high::bind_toggle(l);
        high::bind_touchable(l);
        high::bind_widget(l);
        high::bind_wsprite(l);
    }
}
