/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "property_drawer.hpp"

#include <enduro2d/high/property.hpp>

namespace e2d::dbgui_widgets
{
    //
    // f32
    //

    bool property_drawer::operator()(str_view l, f32& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());
        return ImGui::InputFloat(l_cstr, &p);
    }

    bool property_drawer::operator()(str_view l, property<f32>& p) const {
        f32 v = *p;
        if ( (*this)(l, v) ) {
            p = v;
            return true;
        }
        return false;
    }

    bool property_drawer::operator()(str_view l, drag_property<f32>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        f32 v = *p.value();
        bool changed = false;

        if ( p.step() && p.range() ) {
            changed = ImGui::DragFloat(l_cstr, &v, *p.step(), p.range()->first, p.range()->second);
        } else if ( p.step() ) {
            changed = ImGui::DragFloat(l_cstr, &v, *p.step());
        } else {
            changed = ImGui::DragFloat(l_cstr, &v);
        }

        if ( changed ) {
            p.value() = v;
        }

        return changed;
    }

    bool property_drawer::operator()(str_view l, slider_property<f32>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        f32 v = *p.value();
        bool changed = false;

        changed = ImGui::SliderFloat(l_cstr, &v, p.range().first, p.range().second);

        if ( changed ) {
            p.value() = v;
        }

        return changed;
    }

    //
    // v2f
    //

    bool property_drawer::operator()(str_view l, v2f& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());
        return ImGui::InputFloat2(l_cstr, p.data());
    }

    bool property_drawer::operator()(str_view l, property<v2f>& p) const {
        v2f v = *p;
        if ( (*this)(l, v) ) {
            p = v;
            return true;
        }
        return false;
    }

    bool property_drawer::operator()(str_view l, drag_property<v2f>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        v2f v = *p.value();
        bool changed = false;

        if ( p.step() && p.range() ) {
            changed = ImGui::DragFloat2(l_cstr, v.data(), *p.step(), p.range()->first, p.range()->second);
        } else if ( p.step() ) {
            changed = ImGui::DragFloat2(l_cstr, v.data(), *p.step());
        } else {
            changed = ImGui::DragFloat2(l_cstr, v.data());
        }

        if ( changed ) {
            p.value() = v;
        }

        return changed;
    }

    bool property_drawer::operator()(str_view l, slider_property<v2f>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        v2f v = *p.value();
        bool changed = false;

        changed = ImGui::SliderFloat2(l_cstr, v.data(), p.range().first, p.range().second);

        if ( changed ) {
            p.value() = v;
        }

        return changed;
    }

    //
    // v3f
    //

    bool property_drawer::operator()(str_view l, v3f& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());
        return ImGui::InputFloat3(l_cstr, p.data());
    }

    bool property_drawer::operator()(str_view l, property<v3f>& p) const {
        v3f v = *p;
        if ( (*this)(l, v) ) {
            p = v;
            return true;
        }
        return false;
    }

    bool property_drawer::operator()(str_view l, drag_property<v3f>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        v3f v = *p.value();
        bool changed = false;

        if ( p.step() && p.range() ) {
            changed = ImGui::DragFloat3(l_cstr, v.data(), *p.step(), p.range()->first, p.range()->second);
        } else if ( p.step() ) {
            changed = ImGui::DragFloat3(l_cstr, v.data(), *p.step());
        } else {
            changed = ImGui::DragFloat3(l_cstr, v.data());
        }

        if ( changed ) {
            p.value() = v;
        }

        return changed;
    }

    bool property_drawer::operator()(str_view l, slider_property<v3f>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        v3f v = *p.value();
        bool changed = false;

        changed = ImGui::SliderFloat3(l_cstr, v.data(), p.range().first, p.range().second);

        if ( changed ) {
            p.value() = v;
        }

        return changed;
    }

    //
    // v4f
    //

    bool property_drawer::operator()(str_view l, v4f& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());
        return ImGui::InputFloat4(l_cstr, p.data());
    }

    bool property_drawer::operator()(str_view l, property<v4f>& p) const {
        v4f v = *p;
        if ( (*this)(l, v) ) {
            p = v;
            return true;
        }
        return false;
    }

    bool property_drawer::operator()(str_view l, drag_property<v4f>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        v4f v = *p.value();
        bool changed = false;

        if ( p.step() && p.range() ) {
            changed = ImGui::DragFloat4(l_cstr, v.data(), *p.step(), p.range()->first, p.range()->second);
        } else if ( p.step() ) {
            changed = ImGui::DragFloat4(l_cstr, v.data(), *p.step());
        } else {
            changed = ImGui::DragFloat4(l_cstr, v.data());
        }

        if ( changed ) {
            p.value() = v;
        }

        return changed;
    }

    bool property_drawer::operator()(str_view l, slider_property<v4f>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        v4f v = *p.value();
        bool changed = false;

        changed = ImGui::SliderFloat4(l_cstr, v.data(), p.range().first, p.range().second);

        if ( changed ) {
            p.value() = v;
        }

        return changed;
    }

    //
    // str
    //

    bool property_drawer::operator()(str_view l, str& p) const {
        return imgex::show_input_text(l, &p);
    }

    bool property_drawer::operator()(str_view l, property<str>& p) const {
        str v = *p;
        if ( (*this)(l, v) ) {
            p = std::move(v);
            return true;
        }
        return false;
    }

    bool property_drawer::operator()(str_view l, text_property& p) const {
        str v = *p.value();
        bool changed = false;

        if ( p.multiline() ) {
            changed = imgex::show_input_text_multiline(l, &v);
        } else {
            changed = imgex::show_input_text(l, &v);
        }

        if ( changed ) {
            p.value() = std::move(v);
        }

        return changed;
    }

    //
    // degf
    //

    bool property_drawer::operator()(str_view l, degf& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());
        return ImGui::InputFloat(l_cstr, &p.value);
    }

    bool property_drawer::operator()(str_view l, property<degf>& p) const {
        degf v = *p;
        if ( (*this)(l, v) ) {
            p = v;
            return true;
        }
        return false;
    }

    bool property_drawer::operator()(str_view l, drag_property<degf>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        f32 v = p.value()->value;
        bool changed = false;

        if ( p.step() && p.range() ) {
            changed = ImGui::DragFloat(l_cstr, &v, p.step()->value, p.range()->first.value, p.range()->second.value);
        } else if ( p.step() ) {
            changed = ImGui::DragFloat(l_cstr, &v, p.step()->value);
        } else {
            changed = ImGui::DragFloat(l_cstr, &v);
        }

        if ( changed ) {
            p.value() = make_deg(v);
        }

        return changed;
    }

    bool property_drawer::operator()(str_view l, slider_property<degf>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        f32 v = p.value()->value;
        bool changed = false;

        changed = ImGui::SliderFloat(l_cstr, &v, p.range().first.value, p.range().second.value);

        if ( changed ) {
            p.value() = make_deg(v);
        }

        return changed;
    }

    //
    // radf
    //

    bool property_drawer::operator()(str_view l, radf& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());
        return ImGui::InputFloat(l_cstr, &p.value);
    }

    bool property_drawer::operator()(str_view l, property<radf>& p) const {
        radf v = *p;
        if ( (*this)(l, v) ) {
            p = v;
            return true;
        }
        return false;
    }

    bool property_drawer::operator()(str_view l, drag_property<radf>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        f32 v = p.value()->value;
        bool changed = false;

        if ( p.step() && p.range() ) {
            changed = ImGui::DragFloat(l_cstr, &v, p.step()->value, p.range()->first.value, p.range()->second.value);
        } else if ( p.step() ) {
            changed = ImGui::DragFloat(l_cstr, &v, p.step()->value);
        } else {
            changed = ImGui::DragFloat(l_cstr, &v);
        }

        if ( changed ) {
            p.value() = make_rad(v);
        }

        return changed;
    }

    bool property_drawer::operator()(str_view l, slider_property<radf>& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());

        f32 v = p.value()->value;
        bool changed = false;

        changed = ImGui::SliderFloat(l_cstr, &v, p.range().first.value, p.range().second.value);

        if ( changed ) {
            p.value() = make_rad(v);
        }

        return changed;
    }

    //
    // color
    //

    bool property_drawer::operator()(str_view l, color& p) const {
        char* l_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(l.size() + 1));
        std::memcpy(l_cstr, l.data(), l.size());
        return ImGui::ColorEdit4(l_cstr, p.data());
    }

    bool property_drawer::operator()(str_view l, property<color>& p) const {
        color v = *p;
        if ( (*this)(l, v) ) {
            p = v;
            return true;
        }
        return false;
    }

    //
    // color32
    //

    bool property_drawer::operator()(str_view l, color32& p) const {
        color v = color(p);
        if ( (*this)(l, v) ) {
            p = color32(v);
            return true;
        }
        return false;
    }

    bool property_drawer::operator()(str_view l, property<color32>& p) const {
        color v = color(*p);
        if ( (*this)(l, v) ) {
            p = color32(v);
            return true;
        }
        return false;
    }
}
