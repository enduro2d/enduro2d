/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

namespace
{
    struct ilist_tag1 {};
    struct ilist_tag2 {};

    class obj_t
        : public intrusive_list_hook<ilist_tag1>
        , public intrusive_list_hook<ilist_tag2>
    {
    public:
        int i{42};
        obj_t() = default;
        obj_t(int ni) : i(ni) {}
    };
}

TEST_CASE("intrusive_list") {
    {
        intrusive_list<obj_t, ilist_tag1> l;
        REQUIRE(l.empty());
        REQUIRE_FALSE(l.size());
    }
    SECTION("push_back/push_front") {
        {
            auto o1 = std::make_unique<obj_t>(1);
            auto o2 = std::make_unique<obj_t>(2);
            auto o3 = std::make_unique<obj_t>(3);
            auto o4 = std::make_unique<obj_t>(4);

            intrusive_list<obj_t, ilist_tag1> l;

            l.push_back(*o1);
            REQUIRE(l.back().i == 1);
            REQUIRE(l.front().i == 1);

            l.push_back(*o2);
            REQUIRE(l.back().i == 2);
            REQUIRE(l.front().i == 1);

            l.push_front(*o3);
            REQUIRE(l.back().i == 2);
            REQUIRE(l.front().i == 3);

            l.push_front(*o4);
            REQUIRE(l.back().i == 2);
            REQUIRE(l.front().i == 4);
        }
    }
    SECTION("pop_back/pop_front") {
        {
            auto o1 = std::make_unique<obj_t>(1);
            auto o2 = std::make_unique<obj_t>(2);
            auto o3 = std::make_unique<obj_t>(3);
            auto o4 = std::make_unique<obj_t>(4);

            intrusive_list<obj_t, ilist_tag1> l;
            l.push_back(*o1);
            l.push_back(*o2);
            l.push_back(*o3);
            l.push_back(*o4);

            l.pop_front();
            REQUIRE(l.front().i == 2);
            REQUIRE(l.back().i == 4);

            l.pop_back();
            REQUIRE(l.front().i == 2);
            REQUIRE(l.back().i == 3);

            l.pop_front();
            REQUIRE(l.front().i == 3);
            REQUIRE(l.back().i == 3);

            l.pop_back();
            REQUIRE(l.empty());
        }
    }
    SECTION("pop_back_and_dispose/pop_front_and_dispose") {
        {
            auto o1 = std::make_unique<obj_t>(1);
            auto o2 = std::make_unique<obj_t>(2);
            auto o3 = std::make_unique<obj_t>(3);
            auto o4 = std::make_unique<obj_t>(4);

            intrusive_list<obj_t, ilist_tag1> l;
            l.push_back(*o1);
            l.push_back(*o2);
            l.push_back(*o3);
            l.push_back(*o4);

            std::size_t dispose_count = 0;
            l.pop_front_and_dispose([&dispose_count](obj_t* o){
                ++dispose_count;
                REQUIRE(o->i == 1);
            });
            REQUIRE(l.front().i == 2);
            REQUIRE(l.back().i == 4);

            l.pop_back_and_dispose([&dispose_count](obj_t* o){
                ++dispose_count;
                REQUIRE(o->i == 4);
            });
            REQUIRE(l.front().i == 2);
            REQUIRE(l.back().i == 3);

            l.pop_front_and_dispose([&dispose_count](obj_t* o){
                ++dispose_count;
                REQUIRE(o->i == 2);
            });
            REQUIRE(l.front().i == 3);
            REQUIRE(l.back().i == 3);

            l.pop_front_and_dispose([&dispose_count](obj_t* o){
                ++dispose_count;
                REQUIRE(o->i == 3);
            });
            REQUIRE(l.empty());
            REQUIRE(dispose_count == 4);
        }
    }
    SECTION("empty/size/clear/clear_and_dispose") {
        auto o1 = std::make_unique<obj_t>(1);
        auto o2 = std::make_unique<obj_t>(2);
        auto o3 = std::make_unique<obj_t>(3);
        {
            intrusive_list<obj_t, ilist_tag1> l;
            REQUIRE(l.empty());
            REQUIRE_FALSE(l.size());

            l.push_back(*o1);
            REQUIRE_FALSE(l.empty());
            REQUIRE(l.size() == 1u);

            l.push_front(*o2);
            REQUIRE_FALSE(l.empty());
            REQUIRE(l.size() == 2u);

            l.push_back(*o3);
            REQUIRE_FALSE(l.empty());
            REQUIRE(l.size() == 3u);

            l.pop_front();
            REQUIRE_FALSE(l.empty());
            REQUIRE(l.size() == 2u);

            l.pop_back();
            REQUIRE_FALSE(l.empty());
            REQUIRE(l.size() == 1u);

            l.pop_front();
            REQUIRE(l.empty());
            REQUIRE_FALSE(l.size());
        }
        {
            intrusive_list<obj_t, ilist_tag1> l;
            l.push_back(*o1);
            l.push_back(*o2);
            l.push_back(*o3);
            REQUIRE_FALSE(l.empty());
            REQUIRE(l.size() == 3u);
        }
        {
            intrusive_list<obj_t, ilist_tag1> l;
            l.push_back(*o1);
            l.push_back(*o2);
            l.push_back(*o3);
            l.clear();
            REQUIRE(l.empty());
        }
        {
            intrusive_list<obj_t, ilist_tag1> l;
            l.push_back(*o1);
            l.push_back(*o2);
            l.push_back(*o3);

            std::size_t dispose_count = 0;
            std::array<int, 3> obj_values{3,2,1};
            l.clear_and_dispose([&dispose_count, &obj_values](obj_t* o){
                REQUIRE(obj_values[dispose_count] == o->i);
                ++dispose_count;
            });
            REQUIRE(l.empty());
            REQUIRE(dispose_count == 3);
        }
    }
    SECTION("swap") {
        {
            auto o1 = std::make_unique<obj_t>(1);
            auto o2 = std::make_unique<obj_t>(2);
            auto o3 = std::make_unique<obj_t>(3);
            auto o4 = std::make_unique<obj_t>(4);

            intrusive_list<obj_t, ilist_tag1> l1;
            intrusive_list<obj_t, ilist_tag1> l2;

            l1.push_back(*o1);
            l1.push_back(*o2);

            l2.push_back(*o3);
            l2.push_back(*o4);

            l1.swap(l2);

            REQUIRE(l1.back().i == 4);
            REQUIRE(l1.front().i == 3);

            REQUIRE(l2.back().i == 2);
            REQUIRE(l2.front().i == 1);
        }
    }
    SECTION("insert/erase/erase_and_dispose") {
        auto o1 = std::make_unique<obj_t>(1);
        auto o2 = std::make_unique<obj_t>(2);
        auto o3 = std::make_unique<obj_t>(3);
        auto o4 = std::make_unique<obj_t>(4);
        {
            intrusive_list<obj_t, ilist_tag1> l;

            REQUIRE(l.insert(l.end(), *o1)->i == 1); // 1
            {
                auto i = l.insert(++l.begin(), *o2); // 1 2
                REQUIRE(std::distance(l.begin(), i) == 1);
            }
            {
                auto i = l.insert(l.begin(), *o3); // 3 1 2
                REQUIRE(std::distance(l.begin(), i) == 0);
            }
            {
                auto iter = l.begin();
                REQUIRE(iter->i == 3);
                REQUIRE((++iter)->i == 1);
                REQUIRE((++iter)->i == 2);
                REQUIRE(++iter == l.end());
            }

            // 3 1 2 4
            l.push_back(*o4);

            // 3 1 2
            REQUIRE(l.erase(--l.end()) == l.end());
            REQUIRE(l.size() == 3);
            REQUIRE(l.back().i == 2);
            REQUIRE(l.front().i == 3);

            // 3 2
            REQUIRE(l.erase(++l.begin())->i == 2);
            REQUIRE(l.size() == 2);
            REQUIRE(l.back().i == 2);
            REQUIRE(l.front().i == 3);

            // 2
            REQUIRE(l.erase(l.begin())->i == 2);
            REQUIRE(l.size() == 1);
            REQUIRE(l.back().i == 2);
            REQUIRE(l.front().i == 2);
        }
        {
            intrusive_list<obj_t, ilist_tag1> l;
            l.push_back(*o1);
            l.push_back(*o2);
            l.push_back(*o3);
            l.push_back(*o4);
            std::size_t dispose_count = 0;
            l.erase_and_dispose(l.begin(), [&dispose_count](obj_t* o){
                ++dispose_count;
                REQUIRE(o->i == 1);
            });
            l.erase_and_dispose(++l.begin(), [&dispose_count](obj_t* o){
                ++dispose_count;
                REQUIRE(o->i == 3);
            });
            l.erase_and_dispose(++l.begin(), [&dispose_count](obj_t* o){
                ++dispose_count;
                REQUIRE(o->i == 4);
            });
            l.erase_and_dispose(l.begin(), [&dispose_count](obj_t* o){
                ++dispose_count;
                REQUIRE(o->i == 2);
            });
            REQUIRE(dispose_count == 4);
        }
    }
    SECTION("iterator_to") {
        auto o1 = std::make_unique<obj_t>(1);
        auto o2 = std::make_unique<obj_t>(2);
        auto o3 = std::make_unique<obj_t>(3);
        auto o4 = std::make_unique<obj_t>(4);

        intrusive_list<obj_t, ilist_tag1> l;

        l.push_back(*o1);
        l.push_back(*o2);
        l.push_back(*o3);
        l.push_back(*o4);

        REQUIRE(l.iterator_to(*o1) == l.begin());
        REQUIRE(l.iterator_to(*o2) == ++l.begin());
        {
            const auto& co3 = o3;
            const auto& co4 = o4;
            REQUIRE(l.iterator_to(*co3) == --(--l.end()));
            REQUIRE(l.iterator_to(*co4) == --l.end());
        }
    }
    SECTION("auto_unlink") {
        {
            auto o1 = std::make_unique<obj_t>(1);
            auto o2 = std::make_unique<obj_t>(2);
            intrusive_list<obj_t, ilist_tag1> l;
            l.push_back(*o1);
            l.push_back(*o2);

            o1.reset();
            REQUIRE(l.size() == 1u);
            REQUIRE(l.back().i == 2);
            REQUIRE(l.front().i == 2);
        }
        {
            auto o1 = std::make_unique<obj_t>(1);
            auto o2 = std::make_unique<obj_t>(2);
            intrusive_list<obj_t, ilist_tag1> l;
            l.push_back(*o1);
            l.push_back(*o2);

            o2.reset();
            REQUIRE(l.size() == 1u);
            REQUIRE(l.back().i == 1);
            REQUIRE(l.front().i == 1);
        }
    }
    SECTION("multitag") {
        {
            auto o1 = std::make_unique<obj_t>();
            intrusive_list<obj_t, ilist_tag1> l1;
            intrusive_list<obj_t, ilist_tag2> l2;
            l1.push_back(*o1);
            l2.push_back(*o1);
            REQUIRE(l1.size() == 1);
            REQUIRE(l2.size() == 1);
            REQUIRE(l1.begin()->i == 42);
            REQUIRE(l2.begin()->i == 42);
            l2.pop_back();
            REQUIRE(l1.size() == 1);
            REQUIRE(l1.begin()->i == 42);
            REQUIRE(l2.empty());
        }
    }
    SECTION("iterator") {
        {
            auto o1 = std::make_unique<obj_t>(1);
            auto o2 = std::make_unique<obj_t>(2);
            intrusive_list<obj_t, ilist_tag1> l;
            l.push_back(*o1);
            l.push_back(*o2);
            std::transform(l.begin(), l.end(), l.begin(), [](obj_t& o){
                return o.i * 5;
            });
            {
                auto iter = l.begin();
                REQUIRE((*iter).i == 5);
                REQUIRE((++iter)->i == 10);
            }
            {
                auto iter = l.rbegin();
                REQUIRE((*iter).i == 10);
                REQUIRE((++iter)->i == 5);
            }
            REQUIRE(15 == std::accumulate(l.begin(), l.end(), 0, [](int acc, obj_t& o){
                return acc + o.i;
            }));
            REQUIRE(15 == std::accumulate(l.rbegin(), l.rend(), 0, [](int acc, obj_t& o){
                return acc + o.i;
            }));
            {
                const auto& cl = l;
                REQUIRE(15 == std::accumulate(cl.begin(), cl.end(), 0, [](int acc, const obj_t& o){
                    return acc + o.i;
                }));
                REQUIRE(15 == std::accumulate(cl.rbegin(), cl.rend(), 0, [](int acc, const obj_t& o){
                    return acc + o.i;
                }));
            }
            {
                REQUIRE(l.begin() == l.begin());
                REQUIRE(l.begin() == l.cbegin());
                REQUIRE(l.begin() != l.end());
                REQUIRE(l.begin() != l.cend());

                REQUIRE(l.cbegin() == l.begin());
                REQUIRE(l.cbegin() == l.cbegin());
                REQUIRE(l.cbegin() != l.end());
                REQUIRE(l.cbegin() != l.cend());

                REQUIRE(l.end() == l.end());
                REQUIRE(l.end() == l.cend());
                REQUIRE(l.end() != l.begin());
                REQUIRE(l.end() != l.cbegin());

                REQUIRE(l.cend() == l.end());
                REQUIRE(l.cend() == l.cend());
                REQUIRE(l.cend() != l.begin());
                REQUIRE(l.cend() != l.cbegin());
            }
            {
                REQUIRE(l.rbegin() == l.rbegin());
                REQUIRE(l.rbegin() == l.crbegin());
                REQUIRE(l.rbegin() != l.rend());
                REQUIRE(l.rbegin() != l.crend());

                REQUIRE(l.crbegin() == l.rbegin());
                REQUIRE(l.crbegin() == l.crbegin());
                REQUIRE(l.crbegin() != l.rend());
                REQUIRE(l.crbegin() != l.crend());

                REQUIRE(l.rend() == l.rend());
                REQUIRE(l.rend() == l.crend());
                REQUIRE(l.rend() != l.rbegin());
                REQUIRE(l.rend() != l.crbegin());

                REQUIRE(l.crend() == l.rend());
                REQUIRE(l.crend() == l.crend());
                REQUIRE(l.crend() != l.rbegin());
                REQUIRE(l.crend() != l.crbegin());
            }
        }
    }
}
