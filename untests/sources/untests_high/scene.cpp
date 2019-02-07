/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

namespace
{
    class fake_node : public node {
    public:
        void on_change_parent_() noexcept override {
            ++parent_changes;
            ++s_parent_changes;
        }

        void on_change_children_() noexcept override {
            ++children_changes;
            ++s_children_changes;
        }
    public:
        static void reset_counters() noexcept {
            s_parent_changes = 0;
            s_children_changes = 0;
        }
        std::size_t parent_changes{0};
        std::size_t children_changes{0};
        static std::size_t s_parent_changes;
        static std::size_t s_children_changes;
    public:
        static intrusive_ptr<fake_node> create() {
            return intrusive_ptr<fake_node>(new fake_node());
        }

        static intrusive_ptr<fake_node> create(const node_iptr& parent) {
            auto child = create();
            if ( parent ) {
                parent->add_child(child);
            }
            return child;
        }
    };

    std::size_t fake_node::s_parent_changes{0};
    std::size_t fake_node::s_children_changes{0};
}

TEST_CASE("scene") {
    auto s = scene::create();
    REQUIRE(s);
    REQUIRE(s->root());
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
    SECTION("has_parent") {
        auto p = node::create();
        REQUIRE_FALSE(p->has_parent());
        REQUIRE_FALSE(p->has_parent(nullptr));

        auto n = node::create(p);
        REQUIRE(n->has_parent());
        REQUIRE(n->has_parent(p));
        REQUIRE(n->has_parent(nullptr));

        auto pp = node::create();
        REQUIRE_FALSE(n->has_parent(pp));

        pp->add_child(p);
        REQUIRE(n->has_parent(p));
        REQUIRE(n->has_parent(pp));
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
            auto n = node::create(p);
            p.reset();
            REQUIRE_FALSE(n->parent());
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
    SECTION("auto_remove/remove_all_children/events") {
        {
            fake_node::reset_counters();
            auto p = fake_node::create();
            auto n = fake_node::create(p);
            n.reset();
            REQUIRE(p->children_changes == 2);
            REQUIRE(fake_node::s_parent_changes == 1);
        }
        {
            fake_node::reset_counters();
            auto p = fake_node::create();
            auto n = fake_node::create(p);
            p.reset();
            REQUIRE(fake_node::s_children_changes == 1);
            REQUIRE(n->parent_changes == 2);
        }
        {
            fake_node::reset_counters();
            auto p = fake_node::create();
            auto n1 = fake_node::create(p);
            auto n2 = fake_node::create(p);
            REQUIRE(p->remove_all_children() == 2);
            REQUIRE(p->remove_all_children() == 0);
            REQUIRE(p->children_changes == 3);
            REQUIRE(fake_node::s_parent_changes == 4);
        }
    }
    SECTION("remove_from_parent") {
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
    SECTION("remove_from_parent/events") {
        fake_node::reset_counters();

        auto p = fake_node::create();
        auto n1 = fake_node::create(p);
        auto n2 = fake_node::create(p);

        auto np = fake_node::create();
        np->remove_from_parent();
        REQUIRE(fake_node::s_parent_changes == 2);
        REQUIRE(fake_node::s_children_changes == 2);

        REQUIRE(n1->remove_from_parent());
        REQUIRE(fake_node::s_parent_changes == 3);
        REQUIRE(fake_node::s_children_changes == 3);

        REQUIRE(n2->remove_from_parent());
        REQUIRE(fake_node::s_parent_changes == 4);
        REQUIRE(fake_node::s_children_changes == 4);
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

            REQUIRE(n2->bring_to_back()); // n2 n3 n1
            REQUIRE(n2->next_sibling() == n3);
        }
        {
            auto n = node::create();
            REQUIRE_FALSE(n->send_backward());
            REQUIRE_FALSE(n->bring_to_back());
        }
    }
    SECTION("send_backward/bring_to_back/events") {
        {
            auto p = fake_node::create();
            auto n1 = fake_node::create(p);
            auto n2 = fake_node::create(p);
            auto n3 = fake_node::create(p);

            REQUIRE(n3->send_backward()); // n1 n3 n2
            REQUIRE(n3->send_backward()); // n3 n1 n2
            REQUIRE_FALSE(n3->send_backward()); // n3 n1 n2
            REQUIRE(n2->bring_to_back()); // n2 n3 n1
            REQUIRE(n2->bring_to_back()); // n2 n3 n1

            REQUIRE(p->children_changes == 6);
        }
        {
            auto n = fake_node::create();
            REQUIRE_FALSE(n->send_backward());
            REQUIRE_FALSE(n->bring_to_back());
            REQUIRE(n->children_changes == 0);
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

            REQUIRE(n2->bring_to_front()); // n3 n1 n2
            REQUIRE(n1->next_sibling() == n2);
        }
        {
            auto n = node::create();
            REQUIRE_FALSE(n->send_forward());
            REQUIRE_FALSE(n->bring_to_back());
        }
    }
    SECTION("send_forward/bring_to_front/events") {
        {
            auto p = fake_node::create();
            auto n1 = fake_node::create(p);
            auto n2 = fake_node::create(p);
            auto n3 = fake_node::create(p);

            REQUIRE(n1->send_forward()); // n2 n1 n3
            REQUIRE(n1->send_forward()); // n2 n3 n1
            REQUIRE_FALSE(n1->send_forward()); // n2 n3 n1
            REQUIRE(n2->bring_to_front()); // n3 n1 n2
            REQUIRE(n2->bring_to_front()); // n3 n1 n2

            REQUIRE(p->children_changes == 6);
        }
        {
            auto n = fake_node::create();
            REQUIRE_FALSE(n->send_forward());
            REQUIRE_FALSE(n->bring_to_back());
            REQUIRE(n->children_changes == 0);
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
            REQUIRE(p->add_child_to_back(n1));
            REQUIRE(p->add_child_to_back(n2));
            REQUIRE(p->add_child_to_back(n3)); // n3 n2 n1
            REQUIRE_FALSE(p->add_child_to_back(nullptr));

            REQUIRE(n1->prev_sibling() == n2);
            REQUIRE(n2->prev_sibling() == n3);
            REQUIRE_FALSE(n3->prev_sibling());

            // add to self parent (change order)
            REQUIRE(p->add_child_to_back(n1)); // n1 n3 n2
            REQUIRE(n2->prev_sibling() == n3);
            REQUIRE(n3->prev_sibling() == n1);
            REQUIRE_FALSE(n1->prev_sibling());

            // add to self parent (no change order)
            REQUIRE(p->add_child_to_back(n1)); // n1 n3 n2
            REQUIRE(n2->prev_sibling() == n3);
            REQUIRE(n3->prev_sibling() == n1);
            REQUIRE_FALSE(n1->prev_sibling());

            // add to another parent
            auto p2 = node::create();
            REQUIRE(p2->add_child_to_back(n1));
            REQUIRE(n1->parent() == p2);
            REQUIRE_FALSE(n1->prev_sibling());
            REQUIRE_FALSE(n1->next_sibling());
            REQUIRE(p->child_count() == 2);

            REQUIRE(n1->parent() == p2);
            REQUIRE(n2->parent() == p);
            REQUIRE(n3->parent() == p);
        }
        {
            p->remove_all_children();
            REQUIRE(p->add_child_to_front(n1));
            REQUIRE(p->add_child_to_front(n2));
            REQUIRE(p->add_child_to_front(n3)); // n1 n2 n3
            REQUIRE_FALSE(p->add_child_to_front(nullptr));

            REQUIRE(n1->next_sibling() == n2);
            REQUIRE(n2->next_sibling() == n3);
            REQUIRE_FALSE(n3->next_sibling());

            // add to self parent (change order)
            REQUIRE(p->add_child_to_front(n1)); // n2 n3 n1
            REQUIRE(n2->next_sibling() == n3);
            REQUIRE(n3->next_sibling() == n1);
            REQUIRE_FALSE(n1->next_sibling());

            // add to self parent (no change order)
            REQUIRE(p->add_child_to_front(n1)); // n2 n3 n1
            REQUIRE(n2->next_sibling() == n3);
            REQUIRE(n3->next_sibling() == n1);
            REQUIRE_FALSE(n1->next_sibling());

            // add to another parent
            auto p2 = node::create();
            REQUIRE(p2->add_child_to_front(n1));
            REQUIRE(n1->parent() == p2);
            REQUIRE_FALSE(n1->prev_sibling());
            REQUIRE_FALSE(n1->next_sibling());
            REQUIRE(p->child_count() == 2);

            REQUIRE(n1->parent() == p2);
            REQUIRE(n2->parent() == p);
            REQUIRE(n3->parent() == p);
        }
    }
    SECTION("add_child_to_back/add_child_to_front/events") {
        {
            auto p = fake_node::create();
            auto n1 = fake_node::create();
            auto n2 = fake_node::create();
            auto n3 = fake_node::create();

            REQUIRE(p->add_child_to_back(n1));
            REQUIRE(p->add_child_to_back(n2));
            REQUIRE(p->add_child_to_back(n3)); // n3 n2 n1
            REQUIRE_FALSE(p->add_child_to_back(nullptr));
            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 3);
            REQUIRE(n1->parent_changes == 1);
            REQUIRE(n1->children_changes == 0);

            // add to self parent (change order)
            REQUIRE(p->add_child_to_back(n1)); // n1 n3 n2
            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 4);
            REQUIRE(n1->parent_changes == 1);
            REQUIRE(n1->children_changes == 0);

            // add to self parent (no change order)
            REQUIRE(p->add_child_to_back(n1)); // n1 n3 n2
            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 4);
            REQUIRE(n1->parent_changes == 1);
            REQUIRE(n1->children_changes == 0);

            // add to another parent
            auto p2 = node::create();
            REQUIRE(p2->add_child_to_back(n1));
            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 5);
            REQUIRE(n1->parent_changes == 2);
            REQUIRE(n1->children_changes == 0);

            REQUIRE(n2->parent_changes == 1);
            REQUIRE(n2->children_changes == 0);
        }
        {
            auto p = fake_node::create();
            auto n1 = fake_node::create();
            auto n2 = fake_node::create();
            auto n3 = fake_node::create();

            REQUIRE(p->add_child_to_front(n1));
            REQUIRE(p->add_child_to_front(n2));
            REQUIRE(p->add_child_to_front(n3)); // n1 n2 n3
            REQUIRE_FALSE(p->add_child_to_front(nullptr));
            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 3);
            REQUIRE(n1->parent_changes == 1);
            REQUIRE(n1->children_changes == 0);

            // add to self parent (change order)
            REQUIRE(p->add_child_to_front(n1)); // n2 n3 n1
            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 4);
            REQUIRE(n1->parent_changes == 1);
            REQUIRE(n1->children_changes == 0);

            // add to self parent (no change order)
            REQUIRE(p->add_child_to_front(n1)); // n2 n3 n1
            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 4);
            REQUIRE(n1->parent_changes == 1);
            REQUIRE(n1->children_changes == 0);

            // add to another parent
            auto p2 = node::create();
            REQUIRE(p2->add_child_to_front(n1));
            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 5);
            REQUIRE(n1->parent_changes == 2);
            REQUIRE(n1->children_changes == 0);

            REQUIRE(n2->parent_changes == 1);
            REQUIRE(n2->children_changes == 0);
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

            REQUIRE(n1->add_sibling_before(n2)); // n2 n1
            REQUIRE(n1->add_sibling_before(n3)); // n2 n3 n1

            REQUIRE(n1->add_sibling_before(n1)); // n2 n3 n1
            REQUIRE(n3->add_sibling_before(n2)); // n2 n3 n1

            REQUIRE_FALSE(n1->add_sibling_before(nullptr));
            REQUIRE_FALSE(p->add_child_before(nullptr, n1));
            REQUIRE_FALSE(p->add_child_before(nullptr, nullptr));
            REQUIRE_FALSE(p->add_sibling_before(n3));

            REQUIRE(n2->next_sibling() == n3);
            REQUIRE(n3->next_sibling() == n1);

            REQUIRE(n3->add_sibling_before(n1)); // n2 n1 n3
            REQUIRE(n2->add_sibling_before(n3)); // n3 n2 n1

            REQUIRE(n3->next_sibling() == n2);
            REQUIRE(n2->next_sibling() == n1);

            // to another parent
            auto p2 = node::create();
            auto n4 = node::create(p2); // n4
            REQUIRE(n4->add_sibling_before(n2)); // n2 n4

            REQUIRE(n2->parent() == p2);
            REQUIRE_FALSE(n2->prev_sibling());
            REQUIRE(n2->next_sibling() == n4);
            REQUIRE(p->child_count() == 2);

            REQUIRE(n1->parent() == p);
            REQUIRE(n2->parent() == p2);
            REQUIRE(n3->parent() == p);
        }
        {
            p->remove_all_children();
            REQUIRE(p->add_child(n1)); // n1

            REQUIRE(n1->add_sibling_after(n2)); // n1 n2
            REQUIRE(n1->add_sibling_after(n3)); // n1 n3 n2

            REQUIRE(n1->add_sibling_after(n1)); // n1 n3 n2
            REQUIRE(n3->add_sibling_after(n2)); // n1 n3 n2

            REQUIRE_FALSE(n1->add_sibling_after(nullptr));
            REQUIRE_FALSE(p->add_child_after(nullptr, n1));
            REQUIRE_FALSE(p->add_child_after(nullptr, nullptr));
            REQUIRE_FALSE(p->add_sibling_after(n3));

            REQUIRE(n1->next_sibling() == n3);
            REQUIRE(n3->next_sibling() == n2);

            REQUIRE(n3->add_sibling_after(n1)); // n3 n1 n2
            REQUIRE(n2->add_sibling_after(n3)); // n1 n2 n3

            REQUIRE(n1->next_sibling() == n2);
            REQUIRE(n2->next_sibling() == n3);

            // to another parent
            auto p2 = node::create();
            auto n4 = node::create(p2); // n4
            REQUIRE(n4->add_sibling_after(n2)); // n4 n2

            REQUIRE(n2->parent() == p2);
            REQUIRE(n2->prev_sibling() == n4);
            REQUIRE_FALSE(n2->next_sibling());
            REQUIRE(p->child_count() == 2);

            REQUIRE(n1->parent() == p);
            REQUIRE(n2->parent() == p2);
            REQUIRE(n3->parent() == p);
        }
    }
    SECTION("add_child_after/add_child_before/events") {
        {
            auto p = fake_node::create();
            auto n1 = fake_node::create();
            auto n2 = fake_node::create();
            auto n3 = fake_node::create();

            REQUIRE(p->add_child(n1)); // n1
            REQUIRE(p->add_child_before(n1, n2)); // n2 n1
            REQUIRE(p->add_child_before(n1, n3)); // n2 n3 n1

            REQUIRE(p->add_child_before(n1, n1)); // n2 n3 n1
            REQUIRE(p->add_child_before(n3, n2)); // n2 n3 n1

            REQUIRE_FALSE(p->add_child_before(n1, nullptr));
            REQUIRE_FALSE(p->add_child_before(nullptr, n1));
            REQUIRE_FALSE(p->add_child_before(nullptr, nullptr));
            REQUIRE_FALSE(p->add_child_before(p, n3));

            REQUIRE(p->add_child_before(n3, n1)); // n2 n1 n3
            REQUIRE(p->add_child_before(n2, n3)); // n3 n2 n1

            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 5);

            // to another parent
            auto p2 = node::create();
            auto n4 = node::create(p2); // n4
            REQUIRE(p2->add_child_before(n4, n2)); // n2 n4

            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 6);

            REQUIRE(n1->parent_changes == 1);
            REQUIRE(n2->parent_changes == 2);
        }
        {
            auto p = fake_node::create();
            auto n1 = fake_node::create();
            auto n2 = fake_node::create();
            auto n3 = fake_node::create();

            REQUIRE(p->add_child(n1)); // n1
            REQUIRE(p->add_child_after(n1, n2)); // n1 n2
            REQUIRE(p->add_child_after(n1, n3)); // n1 n3 n2

            REQUIRE(p->add_child_after(n1, n1)); // n1 n3 n2
            REQUIRE(p->add_child_after(n3, n2)); // n1 n3 n2

            REQUIRE_FALSE(p->add_child_after(n1, nullptr));
            REQUIRE_FALSE(p->add_child_after(nullptr, n1));
            REQUIRE_FALSE(p->add_child_after(nullptr, nullptr));
            REQUIRE_FALSE(p->add_child_after(p, n3));

            REQUIRE(p->add_child_after(n3, n1)); // n3 n1 n2
            REQUIRE(p->add_child_after(n2, n3)); // n1 n2 n3

            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 5);

            // to another parent
            auto p2 = node::create();
            auto n4 = node::create(p2); // n4
            REQUIRE(p2->add_child_after(n4, n2)); // n4 n2

            REQUIRE(p->parent_changes == 0);
            REQUIRE(p->children_changes == 6);

            REQUIRE(n1->parent_changes == 1);
            REQUIRE(n2->parent_changes == 2);
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
    SECTION("destroy_node/events") {
        fake_node::reset_counters();

        auto p1 = fake_node::create();
        auto p2 = fake_node::create(p1);
        auto n1 = fake_node::create(p2);
        auto n2 = fake_node::create(p2);

        REQUIRE(fake_node::s_parent_changes == 3);
        REQUIRE(fake_node::s_children_changes == 3);

        p2.reset();

        REQUIRE(fake_node::s_parent_changes == 5);
        REQUIRE(fake_node::s_children_changes == 4);
    }
    SECTION("local_matrix") {
        {
            auto p = node::create();
            p->transform(math::make_translation_trs3(v3f{10.f,0.f,0.f}));

            auto n = node::create(p);
            n->transform(math::make_translation_trs3(v3f{20.f,0.f,0.f}));
            REQUIRE(n->local_matrix() == math::make_translation_matrix4(20.f,0.f,0.f));

            auto v = v4f(5.f,0.f,0.f,1.f);
            REQUIRE(v * n->local_matrix() == v4f{25.f,0.f,0.f,1.f});

            n->transform(math::make_scale_trs3(v3f(1.f,2.f,3.f)));
            REQUIRE(n->local_matrix() == math::make_scale_matrix4(1.f,2.f,3.f));
        }
    }
    SECTION("world_matrix") {
        {
            auto p = node::create();
            p->transform(math::make_translation_trs3(v3f{10.f,0.f,0.f}));

            auto n = node::create(p);
            n->transform(math::make_translation_trs3(v3f{20.f,0.f,0.f}));

            auto v = v4f(5.f,0.f,0.f,1.f);
            REQUIRE(v * n->world_matrix() == v4f{35.f,0.f,0.f,1.f});

            n->transform(math::make_scale_trs3(v3f(1.f,2.f,3.f)));
            REQUIRE(n->world_matrix() ==
                math::make_scale_matrix4(1.f,2.f,3.f) *
                math::make_translation_matrix4(10.f,0.f,0.f));
        }
        {
            auto n = node::create();
            n->transform(math::make_translation_trs3(v3f{20.f,0.f,0.f}));
            REQUIRE(n->world_matrix() ==
                math::make_translation_matrix4(20.f,0.f,0.f));

            auto p = node::create();
            p->transform(math::make_translation_trs3(v3f{10.f,0.f,0.f}));

            p->add_child(n);
            REQUIRE(n->world_matrix() ==
                math::make_translation_matrix4(30.f,0.f,0.f));
        }
        {
            auto p1 = node::create();
            p1->transform(math::make_translation_trs3(v3f{10.f,0.f,0.f}));

            auto p2 = node::create();
            p2->transform(math::make_translation_trs3(v3f{20.f,0.f,0.f}));

            auto n = node::create(p2);
            n->transform(math::make_translation_trs3(v3f{30.f,0.f,0.f}));

            REQUIRE(n->world_matrix() ==
                math::make_translation_matrix4(50.f,0.f,0.f));

            p1->add_child(p2);

            REQUIRE(n->world_matrix() ==
                math::make_translation_matrix4(60.f,0.f,0.f));
        }
    }
}
