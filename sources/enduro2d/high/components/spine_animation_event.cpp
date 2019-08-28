/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/spine_animation_event.hpp>

namespace e2d
{
    const char* factory_loader<spine_animation_event>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "animations" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/spine_animation" }
            }
        },
        "definitions" : {
            "spine_animation" : {
                "type" : "object",
                "required" : [ "track", "name" ],
                "additionalProperties" : false,
                "properties" : {
                    "track" : { "type" : "integer", "minimum" : 0, "maximum": 8 },
                    "name" : { "$ref": "#/common_definitions/name" },
                    "loop" : { "type" : "boolean" },
                    "delay" : { "type" : "number" }
                }
            }
        }
    })json";
    
    bool factory_loader<spine_animation_event>::operator()(
        spine_animation_event& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("animations") ) {
            const auto& animations_json = ctx.root["animations"];
            E2D_ASSERT(animations_json.IsArray());
            
            for ( rapidjson::SizeType i = 0; i < animations_json.Size(); ++i ) {
                E2D_ASSERT(animations_json[i].IsObject());
                const auto& item_json = animations_json[i];

                E2D_ASSERT(item_json.HasMember("track") && item_json.HasMember("name"));
                u32 track = 0;
                secf delay;
                bool loop = false;
                const char* name = "";

                if ( item_json.HasMember("name") ) {
                    name = item_json["name"].GetString();
                }

                if ( item_json.HasMember("track") ) {
                    if ( !json_utils::try_parse_value(item_json["track"], track) ) {
                        the<debug>().error("SPINE_ANIMATION: Incorrect formatting of 'animations.track' property");
                        return false;
                    }
                }

                if ( item_json.HasMember("delay") ) {
                    if ( !json_utils::try_parse_value(item_json["delay"], delay.value) ) {
                        the<debug>().error("SPINE_ANIMATION: Incorrect formatting of 'animations.delay' property");
                        return false;
                    }
                }

                if ( item_json.HasMember("loop") ) {
                    if ( !json_utils::try_parse_value(item_json["loop"], loop) ) {
                        the<debug>().error("SPINE_ANIMATION: Incorrect formatting of 'animations.loop' property");
                        return false;
                    }
                }

                component.commands().emplace_back(spine_animation_event::add_anim{
                    track, name, loop, delay, ""});
            }
        }

        return true;
    }
    
    bool factory_loader<spine_animation_event>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}
