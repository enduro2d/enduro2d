/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

namespace
{
}

TEST_CASE("node") {
    SECTION("empty_node") {
        auto n = node::create();
        REQUIRE(n);
        REQUIRE(n->root() == n);
        REQUIRE_FALSE(n->parent());
        REQUIRE(n->child_count() == 0);
        REQUIRE_FALSE(n->prev_sibling());
        REQUIRE_FALSE(n->next_sibling());
        {
            const_node_iptr cn = n;
            REQUIRE(cn);
            REQUIRE(cn->root() == cn);
            REQUIRE_FALSE(cn->parent());
            REQUIRE(cn->child_count() == 0);
            REQUIRE_FALSE(cn->prev_sibling());
            REQUIRE_FALSE(cn->next_sibling());
        }
    }
    SECTION("parent/root") {
        {
            auto p = node::create();
            auto n = node::create();

            REQUIRE(p->add_child(n));
            REQUIRE(n->parent() == p);
            REQUIRE(p->child_count() == 1);

            REQUIRE(p->add_child(n));
            REQUIRE(n->parent() == p);
            REQUIRE(p->child_count() == 1);

            REQUIRE_FALSE(p->add_child(nullptr));
            REQUIRE(n->parent() == p);
            REQUIRE(p->child_count() == 1);
        }
        {
            auto p = node::create();

            auto n1 = node::create(p);
            REQUIRE(n1->parent() == p);
            REQUIRE(p->child_count() == 1);

            auto n2 = node::create(nullptr);
            REQUIRE_FALSE(n2->parent());
            REQUIRE(p->child_count() == 1);
        }
        {
            auto p1 = node::create();
            auto p2 = node::create();

            auto n = node::create(p1);
            REQUIRE(n->parent() == p1);
            REQUIRE(p1->child_count() == 1);
            REQUIRE(p2->child_count() == 0);

            REQUIRE(p2->add_child(n));
            REQUIRE(n->parent() == p2);
            REQUIRE(p1->child_count() == 0);
            REQUIRE(p2->child_count() == 1);
        }
        {
            auto p1 = node::create();
            auto p2 = node::create(p1);

            auto n1 = node::create(p1);
            auto n2 = node::create(p2);

            REQUIRE(n1->parent() == p1);
            REQUIRE(n2->parent() == p2);

            REQUIRE(n1->root() == p1);
            REQUIRE(n2->root() == p1);

            auto n3 = node::create();
            REQUIRE_FALSE(n3->parent());
            REQUIRE(n3->root() == n3);

            {
                const_node_iptr cn1 = n1;
                const_node_iptr cn2 = n2;

                REQUIRE(cn1->parent() == p1);
                REQUIRE(cn2->parent() == p2);

                REQUIRE(cn1->root() == p1);
                REQUIRE(cn2->root() == p1);

                const_node_iptr cn3 = node::create();
                REQUIRE_FALSE(cn3->parent());
                REQUIRE(cn3->root() == cn3);
            }
        }
    }
    SECTION("auto_remove/remove_all_children") {
        {
            auto p = node::create();
            auto n = node::create(p);
            n.reset();
            REQUIRE(p->child_count() == 0);
        }
        {
            auto p = node::create();
            auto n1 = node::create(p);
            auto n2 = node::create(p);
            REQUIRE(p->child_count() == 2);
            REQUIRE(p->remove_all_children() == 2);
            REQUIRE_FALSE(n1->parent());
            REQUIRE_FALSE(n2->parent());
            REQUIRE(p->child_count() == 0);
            REQUIRE(p->remove_all_children() == 0);
        }
    }
    SECTION("remove_from_parent") {
        {
            auto p = node::create();
            auto n1 = node::create(p);
            auto n2 = node::create(p);

            auto np = node::create();
            np->remove_from_parent();
            REQUIRE(np->root() == np);
            REQUIRE_FALSE(np->parent());

            REQUIRE(p->child_count() == 2);
            REQUIRE(n1->parent() == p);
            REQUIRE(n2->parent() == p);

            REQUIRE(n1->remove_from_parent());
            REQUIRE_FALSE(n1->parent());
            REQUIRE(p->child_count() == 1);

            REQUIRE(n2->remove_from_parent());
            REQUIRE_FALSE(n2->parent());
            REQUIRE(p->child_count() == 0);
        }
    }
    SECTION("child_count/child_count_recursive") {
        auto p1 = node::create();
        REQUIRE(p1->child_count() == 0);
        REQUIRE(p1->child_count_recursive() == 0);

        auto p2 = node::create(p1);
        REQUIRE(p1->child_count() == 1);
        REQUIRE(p1->child_count_recursive() == 1);
        REQUIRE(p2->child_count() == 0);
        REQUIRE(p2->child_count_recursive() == 0);

        auto n1 = node::create(p2);
        auto n2 = node::create(p2);
        REQUIRE(p1->child_count() == 1);
        REQUIRE(p1->child_count_recursive() == 3);
        REQUIRE(p2->child_count() == 2);
        REQUIRE(p2->child_count_recursive() == 2);
    }
    SECTION("send_backward/bring_to_back") {
        {
            auto p = node::create();
            auto n1 = node::create(p);
            auto n2 = node::create(p);
            auto n3 = node::create(p);

            REQUIRE(n3->send_backward()); // n1 n3 n2
            REQUIRE(n1->next_sibling() == n3);

            REQUIRE(n3->send_backward()); // n3 n1 n2
            REQUIRE(n3->next_sibling() == n1);

            REQUIRE_FALSE(n3->send_backward()); // n3 n1 n2
            REQUIRE(n3->next_sibling() == n1);

            REQUIRE(n2->bring_to_back()); // n2 n3 n1
            REQUIRE(n2->next_sibling() == n3);

            REQUIRE_FALSE(n2->bring_to_back()); // n2 n3 n1
            REQUIRE(n2->next_sibling() == n3);
        }
        {
            auto n = node::create();
            REQUIRE_FALSE(n->send_backward());
            REQUIRE_FALSE(n->bring_to_back());
        }
    }
    SECTION("send_forward/bring_to_front") {
        {
            auto p = node::create();
            auto n1 = node::create(p);
            auto n2 = node::create(p);
            auto n3 = node::create(p);

            REQUIRE(n1->send_forward()); // n2 n1 n3
            REQUIRE(n2->next_sibling() == n1);

            REQUIRE(n1->send_forward()); // n2 n3 n1
            REQUIRE(n3->next_sibling() == n1);

            REQUIRE_FALSE(n1->send_forward()); // n2 n3 n1
            REQUIRE(n3->next_sibling() == n1);

            REQUIRE(n2->bring_to_front()); // n3 n1 n2
            REQUIRE(n1->next_sibling() == n2);

            REQUIRE_FALSE(n2->bring_to_front()); // n3 n1 n2
            REQUIRE(n1->next_sibling() == n2);
        }
        {
            auto n = node::create();
            REQUIRE_FALSE(n->send_forward());
            REQUIRE_FALSE(n->bring_to_back());
        }
    }
    SECTION("last_child/first_child") {
        auto p = node::create();
        auto n1 = node::create(p);
        auto n2 = node::create(p);
        auto n3 = node::create(p);

        REQUIRE(p->last_child() == n3);
        REQUIRE(p->first_child() == n1);

        {
            const_node_iptr cp = p;

            REQUIRE(cp->last_child() == n3);
            REQUIRE(cp->first_child() == n1);
        }
    }
    SECTION("prev_sibling/next_sibling") {
        auto p = node::create();

        auto n1 = node::create(p);
        auto n2 = node::create(p);
        auto n3 = node::create(p);

        REQUIRE_FALSE(n1->prev_sibling());
        REQUIRE(n1->next_sibling() == n2);

        REQUIRE(n2->prev_sibling() == n1);
        REQUIRE(n2->next_sibling() == n3);

        REQUIRE(n3->prev_sibling() == n2);
        REQUIRE_FALSE(n3->next_sibling());

        auto n4 = node::create();
        REQUIRE_FALSE(n4->prev_sibling());
        REQUIRE_FALSE(n4->next_sibling());

        {
            const_node_iptr cn1 = n1;
            const_node_iptr cn2 = n2;
            const_node_iptr cn3 = n3;

            REQUIRE_FALSE(cn1->prev_sibling());
            REQUIRE(cn1->next_sibling() == cn2);

            REQUIRE(cn2->prev_sibling() == cn1);
            REQUIRE(cn2->next_sibling() == cn3);

            REQUIRE(cn3->prev_sibling() == cn2);
            REQUIRE_FALSE(cn3->next_sibling());

            const_node_iptr cn4 = n4;
            REQUIRE_FALSE(cn4->prev_sibling());
            REQUIRE_FALSE(cn4->next_sibling());
        }
    }
    SECTION("add_child_to_back/add_child_to_front") {
        auto p = node::create();
        auto n1 = node::create();
        auto n2 = node::create();
        auto n3 = node::create();
        {
            p->remove_all_children();
            REQUIRE(p->add_child_to_front(n1));
            REQUIRE(p->add_child_to_front(n2));
            REQUIRE(p->add_child_to_front(n3));
            REQUIRE_FALSE(p->add_child_to_front(nullptr));

            REQUIRE(n1->next_sibling() == n2);
            REQUIRE(n2->next_sibling() == n3);
            REQUIRE_FALSE(n3->next_sibling());
        }
        {
            p->remove_all_children();
            REQUIRE(p->add_child_to_back(n1));
            REQUIRE(p->add_child_to_back(n2));
            REQUIRE(p->add_child_to_back(n3));
            REQUIRE_FALSE(p->add_child_to_back(nullptr));

            REQUIRE(n1->prev_sibling() == n2);
            REQUIRE(n2->prev_sibling() == n3);
            REQUIRE_FALSE(n3->prev_sibling());
        }
    }
    SECTION("add_child_after/add_child_before") {
        auto p = node::create();
        auto n1 = node::create();
        auto n2 = node::create();
        auto n3 = node::create();
        {
            p->remove_all_children();
            REQUIRE(p->add_child(n1)); // n1

            REQUIRE(p->add_child_before(n1, n2)); // n2 n1
            REQUIRE(p->add_child_before(n1, n3)); // n2 n3 n1
            REQUIRE_FALSE(p->add_child_before(n1, nullptr));
            REQUIRE_FALSE(p->add_child_before(p, n3));

            REQUIRE(n2->next_sibling() == n3);
            REQUIRE(n3->next_sibling() == n1);

            auto n4 = node::create();
            REQUIRE(p->add_child_before(nullptr, n4)); // n4 n2 n3 n1
            REQUIRE(n4->next_sibling() == n2);

            REQUIRE(n1->parent() == p);
            REQUIRE(n2->parent() == p);
            REQUIRE(n3->parent() == p);
            REQUIRE(n4->parent() == p);
        }
        {
            p->remove_all_children();
            REQUIRE(p->add_child(n1)); // n1

            REQUIRE(p->add_child_after(n1, n2)); // n1 n2
            REQUIRE(p->add_child_after(n1, n3)); // n1 n3 n2
            REQUIRE_FALSE(p->add_child_after(n1, nullptr));
            REQUIRE_FALSE(p->add_child_after(p, n3));

            REQUIRE(n1->next_sibling() == n3);
            REQUIRE(n3->next_sibling() == n2);

            auto n4 = node::create();
            REQUIRE(p->add_child_after(nullptr, n4)); // n1 n3 n2 n4
            REQUIRE(n2->next_sibling() == n4);

            REQUIRE(n1->parent() == p);
            REQUIRE(n2->parent() == p);
            REQUIRE(n3->parent() == p);
            REQUIRE(n4->parent() == p);
        }
    }
    SECTION("for_each_child") {
        auto p = node::create();
        array<node_iptr, 3> ns{
            node::create(p),
            node::create(p),
            node::create(p)};
        {
            std::size_t count = 0;
            p->for_each_child([&ns, &count](const node_iptr& n){
                REQUIRE(ns[count++] == n);
            });
            REQUIRE(count == 3);
        }
        {
            const_node_iptr cp = p;
            std::size_t count = 0;
            cp->for_each_child([&ns, &count](const const_node_iptr& n){
                REQUIRE(ns[count++] == n);
            });
            REQUIRE(count == 3);
        }
    }
    SECTION("remove_child_if") {
        auto p = node::create();
        auto n1 = node::create(p);
        auto n2 = node::create(p);
        auto n3 = node::create(p);
        auto n4 = node::create(p);

        std::size_t num = 0;
        std::size_t count = p->remove_child_if([&num](const node_iptr&){
            return (num++) % 2;
        });

        REQUIRE(count == 2);
        REQUIRE(p->child_count() == 2);
        REQUIRE(n1->parent() == p);
        REQUIRE_FALSE(n2->parent());
        REQUIRE(n3->parent() == p);
        REQUIRE_FALSE(n4->parent());
    }
    SECTION("destroy_node") {
        auto p1 = node::create();
        auto p2 = node::create(p1);
        auto n1 = node::create(p2);
        auto n2 = node::create(p2);

        p2.reset();

        REQUIRE(p1->child_count() == 0);
        REQUIRE(p1->child_count_recursive() == 0);

        REQUIRE(n1->root() == n1);
        REQUIRE_FALSE(n1->parent());
        REQUIRE_FALSE(n1->prev_sibling());
        REQUIRE_FALSE(n1->next_sibling());

        REQUIRE(n2->root() == n2);
        REQUIRE_FALSE(n2->parent());
        REQUIRE_FALSE(n2->prev_sibling());
        REQUIRE_FALSE(n2->next_sibling());
    }
}

TEST_CASE("scene") {
    scene_iptr s = scene::create();
    REQUIRE(s);
    REQUIRE(s->root());
}
