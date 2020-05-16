/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/label.hpp>

namespace e2d
{
    const char* factory_loader<label>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "text" : { "type" : "string" },
            "font" : { "$ref": "#/common_definitions/address" },
            "tint" : { "$ref": "#/common_definitions/color" },
            "wrap" : { "$ref": "#/definitions/wraps" },
            "halign" : { "$ref": "#/definitions/haligns" },
            "valign" : { "$ref": "#/definitions/valigns" },
            "leading" : { "type" : "number" },
            "tracking" : { "type" : "number" },
            "glyph_dilate" : { "type" : "number" },
            "outline_width" : { "type" : "number" },
            "outline_color" : { "$ref": "#/common_definitions/color" }
        },
        "definitions" : {
            "wraps" : {
                "type" : "string",
                "enum" : [
                    "no_wrap",
                    "wrap_by_chars",
                    "wrap_by_spaces"
                ]
            },
            "haligns" : {
                "type" : "string",
                "enum" : [
                    "left",
                    "center",
                    "right"
                ]
            },
            "valigns" : {
                "type" : "string",
                "enum" : [
                    "top",
                    "center",
                    "bottom"
                ]
            }
        }
    })json";

    bool factory_loader<label>::operator()(
        label& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("text") ) {
            str text = component.text();
            if ( !json_utils::try_parse_value(ctx.root["text"], text) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'text' property");
                return false;
            }
            component.text(std::move(text));
        }

        if ( ctx.root.HasMember("font") ) {
            auto font = ctx.dependencies.find_asset<font_asset>(
                path::combine(ctx.parent_address, ctx.root["font"].GetString()));
            if ( !font ) {
                the<debug>().error("LABEL: Dependency 'font' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["font"].GetString());
                return false;
            }
            component.font(font);
        }

        if ( ctx.root.HasMember("tint") ) {
            color32 tint = component.tint();
            if ( !json_utils::try_parse_value(ctx.root["tint"], tint) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'tint' property");
                return false;
            }
            component.tint(tint);
        }

        if ( ctx.root.HasMember("wrap") ) {
            label::wraps wrap = component.wrap();
            if ( !json_utils::try_parse_value(ctx.root["wrap"], wrap) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'wrap' property");
                return false;
            }
            component.wrap(wrap);
        }

        if ( ctx.root.HasMember("halign") ) {
            label::haligns halign = component.halign();
            if ( !json_utils::try_parse_value(ctx.root["halign"], halign) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'halign' property");
                return false;
            }
            component.halign(halign);
        }

        if ( ctx.root.HasMember("valign") ) {
            label::valigns valign = component.valign();
            if ( !json_utils::try_parse_value(ctx.root["valign"], valign) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'valign' property");
                return false;
            }
            component.valign(valign);
        }

        if ( ctx.root.HasMember("leading") ) {
            f32 leading = component.leading();
            if ( !json_utils::try_parse_value(ctx.root["leading"], leading) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'leading' property");
                return false;
            }
            component.leading(leading);
        }

        if ( ctx.root.HasMember("tracking") ) {
            f32 tracking = component.tracking();
            if ( !json_utils::try_parse_value(ctx.root["tracking"], tracking) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'tracking' property");
                return false;
            }
            component.tracking(tracking);
        }

        if ( ctx.root.HasMember("glyph_dilate") ) {
            f32 glyph_dilate = component.glyph_dilate();
            if ( !json_utils::try_parse_value(ctx.root["glyph_dilate"], glyph_dilate) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'glyph_dilate' property");
                return false;
            }
            component.glyph_dilate(glyph_dilate);
        }

        if ( ctx.root.HasMember("outline_width") ) {
            f32 outline_width = component.outline_width();
            if ( !json_utils::try_parse_value(ctx.root["outline_width"], outline_width) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'outline_width' property");
                return false;
            }
            component.outline_width(outline_width);
        }

        if ( ctx.root.HasMember("outline_color") ) {
            color32 outline_color = component.outline_color();
            if ( !json_utils::try_parse_value(ctx.root["outline_color"], outline_color) ) {
                the<debug>().error("LABEL: Incorrect formatting of 'outline_color' property");
                return false;
            }
            component.outline_color(outline_color);
        }

        return true;
    }

    bool factory_loader<label>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("font") ) {
            dependencies.add_dependency<font_asset>(
                path::combine(ctx.parent_address, ctx.root["font"].GetString()));
        }

        return true;
    }
}

namespace e2d
{
    const char* component_inspector<label>::title = ICON_FA_PARAGRAPH " label";

    void component_inspector<label>::operator()(gcomponent<label>& c) const {
        if ( bool dirty = c.component<label::dirty>().exists();
            ImGui::Checkbox("dirty", &dirty) )
        {
            if ( dirty ) {
                labels::mark_dirty(c);
            } else {
                labels::unmark_dirty(c);
            }
        }

        ImGui::Separator();

        if ( str text = c->text();
            ImGui::InputTextMultiline("text", &text) )
        {
            labels::change_text(c, std::move(text));
        }

        ///TODO(BlackMat): add 'font' inspector

        if ( color tint = color(c->tint());
            ImGui::ColorEdit4("tint", tint.data()) )
        {
            labels::change_tint(c, color32(tint));
        }

        if ( label::wraps wrap = c->wrap();
            imgui_utils::show_enum_combo_box("wrap", &wrap) )
        {
            labels::change_wrap(c, wrap);
        }

        if ( label::haligns halign = c->halign();
            imgui_utils::show_enum_combo_box("halign", &halign) )
        {
            labels::change_halign(c, halign);
        }

        if ( label::valigns valign = c->valign();
            imgui_utils::show_enum_combo_box("valign", &valign) )
        {
            labels::change_valign(c, valign);
        }

        if ( f32 leading = c->leading();
            ImGui::DragFloat("leading", &leading, 0.01f) )
        {
            labels::change_leading(c, leading);
        }

        if ( f32 tracking = c->tracking();
            ImGui::DragFloat("tracking", &tracking, 0.01f) )
        {
            labels::change_tracking(c, tracking);
        }

        if ( f32 glyph_dilate = c->glyph_dilate();
            ImGui::SliderFloat("glyph_dilate", &glyph_dilate, -1.f, 1.f) )
        {
            labels::change_glyph_dilate(c, glyph_dilate);
        }

        if ( f32 outline_width = c->outline_width();
            ImGui::SliderFloat("outline_width", &outline_width, 0.f, 1.f) )
        {
            labels::change_outline_width(c, outline_width);
        }

        if ( color outline_color = color(c->outline_color());
            ImGui::ColorEdit4("outline_color", outline_color.data()) )
        {
            labels::change_outline_color(c, color32(outline_color));
        }
    }
}

namespace e2d::labels
{
    gcomponent<label> mark_dirty(gcomponent<label> self) {
        if ( self ) {
            self.component<label::dirty>().ensure();
        }
        return self;
    }

    gcomponent<label> unmark_dirty(gcomponent<label> self) {
        if ( self ) {
            self.component<label::dirty>().remove();
        }
        return self;
    }

    bool is_dirty(const const_gcomponent<label>& self) noexcept {
        return self.component<label::dirty>().exists();
    }

    gcomponent<label> change_text(gcomponent<label> self, str value) {
        if ( self ) {
            self->text(std::move(value));
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_font(gcomponent<label> self, const font_asset::ptr& value) {
        if ( self ) {
            self->font(value);
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_tint(gcomponent<label> self, const color32& value) {
        if ( self ) {
            self->tint(value);
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_wrap(gcomponent<label> self, label::wraps value) {
        if ( self ) {
            self->wrap(value);
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_halign(gcomponent<label> self, label::haligns value) {
        if ( self ) {
            self->halign(value);
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_valign(gcomponent<label> self, label::valigns value) {
        if ( self ) {
            self->valign(value);
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_leading(gcomponent<label> self, f32 value) {
        if ( self ) {
            self->leading(value);
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_tracking(gcomponent<label> self, f32 value) {
        if ( self ) {
            self->tracking(value);
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_glyph_dilate(gcomponent<label> self, f32 value) {
        if ( self ) {
            self->glyph_dilate(value);
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_outline_width(gcomponent<label> self, f32 value) {
        if ( self ) {
            self->outline_width(value);
        }
        return mark_dirty(self);
    }

    gcomponent<label> change_outline_color(gcomponent<label> self, const color32& value) {
        if ( self ) {
            self->outline_color(value);
        }
        return mark_dirty(self);
    }
}
