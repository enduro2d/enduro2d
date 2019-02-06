/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

namespace
{
    class safe_starter_initializer final : private noncopyable {
    public:
        safe_starter_initializer() {
            modules::initialize<starter>(0, nullptr,
                starter::parameters(
                    engine::parameters("world_untests", "enduro2d")
                        .without_graphics(true)));
        }

        ~safe_starter_initializer() noexcept {
            modules::shutdown<starter>();
        }
    };
}

TEST_CASE("world"){
    safe_starter_initializer initializer;
    world& w = the<world>();
    REQUIRE(w.scene());

    SECTION("actor/drawable"){
        auto parent_e = w.registry().create_entity();
        auto actor_e = w.registry().create_entity();
        {
            REQUIRE(parent_e.assign_component<drawable>());
            node_iptr parent_n = parent_e.get_component<drawable>().node();
            w.scene()->root()->add_child(parent_n);

            actor_iptr actor_n = actor::create(actor_e);
            REQUIRE(actor_e.assign_component<drawable>(actor_n));
            parent_n->add_child(actor_n);
        }
        REQUIRE(w.scene()->root()->child_count_recursive() == 2);
        w.registry().destroy_entity(parent_e);
        REQUIRE(w.scene()->root()->child_count_recursive() == 0);
        REQUIRE(actor_e.get_component<drawable>().node());
        REQUIRE_FALSE(actor_e.get_component<drawable>().node()->parent());
    }
}
