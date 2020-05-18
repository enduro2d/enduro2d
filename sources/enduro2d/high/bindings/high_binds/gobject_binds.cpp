/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/behaviour.hpp>
#include <enduro2d/high/components/button.hpp>
#include <enduro2d/high/components/camera.hpp>
#include <enduro2d/high/components/colliders.hpp>
#include <enduro2d/high/components/flipbook_player.hpp>
#include <enduro2d/high/components/label.hpp>
#include <enduro2d/high/components/model_renderer.hpp>
#include <enduro2d/high/components/named.hpp>
#include <enduro2d/high/components/progress_handle.hpp>
#include <enduro2d/high/components/progress.hpp>
#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/scene.hpp>
#include <enduro2d/high/components/scroll.hpp>
#include <enduro2d/high/components/scrollbar_handle.hpp>
#include <enduro2d/high/components/scrollbar.hpp>
#include <enduro2d/high/components/slider_handle.hpp>
#include <enduro2d/high/components/slider.hpp>
#include <enduro2d/high/components/spine_player.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>
#include <enduro2d/high/components/toggle_group.hpp>
#include <enduro2d/high/components/toggle.hpp>
#include <enduro2d/high/components/touchable.hpp>
#include <enduro2d/high/components/widget.hpp>

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
            "button", sol::property([](gobject& go){ return component_wrapper<button>{go}; }),
            "camera", sol::property([](gobject& go){ return component_wrapper<camera>{go}; }),
            "circle_collider", sol::property([](gobject& go){ return component_wrapper<circle_collider>{go}; }),
            "flipbook_player", sol::property([](gobject& go){ return component_wrapper<flipbook_player>{go}; }),
            "label", sol::property([](gobject& go){ return component_wrapper<label>{go}; }),
            "model_renderer", sol::property([](gobject& go){ return component_wrapper<model_renderer>{go}; }),
            "named", sol::property([](gobject& go){ return component_wrapper<named>{go}; }),
            "polygon_collider", sol::property([](gobject& go){ return component_wrapper<polygon_collider>{go}; }),
            "progress_handle", sol::property([](gobject& go){ return component_wrapper<progress_handle>{go}; }),
            "progress", sol::property([](gobject& go){ return component_wrapper<progress>{go}; }),
            "rect_collider", sol::property([](gobject& go){ return component_wrapper<rect_collider>{go}; }),
            "renderer", sol::property([](gobject& go){ return component_wrapper<renderer>{go}; }),
            "scene", sol::property([](gobject& go){ return component_wrapper<scene>{go}; }),
            "scroll", sol::property([](gobject& go){ return component_wrapper<scroll>{go}; }),
            "scrollbar_handle", sol::property([](gobject& go){ return component_wrapper<scrollbar_handle>{go}; }),
            "scrollbar", sol::property([](gobject& go){ return component_wrapper<scrollbar>{go}; }),
            "slider_handle", sol::property([](gobject& go){ return component_wrapper<slider_handle>{go}; }),
            "slider", sol::property([](gobject& go){ return component_wrapper<slider>{go}; }),
            "spine_player", sol::property([](gobject& go){ return component_wrapper<spine_player>{go}; }),
            "sprite_renderer", sol::property([](gobject& go){ return component_wrapper<sprite_renderer>{go}; }),
            "toggle_group", sol::property([](gobject& go){ return component_wrapper<toggle_group>{go}; }),
            "toggle", sol::property([](gobject& go){ return component_wrapper<toggle>{go}; }),
            "touchable", sol::property([](gobject& go){ return component_wrapper<touchable>{go}; }),
            "widget", sol::property([](gobject& go){ return component_wrapper<widget>{go}; })
        );
    }
}
