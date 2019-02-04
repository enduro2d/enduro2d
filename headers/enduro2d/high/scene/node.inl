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
}

#endif
