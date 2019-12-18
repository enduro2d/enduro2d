/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "hierarchy_widget.hpp"

#include <enduro2d/high/editor.hpp>
#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/node.hpp>
#include <enduro2d/high/world.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/named.hpp>
#include <enduro2d/high/components/scene.hpp>

namespace
{
    using namespace e2d;

    void show_tree_for_node(const const_node_iptr& root) {
        const gobject owner = root ? root->owner() : gobject();
        if ( !owner ) {
            return;
        }

        const str basic_name = owner.component<scene>().exists()
            ? "Scene"
            : "Node";

        const str component_name = owner.component<named>()
            ? owner.component<named>()->name()
            : str();

        const str tree_node_name = component_name.empty()
            ? basic_name
            : strings::rformat("%0(%1)", basic_name, component_name);

        ImGui::PushID(root.get());
        E2D_DEFER([](){ ImGui::PopID(); });

        ImGuiTreeNodeFlags tree_node_flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick;

        if ( !root->has_children() ) {
            tree_node_flags |= ImGuiTreeNodeFlags_Leaf;
        }

        editor& e = the<editor>();

        if ( e.selection() == owner ) {
            tree_node_flags |= ImGuiTreeNodeFlags_Selected;
        }

        const bool tree_node_opened =
            ImGui::TreeNodeEx(tree_node_name.c_str(), tree_node_flags);

        if ( ImGui::IsItemClicked() ) {
            e.select(owner);
        }

        if ( tree_node_opened ) {
            E2D_DEFER([](){ ImGui::TreePop(); });

            root->for_each_child([](const const_node_iptr& child){
                show_tree_for_node(child);
            });
        }
    }

    void show_tree_for_all_scenes(const ecs::registry& owner) {
        owner.for_joined_components<scene, actor>(
        [](const ecs::const_entity&, const scene&, const actor& a){
            show_tree_for_node(a.node());
        });
    }
}

namespace e2d::dbgui_widgets
{
    hierarchy_widget::hierarchy_widget() {
        desc_.title = "Hierarchy";
        desc_.first_size = v2f(300.f, 400.f);
    }

    bool hierarchy_widget::show() {
        if ( !modules::is_initialized<editor>() ) {
            return false;
        }
        
        if ( !modules::is_initialized<world>() ) {
            return false;
        }

        show_tree_for_all_scenes(the<world>().registry());

        return true;
    }

    const hierarchy_widget::description& hierarchy_widget::desc() const noexcept {
        return desc_;
    }
}
