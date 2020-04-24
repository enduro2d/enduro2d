/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high_binds.hpp"

#include <enduro2d/high/node.hpp>

namespace e2d::bindings::high
{
    void bind_node(sol::state& l) {
        l.new_usertype<node>("node",
            sol::no_constructor,

            "owner", sol::property(
                [](const node& n) -> gobject { return n.owner(); }),

            "transform", sol::property(
                [](const node& n) -> t2f { return n.transform(); },
                [](node& n, const t2f& v) { n.transform(v); }),

            "translation", sol::property(
                [](const node& n) -> v2f { return n.translation(); },
                [](node& n, const v2f& v) { n.translation(v); }),

            "rotation", sol::property(
                [](const node& n) -> f32 { return n.rotation(); },
                [](node& n, f32 v) { n.rotation(v); }),

            "scale", sol::property(
                [](const node& n) -> v2f { return n.scale(); },
                [](node& n, const v2f& v) { n.scale(v); }),

            "local_matrix", sol::property(
                [](const node& n) -> m4f { return n.local_matrix(); }),

            "world_matrix", sol::property(
                [](const node& n) -> m4f { return n.world_matrix(); }),

            "local_to_world", [](const node& n, const v4f& v) -> v4f {
                return n.local_to_world(v);
            },

            "world_to_local", [](const node& n, const v4f& v) -> v4f {
                return n.world_to_local(v);
            },

            "root", sol::property(
                [](node& n) -> node_iptr { return n.root(); }),

            "parent", sol::property(
                [](node& n) -> node_iptr { return n.parent(); }),

            "has_parent", [](const node& n) -> bool {
                return n.has_parent();
            },

            "has_parent_recursive", [](const node& n, const node_iptr& p) -> bool {
                return n.has_parent_recursive(p);
            },

            "has_children", [](const node& n) -> bool {
                return n.has_children();
            },

            "has_child_recursive", [](const node& n, const node_iptr& c) -> bool {
                return n.has_child_recursive(c);
            },

            "remove_from_parent", [](node& n) -> bool {
                return n.remove_from_parent();
            },

            "remove_all_children", [](node& n) -> std::size_t {
                return n.remove_all_children();
            },

            "child_count", sol::property(
                [](const node& n) -> std::size_t { return n.child_count(); }),

            "child_count_recursive", sol::property(
                [](const node& n) -> std::size_t { return n.child_count_recursive(); }),

            "add_child", [](node& n, const node_iptr& c) -> bool {
                return n.add_child(c);
            },

            "add_child_at", [](node& n, const node_iptr& c, i32 i) -> bool {
                return i >= 0
                    ? n.add_child_at(c, math::numeric_cast<std::size_t>(i))
                    : false;
            },

            "add_child_to_back", [](node& n, const node_iptr& c) -> bool {
                return n.add_child_to_back(c);
            },

            "add_child_to_front", [](node& n, const node_iptr& c) -> bool {
                return n.add_child_to_front(c);
            },

            "add_child_before", [](node& n, const node_iptr& b, const node_iptr& c) -> bool {
                return n.add_child_before(b, c);
            },

            "add_child_after", [](node& n, const node_iptr& a, const node_iptr& c) -> bool {
                return n.add_child_after(a, c);
            },

            "add_sibling_before", [](node& n, const node_iptr& s) -> bool {
                return n.add_sibling_before(s);
            },

            "add_sibling_after", [](node& n, const node_iptr& s) -> bool {
                return n.add_sibling_after(s);
            },

            "remove_child", [](node& n, const node_iptr& c) -> bool {
                return n.remove_child(c);
            },

            "remove_child_at", [](node& n, i32 i) -> node_iptr {
                return i >= 0
                    ? n.remove_child_at(math::numeric_cast<std::size_t>(i))
                    : node_iptr();
            },

            "swap_children", [](node& n, const node_iptr& cl, const node_iptr& cr) -> bool {
                return n.swap_children(cl, cr);
            },

            "swap_children_at", [](node& n, i32 cl, i32 cr) -> bool {
                return cl >= 0 && cr >= 0
                    ? n.swap_children_at(math::numeric_cast<std::size_t>(cl), math::numeric_cast<std::size_t>(cr))
                    : false;
            },

            "send_backward", [](node& n) -> bool {
                return n.send_backward();
            },

            "bring_to_back", [](node& n) -> bool {
                return n.bring_to_back();
            },

            "send_forward", [](node& n) -> bool {
                return n.send_forward();
            },

            "bring_to_front", [](node& n) -> bool {
                return n.bring_to_front();
            },

            "first_child", sol::property(
                [](node& n) -> node_iptr { return n.first_child(); }),

            "last_child", sol::property(
                [](node& n) -> node_iptr { return n.last_child(); }),

            "prev_sibling", sol::property(
                [](node& n) -> node_iptr { return n.prev_sibling(); }),

            "next_sibling", sol::property(
                [](node& n) -> node_iptr { return n.next_sibling(); }),

            "child_at", [](node& n, i32 i) -> node_iptr {
                return i >= 0
                    ? n.child_at(math::numeric_cast<std::size_t>(i))
                    : node_iptr();
            },

            "child_index", [](node& n, const node_iptr& c) -> std::pair<std::size_t, bool> {
                return n.child_index(c);
            }
        );
    }
}
