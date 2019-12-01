/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "dbgui.hpp"

namespace
{
    using namespace e2d;

    void show_main_menu_bar() {
        static bool show_engine = false;
        static bool show_window = false;

        if ( ImGui::BeginMenuBar() ) {
            E2D_DEFER([](){ ImGui::EndMenuBar(); });

            if ( ImGui::BeginMenu("Debug") ) {
                E2D_DEFER([](){ ImGui::EndMenu(); });

                ImGui::MenuItem("Engine...", nullptr, &show_engine);
                ImGui::MenuItem("Window...", nullptr, &show_window);

                ImGui::Separator();

                if ( ImGui::MenuItem("Quit") ) {
                    if ( modules::is_initialized<window>() ) {
                        the<window>().set_should_close(true);
                    }
                }
            }
        }

        if ( show_engine ) {
            imgui::windows::show_debug_engine(&show_engine);
        }

        if ( show_window ) {
            imgui::windows::show_debug_window(&show_window);
        }
    }
}

namespace e2d::imgui
{
    void show_main_dock_space() {
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

        show_main_menu_bar();
    }
}
