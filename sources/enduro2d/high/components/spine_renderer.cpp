/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/spine_renderer.hpp>
#include <spine/Skeleton.h>
#include <spine/VertexEffect.h>
#include <spine/SkeletonClipping.h>

namespace e2d
{
    spine_renderer::spine_renderer(const spine_model_asset::ptr& model)
    : model_(model) {
        E2D_ASSERT(model_);
        E2D_ASSERT(model_->content().skeleton());
        E2D_ASSERT(model_->content().atlas());

        skeleton_ = skeleton_ptr(spSkeleton_create(model_->content().skeleton().get()), spSkeleton_dispose);
        clipping_ = clipping_ptr(spSkeletonClipping_create(), spSkeletonClipping_dispose);
    }
    
    spine_renderer& spine_renderer::skin(const str& value) {
        spSkeleton_setSkinByName(skeleton_.get(), value.empty() ? nullptr : value.c_str());
        return *this;
    }

    spine_renderer& spine_renderer::attachment(const str& slot, const str& name) {
        E2D_ASSERT(!slot.empty());
        E2D_ASSERT(!name.empty());
        spSkeleton_setAttachment(skeleton_.get(), slot.c_str(), name.c_str()); // TODO: check result
        return *this;
    }

    const spine_renderer::skeleton_ptr& spine_renderer::skeleton() const noexcept {
        return skeleton_;
    }
    
    const spine_renderer::clipping_ptr& spine_renderer::clipper() const noexcept {
        return clipping_;
    }

    const spine_renderer::effect_ptr& spine_renderer::effect() const noexcept {
        return effect_;
    }

    const spine_model_asset::ptr& spine_renderer::model() const noexcept {
        return model_;
    }
}

namespace e2d
{
    const char* factory_loader<spine_renderer>::schema_source = R"json({
        "type" : "object",
        "required" : [ "model" ],
        "additionalProperties" : false,
        "properties" : {
            "model" : { "$ref": "#/common_definitions/address" },
            "skin" : { "$ref": "#/common_definitions/name" }
        }
    })json";
    
    bool factory_loader<spine_renderer>::operator()(
        spine_renderer& component,
        const fill_context& ctx) const
    {
        return false;
    }

    bool factory_loader<spine_renderer>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        return false;
    }
}
