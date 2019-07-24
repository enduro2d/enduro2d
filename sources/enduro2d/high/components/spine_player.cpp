/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/spine_player.hpp>

#include <spine/AnimationState.h>

namespace e2d
{
    spine_player::spine_player(const spine_model_asset::ptr& model)
    : model_(model) {
        E2D_ASSERT(model_);
        E2D_ASSERT(model_->content().animation());

        animation_ = animation_ptr(spAnimationState_create(model_->content().animation().get()), spAnimationState_dispose);
    }
    
    spine_player& spine_player::set_animation(int track, const str& name, bool loop) {
        E2D_ASSERT(model_);
        spAnimation* anim = spSkeletonData_findAnimation(model_->content().skeleton().operator->(), name.c_str());
        if ( !anim ) {
            E2D_ASSERT(false);  // TODO: exception?
            return *this;
        }
        spAnimationState_setAnimation(animation_.get(), track, anim, loop);
        return *this;
    }

    spine_player& spine_player::add_animation(int track, const str& name, bool loop, secf delay) {
        E2D_ASSERT(model_);
        spAnimation* anim = spSkeletonData_findAnimation(model_->content().skeleton().operator->(), name.c_str());
        if ( !anim ) {
            E2D_ASSERT(false);  // TODO: exception?
            return *this;
        }
        spAnimationState_addAnimation(animation_.get(), track, anim, loop, delay.value);
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
    
    bool factory_loader<spine_player>::operator()(
        spine_player& component,
        const fill_context& ctx) const
    {
        return false;
    }
    
    bool factory_loader<spine_player>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}
