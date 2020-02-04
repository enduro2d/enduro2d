/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "node.hpp"

namespace e2d
{
    template < typename F >
    void node::for_each_child(F&& f) {
        for ( auto iter = children_.begin(); iter != children_.end(); ++iter ) {
            f(node_iptr(&*iter));
        }
    }

    template < typename F >
    void node::for_each_child(F&& f) const {
        for ( auto iter = children_.begin(); iter != children_.end(); ++iter ) {
            f(const_node_iptr(&*iter));
        }
    }

    template < typename F >
    void node::for_each_child_reversed(F&& f) {
        for ( auto iter = children_.rbegin(); iter != children_.rend(); ++iter ) {
            f(node_iptr(&*iter));
        }
    }

    template < typename F >
    void node::for_each_child_reversed(F&& f) const {
        for ( auto iter = children_.rbegin(); iter != children_.rend(); ++iter ) {
            f(const_node_iptr(&*iter));
        }
    }
}

namespace e2d::nodes
{
    template < typename Iter >
    std::size_t extract_nodes(const node_iptr& root, Iter iter) {
        std::size_t count{0u};
        if ( root ) {
            ++count;
            iter++ = root;
            root->for_each_child([&iter, &count](const node_iptr& child){
                count += extract_nodes(child, iter);
            });
        }
        return count;
    }

    template < typename Iter >
    std::size_t extract_nodes(const const_node_iptr& root, Iter iter) {
        std::size_t count{0u};
        if ( root ) {
            ++count;
            iter++ = root;
            root->for_each_child([&iter, &count](const const_node_iptr& child){
                count += extract_nodes(child, iter);
            });
        }
        return count;
    }

    template < typename Iter >
    std::size_t extract_nodes_reversed(const node_iptr& root, Iter iter) {
        std::size_t count{0u};
        if ( root ) {
            root->for_each_child_reversed([&iter, &count](const node_iptr& child){
                count += extract_nodes_reversed(child, iter);
            });
            ++count;
            iter++ = root;
        }
        return count;
    }

    template < typename Iter >
    std::size_t extract_nodes_reversed(const const_node_iptr& root, Iter iter) {
        std::size_t count{0u};
        if ( root ) {
            root->for_each_child_reversed([&iter, &count](const const_node_iptr& child){
                count += extract_nodes_reversed(child, iter);
            });
            ++count;
            iter++ = root;
        }
        return count;
    }
}

namespace e2d::nodes
{
    template < typename Iter >
    std::size_t extract_parents(const node_iptr& root, Iter iter) {
        std::size_t count{0u};
        if ( root ) {
            ++count;
            iter++ = root;
            count += extract_parents(root->parent(), iter);
        }
        return count;
    }

    template < typename Iter >
    std::size_t extract_parents(const const_node_iptr& root, Iter iter) {
        std::size_t count{0u};
        if ( root ) {
            ++count;
            iter++ = root;
            count += extract_parents(root->parent(), iter);
        }
        return count;
    }

    template < typename Iter >
    std::size_t extract_parents_reversed(const node_iptr& root, Iter iter) {
        std::size_t count{0u};
        if ( root ) {
            count += extract_parents(root->parent(), iter);
            ++count;
            iter++ = root;
        }
        return count;
    }

    template < typename Iter >
    std::size_t extract_parents_reversed(const const_node_iptr& root, Iter iter) {
        std::size_t count{0u};
        if ( root ) {
            count += extract_parents(root->parent(), iter);
            ++count;
            iter++ = root;
        }
        return count;
    }
}

namespace e2d::nodes
{
    template < typename F >
    void for_extracted_nodes(const node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<node_iptr> nodes;

        const std::size_t begin_index = nodes.size();
        E2D_DEFER([begin_index](){
            nodes.erase(
                nodes.begin() + begin_index,
                nodes.end());
        });

        extract_nodes(
            root,
            std::back_inserter(nodes));

        const std::size_t end_index = nodes.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            f(nodes[i]);
        }
    }

    template < typename F >
    void for_extracted_nodes(const const_node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<const_node_iptr> nodes;

        const std::size_t begin_index = nodes.size();
        E2D_DEFER([begin_index](){
            nodes.erase(
                nodes.begin() + begin_index,
                nodes.end());
        });

        extract_nodes(
            root,
            std::back_inserter(nodes));

        const std::size_t end_index = nodes.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            f(nodes[i]);
        }
    }

    template < typename F >
    void for_extracted_nodes_reversed(const node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<node_iptr> nodes;

        const std::size_t begin_index = nodes.size();
        E2D_DEFER([begin_index](){
            nodes.erase(
                nodes.begin() + begin_index,
                nodes.end());
        });

        extract_nodes_reversed(
            root,
            std::back_inserter(nodes));

        const std::size_t end_index = nodes.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            f(nodes[i]);
        }
    }

    template < typename F >
    void for_extracted_nodes_reversed(const const_node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<const_node_iptr> nodes;

        const std::size_t begin_index = nodes.size();
        E2D_DEFER([begin_index](){
            nodes.erase(
                nodes.begin() + begin_index,
                nodes.end());
        });

        extract_nodes_reversed(
            root,
            std::back_inserter(nodes));

        const std::size_t end_index = nodes.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            f(nodes[i]);
        }
    }
}

namespace e2d::nodes
{
    template < typename F >
    void for_extracted_parents(const node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<node_iptr> parents;

        const std::size_t begin_index = parents.size();
        E2D_DEFER([begin_index](){
            parents.erase(
                parents.begin() + begin_index,
                parents.end());
        });

        extract_parents(
            root,
            std::back_inserter(parents));

        const std::size_t end_index = parents.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            f(parents[i]);
        }
    }

    template < typename F >
    void for_extracted_parents(const const_node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<const_node_iptr> parents;

        const std::size_t begin_index = parents.size();
        E2D_DEFER([begin_index](){
            parents.erase(
                parents.begin() + begin_index,
                parents.end());
        });

        extract_parents(
            root,
            std::back_inserter(parents));

        const std::size_t end_index = parents.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            f(parents[i]);
        }
    }

    template < typename F >
    void for_extracted_parents_reversed(const node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<node_iptr> parents;

        const std::size_t begin_index = parents.size();
        E2D_DEFER([begin_index](){
            parents.erase(
                parents.begin() + begin_index,
                parents.end());
        });

        extract_parents_reversed(
            root,
            std::back_inserter(parents));

        const std::size_t end_index = parents.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            f(parents[i]);
        }
    }

    template < typename F >
    void for_extracted_parents_reversed(const const_node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<const_node_iptr> parents;

        const std::size_t begin_index = parents.size();
        E2D_DEFER([begin_index](){
            parents.erase(
                parents.begin() + begin_index,
                parents.end());
        });

        extract_parents_reversed(
            root,
            std::back_inserter(parents));

        const std::size_t end_index = parents.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            f(parents[i]);
        }
    }
}

namespace e2d::nodes
{
    template < typename Component >
    gcomponent<Component> get_component_in_parent(const const_node_iptr& root) {
        if ( !root ) {
            return {};
        }

        if ( auto component = root->owner().component<Component>() ) {
            return component;
        }

        return get_component_in_parent<Component>(root->parent());
    }

    template < typename Component, typename Iter >
    std::size_t get_components_in_parent(const const_node_iptr& root, Iter iter) {
        std::size_t count{0u};

        if ( !root ) {
            return count;
        }

        if ( auto component = root->owner().component<Component>() ) {
            ++count;
            iter++ = component;
        }

        return count + get_components_in_parent<Component>(root->parent(), iter);
    }

    template < typename Component >
    gcomponent<Component> get_component_in_children(const const_node_iptr& root) {
        if ( !root ) {
            return {};
        }

        if ( auto component = root->owner().component<Component>() ) {
            return component;
        }

        for ( const_node_iptr child = root->first_child(); child; child = child->next_sibling() ) {
            if ( auto component = get_component_in_children<Component>(child) ) {
                return component;
            }
        }

        return {};
    }

    template < typename Component, typename Iter >
    std::size_t get_components_in_children(const const_node_iptr& root, Iter iter) {
        std::size_t count{0u};

        if ( !root ) {
            return count;
        }

        if ( auto component = root->owner().component<Component>() ) {
            ++count;
            iter++ = component;
        }

        for ( const_node_iptr child = root->first_child(); child; child = child->next_sibling() ) {
            count += get_components_in_children<Component>(child, iter);
        }

        return count;
    }
}
