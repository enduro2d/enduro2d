/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/behaviour.hpp>
#include <enduro2d/high/components/camera.hpp>
#include <enduro2d/high/components/colliders.hpp>
#include <enduro2d/high/components/flipbook_player.hpp>
#include <enduro2d/high/components/label.hpp>
#include <enduro2d/high/components/model_renderer.hpp>
#include <enduro2d/high/components/named.hpp>
#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/scene.hpp>
#include <enduro2d/high/components/spine_player.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>

namespace e2d::bindings::high
{
    void bind_gobject(sol::state& l) {
        l.new_usertype<gobject>("gobject",
            sol::no_constructor,

            "alive", sol::property([](const gobject& go) -> bool {
                return go.alive();
            }),

            "valid", sol::property([](const gobject& go) -> bool {
                return go.valid();
            }),

            "destroy", [](gobject& go){
                go.destroy();
            },

            "actor", sol::property([](gobject& go){ return component_wrapper<actor>{go}; }),
            "behaviour", sol::property([](gobject& go){ return component_wrapper<behaviour>{go}; }),
            "camera", sol::property([](gobject& go){ return component_wrapper<camera>{go}; }),
            "rect_collider", sol::property([](gobject& go){ return component_wrapper<rect_collider>{go}; }),
            "circle_collider", sol::property([](gobject& go){ return component_wrapper<circle_collider>{go}; }),
            "polygon_collider", sol::property([](gobject& go){ return component_wrapper<polygon_collider>{go}; }),
            "flipbook_player", sol::property([](gobject& go){ return component_wrapper<flipbook_player>{go}; }),
            "label", sol::property([](gobject& go){ return component_wrapper<label>{go}; }),
            "model_renderer", sol::property([](gobject& go){ return component_wrapper<model_renderer>{go}; }),
            "named", sol::property([](gobject& go){ return component_wrapper<named>{go}; }),
            "renderer", sol::property([](gobject& go){ return component_wrapper<renderer>{go}; }),
            "scene", sol::property([](gobject& go){ return component_wrapper<scene>{go}; }),
            "spine_player", sol::property([](gobject& go){ return component_wrapper<spine_player>{go}; }),
            "sprite_renderer", sol::property([](gobject& go){ return component_wrapper<sprite_renderer>{go}; })
        );
    }
}
