/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "color.hpp"
#include "color32.hpp"
#include "strings.hpp"

#include <3rdparty/imgui/imgui.h>
#include <3rdparty/imgui/imgui_stdlib.h>
#include <3rdparty/imgui/imgui_internal.h>

#include <3rdparty/icons/icon_fa.h>
#include <3rdparty/icons/icon_md.h>

namespace e2d::imgui_utils
{
    template < typename... Args >
    void show_formatted_text(str_view fmt, Args&&... args) {
        if constexpr ( sizeof... (Args) ) {
            ImGui::TextUnformatted(strings::rformat(
                fmt,
                std::forward<Args>(args)...).c_str());
        } else {
            char* fmt_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(fmt.size() + 1));
            std::memcpy(fmt_cstr, fmt.data(), fmt.size());
            ImGui::TextUnformatted(fmt_cstr);
        }
    }

    template < typename... Args >
    void show_colored_text(const color& color, str_view fmt, Args&&... args) {
        ImGui::PushStyleColor(ImGuiCol_Text, make_vec4(color));
        DEFER([](){ ImGui::PopStyleColor(); });

        show_formatted_text(fmt, std::forward<Args>(args)...);
    }

    template < typename E >
    bool show_enum_combo_box(str_view label, E* value) {
        char* label_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(label.size() + 1));
        std::memcpy(label_cstr, label.data(), label.size());

        std::optional<str_view> preview = enum_hpp::to_string(*value);
        char* preview_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA((preview ? preview->size() : 0) + 1));
        if ( preview ) {
            std::memcpy(preview_cstr, preview->data(), preview->size());
        }

        if ( ImGui::BeginCombo(label_cstr, preview_cstr) ) {
            DEFER([](){ ImGui::EndCombo(); });

            for ( std::size_t i = 0; i < enum_hpp::size<E>(); ++i ) {
                str_view item_name = enum_hpp::names<E>()[i];
                char* item_name_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(item_name.size() + 1));
                std::memcpy(item_name_cstr, item_name.data(), item_name.size());

                bool selected = *value == enum_hpp::values<E>()[i];
                if ( ImGui::Selectable(item_name_cstr, selected) ) {
                    *value = enum_hpp::values<E>()[i];
                    return true;
                } else if ( selected ) {
                    ImGui::SetItemDefaultFocus();
                }
            }
        }

        return false;
    }
}

namespace e2d::imgui_utils
{
    template < typename F, typename... Args >
    void with_disabled_flag(F&& f, Args&&... args) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        DEFER([](){ ImGui::PopItemFlag(); });

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        DEFER([](){ ImGui::PopStyleVar(); });

        std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template < typename F, typename... Args >
    void with_disabled_flag_ex(bool disabled, F&& f, Args&&... args) {
        if ( disabled ) {
            with_disabled_flag(std::forward<F>(f), std::forward<Args>(args)...);
        } else {
            std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        }
    }
}
