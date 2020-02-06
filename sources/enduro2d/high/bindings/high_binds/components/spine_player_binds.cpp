/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/commands.hpp>
#include <enduro2d/high/components/spine_player.hpp>

namespace e2d::bindings::high
{
    void bind_spine_player(sol::state& l) {
        l.new_usertype<gcomponent<spine_player>>("spine_player",
            sol::no_constructor,

            "spine", sol::property(
                [](const gcomponent<spine_player>& c) -> spine_asset::ptr {
                    return c->spine();
                },
                [](gcomponent<spine_player>& c, const spine_asset::ptr& v){
                    c->spine(v);
                }),

            "skin", [](gcomponent<spine_player>& c, str_view name) -> bool {
                return c->skin(name);
            },

            "attachment", [](gcomponent<spine_player>& c, str_view slot, str_view name) -> bool {
                return c->attachment(slot, name);
            },

            "has_skin", [](const gcomponent<spine_player>& c, str_view name){
                return c->has_skin(name);
            },

            "has_animation", [](const gcomponent<spine_player>& c, str_view name){
                return c->has_animation(name);
            },

            "add_command", [](
                gcomponent<spine_player>& c,
                spine_player_commands::command cmd)
            {
                c.owner()
                .component<commands<spine_player_commands::command>>()
                .ensure()
                .add(std::move(cmd));
            }
        );

        //
        // events
        //

        l["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_events::custom_evt>("custom_evt",
            sol::constructors<
                spine_player_events::custom_evt(),
                spine_player_events::custom_evt(str)>(),

            "name", sol::property(
                [](const spine_player_events::custom_evt& c) -> str {
                    return c.name();
                },
                sol::resolve<spine_player_events::custom_evt&(str)>(
                    &spine_player_events::custom_evt::name)),

            "int_value", sol::property(
                [](const spine_player_events::custom_evt& c) -> i32 {
                    return c.int_value();
                },
                sol::resolve<spine_player_events::custom_evt&(i32)>(
                    &spine_player_events::custom_evt::int_value)),

            "float_value", sol::property(
                [](const spine_player_events::custom_evt& c) -> f32 {
                    return c.float_value();
                },
                sol::resolve<spine_player_events::custom_evt&(f32)>(
                    &spine_player_events::custom_evt::float_value)),

            "string_value", sol::property(
                [](const spine_player_events::custom_evt& c) -> str {
                    return c.string_value();
                },
                sol::resolve<spine_player_events::custom_evt&(str)>(
                    &spine_player_events::custom_evt::string_value))
        );

        l["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_events::end_evt>("end_evt",
            sol::constructors<
                spine_player_events::end_evt(),
                spine_player_events::end_evt(str)>(),

            "message", sol::property(
                [](const spine_player_events::end_evt& c) -> str {
                    return c.message();
                },
                sol::resolve<spine_player_events::end_evt&(str)>(
                    &spine_player_events::end_evt::message))
        );

        l["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_events::complete_evt>("complete_evt",
            sol::constructors<
                spine_player_events::complete_evt(),
                spine_player_events::complete_evt(str)>(),

            "message", sol::property(
                [](const spine_player_events::complete_evt& c) -> str {
                    return c.message();
                },
                sol::resolve<spine_player_events::complete_evt&(str)>(
                    &spine_player_events::complete_evt::message))
        );

        //
        // commands
        //

        l["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_commands::clear_track_cmd>("clear_track_cmd",
            sol::constructors<spine_player_commands::clear_track_cmd(u32)>(),

            "track", sol::property(
                [](const spine_player_commands::clear_track_cmd& c) -> u32 {
                    return c.track();
                })
        );

        l["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_commands::set_anim_cmd>("set_anim_cmd",
            sol::constructors<spine_player_commands::set_anim_cmd(u32,str)>(),

            "track", sol::property(
                [](const spine_player_commands::set_anim_cmd& c) -> u32 {
                    return c.track();
                }),

            "name", sol::property(
                [](const spine_player_commands::set_anim_cmd& c) -> str {
                    return c.name();
                }),

            "loop", sol::property(
                [](const spine_player_commands::set_anim_cmd& c) -> bool {
                    return c.loop();
                },
                sol::resolve<spine_player_commands::set_anim_cmd&(bool)>(
                    &spine_player_commands::set_anim_cmd::loop)),

            "end_message", sol::property(
                [](const spine_player_commands::set_anim_cmd& c) -> str {
                    return c.end_message();
                },
                sol::resolve<spine_player_commands::set_anim_cmd&(str)>(
                    &spine_player_commands::set_anim_cmd::end_message)),

            "complete_message", sol::property(
                [](const spine_player_commands::set_anim_cmd& c) -> str {
                    return c.complete_message();
                },
                sol::resolve<spine_player_commands::set_anim_cmd&(str)>(
                    &spine_player_commands::set_anim_cmd::complete_message))
        );

        l["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_commands::add_anim_cmd>("add_anim_cmd",
            sol::constructors<spine_player_commands::add_anim_cmd(u32,str)>(),

            "track", sol::property(
                [](const spine_player_commands::add_anim_cmd& c) -> u32 {
                    return c.track();
                }),

            "name", sol::property(
                [](const spine_player_commands::add_anim_cmd& c) -> str {
                    return c.name();
                }),

            "loop", sol::property(
                [](const spine_player_commands::add_anim_cmd& c) -> bool {
                    return c.loop();
                },
                sol::resolve<spine_player_commands::add_anim_cmd&(bool)>(
                    &spine_player_commands::add_anim_cmd::loop)),

            "delay", sol::property(
                [](const spine_player_commands::add_anim_cmd& c) -> f32 {
                    return c.delay();
                },
                sol::resolve<spine_player_commands::add_anim_cmd&(f32)>(
                    &spine_player_commands::add_anim_cmd::delay)),

            "end_message", sol::property(
                [](const spine_player_commands::add_anim_cmd& c) -> str {
                    return c.end_message();
                },
                sol::resolve<spine_player_commands::add_anim_cmd&(str)>(
                    &spine_player_commands::add_anim_cmd::end_message)),

            "complete_message", sol::property(
                [](const spine_player_commands::add_anim_cmd& c) -> str {
                    return c.complete_message();
                },
                sol::resolve<spine_player_commands::add_anim_cmd&(str)>(
                    &spine_player_commands::add_anim_cmd::complete_message))
        );

        l["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_commands::set_empty_anim_cmd>("set_empty_anim_cmd",
            sol::constructors<spine_player_commands::set_empty_anim_cmd(u32)>(),

            "track", sol::property(
                [](const spine_player_commands::set_empty_anim_cmd& c) -> u32 {
                    return c.track();
                }),

            "mix_duration", sol::property(
                [](const spine_player_commands::set_empty_anim_cmd& c) -> f32 {
                    return c.mix_duration();
                },
                sol::resolve<spine_player_commands::set_empty_anim_cmd&(f32)>(
                    &spine_player_commands::set_empty_anim_cmd::mix_duration)),

            "end_message", sol::property(
                [](const spine_player_commands::set_empty_anim_cmd& c) -> str {
                    return c.end_message();
                },
                sol::resolve<spine_player_commands::set_empty_anim_cmd&(str)>(
                    &spine_player_commands::set_empty_anim_cmd::end_message)),

            "complete_message", sol::property(
                [](const spine_player_commands::set_empty_anim_cmd& c) -> str {
                    return c.complete_message();
                },
                sol::resolve<spine_player_commands::set_empty_anim_cmd&(str)>(
                    &spine_player_commands::set_empty_anim_cmd::complete_message))
        );

        l["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_commands::add_empty_anim_cmd>("add_empty_anim_cmd",
            sol::constructors<spine_player_commands::add_empty_anim_cmd(u32)>(),

            "track", sol::property(
                [](const spine_player_commands::add_empty_anim_cmd& c) -> u32 {
                    return c.track();
                }),

            "delay", sol::property(
                [](const spine_player_commands::add_empty_anim_cmd& c) -> f32 {
                    return c.delay();
                },
                sol::resolve<spine_player_commands::add_empty_anim_cmd&(f32)>(
                    &spine_player_commands::add_empty_anim_cmd::delay)),

            "mix_duration", sol::property(
                [](const spine_player_commands::add_empty_anim_cmd& c) -> f32 {
                    return c.mix_duration();
                },
                sol::resolve<spine_player_commands::add_empty_anim_cmd&(f32)>(
                    &spine_player_commands::add_empty_anim_cmd::mix_duration)),

            "end_message", sol::property(
                [](const spine_player_commands::add_empty_anim_cmd& c) -> str {
                    return c.end_message();
                },
                sol::resolve<spine_player_commands::add_empty_anim_cmd&(str)>(
                    &spine_player_commands::add_empty_anim_cmd::end_message)),

            "complete_message", sol::property(
                [](const spine_player_commands::add_empty_anim_cmd& c) -> str {
                    return c.complete_message();
                },
                sol::resolve<spine_player_commands::add_empty_anim_cmd&(str)>(
                    &spine_player_commands::add_empty_anim_cmd::complete_message))
        );
    }
}
