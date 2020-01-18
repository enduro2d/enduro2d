/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "gobject.hpp"

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

        static node_iptr create();
        static node_iptr create(const t2f& transform);

        static node_iptr create(const node_iptr& parent);
        static node_iptr create(const node_iptr& parent, const t2f& transform);

        static node_iptr create(gobject owner);
        static node_iptr create(gobject owner, const t2f& transform);

        static node_iptr create(gobject owner, const node_iptr& parent);
        static node_iptr create(gobject owner, const node_iptr& parent, const t2f& transform);

        void owner(gobject owner) noexcept;
        gobject owner() const noexcept;

        void transform(const t2f& transform) noexcept;
        const t2f& transform() const noexcept;

        void translation(const v2f& translation) noexcept;
        const v2f& translation() const noexcept;

        void rotation(f32 rotation) noexcept;
        f32 rotation() const noexcept;

        void scale(const v2f& scale) noexcept;
        const v2f& scale() const noexcept;

        const m4f& local_matrix() const noexcept;
        const m4f& world_matrix() const noexcept;

        v4f local_to_world(const v4f& local) const noexcept;
        v4f world_to_local(const v4f& world) const noexcept;

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

        template < typename F >
        void for_each_child_reversed(F&& f);
        template < typename F >
        void for_each_child_reversed(F&& f) const;
    protected:
        node() = default;
        node(gobject owner);
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
        t2f transform_;
        gobject owner_;
        node* parent_{nullptr};
        node_children children_;
    private:
        mutable u32 flags_{0u};
        mutable m4f local_matrix_;
        mutable m4f world_matrix_;
    };
}

namespace e2d::nodes
{
    template < typename Iter >
    std::size_t extract_nodes(const node_iptr& root, Iter iter);
    template < typename Iter >
    std::size_t extract_nodes(const const_node_iptr& root, Iter iter);

    template < typename Iter >
    std::size_t extract_nodes_reversed(const node_iptr& root, Iter iter);
    template < typename Iter >
    std::size_t extract_nodes_reversed(const const_node_iptr& root, Iter iter);
}

namespace e2d::nodes
{
    vector<node_iptr> extract_nodes(const node_iptr& root);
    vector<const_node_iptr> extract_nodes(const const_node_iptr& root);

    vector<node_iptr> extract_nodes_reversed(const node_iptr& root);
    vector<const_node_iptr> extract_nodes_reversed(const const_node_iptr& root);
}

namespace e2d::nodes
{
    template < typename F >
    void for_extracted_nodes(const node_iptr& root, F&& f);
    template < typename F >
    void for_extracted_nodes(const const_node_iptr& root, F&& f);

    template < typename F >
    void for_extracted_nodes_reversed(const node_iptr& root, F&& f);
    template < typename F >
    void for_extracted_nodes_reversed(const const_node_iptr& root, F&& f);
}

namespace e2d::nodes
{
    template < typename Component >
    gcomponent<Component> get_component_in_parent(const const_node_iptr& root);
    template < typename Component, typename Iter >
    std::size_t get_components_in_parent(const const_node_iptr& root, Iter iter);

    template < typename Component >
    gcomponent<Component> get_component_in_children(const const_node_iptr& root);
    template < typename Component, typename Iter >
    std::size_t get_components_in_children(const const_node_iptr& root, Iter iter);
}

#include "node.inl"
