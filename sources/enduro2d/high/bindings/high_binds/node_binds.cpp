/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "_high_binds.hpp"

#include <enduro2d/high/node.hpp>

namespace e2d::bindings::high
{
    void bind_node(sol::state& l) {
        l["e2d"].get_or_create<sol::table>()
        ["high"].get_or_create<sol::table>()
        .new_usertype<node>("node",
            "owner", sol::property(
                [](const node& n){ return n.owner(); }),

            "transform", sol::property(
                [](const node& n){ return n.transform(); },
                sol::resolve<void(const t3f&)>(&node::transform)),
            "translation", sol::property(
                [](const node& n){ return n.translation(); },
                sol::resolve<void(const v3f&)>(&node::translation)),
            "rotation", sol::property(
                [](const node& n){ return n.rotation(); },
                sol::resolve<void(const q4f&)>(&node::rotation)),
            "scale", sol::property(
                [](const node& n){ return n.scale(); },
                sol::resolve<void(const v3f&)>(&node::scale)),

            "local_matrix", sol::property(
                [](const node& n){ return n.local_matrix(); }),
            "world_matrix", sol::property(
                [](const node& n){ return n.world_matrix(); }),

            "root", sol::property(
                [](const node& n){ return n.root(); }),
            "parent", sol::property(
                [](const node& n){ return n.parent(); }),

            "remove_from_parent", &node::remove_from_parent,
            "remove_all_children", &node::remove_all_children,

            "child_count", sol::property(
                [](const node& n){ return n.child_count(); }),
            "child_count_recursive", sol::property(
                [](const node& n){ return n.child_count_recursive(); }),

            "add_child", &node::add_child,
            "add_child_to_back", &node::add_child_to_back,
            "add_child_to_front", &node::add_child_to_front,
            "add_child_before", &node::add_child_before,
            "add_child_after", &node::add_child_after,
            "add_sibling_before", &node::add_sibling_before,
            "add_sibling_after", &node::add_sibling_after,
            "remove_child", &node::remove_child,

            "send_backward", &node::send_backward,
            "bring_to_back", &node::bring_to_back,
            "send_forward", &node::send_forward,
            "bring_to_front", &node::bring_to_front,

            "first_child", sol::property(
                [](const node& n){ return n.first_child(); }),
            "last_child", sol::property(
                [](const node& n){ return n.last_child(); }),
            "prev_sibling", sol::property(
                [](const node& n){ return n.prev_sibling(); }),
            "next_sibling", sol::property(
                [](const node& n){ return n.next_sibling(); })
        );
    }
}
