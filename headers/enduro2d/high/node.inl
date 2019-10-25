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
    template < typename F >
    void for_extracted_nodes(const node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<node_iptr> nodes;
        try {
            extract_nodes(root, std::back_inserter(nodes));
            for ( const node_iptr& n : nodes ) {
                f(n);
            }
        } catch (...) {
            nodes.clear();
            throw;
        }
        nodes.clear();
    }

    template < typename F >
    void for_extracted_nodes(const const_node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<const_node_iptr> nodes;
        try {
            extract_nodes(root, std::back_inserter(nodes));
            for ( const const_node_iptr& n : nodes ) {
                f(n);
            }
        } catch (...) {
            nodes.clear();
            throw;
        }
        nodes.clear();
    }

    template < typename F >
    void for_extracted_nodes_reversed(const node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<node_iptr> nodes;
        try {
            extract_nodes_reversed(root, std::back_inserter(nodes));
            for ( const node_iptr& n : nodes ) {
                f(n);
            }
        } catch (...) {
            nodes.clear();
            throw;
        }
        nodes.clear();
    }

    template < typename F >
    void for_extracted_nodes_reversed(const const_node_iptr& root, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<const_node_iptr> nodes;
        try {
            extract_nodes_reversed(root, std::back_inserter(nodes));
            for ( const const_node_iptr& n : nodes ) {
                f(n);
            }
        } catch (...) {
            nodes.clear();
            throw;
        }
        nodes.clear();
    }
}
