/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/core/dbgui.hpp>

#include <enduro2d/core/debug.hpp>
#include <enduro2d/core/engine.hpp>
#include <enduro2d/core/input.hpp>
#include <enduro2d/core/render.hpp>
#include <enduro2d/core/window.hpp>

#include <3rdparty/imgui/imgui.h>
#include <3rdparty/imgui/imgui_internal.h>

namespace e2d::imgui
{
    const char* vertex_source_cstr() noexcept;
    const char* fragment_source_cstr() noexcept;

    window::cursor_shapes convert_mouse_cursor(ImGuiMouseCursor mc) noexcept;
}

namespace e2d::imgui
{
    class imgui_event_listener final : public window::event_listener {
    public:
        imgui_event_listener(ImGuiIO& io, window& w);

        void on_input_char(char32_t uchar) noexcept final;
        void on_move_cursor(const v2f& pos) noexcept final;
        void on_mouse_scroll(const v2f& delta) noexcept final;
        void on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept final;
    private:
        ImGuiIO& io_;
        window& window_;
    };
}

namespace e2d::imgex
{
    bool show_button(str_view label);

    bool show_checkbox(str_view label, bool* value);
    void show_disabled_checkbox(str_view label, bool value);

    template < typename... Args >
    void show_text(str_view text, Args&&... args) {
        if constexpr ( sizeof... (Args) ) {
            ImGui::TextUnformatted(strings::rformat(
                text,
                std::forward<Args>(args)...).c_str());
        } else {
            char* text_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(text.size() + 1));
            std::memcpy(text_cstr, text.data(), text.size());
            ImGui::TextUnformatted(text_cstr);
        }
    }

    template < typename... Args >
    void show_colored_text(str_view text, const color& color, Args&&... args) {
        ImGui::PushStyleColor(ImGuiCol_Text, make_vec4(color));
        E2D_DEFER([](){ ImGui::PopStyleColor(); });

        show_text(text, std::forward<Args>(args)...);
    }
}

namespace e2d::imgex
{
    template < typename F >
    void with_child(str_view id, bool border, ImGuiWindowFlags flags, F&& f) {
        char* id_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(id.size() + 1));
        std::memcpy(id_cstr, id.data(), id.size());

        if ( E2D_DEFER([](){ ImGui::EndChild(); });
            ImGui::BeginChild(id_cstr, ImVec2(0.f, 0.f), border, flags) )
        {
            f();
        }
    }

    template < typename F >
    void with_menu(str_view label, F&& f) {
        char* label_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(label.size() + 1));
        std::memcpy(label_cstr, label.data(), label.size());

        if ( ImGui::BeginMenu(label_cstr) ) {
            E2D_DEFER([](){ ImGui::EndMenu(); });

            f();
        }
    }

    template < typename F >
    void with_menu_bar(F&& f) {
        if ( ImGui::BeginMenuBar() ) {
            E2D_DEFER([](){ ImGui::EndMenuBar(); });

            f();
        }
    }

    template < typename F >
    void with_popup(str_view id, F&& f) {
        char* id_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(id.size() + 1));
        std::memcpy(id_cstr, id.data(), id.size());

        if ( ImGui::BeginPopup(id_cstr) ) {
            E2D_DEFER([](){ ImGui::EndPopup(); });
            f();
        }
    }

    template < typename F >
    void with_window(str_view name, bool* opened, ImGuiWindowFlags flags, F&& f) {
        char* name_cstr = static_cast<char*>(E2D_CLEAR_ALLOCA(name.size() + 1));
        std::memcpy(name_cstr, name.data(), name.size());

        if ( E2D_DEFER([](){ ImGui::End(); });
            ImGui::Begin(name_cstr, opened, flags) )
        {
            f();
        }
    }

    template < typename F >
    void with_main_dock_space(F&& f) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoBringToFrontOnFocus;

        ImGuiDockNodeFlags dock_node_flags =
            ImGuiDockNodeFlags_PassthruCentralNode |
            ImGuiDockNodeFlags_NoDockingInCentralNode;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));

        ImGui::Begin("e2d_dock_space_window", nullptr, window_flags);
        E2D_DEFER([](){ ImGui::End(); });

        ImGui::PopStyleVar(3);

        ImGui::DockSpace(
            ImGui::GetID("e2d_dock_space"),
            ImVec2(0.f, 0.f),
            dock_node_flags);

        f();
    }
}
