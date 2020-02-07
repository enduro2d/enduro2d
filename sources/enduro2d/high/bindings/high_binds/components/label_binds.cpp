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
                    return c.owner().component<label::dirty>().exists();
                },
                [](gcomponent<label>& c, bool yesno){
                    if ( yesno ) {
                        c.owner().component<label::dirty>().ensure();
                    } else {
                        c.owner().component<label::dirty>().remove();
                    }
                }
            ),

            "text", sol::property(
                [](const gcomponent<label>& c) -> str {
                    return c->text();
                },
                [](gcomponent<label>& c, str_view v){
                    c->text(str(v));
                    c.owner().component<label::dirty>().ensure();
                }),

            "font", sol::property(
                [](const gcomponent<label>& c) -> font_asset::ptr {
                    return c->font();
                },
                [](gcomponent<label>& c, const font_asset::ptr& v){
                    c->font(v);
                    c.owner().component<label::dirty>().ensure();
                }),

            "tint", sol::property(
                [](const gcomponent<label>& c) -> color32 {
                    return c->tint();
                },
                [](gcomponent<label>& c, const color32& v){
                    c->tint(v);
                    c.owner().component<label::dirty>().ensure();
                }),

            "halign", sol::property(
                [](const gcomponent<label>& c) -> label::haligns {
                    return c->halign();
                },
                [](gcomponent<label>& c, label::haligns v){
                    c->halign(v);
                    c.owner().component<label::dirty>().ensure();
                }),

            "valign", sol::property(
                [](const gcomponent<label>& c) -> label::valigns {
                    return c->valign();
                },
                [](gcomponent<label>& c, label::valigns v){
                    c->valign(v);
                    c.owner().component<label::dirty>().ensure();
                }),

            "leading", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->leading();
                },
                [](gcomponent<label>& c, f32 v){
                    c->leading(v);
                    c.owner().component<label::dirty>().ensure();
                }),

            "tracking", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->tracking();
                },
                [](gcomponent<label>& c, f32 v){
                    c->tracking(v);
                    c.owner().component<label::dirty>().ensure();
                }),

            "text_width", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->text_width();
                },
                [](gcomponent<label>& c, f32 v){
                    c->text_width(v);
                    c.owner().component<label::dirty>().ensure();
                }),

            "glyph_dilate", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->glyph_dilate();
                },
                [](gcomponent<label>& c, f32 v){
                    c->glyph_dilate(v);
                    c.owner().component<label::dirty>().ensure();
                }),

            "outline_width", sol::property(
                [](const gcomponent<label>& c) -> f32 {
                    return c->outline_width();
                },
                [](gcomponent<label>& c, f32 v){
                    c->outline_width(v);
                    c.owner().component<label::dirty>().ensure();
                }),

            "outline_color", sol::property(
                [](const gcomponent<label>& c) -> color32 {
                    return c->outline_color();
                },
                [](gcomponent<label>& c, const color32& v){
                    c->outline_color(v);
                    c.owner().component<label::dirty>().ensure();
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
