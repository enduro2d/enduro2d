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

namespace e2d::imgui
{
    template < typename F >
    void show_main_dock_space(F&& f) {
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
