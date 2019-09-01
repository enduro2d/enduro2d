/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/spine_player.hpp>

#include <spine/spine.h>

namespace e2d
{
    spine_player::spine_player(const spine_model_asset::ptr& model) {
        this->model(model);
    }
    
    spine_player& spine_player::model(const spine_model_asset::ptr& value) {
        clipping_ptr new_clipping = clipping_ptr(
            spSkeletonClipping_create(),
            spSkeletonClipping_dispose);
        if ( !new_clipping ) {
            throw std::bad_alloc();
        }

        skeleton_ptr new_skeleton;
        if ( value && value->content().skeleton() ) {
            new_skeleton = skeleton_ptr(
                spSkeleton_create(value->content().skeleton().get()),
                spSkeleton_dispose);
            if ( !new_skeleton ) {
                throw std::bad_alloc();
            }
        }

        animation_ptr new_animation;
        if ( value && value->content().animation() ) {
            new_animation = animation_ptr(
                spAnimationState_create(value->content().animation().get()),
                spAnimationState_dispose);
            if ( !new_animation ) {
                throw std::bad_alloc();
            }
        }

        model_ = value;
        clipping_ = std::move(new_clipping);
        skeleton_ = std::move(new_skeleton);
        animation_ = std::move(new_animation);
        return *this;
    }

    spine_player& spine_player::skin(const str& value) noexcept {
        if ( !spSkeleton_setSkinByName(skeleton_.get(), value.empty() ? nullptr : value.c_str()) ) {
            the<debug>().error("SPINE_PLAYER: can't set skin '%0'", value);
        }
        return *this;
    }
    
    spine_player& spine_player::attachment(const str& slot, const str& name) noexcept {
        if ( !spSkeleton_setAttachment(skeleton_.get(), slot.c_str(), name.c_str()) ) {
            the<debug>().error("SPINE_PLAYER: can't set attachment '%0' to slot '%1'", name, slot);
        }
        return *this;
    }

    bool spine_player::has_skin(const str& name) const noexcept {
        return model_
            && model()->content().skeleton()
            && spSkeletonData_findSkin(model()->content().skeleton().get(), name.c_str());
    }
    
    bool spine_player::has_animation(const str& name) const noexcept {
        return model_
            && model()->content().skeleton()
            && spSkeletonData_findAnimation(model()->content().skeleton().get(), name.c_str());
    }

    const spine_player::clipping_ptr& spine_player::clipper() const noexcept {
        return clipping_;
    }

    const spine_player::skeleton_ptr& spine_player::skeleton() const noexcept {
        return skeleton_;
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
            "model" : { "$ref": "#/common_definitions/address" },
            "skin" : { "$ref": "#/common_definitions/name" },
            "attachments" : { "$ref": "#/definitions/attachments" }
        },
        "definitions" : {
            "attachments" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/attachment" }
            },
            "attachment" : {
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
    
    bool factory_loader<spine_player>::operator()(
        spine_player& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("model") ) {
            auto model = ctx.dependencies.find_asset<spine_model_asset>(
                path::combine(ctx.parent_address, ctx.root["model"].GetString()));
            if ( !model ) {
                the<debug>().error("SPINE_PLAYER: Dependency 'model' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["model"].GetString());
                return false;
            }
            component.model(model);
        }
        
        if ( ctx.root.HasMember("skin") ) {
            str skin;
            if ( !json_utils::try_parse_value(ctx.root["skin"], skin) ) {
                the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'skin' property");
                return false;
            }
            component.skin(skin);
        }

        if ( ctx.root.HasMember("attachments") ) {
            const auto& attachments_json = ctx.root["attachments"];
            E2D_ASSERT(attachments_json.IsArray());
            
            for ( rapidjson::SizeType i = 0; i < attachments_json.Size(); ++i ) {
                E2D_ASSERT(attachments_json[i].IsObject());
                const auto& attachment_json = attachments_json[i];

                str slot;
                E2D_ASSERT(attachment_json.HasMember("slot"));
                if ( !json_utils::try_parse_value(attachment_json["slot"], slot) ) {
                    the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'attachment.slot' property");
                    return false;
                }

                str name;
                E2D_ASSERT(attachment_json.HasMember("name"));
                if ( !json_utils::try_parse_value(attachment_json["name"], name) ) {
                    the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'attachment.name' property");
                    return false;
                }

                component.attachment(slot, name);
            }
        }

        return true;
    }
    
    bool factory_loader<spine_player>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("model") ) {
            dependencies.add_dependency<spine_model_asset>(
                path::combine(ctx.parent_address, ctx.root["model"].GetString()));
        }

        return true;
    }
}
