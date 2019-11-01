/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/spine_player.hpp>
#include <enduro2d/high/components/spine_player_cmd.hpp>
#include <enduro2d/high/components/spine_player_evt.hpp>

namespace e2d::bindings::high
{
    void bind_spine_player(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["components"].get_or_create<sol::table>()
        .new_usertype<gcomponent<spine_player>>("spine_player",
            "spine", sol::property(
                [](const gcomponent<spine_player>& sp){ return sp->spine(); },
                [](gcomponent<spine_player>& sp, const spine_asset::ptr& v){ sp->spine(v); }),

            "skin", [](gcomponent<spine_player>& sp, const char* name){
                sp->skin(name);
            },

            "attachment", [](gcomponent<spine_player>& sp, const char* slot, const char* name){
                sp->attachment(slot, name);
            },

            "has_skin", [](const gcomponent<spine_player>& sp, const char* name){
                return sp->has_skin(name);
            },

            "has_animation", [](const gcomponent<spine_player>& sp, const char* name){
                return sp->has_animation(name);
            },

            "add_command", [](gcomponent<spine_player>& sp, spine_player_cmd::command cmd){
                sp.owner().component<spine_player_cmd>().ensure().add_command(std::move(cmd));
            }
        );

        //
        // commands
        //

        l["e2d"].get_or_create<sol::table>()
        ["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_cmd::clear_track_cmd>("clear_track_cmd",
            sol::constructors<spine_player_cmd::clear_track_cmd(u32)>(),
            "track", sol::property(&spine_player_cmd::clear_track_cmd::track)
        );

        l["e2d"].get_or_create<sol::table>()
        ["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_cmd::set_anim_cmd>("set_anim_cmd",
            sol::constructors<spine_player_cmd::set_anim_cmd(u32,str)>(),
            "track", sol::property(&spine_player_cmd::set_anim_cmd::track),
            "name", sol::property(&spine_player_cmd::set_anim_cmd::name),

            "loop", sol::property(
                sol::resolve<bool()const>(&spine_player_cmd::set_anim_cmd::loop),
                sol::resolve<spine_player_cmd::set_anim_cmd&(bool)>(&spine_player_cmd::set_anim_cmd::loop)),
            "end_message", sol::property(
                sol::resolve<const str&()const>(&spine_player_cmd::set_anim_cmd::end_message),
                sol::resolve<spine_player_cmd::set_anim_cmd&(str)>(&spine_player_cmd::set_anim_cmd::end_message)),
            "complete_message", sol::property(
                sol::resolve<const str&()const>(&spine_player_cmd::set_anim_cmd::complete_message),
                sol::resolve<spine_player_cmd::set_anim_cmd&(str)>(&spine_player_cmd::set_anim_cmd::complete_message))
        );

        l["e2d"].get_or_create<sol::table>()
        ["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_cmd::add_anim_cmd>("add_anim_cmd",
            sol::constructors<spine_player_cmd::add_anim_cmd(u32,str)>(),
            "track", sol::property(&spine_player_cmd::add_anim_cmd::track),
            "name", sol::property(&spine_player_cmd::add_anim_cmd::name),

            "loop", sol::property(
                sol::resolve<bool()const>(&spine_player_cmd::add_anim_cmd::loop),
                sol::resolve<spine_player_cmd::add_anim_cmd&(bool)>(&spine_player_cmd::add_anim_cmd::loop)),
            "delay", sol::property(
                sol::resolve<secf()const>(&spine_player_cmd::add_anim_cmd::delay),
                sol::resolve<spine_player_cmd::add_anim_cmd&(secf)>(&spine_player_cmd::add_anim_cmd::delay)),
            "end_message", sol::property(
                sol::resolve<const str&()const>(&spine_player_cmd::add_anim_cmd::end_message),
                sol::resolve<spine_player_cmd::add_anim_cmd&(str)>(&spine_player_cmd::add_anim_cmd::end_message)),
            "complete_message", sol::property(
                sol::resolve<const str&()const>(&spine_player_cmd::add_anim_cmd::complete_message),
                sol::resolve<spine_player_cmd::add_anim_cmd&(str)>(&spine_player_cmd::add_anim_cmd::complete_message))
        );

        l["e2d"].get_or_create<sol::table>()
        ["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_cmd::set_empty_anim_cmd>("set_empty_anim_cmd",
            sol::constructors<spine_player_cmd::set_empty_anim_cmd(u32)>(),
            "track", sol::property(&spine_player_cmd::set_empty_anim_cmd::track),

            "mix_duration", sol::property(
                sol::resolve<secf()const>(&spine_player_cmd::set_empty_anim_cmd::mix_duration),
                sol::resolve<spine_player_cmd::set_empty_anim_cmd&(secf)>(&spine_player_cmd::set_empty_anim_cmd::mix_duration)),
            "end_message", sol::property(
                sol::resolve<const str&()const>(&spine_player_cmd::set_empty_anim_cmd::end_message),
                sol::resolve<spine_player_cmd::set_empty_anim_cmd&(str)>(&spine_player_cmd::set_empty_anim_cmd::end_message)),
            "complete_message", sol::property(
                sol::resolve<const str&()const>(&spine_player_cmd::set_empty_anim_cmd::complete_message),
                sol::resolve<spine_player_cmd::set_empty_anim_cmd&(str)>(&spine_player_cmd::set_empty_anim_cmd::complete_message))
        );

        l["e2d"].get_or_create<sol::table>()
        ["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_cmd::add_empty_anim_cmd>("add_empty_anim_cmd",
            sol::constructors<spine_player_cmd::add_empty_anim_cmd(u32)>(),
            "track", sol::property(&spine_player_cmd::add_empty_anim_cmd::track),

            "delay", sol::property(
                sol::resolve<secf()const>(&spine_player_cmd::add_empty_anim_cmd::delay),
                sol::resolve<spine_player_cmd::add_empty_anim_cmd&(secf)>(&spine_player_cmd::add_empty_anim_cmd::delay)),
            "mix_duration", sol::property(
                sol::resolve<secf()const>(&spine_player_cmd::add_empty_anim_cmd::mix_duration),
                sol::resolve<spine_player_cmd::add_empty_anim_cmd&(secf)>(&spine_player_cmd::add_empty_anim_cmd::mix_duration)),
            "end_message", sol::property(
                sol::resolve<const str&()const>(&spine_player_cmd::add_empty_anim_cmd::end_message),
                sol::resolve<spine_player_cmd::add_empty_anim_cmd&(str)>(&spine_player_cmd::add_empty_anim_cmd::end_message)),
            "complete_message", sol::property(
                sol::resolve<const str&()const>(&spine_player_cmd::add_empty_anim_cmd::complete_message),
                sol::resolve<spine_player_cmd::add_empty_anim_cmd&(str)>(&spine_player_cmd::add_empty_anim_cmd::complete_message))
        );

        //
        // events
        //

        l["e2d"].get_or_create<sol::table>()
        ["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_evt::custom_evt>("custom_evt",
            sol::constructors<spine_player_evt::custom_evt(str_hash)>(),
            "name", sol::property(&spine_player_evt::custom_evt::name),

            "int_value", sol::property(
                sol::resolve<i32()const>(&spine_player_evt::custom_evt::int_value),
                sol::resolve<spine_player_evt::custom_evt&(i32)>(&spine_player_evt::custom_evt::int_value)),
            "float_value", sol::property(
                sol::resolve<f32()const>(&spine_player_evt::custom_evt::float_value),
                sol::resolve<spine_player_evt::custom_evt&(f32)>(&spine_player_evt::custom_evt::float_value)),
            "string_value", sol::property(
                sol::resolve<const str&()const>(&spine_player_evt::custom_evt::string_value),
                sol::resolve<spine_player_evt::custom_evt&(str)>(&spine_player_evt::custom_evt::string_value))
        );

        l["e2d"].get_or_create<sol::table>()
        ["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_evt::end_evt>("end_evt",
            sol::constructors<spine_player_evt::end_evt(str)>(),
            "message", sol::property(&spine_player_evt::end_evt::message)
        );

        l["e2d"].get_or_create<sol::table>()
        ["spine_player"].get_or_create<sol::table>()
        .new_usertype<spine_player_evt::complete_evt>("complete_evt",
            sol::constructors<spine_player_evt::complete_evt(str)>(),
            "message", sol::property(&spine_player_evt::complete_evt::message)
        );
    }
}
