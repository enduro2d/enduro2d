/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "bindings.hpp"

#include <enduro2d/high/library.hpp>
#include <enduro2d/high/luasol.hpp>
#include <enduro2d/high/world.hpp>

#include <enduro2d/high/node.hpp>
#include <enduro2d/high/gobject.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/behaviour.hpp>

namespace
{
    using namespace e2d;

    void bind_library(sol::state& l) {
        l.new_usertype<library>("library");
    }

    void bind_luasol(sol::state& l) {
        l.new_usertype<luasol>("luasol");
    }

    void bind_world(sol::state& l) {
        l.new_usertype<world>("world");
    }
}

namespace
{
    using namespace e2d;

    void bind_node(sol::state& l) {
        l.new_usertype<node>("node");
    }

    void bind_gobject(sol::state& l) {
        l.new_usertype<gobject>("gobject");
    }
}

namespace
{
    using namespace e2d;

    void bind_actor(sol::state& l) {
        l.new_usertype<actor>("actor");
    }

    void bind_behaviour(sol::state& l) {
        l.new_usertype<behaviour>("behaviour");
    }
}

namespace e2d::bindings
{
    void bind_high(sol::state& l) {
        bind_library(l);
        bind_luasol(l);
        bind_world(l);

        bind_node(l);
        bind_gobject(l);

        bind_actor(l);
        bind_behaviour(l);
    }
}
