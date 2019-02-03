/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_27EFFE66AA124FFB862BE95C2FF38017
#define E2D_INCLUDE_GUARD_27EFFE66AA124FFB862BE95C2FF38017
#pragma once

#include "scene.hpp"

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

    template < typename F >
    std::size_t node::remove_child_if(F&& f) {
        std::size_t count = 0u;
        for ( auto iter = children_.begin(); iter != children_.end(); ) {
            node_iptr child(&*iter);
            if ( f(child) ) {
                ++count;
                ++iter;
                child->remove_from_parent();
            } else {
                ++iter;
            }
        }
        return count;
    }
}

#endif
