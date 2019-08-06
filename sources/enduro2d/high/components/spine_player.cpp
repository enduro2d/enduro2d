/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/spine_player.hpp>

#include <spine/AnimationState.h>

namespace
{
    // spine will allocate storage for each track, so keep number of track as small as possible
    constexpr e2d::u32 max_track_count = 8;
}

namespace e2d
{
    spine_player::spine_player(const spine_model_asset::ptr& model)
    : model_(model) {
        E2D_ASSERT(model_);
        E2D_ASSERT(model_->content().animation());

        animation_ = animation_ptr(
            spAnimationState_create(model_->content().animation().get()),
            spAnimationState_dispose);
    }
    
    spine_player& spine_player::time_scale(float value) noexcept {
        E2D_ASSERT(animation_);
        animation_->timeScale = value;
        return *this;
    }

    float spine_player::time_scale() const noexcept {
        E2D_ASSERT(animation_);
        return animation_->timeScale;
    }
    
    bool spine_player::has_animation(const str& name) const noexcept {
        if ( !model_ ) {
            return false;
        }
        spAnimation* anim = spSkeletonData_findAnimation(
            model_->content().skeleton().operator->(),
            name.c_str());
        return anim != nullptr;
    }

    spine_player& spine_player::set_animation(u32 track, const str& name, bool loop) noexcept {
        E2D_ASSERT(model_ && animation_);
        E2D_ASSERT(track < max_track_count);
        spAnimation* anim = spSkeletonData_findAnimation(
            model_->content().skeleton().operator->(),
            name.c_str());

        if ( !anim ) {
            the<debug>().error("SPINE_PLAYER: animation '%0' is not found", name);
            return *this;
        }
        spAnimationState_setAnimation(animation_.get(), track, anim, loop);
        return *this;
    }

    spine_player& spine_player::add_animation(u32 track, const str& name, secf delay) noexcept {
        return add_animation(track, name, false, delay);
    }

    spine_player& spine_player::add_animation(u32 track, const str& name, bool loop, secf delay) noexcept {
        E2D_ASSERT(model_ && animation_);
        E2D_ASSERT(track < max_track_count);
        spAnimation* anim = spSkeletonData_findAnimation(
            model_->content().skeleton().operator->(),
            name.c_str());

        if ( !anim ) {
            the<debug>().error("SPINE_PLAYER: animation '%0' is not found", name);
            return *this;
        }
        spAnimationState_addAnimation(animation_.get(), track, anim, loop, delay.value);
        return *this;
    }
    
    spine_player& spine_player::add_empty_animation(u32 track, secf duration, secf delay) noexcept {
        E2D_ASSERT(animation_);
        E2D_ASSERT(track < max_track_count);
        spAnimationState_addEmptyAnimation(animation_.get(), track, duration.value, delay.value);
        return *this;
    }
    
    spine_player& spine_player::clear(u32 track) noexcept {
        E2D_ASSERT(animation_);
        E2D_ASSERT(track < max_track_count);
        spAnimationState_clearTrack(animation_.get(), track);
        return *this;
    }
    
    spine_player& spine_player::clear() noexcept {
        E2D_ASSERT(animation_);
        spAnimationState_clearTracks(animation_.get());
        return *this;
    }

    const spine_player::animation_ptr& spine_player::animation() const noexcept {
        return animation_;
    }
    
    const spine_model_asset::ptr& spine_player::model() const noexcept {
        return model_;
    }
}

namespace e2d
{
    const char* factory_loader<spine_player>::schema_source = R"json({
        "type" : "object",
        "required" : [ "model" ],
        "additionalProperties" : false,
        "properties" : {
            "model" : { "$ref": "#/common_definitions/address" },
            "animations" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/spine_animation_array" }
            }
        },
        "definitions" : {
            "spine_animation_array" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/spine_animation" }
            },
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
    
    bool factory_loader<spine_player>::operator()(
        spine_player& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("model") ) {
            auto model = ctx.dependencies.find_asset<spine_model_asset>(
                path::combine(ctx.parent_address, ctx.root["model"].GetString()));
            if ( !model ) {
                the<debug>().error("SPINE_PLAYER: Dependency 'model' is not found:\n"
                    "--> Parent address: %s\n"
                    "--> Dependency address: $s",
                    ctx.parent_address,
                    ctx.root["model"].GetString());
                return false;
            }
            component = spine_player(model);
        } else {
            the<debug>().error("SPINE_PLAYER: Property 'model' is not found");
            return false;
        }

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
                        the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'track' property");
                        return false;
                    }
                }

                if ( item_json.HasMember("delay") ) {
                    if ( !json_utils::try_parse_value(item_json["delay"], delay.value) ) {
                        the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'delay' property");
                        return false;
                    }
                }

                if ( item_json.HasMember("loop") ) {
                    if ( !json_utils::try_parse_value(item_json["loop"], loop) ) {
                        the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'loop' property");
                        return false;
                    }
                }

                component.add_animation(track, name, loop, delay);
            }
        }
        return true;
    }
    
    bool factory_loader<spine_player>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}
