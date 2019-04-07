/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "dbgui.hpp"

namespace
{
    using namespace e2d;

    void show_debug_engine(bool* open) {
        if ( !modules::is_initialized<engine>() ) {
            if ( open ) {
                *open = false;
            }
            return;
        }
        engine& e = the<engine>();
        const char* window_title = "Debug Engine";
        if ( !ImGui::Begin(window_title, open, ImGuiWindowFlags_NoResize) ) {
            ImGui::End();
            return;
        }
        try {
            {
                ImGui::Text("%s", strings::rformat("time: %0", e.time()).c_str());
                ImGui::Text("%s", strings::rformat("delta time: %0", e.delta_time()).c_str());
            }
            ImGui::Separator();
            {
                ImGui::Text("%s", strings::rformat("frame rate: %0", e.frame_rate()).c_str());
                ImGui::Text("%s", strings::rformat("frame count: %0", e.frame_count()).c_str());
                ImGui::Text("%s", strings::rformat("realtime time: %0", e.realtime_time()).c_str());
            }
            ImGui::SetWindowSize(window_title, v2f::zero());
        } catch (...) {
            ImGui::End();
            throw;
        }
        ImGui::End();
    }

    void show_debug_window(bool* open) {
        if ( !modules::is_initialized<window>() ) {
            if ( open ) {
                *open = false;
            }
            return;
        }
        window& w = the<window>();
        const char* window_title = "Debug Window";
        if ( !ImGui::Begin(window_title, open, ImGuiWindowFlags_NoResize) ) {
            ImGui::End();
            return;
        }
        try {
            {
                bool visible = w.visible();
                if ( ImGui::Checkbox("visible", &visible) ) {
                    if ( visible ) {
                        w.show();
                    } else {
                        w.hide();
                    }
                }
            }
            {
                bool focused = w.focused();
                ImGui::Checkbox("focused", &focused);
            }
            {
                bool minimized = w.minimized();
                if ( ImGui::Checkbox("minimized", &minimized) ) {
                    if ( minimized ) {
                        w.minimize();
                    } else {
                        w.restore();
                    }
                }
            }
            ImGui::Separator();
            {
                ImGui::Text("%s", strings::rformat("real size: %0", w.real_size()).c_str());
                ImGui::Text("%s", strings::rformat("virtual size: %0", w.virtual_size()).c_str());
                ImGui::Text("%s", strings::rformat("framebuffer size: %0", w.framebuffer_size()).c_str());
            }
            ImGui::Separator();
            {
                bool fullscreen = w.fullscreen();
                if ( ImGui::Checkbox("fullscreen", &fullscreen) ) {
                    w.toggle_fullscreen(fullscreen);
                }
            }
            {
                bool cursor = !w.is_cursor_hidden();
                if ( ImGui::Checkbox("system cursor", &cursor) ) {
                    if ( cursor ) {
                        w.show_cursor();
                    } else {
                        w.hide_cursor();
                    }
                }
            }
            {
                char title_buf[128] = {0};
                strings::format(title_buf, E2D_COUNTOF(title_buf), w.title());
                if ( ImGui::InputText("title", title_buf, E2D_COUNTOF(title_buf)) ) {
                    w.set_title(title_buf);
                }
            }
            ImGui::SetWindowSize(window_title, v2f::zero());
        } catch (...) {
            ImGui::End();
            throw;
        }
        ImGui::End();
    }
}

namespace e2d { namespace dbgui_widgets
{
    void show_main_menu() {
        static bool show_engine = false;
        static bool show_window = false;

        if ( ImGui::BeginMainMenuBar() ) {
            if ( ImGui::BeginMenu("Debug") ) {
                ImGui::MenuItem("Engine...", nullptr, &show_engine);
                ImGui::MenuItem("Window...", nullptr, &show_window);
                ImGui::Separator();
                if ( ImGui::MenuItem("Quit") ) {
                    if ( modules::is_initialized<window>() ) {
                        the<window>().set_should_close(true);
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if ( show_engine ) {
            show_debug_engine(&show_engine);
        }

        if ( show_window ) {
            show_debug_window(&show_window);
        }
    }
}}
