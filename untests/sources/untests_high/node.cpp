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

    class fake_node final : public node {
    protected:
        fake_node(world& world) : node(world) {
            ++s_ctor_count;
        }

        ~fake_node() noexcept final {
            ++s_dtor_count;
        }
    public:
        static void reset_counters() noexcept {
            s_ctor_count = 0;
            s_dtor_count = 0;
        }
        static std::size_t s_ctor_count;
        static std::size_t s_dtor_count;
    public:
        static intrusive_ptr<fake_node> create(world& world) {
            return intrusive_ptr<fake_node>(new fake_node(world));
        }

        static intrusive_ptr<fake_node> create(world& world, const node_iptr& parent) {
            auto child = create(world);
            if ( parent ) {
                parent->add_child(child);
            }
            return child;
        }
    };

    std::size_t fake_node::s_ctor_count{0};
    std::size_t fake_node::s_dtor_count{0};
}

TEST_CASE("node") {
    safe_starter_initializer initializer;
    world& w = the<world>();
    SECTION("empty_node") {
        auto n = node::create(w);
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
            auto p = node::create(w);
            auto n = node::create(w);

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
            auto p = node::create(w);

            auto n1 = node::create(w, p);
            REQUIRE(n1->parent() == p);
            REQUIRE(p->child_count() == 1);

            auto n2 = node::create(w, nullptr);
            REQUIRE_FALSE(n2->parent());
            REQUIRE(p->child_count() == 1);
        }
        {
            auto p1 = node::create(w);
            auto p2 = node::create(w);

            auto n = node::create(w, p1);
            REQUIRE(n->parent() == p1);
            REQUIRE(p1->child_count() == 1);
            REQUIRE(p2->child_count() == 0);

            REQUIRE(p2->add_child(n));
            REQUIRE(n->parent() == p2);
            REQUIRE(p1->child_count() == 0);
            REQUIRE(p2->child_count() == 1);
        }
        {
            auto p1 = node::create(w);
            auto p2 = node::create(w, p1);

            auto n1 = node::create(w, p1);
            auto n2 = node::create(w, p2);

            REQUIRE(n1->parent() == p1);
            REQUIRE(n2->parent() == p2);

            REQUIRE(n1->root() == p1);
            REQUIRE(n2->root() == p1);

            auto n3 = node::create(w);
            REQUIRE_FALSE(n3->parent());
            REQUIRE(n3->root() == n3);

            {
                const_node_iptr cn1 = n1;
                const_node_iptr cn2 = n2;

                REQUIRE(cn1->parent() == p1);
                REQUIRE(cn2->parent() == p2);

                REQUIRE(cn1->root() == p1);
                REQUIRE(cn2->root() == p1);

                const_node_iptr cn3 = node::create(w);
                REQUIRE_FALSE(cn3->parent());
                REQUIRE(cn3->root() == cn3);
            }
        }
    }
    SECTION("has_parent/has_parent_recursive") {
        auto p = node::create(w);
        REQUIRE_FALSE(p->has_parent());
        REQUIRE_FALSE(p->has_parent_recursive(nullptr));

        auto n = node::create(w, p);

        REQUIRE_FALSE(p->has_parent());
        REQUIRE_FALSE(p->has_parent_recursive(n));

        REQUIRE(n->has_parent());
        REQUIRE(n->has_parent_recursive(p));
        REQUIRE_FALSE(n->has_parent_recursive(nullptr));

        auto pp = node::create(w);
        REQUIRE_FALSE(n->has_parent_recursive(pp));
        REQUIRE_FALSE(pp->has_parent_recursive(pp));

        pp->add_child(p);
        REQUIRE(n->has_parent_recursive(p));
        REQUIRE(n->has_parent_recursive(pp));

        REQUIRE_FALSE(n->has_parent_recursive(n));
        REQUIRE_FALSE(pp->has_parent_recursive(pp));
    }
    SECTION("has_children/has_child_recursive") {
        auto p = node::create(w);
        REQUIRE_FALSE(p->has_children());
        REQUIRE_FALSE(p->has_child_recursive(nullptr));

        auto n = node::create(w, p);

        REQUIRE(p->has_children());
        REQUIRE(p->has_child_recursive(n));
        REQUIRE_FALSE(p->has_child_recursive(nullptr));

        REQUIRE_FALSE(n->has_children());
        REQUIRE_FALSE(n->has_child_recursive(p));
        REQUIRE_FALSE(n->has_child_recursive(nullptr));

        auto pp = node::create(w);
        REQUIRE_FALSE(n->has_child_recursive(pp));
        REQUIRE_FALSE(pp->has_child_recursive(pp));

        pp->add_child(p);
        REQUIRE_FALSE(n->has_child_recursive(p));
        REQUIRE_FALSE(n->has_child_recursive(pp));
        REQUIRE(pp->has_child_recursive(p));
        REQUIRE(pp->has_child_recursive(n));
    }
    SECTION("auto_remove/remove_all_children") {
        {
            auto p = node::create(w);
            auto n = node::create(w, p);
            n->remove_from_parent();
            REQUIRE(p->child_count() == 0);
        }
        {
            auto p = node::create(w);
            auto n = node::create(w, p);
            p.reset();
            REQUIRE_FALSE(n->parent());
        }
        {
            auto p = node::create(w);
            auto n1 = node::create(w, p);
            auto n2 = node::create(w, p);
            REQUIRE(p->child_count() == 2);
            REQUIRE(p->remove_all_children() == 2);
            REQUIRE_FALSE(n1->parent());
            REQUIRE_FALSE(n2->parent());
            REQUIRE(p->child_count() == 0);
            REQUIRE(p->remove_all_children() == 0);
        }
    }
    SECTION("remove_from_parent") {
        auto p = node::create(w);
        auto n1 = node::create(w, p);
        auto n2 = node::create(w, p);

        auto np = node::create(w);
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
    SECTION("child_count/child_count_recursive") {
        auto p1 = node::create(w);
        REQUIRE(p1->child_count() == 0);
        REQUIRE(p1->child_count_recursive() == 0);

        auto p2 = node::create(w, p1);
        REQUIRE(p1->child_count() == 1);
        REQUIRE(p1->child_count_recursive() == 1);
        REQUIRE(p2->child_count() == 0);
        REQUIRE(p2->child_count_recursive() == 0);

        auto n1 = node::create(w, p2);
        auto n2 = node::create(w, p2);
        REQUIRE(p1->child_count() == 1);
        REQUIRE(p1->child_count_recursive() == 3);
        REQUIRE(p2->child_count() == 2);
        REQUIRE(p2->child_count_recursive() == 2);
    }
    SECTION("send_backward/bring_to_back") {
        {
            auto p = node::create(w);
            auto n1 = node::create(w, p);
            auto n2 = node::create(w, p);
            auto n3 = node::create(w, p);

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
            auto n = node::create(w);
            REQUIRE_FALSE(n->send_backward());
            REQUIRE_FALSE(n->bring_to_back());
        }
    }
    SECTION("send_backward/bring_to_back/events") {
        {
            auto p = fake_node::create(w);
            auto n1 = fake_node::create(w, p);
            auto n2 = fake_node::create(w, p);
            auto n3 = fake_node::create(w, p);

            REQUIRE(n3->send_backward()); // n1 n3 n2
            REQUIRE(n3->send_backward()); // n3 n1 n2
            REQUIRE_FALSE(n3->send_backward()); // n3 n1 n2
            REQUIRE(n2->bring_to_back()); // n2 n3 n1
            REQUIRE(n2->bring_to_back()); // n2 n3 n1
        }
        {
            auto n = fake_node::create(w);
            REQUIRE_FALSE(n->send_backward());
            REQUIRE_FALSE(n->bring_to_back());
        }
    }
    SECTION("send_forward/bring_to_front") {
        {
            auto p = node::create(w);
            auto n1 = node::create(w, p);
            auto n2 = node::create(w, p);
            auto n3 = node::create(w, p);

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
            auto n = node::create(w);
            REQUIRE_FALSE(n->send_forward());
            REQUIRE_FALSE(n->bring_to_back());
        }
    }
    SECTION("send_forward/bring_to_front/events") {
        {
            auto p = fake_node::create(w);
            auto n1 = fake_node::create(w, p);
            auto n2 = fake_node::create(w, p);
            auto n3 = fake_node::create(w, p);

            REQUIRE(n1->send_forward()); // n2 n1 n3
            REQUIRE(n1->send_forward()); // n2 n3 n1
            REQUIRE_FALSE(n1->send_forward()); // n2 n3 n1
            REQUIRE(n2->bring_to_front()); // n3 n1 n2
            REQUIRE(n2->bring_to_front()); // n3 n1 n2
        }
        {
            auto n = fake_node::create(w);
            REQUIRE_FALSE(n->send_forward());
            REQUIRE_FALSE(n->bring_to_back());
        }
    }
    SECTION("last_child/first_child") {
        auto p = node::create(w);

        REQUIRE_FALSE(p->last_child());
        REQUIRE_FALSE(p->first_child());

        {
            const_node_iptr cp = p;
            REQUIRE_FALSE(cp->last_child());
            REQUIRE_FALSE(cp->first_child());
        }

        auto n1 = node::create(w, p);
        auto n2 = node::create(w, p);
        auto n3 = node::create(w, p);

        REQUIRE(p->last_child() == n3);
        REQUIRE(p->first_child() == n1);

        {
            const_node_iptr cp = p;

            REQUIRE(cp->last_child() == n3);
            REQUIRE(cp->first_child() == n1);
        }
    }
    SECTION("prev_sibling/next_sibling") {
        auto p = node::create(w);

        auto n1 = node::create(w, p);
        auto n2 = node::create(w, p);
        auto n3 = node::create(w, p);

        REQUIRE_FALSE(n1->prev_sibling());
        REQUIRE(n1->next_sibling() == n2);

        REQUIRE(n2->prev_sibling() == n1);
        REQUIRE(n2->next_sibling() == n3);

        REQUIRE(n3->prev_sibling() == n2);
        REQUIRE_FALSE(n3->next_sibling());

        auto n4 = node::create(w);
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
        auto p = node::create(w);
        auto n1 = node::create(w);
        auto n2 = node::create(w);
        auto n3 = node::create(w);
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
            auto p2 = node::create(w);
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
            auto p2 = node::create(w);
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
    SECTION("add_child_after/add_child_before") {
        auto p = node::create(w);
        auto n1 = node::create(w);
        auto n2 = node::create(w);
        auto n3 = node::create(w);
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
            auto p2 = node::create(w);
            auto n4 = node::create(w, p2); // n4
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
            auto p2 = node::create(w);
            auto n4 = node::create(w, p2); // n4
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
    SECTION("for_each_child") {
        auto p = node::create(w);
        array<node_iptr, 3> ns{
            node::create(w, p),
            node::create(w, p),
            node::create(w, p)};
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
        auto p1 = node::create(w);
        auto p2 = node::create(w, p1);
        auto n1 = node::create(w, p2);
        auto n2 = node::create(w, p2);

        p2->remove_from_parent();
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
    SECTION("transform") {
        auto p = node::create(w);

        REQUIRE(p->transform() == t3f::identity());
        REQUIRE(p->translation() == v3f::zero());
        REQUIRE(p->rotation() == q4f::identity());
        REQUIRE(p->scale() == v3f::unit());

        p->translation(v3f(1,2,3));
        REQUIRE(p->translation() == v3f(1,2,3));

        p->rotation(q4f(1,2,3,4));
        REQUIRE(p->rotation() == q4f(1,2,3,4));

        p->scale(v3f(1,2,3));
        REQUIRE(p->scale() == v3f(1,2,3));
    }
    SECTION("local_matrix") {
        {
            auto p = node::create(w);
            p->transform(math::make_translation_trs3(v3f{10.f,0.f,0.f}));

            auto n = node::create(w, p);
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
            auto p = node::create(w);
            p->translation({10.f,0.f,0.f});

            auto n = node::create(w, p);
            n->translation({20.f,0.f,0.f});

            auto v = v4f(5.f,0.f,0.f,1.f);
            REQUIRE(v * n->world_matrix() == v4f{35.f,0.f,0.f,1.f});

            n->transform(math::make_scale_trs3(v3f(1.f,2.f,3.f)));
            REQUIRE(n->world_matrix() ==
                math::make_scale_matrix4(1.f,2.f,3.f) *
                math::make_translation_matrix4(10.f,0.f,0.f));
        }
        {
            auto n = node::create(w);
            n->translation({20.f,0.f,0.f});
            REQUIRE(n->world_matrix() ==
                math::make_translation_matrix4(20.f,0.f,0.f));

            auto p = node::create(w);
            p->transform(math::make_translation_trs3(v3f{10.f,0.f,0.f}));

            p->add_child(n);
            REQUIRE(n->world_matrix() ==
                math::make_translation_matrix4(30.f,0.f,0.f));
        }
        {
            auto p1 = node::create(w);
            p1->translation({10.f,0.f,0.f});

            auto p2 = node::create(w);
            p2->transform(math::make_translation_trs3(v3f{20.f,0.f,0.f}));

            auto n = node::create(w, p2);
            n->transform(math::make_translation_trs3(v3f{30.f,0.f,0.f}));

            REQUIRE(n->world_matrix() ==
                math::make_translation_matrix4(50.f,0.f,0.f));

            p1->add_child(p2);

            REQUIRE(n->world_matrix() ==
                math::make_translation_matrix4(60.f,0.f,0.f));
        }
    }
    SECTION("lifetime") {
        {
            fake_node::reset_counters();
            auto p = fake_node::create(w);

            auto n = fake_node::create(w);
            p->add_child(n);
            REQUIRE(fake_node::s_ctor_count == 2);
            REQUIRE(fake_node::s_dtor_count == 0);

            n.reset();
            REQUIRE(fake_node::s_ctor_count == 2);
            REQUIRE(fake_node::s_dtor_count == 0);
            REQUIRE(p->child_count() == 1);

            p.reset();
            REQUIRE(fake_node::s_ctor_count == 2);
            REQUIRE(fake_node::s_dtor_count == 2);
        }
        {
            fake_node::reset_counters();
            auto p = fake_node::create(w);
            {
                auto n1 = fake_node::create(w, p);
                auto n2 = fake_node::create(w, p);
            }
            REQUIRE(fake_node::s_ctor_count == 3);
            REQUIRE(fake_node::s_dtor_count == 0);
            p->remove_all_children();
            REQUIRE(fake_node::s_ctor_count == 3);
            REQUIRE(fake_node::s_dtor_count == 2);
        }
    }
}
