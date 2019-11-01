/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/label.hpp>

namespace e2d::bindings::high
{
    void bind_label(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["components"].get_or_create<sol::table>()
        .new_usertype<gcomponent<label>>("label",
            "text", sol::property(
                [](const gcomponent<label>& l){
                    return l->text();
                },
                [](gcomponent<label>& l, str v){
                    l->text(std::move(v));
                    l.owner().component<label::dirty>().assign();
                }),
            "font", sol::property(
                [](const gcomponent<label>& l){
                    return l->font();
                },
                [](gcomponent<label>& l, const font_asset::ptr& v){
                    l->font(v);
                    l.owner().component<label::dirty>().assign();
                }),
            "tint", sol::property(
                [](const gcomponent<label>& l){
                    return l->tint();
                },
                [](gcomponent<label>& l, const color32& v){
                    l->tint(v);
                    l.owner().component<label::dirty>().assign();
                }),
            "halign", sol::property(
                [](const gcomponent<label>& l){
                    return l->halign();
                },
                [](gcomponent<label>& l, label::haligns v){
                    l->halign(v);
                    l.owner().component<label::dirty>().assign();
                }),
            "valign", sol::property(
                [](const gcomponent<label>& l){
                    return l->valign();
                },
                [](gcomponent<label>& l, label::valigns v){
                    l->valign(v);
                    l.owner().component<label::dirty>().assign();
                }),
            "leading", sol::property(
                [](const gcomponent<label>& l){
                    return l->leading();
                },
                [](gcomponent<label>& l, f32 v){
                    l->leading(v);
                    l.owner().component<label::dirty>().assign();
                }),
            "tracking", sol::property(
                [](const gcomponent<label>& l){
                    return l->tracking();
                },
                [](gcomponent<label>& l, f32 v){
                    l->tracking(v);
                    l.owner().component<label::dirty>().assign();
                }),
            "text_width", sol::property(
                [](const gcomponent<label>& l){
                    return l->text_width();
                },
                [](gcomponent<label>& l, f32 v){
                    l->text_width(v);
                    l.owner().component<label::dirty>().assign();
                }),
            "glyph_dilate", sol::property(
                [](const gcomponent<label>& l){
                    return l->glyph_dilate();
                },
                [](gcomponent<label>& l, f32 v){
                    l->glyph_dilate(v);
                    l.owner().component<label::dirty>().assign();
                }),
            "outline_width", sol::property(
                [](const gcomponent<label>& l){
                    return l->outline_width();
                },
                [](gcomponent<label>& l, f32 v){
                    l->outline_width(v);
                    l.owner().component<label::dirty>().assign();
                }),
            "outline_color", sol::property(
                [](const gcomponent<label>& l){
                    return l->outline_color();
                },
                [](gcomponent<label>& l, const color32& v){
                    l->outline_color(v);
                    l.owner().component<label::dirty>().assign();
                })
        );

    #define LABEL_HALIGN_PAIR(x) {#x, label::haligns::x},
        l["e2d"].get_or_create<sol::table>()
        ["label"].get_or_create<sol::table>()
        .new_enum<label::haligns>("haligns", {
            LABEL_HALIGN_PAIR(left)
            LABEL_HALIGN_PAIR(center)
            LABEL_HALIGN_PAIR(right)
        });
    #undef LABEL_HALIGN_PAIR

    #define LABEL_VALIGN_PAIR(x) {#x, label::valigns::x},
        l["e2d"].get_or_create<sol::table>()
        ["label"].get_or_create<sol::table>()
        .new_enum<label::valigns>("valigns", {
            LABEL_VALIGN_PAIR(top)
            LABEL_VALIGN_PAIR(center)
            LABEL_VALIGN_PAIR(bottom)
            LABEL_VALIGN_PAIR(baseline)
        });
    #undef LABEL_VALIGN_PAIR
    }
}
