/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_27EFFE66AA124FFB862BE95C2FF38017
#define E2D_INCLUDE_GUARD_27EFFE66AA124FFB862BE95C2FF38017
#pragma once

#include "node.hpp"

namespace e2d
{
    template < typename F >
    void node::for_each_child(F&& f) {
        for ( node& child : children_ ) {
            f(node_iptr(&child));
        }
    }

    template < typename F >
    void node::for_each_child(F&& f) const {
        for ( const node& child : children_ ) {
            f(const_node_iptr(&child));
        }
    }

    template < typename Iter >
    std::size_t node::extract_all_nodes(Iter iter) {
        std::size_t count{1u};
        iter++ = node_iptr(this);
        for_each_child([&iter, &count](const node_iptr& child){
            count += child->extract_all_nodes(iter);
        });
        return count;
    }

    template < typename Iter >
    std::size_t node::extract_all_nodes(Iter iter) const {
        std::size_t count{1u};
        iter++ = const_node_iptr(this);
        for_each_child([&iter, &count](const const_node_iptr& child){
            count += child->extract_all_nodes(iter);
        });
        return count;
    }
}

#endif
