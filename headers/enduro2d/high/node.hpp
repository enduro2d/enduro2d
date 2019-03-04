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

    class node_children_ilist_tag {};
    using node_children = intrusive_list<node, node_children_ilist_tag>;
}

namespace e2d
{
    class node
        : private noncopyable
        , public ref_counter<node>
        , public intrusive_list_hook<node_children_ilist_tag> {
    public:
        virtual ~node() noexcept;

        static node_iptr create(world& world);
        static node_iptr create(world& world, const node_iptr& parent);

        static node_iptr create(const ecs::entity& entity);
        static node_iptr create(const ecs::entity& entity, const node_iptr& parent);

        ecs::entity entity() noexcept;
        ecs::const_entity entity() const noexcept;

        void transform(const t3f& transform) noexcept;
        const t3f& transform() const noexcept;

        void translation(const v3f& translation) noexcept;
        const v3f& translation() const noexcept;

        void rotation(const q4f& rotation) noexcept;
        const q4f& rotation() const noexcept;

        void scale(const v3f& scale) noexcept;
        const v3f& scale() const noexcept;

        const m4f& local_matrix() const noexcept;
        const m4f& world_matrix() const noexcept;

        node_iptr root() noexcept;
        const_node_iptr root() const noexcept;

        node_iptr parent() noexcept;
        const_node_iptr parent() const noexcept;

        bool has_parent() const noexcept;
        bool has_parent_recursive(
            const const_node_iptr& parent) const noexcept;

        bool has_children() const noexcept;
        bool has_child_recursive(
            const const_node_iptr& child) const noexcept;

        bool remove_from_parent() noexcept;
        std::size_t remove_all_children() noexcept;

        std::size_t child_count() const noexcept;
        std::size_t child_count_recursive() const noexcept;

        bool add_child(
            const node_iptr& child) noexcept;

        bool add_child_to_back(
            const node_iptr& child) noexcept;

        bool add_child_to_front(
            const node_iptr& child) noexcept;

        bool add_child_before(
            const node_iptr& before,
            const node_iptr& child) noexcept;

        bool add_child_after(
            const node_iptr& after,
            const node_iptr& child) noexcept;

        bool add_sibling_before(
            const node_iptr& sibling) noexcept;

        bool add_sibling_after(
            const node_iptr& sibling) noexcept;

        bool remove_child(
            const node_iptr& child) noexcept;

        bool send_backward() noexcept;
        bool bring_to_back() noexcept;

        bool send_forward() noexcept;
        bool bring_to_front() noexcept;

        node_iptr first_child() noexcept;
        const_node_iptr first_child() const noexcept;

        node_iptr last_child() noexcept;
        const_node_iptr last_child() const noexcept;

        node_iptr prev_sibling() noexcept;
        const_node_iptr prev_sibling() const noexcept;

        node_iptr next_sibling() noexcept;
        const_node_iptr next_sibling() const noexcept;

        template < typename F >
        void for_each_child(F&& f);

        template < typename F >
        void for_each_child(F&& f) const;

        template < typename Iter >
        std::size_t extract_all_nodes(Iter iter);

        template < typename Iter >
        std::size_t extract_all_nodes(Iter iter) const;
    protected:
        node(world& world);
        node(const ecs::entity& entity);
    private:
        enum flag_masks : u32 {
            fm_dirty_local_matrix = 1u << 0,
            fm_dirty_world_matrix = 1u << 1,
        };
        void mark_dirty_local_matrix_() noexcept;
        void mark_dirty_world_matrix_() noexcept;
        void update_local_matrix_() const noexcept;
        void update_world_matrix_() const noexcept;
    private:
        t3f transform_;
        ecs::entity entity_;
        node* parent_{nullptr};
        node_children children_;
    private:
        mutable u32 flags_{0u};
        mutable m4f local_matrix_;
        mutable m4f world_matrix_;
    };
}

#include "node.inl"
#endif
