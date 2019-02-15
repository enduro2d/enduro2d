/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "dbgui.hpp"

namespace e2d { namespace dbgui_widgets
{
    void show_main_menu() {
        if ( !modules::is_initialized<window>() ) {
            return;
        }
        window& w = the<window>();
        if ( ImGui::BeginMainMenuBar() ) {
            if ( ImGui::BeginMenu("Window") ) {
                if ( w.fullscreen() ) {
                    if ( ImGui::MenuItem("Exit Full Screen") ) {
                        w.toggle_fullscreen(false);
                    }
                } else {
                    if ( ImGui::MenuItem("Enter Full Screen") ) {
                        w.toggle_fullscreen(true);
                    }
                }
                ImGui::Separator();
                if ( ImGui::MenuItem("Close", "Cmd+Q") ) {
                    w.set_should_close(true);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void show_engine_stats() {
        if ( !modules::is_initialized<engine>() ) {
            return;
        }
        const engine& e = the<engine>();
        if ( ImGui::Begin("Engine info") ) {
            const f32 delta_time = e.delta_time();
            const u32 frame_rate = e.frame_rate();
            const u32 frame_count = e.frame_count();
            ImGui::Text("delta time: %f", delta_time);
            ImGui::Text("frame rate: %u", frame_rate);
            ImGui::Text("frame count: %u", frame_count);
        }
        ImGui::End();
    }

    void show_window_stats() {
        if ( !modules::is_initialized<window>() ) {
            return;
        }
        const window& w = the<window>();
        if ( ImGui::Begin("Window Stats") ) {
            v2u real_size = w.real_size();
            v2u virtual_size = w.virtual_size();
            v2u framebuffer_size = w.framebuffer_size();
            ImGui::Text("real size: (%u,%u)", real_size.x, real_size.y);
            ImGui::Text("virtual size: (%u,%u)", virtual_size.x, virtual_size.y);
            ImGui::Text("framebuffer size: (%u,%u)", framebuffer_size.x, framebuffer_size.y);
        }
        ImGui::End();
    }
}}
