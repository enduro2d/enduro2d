/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "input_widget.hpp"

namespace e2d::dbgui_widgets
{
    bool input_widget::show() {
        if ( !modules::is_initialized<input>() ) {
            return false;
        }

        input& i = the<input>();

        {
            imgui_utils::show_formatted_text(
                "cursor pos: %0",
                strings::make_format_arg(i.mouse().cursor_pos(), 0u, 2u));

            imgui_utils::show_formatted_text(
                "scroll delta: %0",
                strings::make_format_arg(i.mouse().scroll_delta(), 0u, 2u));

            ImGui::Separator();

            imgui_utils::show_formatted_text(
                "input text: %0",
                i.keyboard().input_text());

            {
                ImGui::Text("pressed keys:");

                ImGui::Indent();
                DEFER([](){ ImGui::Unindent(); });

                const vector<keyboard_key> pressed_keys = i.keyboard().pressed_keys();
                for ( keyboard_key pk : pressed_keys ) {
                    imgui_utils::show_formatted_text("%0", pk);
                }
            }

            {
                ImGui::Text("pressed buttons:");

                ImGui::Indent();
                DEFER([](){ ImGui::Unindent(); });

                const vector<mouse_button> pressed_buttons = i.mouse().pressed_buttons();
                for ( mouse_button mb : pressed_buttons ) {
                    imgui_utils::show_formatted_text("%0", mb);
                }
            }
        }

        return true;
    }

    const input_widget::description& input_widget::desc() const noexcept {
        return desc_;
    }
}
