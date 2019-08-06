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
    spine_renderer::spine_renderer(const spine_model_asset::ptr& model) {
        this->model(model);
    }
    
    spine_renderer& spine_renderer::model(const spine_model_asset::ptr& value) noexcept {
        E2D_ASSERT(value);
        E2D_ASSERT(value->content().skeleton());
        E2D_ASSERT(value->content().atlas());
        model_ = value;

        skeleton_ = skeleton_ptr(spSkeleton_create(model_->content().skeleton().get()), spSkeleton_dispose);
        clipping_ = clipping_ptr(spSkeletonClipping_create(), spSkeletonClipping_dispose);
        return *this;
    }

    spine_renderer& spine_renderer::skin(const str& value) noexcept {
        spSkeleton_setSkinByName(skeleton_.get(), value.empty() ? nullptr : value.c_str());
        return *this;
    }

    spine_renderer& spine_renderer::attachment(const str& slot, const str& name) noexcept {
        E2D_ASSERT(!slot.empty());
        E2D_ASSERT(!name.empty());
        if ( !spSkeleton_setAttachment(skeleton_.get(), slot.c_str(), name.c_str()) ) {
            the<debug>().error("SPINE_RENDERER: can't set attachment '%0' to slot '%1'", name, slot);
        }
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
            "skin" : { "$ref": "#/common_definitions/name" },
            "attachments" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/spine_attachment_array" }
            }
        },
        "definitions" : {
            "spine_attachment_array" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/spine_attachment" }
            },
            "spine_attachment" : {
                "type" : "object",
                "required" : [ "slot", "name" ],
                "additionalProperties" : false,
                "properties" : {
                    "slot" : { "$ref": "#/common_definitions/name" },
                    "name" : { "$ref": "#/common_definitions/name" }
                }
            }
        }
    })json";
    
    bool factory_loader<spine_renderer>::operator()(
        spine_renderer& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("model") ) {
            auto model = ctx.dependencies.find_asset<spine_model_asset>(
                path::combine(ctx.parent_address, ctx.root["model"].GetString()));
            if ( !model ) {
                the<debug>().error("SPINE RENDERER: Dependency 'model' is not found:\n"
                    "--> Parent address: %s\n"
                    "--> Dependency address: $s",
                    ctx.parent_address,
                    ctx.root["model"].GetString());
                return false;
            }
            component.model(model);
        }

        if ( ctx.root.HasMember("skin") ) {
            str skin;
            if ( !json_utils::try_parse_value(ctx.root["skin"], skin) ) {
                the<debug>().error("SPINE RENDERER: Incorrect formatting of 'skin' property");
                return false;
            }
            component.skin(skin);
        }

        return false;
    }

    bool factory_loader<spine_renderer>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("model") ) {
            dependencies.add_dependency<spine_model_asset>(
                path::combine(ctx.parent_address, ctx.root["model"].GetString()));
        }
        
        if ( ctx.root.HasMember("skin") ) {
            dependencies.add_dependency<spine_model_asset>(
                path::combine(ctx.parent_address, ctx.root["skin"].GetString()));
        }

        return false;
    }
}
