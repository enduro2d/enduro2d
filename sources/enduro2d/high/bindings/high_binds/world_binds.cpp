/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high_binds.hpp"

#include <enduro2d/high/world.hpp>

namespace e2d::bindings::high
{
    void bind_world(sol::state& l) {
        l.new_usertype<world>("world",
            sol::no_constructor,

            "instantiate", sol::overload(
                [](world& w) -> gobject {
                    return w.instantiate();
                },
                [](world& w, const t2f& transform) -> gobject {
                    return w.instantiate(transform);
                },
                [](world& w, const node_iptr& parent) -> gobject {
                    return w.instantiate(parent);
                },
                [](world& w, const node_iptr& parent, const t2f& transform) -> gobject {
                    return w.instantiate(parent, transform);
                },
                [](world& w, const prefab& prefab) -> gobject {
                    return w.instantiate(prefab);
                },
                [](world& w, const prefab& prefab, const t2f& transform) -> gobject {
                    return w.instantiate(prefab, transform);
                },
                [](world& w, const prefab& prefab, const node_iptr& parent) -> gobject {
                    return w.instantiate(prefab, parent);
                },
                [](world& w, const prefab& prefab, const node_iptr& parent, const t2f& transform) -> gobject {
                    return w.instantiate(prefab, parent, transform);
                }
            )
        );
    }
}
