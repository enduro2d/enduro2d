/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_8703CE4A74D94C3CA27ED91AFF906936
#define E2D_INCLUDE_GUARD_8703CE4A74D94C3CA27ED91AFF906936
#pragma once

#include "_high.hpp"

namespace e2d
{
    class node;
    using node_iptr = intrusive_ptr<node>;
    using const_node_iptr = intrusive_ptr<const node>;

    class scene;
    using scene_iptr = intrusive_ptr<scene>;
    using const_scene_iptr = intrusive_ptr<const scene>;

    class node_children_ilist_tag {};
    using node_children = intrusive_list<node, node_children_ilist_tag>;
}

namespace e2d
{
    class node final
        : private noncopyable
        , public ref_counter<node>
        , public intrusive_list_hook<node_children_ilist_tag> {
    public:
        ~node() noexcept;
        static node_iptr create();
        static node_iptr create(const node_iptr& parent);

        node_iptr root() noexcept;
        const_node_iptr root() const noexcept;

        node_iptr parent() noexcept;
        const_node_iptr parent() const noexcept;

        bool remove_from_parent() noexcept;
        std::size_t remove_all_children() noexcept;

        bool add_child(
            const node_iptr& child) noexcept;

        bool add_child_to_back(
            const node_iptr& child) noexcept;

        bool add_child_to_front(
            const node_iptr& child) noexcept;

        bool add_child_after(
            const const_node_iptr& after,
            const node_iptr& child) noexcept;

        bool add_child_before(
            const const_node_iptr& before,
            const node_iptr& child) noexcept;

        bool send_backward() noexcept;
        bool bring_to_back() noexcept;

        bool send_forward() noexcept;
        bool bring_to_front() noexcept;

        std::size_t child_count() const noexcept;
        std::size_t child_count_recursive() const noexcept;

        node_iptr last_child() noexcept;
        const_node_iptr last_child() const noexcept;

        node_iptr first_child() noexcept;
        const_node_iptr first_child() const noexcept;

        node_iptr prev_sibling() noexcept;
        const_node_iptr prev_sibling() const noexcept;

        node_iptr next_sibling() noexcept;
        const_node_iptr next_sibling() const noexcept;

        template < typename F >
        void for_each_child(F&& f);

        template < typename F >
        void for_each_child(F&& f) const;

        template < typename F >
        std::size_t remove_child_if(F&& f);
    private:
        node();
    private:
        node* parent_{nullptr};
        node_children children_;
    };
}

namespace e2d
{
    class scene final
        : private noncopyable
        , public ref_counter<scene> {
    public:
        ~scene() noexcept;
        static scene_iptr create();

        const node_iptr& root() const noexcept;
    private:
        scene();
    private:
        node_iptr root_ = node::create();
    };
}

#include "scene.inl"
#endif
