/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "flipbook_player.hpp"

namespace e2d
{
    template <>
    class component_loader<flipbook_player> {
    public:
        constexpr static const char*
        schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {
                "time" : { "type" : "number" },
                "speed" : { "type" : "number" },
                "looped" : { "type" : "boolean" },
                "stopped" : { "type" : "boolean" },
                "playing" : { "type" : "boolean" },
                "sequence" : { "$ref": "#/common_definitions/name" }
            }
        })json";

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            asset_dependencies& dependencies) const
        {
            E2D_UNUSED(
                parent_address,
                root,
                dependencies);

            return true;
        }

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            const asset_group& dependencies,
            flipbook_player& component) const
        {
            E2D_UNUSED(
                parent_address,
                dependencies);

            if ( root.HasMember("time") ) {
                auto time = component.time();
                if ( !json_utils::try_parse_value(root["time"], time) ) {
                    the<debug>().error("FLIPBOOK_PLAYER: Incorrect formatting of 'time' property");
                    return false;
                }
                component.time(time);
            }

            if ( root.HasMember("speed") ) {
                auto speed = component.speed();
                if ( !json_utils::try_parse_value(root["speed"], speed) ) {
                    the<debug>().error("FLIPBOOK_PLAYER: Incorrect formatting of 'speed' property");
                    return false;
                }
                component.speed(speed);
            }

            if ( root.HasMember("looped") ) {
                auto looped = component.looped();
                if ( !json_utils::try_parse_value(root["looped"], looped) ) {
                    the<debug>().error("FLIPBOOK_PLAYER: Incorrect formatting of 'looped' property");
                    return false;
                }
                component.looped(looped);
            }

            if ( root.HasMember("stopped") ) {
                auto stopped = component.stopped();
                if ( !json_utils::try_parse_value(root["stopped"], stopped) ) {
                    the<debug>().error("FLIPBOOK_PLAYER: Incorrect formatting of 'stopped' property");
                    return false;
                }
                component.stopped(stopped);
            }

            if ( root.HasMember("playing") ) {
                auto playing = component.playing();
                if ( !json_utils::try_parse_value(root["playing"], playing) ) {
                    the<debug>().error("FLIPBOOK_PLAYER: Incorrect formatting of 'playing' property");
                    return false;
                }
                component.playing(playing);
            }

            if ( root.HasMember("sequence") ) {
                auto sequence = component.sequence();
                if ( !json_utils::try_parse_value(root["sequence"], sequence) ) {
                    the<debug>().error("FLIPBOOK_PLAYER: Incorrect formatting of 'sequence' property");
                    return false;
                }
                component.sequence(sequence);
            }

            return true;
        }
    };
}
