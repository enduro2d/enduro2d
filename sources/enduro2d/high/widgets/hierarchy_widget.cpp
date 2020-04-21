/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
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

    void show_tree_for_node(editor& e, input& i, world& w, const node_iptr& root) {
        const gobject owner = root ? root->owner() : gobject();
        if ( !owner ) {
            return;
        }

        const str icon_name = owner.component<scene>().exists()
            ? ICON_FA_CUBES
            : ICON_FA_CUBE;

        const str component_name = owner.component<named>()
            ? owner.component<named>()->name()
            : str();

        const str tree_node_name = component_name.empty()
            ? icon_name
            : strings::rformat("%0 %1", icon_name, component_name);

        ImGui::PushID(root.get());
        E2D_DEFER([](){ ImGui::PopID(); });

        ImGuiTreeNodeFlags tree_node_flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanFullWidth |
            ImGuiTreeNodeFlags_OpenOnDoubleClick;

        if ( !root->has_children() ) {
            tree_node_flags |= ImGuiTreeNodeFlags_Leaf;
        }

        if ( e.selection() && e.selection() == owner ) {
            tree_node_flags |= ImGuiTreeNodeFlags_Selected;
        }

        const bool tree_node_opened = ImGui::TreeNodeEx(
            root.get(),
            tree_node_flags,
            "%s", tree_node_name.c_str());

        E2D_DEFER([tree_node_opened](){
            if ( tree_node_opened ) {
                ImGui::TreePop();
            }
        });

        if ( ImGui::IsItemClicked() ) {
            if ( ImGui::IsItemToggledOpen() || e.selection() != owner ) {
                e.select(owner);
            } else {
                e.clear_selection();
            }
        }

        if ( ImGui::BeginPopupContextItem() ) {
            E2D_DEFER([](){ ImGui::EndPopup(); });

            if ( ImGui::MenuItem("Add child") ) {
                gobject inst = w.instantiate();
                if ( !root->add_child(inst.component<actor>()->node()) ) {
                    inst.destroy();
                }
            }

            imgui_utils::with_disabled_flag_ex(!root->has_parent(), [&w, &root](){
                if ( ImGui::MenuItem("Add sibling after") ) {
                    gobject inst = w.instantiate();
                    if ( !root->add_sibling_after(inst.component<actor>()->node()) ) {
                        inst.destroy();
                    }
                }

                if ( ImGui::MenuItem("Add sibling before") ) {
                    gobject inst = w.instantiate();
                    if ( !root->add_sibling_before(inst.component<actor>()->node()) ) {
                        inst.destroy();
                    }
                }
            });

            ImGui::Separator();

            if ( ImGui::MenuItem("Destroy") ) {
                w.destroy_instance(owner);
                return;
            }
        }

        if ( tree_node_opened ) {
            nodes::for_each_child(root, [&e, &i, &w](const node_iptr& child){
                show_tree_for_node(e, i, w, child);
            });
        }
    }

    void show_tree_for_all_free_nodes(editor& e, input& i, world& w) {
        ecsex::for_extracted_components<actor>(w.registry(), [&e, &i, &w](
            const ecs::const_entity&,
            actor& a)
        {
            if ( a.node() && !a.node()->has_parent() ) {
                show_tree_for_node(e, i, w, a.node());
            }
        });

        ImGui::Separator();

        if ( ImGui::Button("+ Add Node") ) {
            w.instantiate();
        }
    }

    void process_tree_selection(editor& e, input& i) {
        if ( !e.selection() || !ImGui::IsWindowFocused() ) {
            return;
        }

        const keyboard& k = i.keyboard();
        if ( k.is_key_just_pressed(keyboard_key::del)
            || k.is_key_just_pressed(keyboard_key::backspace) )
        {
            e.selection().destroy();
        }
    }
}

namespace e2d::dbgui_widgets
{
    hierarchy_widget::hierarchy_widget() {
        desc_.first_size = v2f(300.f, 400.f);
    }

    bool hierarchy_widget::show() {
        if ( !modules::is_initialized<editor, input, world>() ) {
            return false;
        }

        show_tree_for_all_free_nodes(
            the<editor>(),
            the<input>(),
            the<world>());

        process_tree_selection(
            the<editor>(),
            the<input>());

        return true;
    }

    const hierarchy_widget::description& hierarchy_widget::desc() const noexcept {
        return desc_;
    }
}
