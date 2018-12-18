/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("buffer") {
    {
        REQUIRE(buffer().size() == 0);
        REQUIRE(buffer().data() == nullptr);
        REQUIRE(buffer(10).size() == 10);
        REQUIRE(buffer(10).data() != nullptr);
    }
    {
        buffer b("hello", 5);
        REQUIRE(b.size() == 5);
        REQUIRE(std::memcmp(b.data(), "hello", 5) == 0);
        b.clear();
        REQUIRE(b.size() == 0);
        REQUIRE(b.data() == nullptr);
    }
    {
        const buffer b0("hello", 5);
        buffer b1(b0);
        REQUIRE(b0.size() == 5);
        REQUIRE(std::memcmp(b0.data(), "hello", 5) == 0);
        REQUIRE(b1.size() == 5);
        REQUIRE(std::memcmp(b1.data(), "hello", 5) == 0);
        REQUIRE(b0.data() != b1.data());
        const u8* b1_data = b1.data();
        const buffer b2("world", 5);
        b1 = b2;
        REQUIRE(b1_data == b1.data());
        REQUIRE(b1.size() == 5);
        REQUIRE(std::memcmp(b1.data(), "world", 5) == 0);
    }
    {
        buffer b0("hello", 5);
        buffer b1(std::move(b0));
        REQUIRE(b0.size() == 0);
        REQUIRE(b0.data() == nullptr);
        REQUIRE(b1.size() == 5);
        REQUIRE(std::memcmp(b1.data(), "hello", 5) == 0);
        b1 = buffer("hello, world", 12);
        REQUIRE(b1.size() == 12);
        REQUIRE(std::memcmp(b1.data(), "hello, world", 12) == 0);
    }
    {
        buffer b0("hello", 5);
        buffer b1 = b0;
        REQUIRE(b0 == b1);
        REQUIRE_FALSE(b0 == buffer("world", 5));
        REQUIRE_FALSE(b0 != b1);
        REQUIRE(b0 != buffer("world", 5));
        REQUIRE_FALSE(b0 == buffer());
        REQUIRE(b0 != buffer());
        REQUIRE(buffer() == buffer());
        REQUIRE_FALSE(buffer() != buffer());
    }
    {
        buffer b0("hello", 5);
        buffer b1 = b0;
        b0.clear();
        REQUIRE(b1.size() == 5);
        REQUIRE(std::memcmp(b1.data(), "hello", 5) == 0);
    }
    {
        buffer b0;
        REQUIRE((&b0 == &b0.fill('a') && b0.size() == 0));
        b0.resize(5);
        REQUIRE_FALSE(std::memcmp(b0.data(), "aaaaa", 5) == 0);
        REQUIRE((&b0 == &b0.fill('a') && b0.size() == 5));
        REQUIRE(std::memcmp(b0.data(), "aaaaa", 5) == 0);
        REQUIRE((&b0 == &b0.fill('b') && b0.size() == 5));
        REQUIRE(std::memcmp(b0.data(), "bbbbb", 5) == 0);
        b0.resize(7);
        REQUIRE_FALSE(std::memcmp(b0.data(), "ccccccc", 7) == 0);
        b0.fill('c');
        REQUIRE(std::memcmp(b0.data(), "ccccccc", 7) == 0);
        b0.resize(7);
        REQUIRE(std::memcmp(b0.data(), "ccccccc", 7) == 0);
    }
    {
        buffer b0("hello", 5);
        buffer b1;
        REQUIRE(b1.empty());
        REQUIRE(&b1 == &b1.assign(std::move(b0)));
        REQUIRE_FALSE(b1.empty());
        REQUIRE((b0.empty() && b0.data() == nullptr && b0.size() == 0));
        REQUIRE((std::memcmp(b1.data(), "hello", 5) == 0 && b1.size() == 5));
        REQUIRE(&b1 == &b1.resize(4));
        REQUIRE((std::memcmp(b1.data(), "hell", 4) == 0 && b1.size() == 4));
        REQUIRE(&b1 == &b1.assign("world", 5));
        REQUIRE((std::memcmp(b1.data(), "world", 5) == 0 && b1.size() == 5));
        REQUIRE(&b1 == &b1.assign("hello", 5));
        REQUIRE((std::memcmp(b1.data(), "hello", 5) == 0 && b1.size() == 5));
        REQUIRE(&b1 == &b1.resize(0));
        REQUIRE((b1.data() == nullptr && b1.size() == 0));
    }
    {
        buffer b1("hello", 5);
        b1.assign(b1.data(), 3);
        REQUIRE(b1.size() == 3);
        REQUIRE(std::memcmp(b1.data(), "hel", 3) == 0);
    }
    {
        REQUIRE_FALSE(buffer() < buffer());
        REQUIRE_FALSE(buffer("aa",2) < buffer("aa",2));

        REQUIRE(buffer() < buffer("a",1));
        REQUIRE_FALSE(buffer("a",1) < buffer());

        REQUIRE(buffer("aa",2) < buffer("ab",2));
        REQUIRE_FALSE(buffer("ab",2) < buffer("aa",2));

        REQUIRE(buffer("aa",2) < buffer("aaa",3));
        REQUIRE(buffer("aa",2) < buffer("abb",3));
        REQUIRE_FALSE(buffer("aaa",3) < buffer("aa",2));
        REQUIRE_FALSE(buffer("abb",3) < buffer("aa",2));
    }
    {
        REQUIRE(buffer() == buffer());
        REQUIRE_FALSE(buffer() != buffer());

        REQUIRE_FALSE(buffer() == buffer("hello",5));
        REQUIRE_FALSE(buffer("hello",5) == buffer());
        REQUIRE(buffer("hello",5) == buffer("hello",5));
        REQUIRE_FALSE(buffer("hello",5) != buffer("hello",5));

        REQUIRE(buffer("hello",5) != buffer("world",5));
        REQUIRE_FALSE(buffer("hello",5) == buffer("world",5));

        REQUIRE(buffer("hello",5) != buffer("hello, world",12));
        REQUIRE_FALSE(buffer("hello",5) == buffer("hello, world",12));
    }
    {
        buffer b0("hello", 5);
        buffer b1("hello world", 11);
        b0.swap(b1);
        REQUIRE((std::memcmp(b1.data(), "hello", 5) == 0 && b1.size() == 5));
        REQUIRE((std::memcmp(b0.data(), "hello world", 11) == 0 && b0.size() == 11));
        swap(b0, b1);
        REQUIRE((std::memcmp(b0.data(), "hello", 5) == 0 && b0.size() == 5));
        REQUIRE((std::memcmp(b1.data(), "hello world", 11) == 0 && b1.size() == 11));
    }
    {
    #ifndef E2D_BUILD_WITH_SANITIZER
        const std::size_t msize = std::numeric_limits<std::size_t>::max();
        REQUIRE_THROWS_AS(buffer(msize), std::bad_alloc);
        REQUIRE_THROWS_AS(buffer(nullptr, msize), std::bad_alloc);
        {
            buffer b0;
            REQUIRE_THROWS_AS(b0.resize(msize), std::bad_alloc);
            REQUIRE((b0.data() == nullptr && b0.size() == 0));
            buffer b1("hello", 5);
            REQUIRE_THROWS_AS(b1.resize(msize), std::bad_alloc);
            REQUIRE((std::memcmp(b1.data(), "hello", 5) == 0 && b1.size() == 5));
        }
        {
            buffer b0;
            REQUIRE_THROWS_AS(b0.assign(nullptr, msize), std::bad_alloc);
            REQUIRE((b0.data() == nullptr && b0.size() == 0));

            buffer b1("hello", 5);
            REQUIRE_THROWS_AS(b1.assign(nullptr, msize), std::bad_alloc);
            REQUIRE((std::memcmp(b1.data(), "hello", 5) == 0 && b1.size() == 5));
        }
    #endif
    }
}
