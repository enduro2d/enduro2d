/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class gobject;
    using gobject_iptr = intrusive_ptr<gobject>;
    using const_gobject_iptr = intrusive_ptr<const gobject>;

    class world_gobjects_ilist_tag;
    using world_gobjects = intrusive_list<gobject, world_gobjects_ilist_tag>;
}

namespace e2d
{
    class gobject final
        : private noncopyable
        , public ref_counter<gobject>
        , public intrusive_list_hook<world_gobjects_ilist_tag> {
    public:
        gobject(ecs::registry& registry);
        gobject(ecs::registry& registry, const gobject& source);
        gobject(ecs::registry& registry, const ecs::prototype& proto);
        ~gobject() noexcept;
    private:
        ecs::entity entity_;
    };
}
