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
            "halign" : { "$ref": "#/definitions/haligns" },
            "valign" : { "$ref": "#/definitions/valigns" },
            "direction" : { "$ref": "#/definitions/directions" },
            "size" : { "$ref": "#/common_definitions/v2" },
            "margin" : { "$ref": "#/common_definitions/v2" },
            "padding" : { "$ref": "#/common_definitions/v2" }
        },
        "definitions" : {
            "haligns" : {
                "type" : "string",
                "enum" : [
                    "left",
                    "center",
                    "right",
                    "space_around",
                    "space_evenly",
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
                    "space_evenly",
                    "space_between"
                ]
            },
            "directions" : {
                "type" : "string",
                "enum" : [
                    "row",
                    "row_reversed",
                    "column",
                    "column_reversed"
                ]
            }
        }
    })json";

    bool factory_loader<layout>::operator()(
        layout& component,
        const fill_context& ctx) const
    {
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

        if ( ctx.root.HasMember("direction") ) {
            layout::directions direction = component.direction();
            if ( !json_utils::try_parse_value(ctx.root["direction"], direction) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'direction' property");
                return false;
            }
            component.direction(direction);
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
        if ( bool dirty = c.owner().component<layout::dirty>().exists();
            ImGui::Checkbox("dirty", &dirty) )
        {
            if ( dirty ) {
                layouts::mark_dirty(c);
            } else {
                layouts::unmark_dirty(c);
            }
        }

        ImGui::Separator();

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

        if ( layout::directions direction = c->direction();
            imgui_utils::show_enum_combo_box("direction", &direction) )
        {
            layouts::change_direction(c, direction);
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

    gcomponent<layout> change_direction(gcomponent<layout> self, layout::directions value) {
        if ( self ) {
            self->direction(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_size(gcomponent<layout> self, const v2f& value) {
        if ( self ) {
            self->size(value);
        }
        mark_dirty(find_parent_layout(self));
        return mark_dirty(self);
    }

    gcomponent<layout> change_margin(gcomponent<layout> self, const v2f& value) {
        if ( self ) {
            self->margin(value);
        }
        mark_dirty(find_parent_layout(self));
        return mark_dirty(self);
    }

    gcomponent<layout> change_padding(gcomponent<layout> self, const v2f& value) {
        if ( self ) {
            self->padding(value);
        }
        mark_dirty(find_parent_layout(self));
        return mark_dirty(self);
    }

    gcomponent<layout> find_parent_layout(const_gcomponent<layout> self) noexcept {
        const_gcomponent<actor> self_actor = self.owner().component<actor>();
        return self_actor
            ? nodes::find_component_from_parents<layout>(self_actor->node())
            : gcomponent<layout>();
    }
}
