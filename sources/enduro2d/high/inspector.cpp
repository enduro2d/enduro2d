/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/inspector.hpp>

namespace e2d
{
    void inspector::show_for(gobject go) {
        std::lock_guard<std::mutex> guard(mutex_);

        for ( const auto& p : inspector_impls_ ) {
            (*p.second.drawer)(go);
        }

        ImGui::Separator();

        {
            const char* add_component_popup_str_id = "e2d_add_component_popup";

            if ( ImGui::Button("+ Add Component") ) {
                ImGui::OpenPopup(add_component_popup_str_id);
            }

            if ( ImGui::BeginPopup(add_component_popup_str_id) ) {
                E2D_DEFER([](){ ImGui::EndPopup(); });

                for ( const auto& p : inspector_impls_ ) {
                    if ( ImGui::Button(p.second.creator->title()) ) {
                        p.second.creator->ensure(go);
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
        }
    }

    void inspector::show_for(
        gobject go,
        component_inspector<>::gizmos_context& ctx)
    {
        std::lock_guard<std::mutex> guard(mutex_);
        for ( auto& p : inspector_impls_ ) {
            (*p.second.drawer)(go, ctx);
        }
    }
}
