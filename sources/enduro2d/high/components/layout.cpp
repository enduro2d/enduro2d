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
            "direction" : { "$ref": "#/definitions/directions" },
            "align_items" : { "$ref": "#/definitions/align_modes" },
            "align_content" : { "$ref": "#/definitions/align_modes" },
            "justify_content" : { "$ref": "#/definitions/justify_modes" },
            "flex_wrap" : { "$ref": "#/definitions/flex_wraps" },
            "flex_direction" : { "$ref": "#/definitions/flex_directions" }
        },
        "definitions" : {
            "directions" : {
                "type" : "string",
                "enum" : [
                    "ltr",
                    "rtl"
                ]
            },
            "align_modes" : {
                "type" : "string",
                "enum" : [
                    "flex_start",
                    "center",
                    "flex_end",
                    "space_between",
                    "space_around"
                ]
            },
            "justify_modes" : {
                "type" : "string",
                "enum" : [
                    "flex_start",
                    "center",
                    "flex_end",
                    "space_between",
                    "space_around",
                    "space_evenly"
                ]
            },
            "flex_wraps" : {
                "type" : "string",
                "enum" : [
                    "no_wrap",
                    "wrap",
                    "wrap_reversed"
                ]
            },
            "flex_directions" : {
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
        if ( ctx.root.HasMember("direction") ) {
            layout::directions direction = component.direction();
            if ( !json_utils::try_parse_value(ctx.root["direction"], direction) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'direction' property");
                return false;
            }
            component.direction(direction);
        }

        if ( ctx.root.HasMember("align_items") ) {
            layout::align_modes align_items = component.align_items();
            if ( !json_utils::try_parse_value(ctx.root["align_items"], align_items) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'align_items' property");
                return false;
            }
            component.align_items(align_items);
        }

        if ( ctx.root.HasMember("align_content") ) {
            layout::align_modes align_content = component.align_content();
            if ( !json_utils::try_parse_value(ctx.root["align_content"], align_content) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'align_content' property");
                return false;
            }
            component.align_content(align_content);
        }

        if ( ctx.root.HasMember("justify_content") ) {
            layout::justify_modes justify_content = component.justify_content();
            if ( !json_utils::try_parse_value(ctx.root["justify_content"], justify_content) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'justify_content' property");
                return false;
            }
            component.justify_content(justify_content);
        }

        if ( ctx.root.HasMember("flex_wrap") ) {
            layout::flex_wraps flex_wrap = component.flex_wrap();
            if ( !json_utils::try_parse_value(ctx.root["flex_wrap"], flex_wrap) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'flex_wrap' property");
                return false;
            }
            component.flex_wrap(flex_wrap);
        }

        if ( ctx.root.HasMember("flex_direction") ) {
            layout::flex_directions flex_direction = component.flex_direction();
            if ( !json_utils::try_parse_value(ctx.root["flex_direction"], flex_direction) ) {
                the<debug>().error("LAYOUT: Incorrect formatting of 'flex_direction' property");
                return false;
            }
            component.flex_direction(flex_direction);
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

        if ( layout::directions direction = c->direction();
            imgui_utils::show_enum_combo_box("direction", &direction) )
        {
            layouts::change_direction(c, direction);
        }

        if ( layout::align_modes align_items = c->align_items();
            imgui_utils::show_enum_combo_box("align_items", &align_items) )
        {
            layouts::change_align_items(c, align_items);
        }

        if ( layout::align_modes align_content = c->align_content();
            imgui_utils::show_enum_combo_box("align_content", &align_content) )
        {
            layouts::change_align_content(c, align_content);
        }

        if ( layout::justify_modes justify_content = c->justify_content();
            imgui_utils::show_enum_combo_box("justify_content", &justify_content) )
        {
            layouts::change_justify_content(c, justify_content);
        }

        if ( layout::flex_wraps flex_wrap = c->flex_wrap();
            imgui_utils::show_enum_combo_box("flex_wrap", &flex_wrap) )
        {
            layouts::change_flex_wrap(c, flex_wrap);
        }

        if ( layout::flex_directions flex_direction = c->flex_direction();
            imgui_utils::show_enum_combo_box("flex_direction", &flex_direction) )
        {
            layouts::change_flex_direction(c, flex_direction);
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

    gcomponent<layout> change_direction(gcomponent<layout> self, layout::directions value) {
        if ( self ) {
            self->direction(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_align_items(gcomponent<layout> self, layout::align_modes value) {
        if ( self ) {
            self->align_items(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_align_content(gcomponent<layout> self, layout::align_modes value) {
        if ( self ) {
            self->align_content(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_justify_content(gcomponent<layout> self, layout::justify_modes value) {
        if ( self ) {
            self->justify_content(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_flex_wrap(gcomponent<layout> self, layout::flex_wraps value) {
        if ( self ) {
            self->flex_wrap(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> change_flex_direction(gcomponent<layout> self, layout::flex_directions value) {
        if ( self ) {
            self->flex_direction(value);
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
