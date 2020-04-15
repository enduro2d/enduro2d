/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/widget.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/layout.hpp>

namespace e2d
{
    const char* factory_loader<widget>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "size" : { "$ref": "#/common_definitions/v2" },
            "margin" : { "$ref": "#/common_definitions/v2" },
            "padding" : { "$ref": "#/common_definitions/v2" }
        }
    })json";

    bool factory_loader<widget>::operator()(
        widget& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("size") ) {
            v2f size = component.size();
            if ( !json_utils::try_parse_value(ctx.root["size"], size) ) {
                the<debug>().error("WIDGET: Incorrect formatting of 'size' property");
                return false;
            }
            component.size(size);
        }

        if ( ctx.root.HasMember("margin") ) {
            v2f margin = component.margin();
            if ( !json_utils::try_parse_value(ctx.root["margin"], margin) ) {
                the<debug>().error("WIDGET: Incorrect formatting of 'margin' property");
                return false;
            }
            component.margin(margin);
        }

        if ( ctx.root.HasMember("padding") ) {
            v2f padding = component.padding();
            if ( !json_utils::try_parse_value(ctx.root["padding"], padding) ) {
                the<debug>().error("WIDGET: Incorrect formatting of 'padding' property");
                return false;
            }
            component.padding(padding);
        }

        return true;
    }

    bool factory_loader<widget>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<widget::dirty>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<widget::dirty>::operator()(
        widget::dirty& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<widget::dirty>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<widget>::title = ICON_FA_VECTOR_SQUARE " widget";

    void component_inspector<widget>::operator()(gcomponent<widget>& c) const {
        if ( bool dirty = c.component<widget::dirty>().exists();
            ImGui::Checkbox("dirty", &dirty) )
        {
            if ( dirty ) {
                widgets::mark_dirty(c);
            } else {
                widgets::unmark_dirty(c);
            }
        }

        ImGui::Separator();

        if ( v2f size = c->size();
            ImGui::DragFloat2("size", size.data(), 1.f) )
        {
            widgets::change_size(c, size);
        }

        if ( v2f margin = c->margin();
            ImGui::DragFloat2("margin", margin.data(), 1.f) )
        {
            widgets::change_margin(c, margin);
        }

        if ( v2f padding = c->padding();
            ImGui::DragFloat2("padding", padding.data(), 1.f) )
        {
            widgets::change_padding(c, padding);
        }
    }

    void component_inspector<widget>::operator()(
        gcomponent<widget>& c,
        gizmos_context& ctx) const
    {
        const color32 white_c{255,255,255};
        const color32 gray_c{255,255,255,127};

        ctx.draw_wire_rect(
            c->size() * 0.5f,
            c->size(),
            ctx.selected() ? white_c : gray_c);

        if ( ctx.selected() ) {
            if ( c->margin() != v2f::zero() ) {
                ctx.draw_wire_rect(
                    c->size() * 0.5f,
                    c->size() + c->margin() * 2.f,
                    ctx.selected() ? white_c : gray_c);
            }

            if ( c->padding() != v2f::zero() ) {
                ctx.draw_wire_rect(
                    c->size() * 0.5f,
                    c->size() - c->padding() * 2.f,
                    ctx.selected() ? white_c : gray_c);
            }
        }
    }
}

namespace e2d::widgets
{
    gcomponent<widget> mark_dirty(gcomponent<widget> self) {
        if ( self ) {
            self.component<widget::dirty>().ensure();
        }
        return self;
    }

    gcomponent<widget> unmark_dirty(gcomponent<widget> self) {
        if ( self ) {
            self.component<widget::dirty>().remove();
        }
        return self;
    }

    bool is_dirty(const const_gcomponent<widget>& self) noexcept {
        return self.component<widget::dirty>().exists();
    }

    gcomponent<widget> change_size(gcomponent<widget> self, const v2f& value) {
        if ( self ) {
            self->size(value);
        }
        return mark_dirty(self);
    }

    gcomponent<widget> change_margin(gcomponent<widget> self, const v2f& value) {
        if ( self ) {
            self->margin(value);
        }
        return mark_dirty(self);
    }

    gcomponent<widget> change_padding(gcomponent<widget> self, const v2f& value) {
        if ( self ) {
            self->padding(value);
        }
        return mark_dirty(self);
    }

    gcomponent<layout> find_parent_layout(const_gcomponent<widget> self) noexcept {
        const_gcomponent<actor> self_actor = self.component<actor>();
        return self_actor
            ? nodes::find_component_from_parents<layout>(self_actor->node())
            : gcomponent<layout>();
    }
}
