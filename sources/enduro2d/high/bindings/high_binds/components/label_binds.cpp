/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/label.hpp>

namespace e2d::bindings::high
{
    void bind_label(sol::state& l) {
        l.new_usertype<gcomponent<label>>("label",
            sol::no_constructor,

            "dirty", sol::property(
                [](const gcomponent<label>& c) -> bool {
                    return labels::is_dirty(c);
                },
                [](gcomponent<label>& c, bool yesno){
                    if ( yesno ) {
                        labels::mark_dirty(c);
                    } else {
                        labels::unmark_dirty(c);
                    }
                }
            ),

            "text", sol::property(
                [](const gcomponent<label>& c) -> str {
                    return c->text();
                },
                [](gcomponent<label>& c, str_view v){
                    labels::change_text(c, str(v));
                }),

            "font", sol::property(
                [](const gcomponent<label>& c) -> font_asset::ptr {
                    return c->font();
                },
                [](gcomponent<label>& c, const font_asset::ptr& v){
                    labels::change_font(c, v);
                }),

            "tint", sol::property(
                [](const gcomponent<label>& c) -> color32 {
                    return c->tint();
                },
                [](gcomponent<label>& c, const color32& v){
                    labels::change_tint(c, v);
                }),

            "halign", sol::property(
                [](const gcomponent<label>& c) -> label::haligns {
                    return c->halign();
                },
                [](gcomponent<label>& c, label::haligns v){
                    labels::change_halign(c, v);
                }),

            "valign", sol::property(
                [](const gcomponent<label>& c) -> label::valigns {
                    return c->valign();
                },
                [](gcomponent<label>& c, label::valigns v){
                    labels::change_valign(c, v);
                }),

            "leading", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->leading();
                },
                [](gcomponent<label>& c, f32 v){
                    labels::change_leading(c, v);
                }),

            "tracking", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->tracking();
                },
                [](gcomponent<label>& c, f32 v){
                    labels::change_tracking(c, v);
                }),

            "text_width", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->text_width();
                },
                [](gcomponent<label>& c, f32 v){
                    labels::change_text_width(c, v);
                }),

            "glyph_dilate", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->glyph_dilate();
                },
                [](gcomponent<label>& c, f32 v){
                    labels::change_glyph_dilate(c, v);
                }),

            "outline_width", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->outline_width();
                },
                [](gcomponent<label>& c, f32 v){
                    labels::change_outline_width(c, v);
                }),

            "outline_color", sol::property(
                [](const gcomponent<label>& c) -> color32 {
                    return c->outline_color();
                },
                [](gcomponent<label>& c, const color32& v){
                    labels::change_outline_color(c, v);
                })
        );

    #define LABEL_HALIGN_PAIR(x) {#x, label::haligns::x},
        l["label"].get_or_create<sol::table>()
        .new_enum<label::haligns>("haligns", {
            LABEL_HALIGN_PAIR(left)
            LABEL_HALIGN_PAIR(center)
            LABEL_HALIGN_PAIR(right)
        });
    #undef LABEL_HALIGN_PAIR

    #define LABEL_VALIGN_PAIR(x) {#x, label::valigns::x},
        l["label"].get_or_create<sol::table>()
        .new_enum<label::valigns>("valigns", {
            LABEL_VALIGN_PAIR(top)
            LABEL_VALIGN_PAIR(center)
            LABEL_VALIGN_PAIR(bottom)
            LABEL_VALIGN_PAIR(baseline)
        });
    #undef LABEL_VALIGN_PAIR
    }
}
