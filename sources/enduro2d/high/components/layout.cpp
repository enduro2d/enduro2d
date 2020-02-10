/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/layout.hpp>

#include <enduro2d/high/components/actor.hpp>

namespace e2d
{
    const char* factory_loader<layout>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "mode" : { "$ref": "#/definitions/modes" },
            "halign" : { "$ref": "#/definitions/haligns" },
            "valign" : { "$ref": "#/definitions/valigns" },
            "size" : { "$ref": "#/common_definitions/v2" },
            "margin" : { "$ref": "#/common_definitions/v2" },
            "padding" : { "$ref": "#/common_definitions/v2" }
        },
        "definitions" : {
            "modes" : {
                "type" : "string",
                "enum" : [
                    "horizontal",
                    "vertical"
                ]
            },
            "haligns" : {
                "type" : "string",
                "enum" : [
                    "left",
                    "center",
                    "right",
                    "space_around",
                    "space_between"
                ]
            },
            "valigns" : {
                "type" : "string",
                "enum" : [
                    "top",
                    "center",
                    "bottom",
                    "space_around",
                    "space_between"
                ]
            }
        }
    })json";

    bool factory_loader<layout>::operator()(
        layout& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("mode") ) {
            layout::modes mode = component.mode();
            if ( !json_utils::try_parse_value(ctx.root["mode"], mode) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'mode' property");
                return false;
            }
            component.mode(mode);
        }

        if ( ctx.root.HasMember("halign") ) {
            layout::haligns halign = component.halign();
            if ( !json_utils::try_parse_value(ctx.root["halign"], halign) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'halign' property");
                return false;
            }
            component.halign(halign);
        }

        if ( ctx.root.HasMember("valign") ) {
            layout::valigns valign = component.valign();
            if ( !json_utils::try_parse_value(ctx.root["valign"], valign) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'valign' property");
                return false;
            }
            component.valign(valign);
        }

        if ( ctx.root.HasMember("size") ) {
            v2f size = component.size();
            if ( !json_utils::try_parse_value(ctx.root["size"], size) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'size' property");
                return false;
            }
            component.size(size);
        }

        if ( ctx.root.HasMember("margin") ) {
            v2f margin = component.margin();
            if ( !json_utils::try_parse_value(ctx.root["margin"], margin) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'margin' property");
                return false;
            }
            component.margin(margin);
        }

        if ( ctx.root.HasMember("padding") ) {
            v2f padding = component.padding();
            if ( !json_utils::try_parse_value(ctx.root["padding"], padding) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'padding' property");
                return false;
            }
            component.padding(padding);
        }

        return true;
    }

    bool factory_loader<layout>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<layout::dirty>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<layout::dirty>::operator()(
        layout::dirty& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<layout::dirty>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<layout>::title = ICON_FA_BARS " layout";

    void component_inspector<layout>::operator()(gcomponent<layout>& c) const {
        if ( layout::modes mode = c->mode();
            imgui_utils::show_enum_combo_box("mode", &mode) )
        {
            layouts::change_mode(c, mode);
        }

        if ( layout::haligns halign = c->halign();
            imgui_utils::show_enum_combo_box("halign", &halign) )
        {
            layouts::change_halign(c, halign);
        }

        if ( layout::valigns valign = c->valign();
            imgui_utils::show_enum_combo_box("valign", &valign) )
        {
            layouts::change_valign(c, valign);
        }

        if ( v2f size = c->size();
            ImGui::DragFloat2("size", size.data(), 1.f) )
        {
            layouts::change_size(c, size);
        }

        if ( v2f margin = c->margin();
            ImGui::DragFloat2("margin", margin.data(), 1.f) )
        {
            layouts::change_margin(c, margin);
        }

        if ( v2f padding = c->padding();
            ImGui::DragFloat2("padding", padding.data(), 1.f) )
        {
            layouts::change_padding(c, padding);
        }
    }

    void component_inspector<layout>::operator()(
        gcomponent<layout>& c,
        gizmos_context& ctx) const
    {
        ctx.draw_wire_rect(
            c->size() * 0.5f,
            c->size(),
            ctx.selected() ? color32(255,255,255) : color32(127,127,127));

        if ( ctx.selected() ) {
            if ( c->margin() != v2f::zero() ) {
                ctx.draw_wire_rect(
                    c->size() * 0.5f,
                    c->size() + c->margin() * 2.f,
                    ctx.selected() ? color32(255,255,255) : color32(127,127,127));
            }

            if ( c->padding() != v2f::zero() ) {
                ctx.draw_wire_rect(
                    c->size() * 0.5f,
                    c->size() - c->padding() * 2.f,
                    ctx.selected() ? color32(255,255,255) : color32(127,127,127));
            }
        }
    }
}

namespace e2d::layouts
{
    gcomponent<layout> mark_dirty(gcomponent<layout> self) {
        if ( self ) {
            self.owner().component<layout::dirty>().ensure();
            if ( gcomponent<layout> parent = find_parent_layout(self) ) {
                parent.owner().component<layout::dirty>().ensure();
            }
        }
        return self;
    }

    gcomponent<layout> unmark_dirty(gcomponent<layout> self) {
        if ( self ) {
            self.owner().component<layout::dirty>().remove();
        }
        return self;
    }

    bool is_dirty(const const_gcomponent<layout>& self) noexcept {
        return self.owner().component<layout::dirty>().exists();
    }

    gcomponent<layout> change_mode(gcomponent<layout> self, layout::modes value) {
        if ( self ) {
            self->mode(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_halign(gcomponent<layout> self, layout::haligns value) {
        if ( self ) {
            self->halign(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_valign(gcomponent<layout> self, layout::valigns value) {
        if ( self ) {
            self->valign(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_size(gcomponent<layout> self, const v2f& value) {
        if ( self ) {
            self->size(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_margin(gcomponent<layout> self, const v2f& value) {
        if ( self ) {
            self->margin(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_padding(gcomponent<layout> self, const v2f& value) {
        if ( self ) {
            self->padding(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> find_parent_layout(const_gcomponent<layout> self) noexcept {
        const_gcomponent<actor> self_actor = self.owner().component<actor>();
        return self_actor
            ? nodes::find_component_from_parents<layout>(self_actor->node())
            : gcomponent<layout>();
    }
}
