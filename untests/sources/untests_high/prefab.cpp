/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
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
                    engine::parameters("prefab_untests", "enduro2d")));
        }

        ~safe_starter_initializer() noexcept {
            modules::shutdown<starter>();
        }
    };
}

TEST_CASE("prefab"){
    safe_starter_initializer initializer;
    library& l = the<library>();
    {
        auto parent_res = l.load_asset<prefab_asset>("parent.json");
        REQUIRE(parent_res);
        REQUIRE(parent_res->content().uuid() == "73740BC4-CE9F-4A7F-A029-4AB65027A8AE");

        REQUIRE(parent_res->content().children().size() == 2u);
        REQUIRE(parent_res->content().children()[0].uuid() == "4A93547E-4635-4C2F-9C59-3546E11B1722");
        REQUIRE(parent_res->content().children()[1].uuid() == "58063213-9FC1-457C-B773-B826BE1BE6D7");

        auto go = the<world>().instantiate(parent_res->content());
        REQUIRE(go);

        REQUIRE(go.component<named>());
        REQUIRE(go.component<named>()->name() == "parent");

        REQUIRE(go.component<circle_collider>());
        REQUIRE(math::approximately(go.component<circle_collider>()->radius(), 5.f));
        REQUIRE(go.component<circle_collider>()->offset() == v2f(4.f,2.f));

        {
            node_iptr go_node = go.component<actor>()
                ? go.component<actor>()->node()
                : node_iptr();

            REQUIRE(go_node);
            REQUIRE(go_node->owner() == go);
            REQUIRE(go_node->child_count() == 2u);

            node_iptr child1 = go_node->child_at(0u);
            node_iptr child2 = go_node->child_at(1u);

            REQUIRE(child1);
            REQUIRE(child2);

            gobject child1_go = child1->owner();
            gobject child2_go = child2->owner();

            REQUIRE(child1_go);
            REQUIRE(child2_go);

            REQUIRE(child1_go.component<named>());
            REQUIRE(child1_go.component<named>()->name() == "child(1)");

            REQUIRE(child2_go.component<named>());
            REQUIRE(child2_go.component<named>()->name() == "child(2)");
        }
    }

    {
        auto prefab_res = l.load_asset<prefab_asset>("prefab.json");
        REQUIRE(prefab_res);
        REQUIRE(prefab_res->content().uuid().empty());

        REQUIRE(prefab_res->content().children().size() == 2u);
        REQUIRE(prefab_res->content().children()[0].uuid() == "4A93547E-4635-4C2F-9C59-3546E11B1722");
        REQUIRE(prefab_res->content().children()[1].uuid() == "58063213-9FC1-457C-B773-B826BE1BE6D7");

        auto go = the<world>().instantiate(prefab_res->content());
        REQUIRE(go);

        REQUIRE(go.component<named>());
        REQUIRE(go.component<named>()->name() == "prefab");

        REQUIRE(go.component<rect_collider>());
        REQUIRE(go.component<rect_collider>()->size() == v2f(1.f,2.f));
        REQUIRE(go.component<rect_collider>()->offset() == v2f(2.f,4.f));

        REQUIRE(go.component<circle_collider>());
        REQUIRE(math::approximately(go.component<circle_collider>()->radius(), 5.f));
        REQUIRE(go.component<circle_collider>()->offset() == v2f(4.f,2.f));

        REQUIRE(go.component<polygon_collider>());
        REQUIRE(go.component<polygon_collider>()->points() == vector<v2f>{{1,2},{2,3},{3,4}});
        REQUIRE(go.component<polygon_collider>()->offset() == v2f(8.f,4.f));

        {
            node_iptr go_node = go.component<actor>()
                ? go.component<actor>()->node()
                : node_iptr();

            REQUIRE(go_node);
            REQUIRE(go_node->owner() == go);
            REQUIRE(go_node->child_count() == 2u);

            node_iptr child1 = go_node->child_at(0u);
            node_iptr child2 = go_node->child_at(1u);

            REQUIRE(child1);
            REQUIRE(child2);

            gobject child1_go = child1->owner();
            gobject child2_go = child2->owner();

            REQUIRE(child1_go);
            REQUIRE(child2_go);

            REQUIRE(child1_go.component<named>());
            REQUIRE(child1_go.component<named>()->name() == "child(1)");

            REQUIRE(child1_go.component<widget>());
            REQUIRE(child1_go.component<widget>()->size() == v2f(10.f, 10.f));

            REQUIRE(child2_go.component<named>());
            REQUIRE(child2_go.component<named>()->name() == "child(2)");
            REQUIRE(child2_go.component<widget>()->size() == v2f(20.f, 20.f));
        }
    }
}
