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
        auto prefab_root_res = l.load_asset<prefab_asset>("prefab_root.json");
        REQUIRE(prefab_root_res);

        const prefab& prefab_root = prefab_root_res->content();
        REQUIRE(prefab_root.uuid() == "73740BC4-CE9F-4A7F-A029-4AB65027A8AE");

        REQUIRE(prefab_root.children().size() == 2u);
        REQUIRE(prefab_root.children()[0].uuid() == "4A93547E-4635-4C2F-9C59-3546E11B1722");
        REQUIRE(prefab_root.children()[1].uuid() == "58063213-9FC1-457C-B773-B826BE1BE6D7");

        auto go = the<world>().instantiate(prefab_root);

        {
            REQUIRE(go.component<named>());
            REQUIRE(go.component<named>()->name() == "root");

            REQUIRE(go.component<circle_collider>());
            REQUIRE(go.component<circle_collider>()->radius() == Approx(5.f));
            REQUIRE(go.component<circle_collider>()->offset() == v2f(4.f,2.f));
        }

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

            node_iptr child1_node = child1_go.component<actor>()
                ? child1_go.component<actor>()->node()
                : node_iptr();

            node_iptr child2_node = child2_go.component<actor>()
                ? child2_go.component<actor>()->node()
                : node_iptr();

            REQUIRE(child1_node);
            REQUIRE(child1_node->owner() == child1_go);
            REQUIRE(child1_node->child_count() == 0u);

            REQUIRE(child2_node);
            REQUIRE(child2_node->owner() == child2_go);
            REQUIRE(child2_node->child_count() == 0u);
        }
    }

    {
        auto prefab_child_res = l.load_asset<prefab_asset>("prefab_child.json");
        REQUIRE(prefab_child_res);

        const prefab& prefab_child = prefab_child_res->content();
        REQUIRE(prefab_child.uuid().empty());

        REQUIRE(prefab_child.children().size() == 3u);
        REQUIRE(prefab_child.children()[0].uuid() == "4A93547E-4635-4C2F-9C59-3546E11B1722");
        REQUIRE(prefab_child.children()[1].uuid() == "58063213-9FC1-457C-B773-B826BE1BE6D7");
        REQUIRE(prefab_child.children()[2].uuid() == "C07CDC21-8D1A-45E5-9321-AC7B6FADA847");

        auto go = the<world>().instantiate(prefab_child);

        {
            REQUIRE(go.component<named>());
            REQUIRE(go.component<named>()->name() == "child");

            REQUIRE(go.component<rect_collider>());
            REQUIRE(go.component<rect_collider>()->size() == v2f(1.f,2.f));
            REQUIRE(go.component<rect_collider>()->offset() == v2f(2.f,4.f));

            REQUIRE(go.component<circle_collider>());
            REQUIRE(go.component<circle_collider>()->radius() == Approx(5.f));
            REQUIRE(go.component<circle_collider>()->offset() == v2f(4.f,2.f));

            REQUIRE(go.component<polygon_collider>());
            REQUIRE(go.component<polygon_collider>()->points() == vector<v2f>{{1,2},{2,3},{3,4}});
            REQUIRE(go.component<polygon_collider>()->offset() == v2f(8.f,4.f));
        }

        {
            node_iptr go_node = go.component<actor>()
                ? go.component<actor>()->node()
                : node_iptr();

            REQUIRE(go_node);
            REQUIRE(go_node->owner() == go);
            REQUIRE(go_node->child_count() == 3u);

            node_iptr child1 = go_node->child_at(0u);
            node_iptr child2 = go_node->child_at(1u);
            node_iptr child3 = go_node->child_at(2u);

            REQUIRE(child1);
            REQUIRE(child2);
            REQUIRE(child3);

            gobject child1_go = child1->owner();
            gobject child2_go = child2->owner();
            gobject child3_go = child3->owner();

            REQUIRE(child1_go);
            REQUIRE(child2_go);
            REQUIRE(child3_go);

            REQUIRE(child1_go.component<named>());
            REQUIRE(child1_go.component<widget>());
            REQUIRE(child1_go.component<named>()->name() == "child(1)");
            REQUIRE(child1_go.component<widget>()->size() == v2f(10.f, 10.f));

            REQUIRE(child2_go.component<named>());
            REQUIRE(child2_go.component<widget>());
            REQUIRE(child2_go.component<named>()->name() == "child(2)");
            REQUIRE(child2_go.component<widget>()->size() == v2f(20.f, 20.f));

            REQUIRE(child3_go.component<named>());
            REQUIRE(child3_go.component<widget>());
            REQUIRE(child3_go.component<named>()->name() == "child(3)");
            REQUIRE(child3_go.component<widget>()->size() == v2f(30.f, 30.f));

            node_iptr child1_node = child1_go.component<actor>()
                ? child1_go.component<actor>()->node()
                : node_iptr();

            node_iptr child2_node = child2_go.component<actor>()
                ? child2_go.component<actor>()->node()
                : node_iptr();

            node_iptr child3_node = child3_go.component<actor>()
                ? child3_go.component<actor>()->node()
                : node_iptr();

            REQUIRE(child1_node);
            REQUIRE(child1_node->owner() == child1_go);
            REQUIRE(child1_node->child_count() == 1u);
            REQUIRE(child1_node->first_child()->child_count() == 2u);

            REQUIRE(child2_node);
            REQUIRE(child2_node->owner() == child2_go);
            REQUIRE(child2_node->child_count() == 1u);
            REQUIRE(child2_node->first_child()->child_count() == 2u);

            REQUIRE(child3_node);
            REQUIRE(child3_node->owner() == child3_go);
            REQUIRE(child3_node->child_count() == 0u);
        }
    }
}
