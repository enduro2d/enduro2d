/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/label.hpp>

namespace e2d
{
    const char* factory_loader<label>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "model" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    bool factory_loader<label>::operator()(
        label& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("label") ) {
//            auto model = ctx.dependencies.find_asset<model_asset>(
//                path::combine(ctx.parent_address, ctx.root["model"].GetString()));
//            if ( !model ) {
//                the<debug>().error("label: Dependency 'model' is not found:\n"
//                    "--> Parent address: %0\n"
//                    "--> Dependency address: %1",
//                    ctx.parent_address,
//                    ctx.root["model"].GetString());
//                return false;
//            }
//            component.label(model);
        }

        return true;;
    }

    bool factory_loader<label>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("model") ) {
//            dependencies.add_dependency<model_asset>(
//                path::combine(ctx.parent_address, ctx.root["model"].GetString()));
        }

        return true;
    }
}

namespace e2d
{
    label::label(label&& other) noexcept {
        assign(std::move(other));
    }

    label& label::operator=(label&& other) noexcept {
        return assign(std::move(other));
    }

    label::label(const label& other) {
        assign(other);
    }

    label& label::operator=(const label& other) {
        return assign(other);
    }

    label::label(str32_view text, font_asset::ptr font, color32 tint) {
        assign(text, font, tint);
    }

    label& label::assign(label&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    label& label::assign(const label& other) {
        if ( this != &other ) {
            dirty_ = true;
            text_ = other.text_;
            font_ = other.font_;
            tint_ = other.tint_;
        }
        return *this;
    }

    label& label::assign(str32_view text, font_asset::ptr font, color32 tint) {
        dirty_ = false;
        text_ = text;
        font_ = font;
        tint_ = tint;
    }

    void label::swap(label& other) noexcept {
        using std::swap;
        swap(dirty_, other.dirty_);
        swap(text_, other.text_);
        swap(font_, other.font_);
        swap(tint_, other.tint_);
    }

    void label::clear() noexcept {
        dirty_ = true;
        text_.clear();
        font_ = nullptr;
        tint_ = color32::white();
    }

    bool label::empty() const noexcept {
        return text_.empty();
    }
}
