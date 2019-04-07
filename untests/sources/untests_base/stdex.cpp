/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_base.hpp"
using namespace e2d;

TEST_CASE("stdex") {
    {
        basic_string_view<char> v0;
        REQUIRE(v0.data() == nullptr);
        REQUIRE(v0.size() == 0);
        REQUIRE(v0.length() == 0);

        const char* s = "hello";

        basic_string_view<char> v1(s);
        REQUIRE(v1.data() == s);
        REQUIRE(v1.size() == 5);
        REQUIRE(v1.length() == 5);

        basic_string_view<char> v2(s, 4);
        REQUIRE(v2.data() == s);
        REQUIRE(v2.size() == 4);
        REQUIRE(v2.length() == 4);
    }
    {
        const char* s0 = "hell";
        const char* s1 = "world";
        basic_string_view<char> v0(s0);
        basic_string_view<char> v1(s1);
        v0.swap(v1);
        REQUIRE(v0.data() == s1);
        REQUIRE(v0.size() == 5);
        REQUIRE(v1.data() == s0);
        REQUIRE(v1.size() == 4);
    }
    {
        const char* s = "hello";
        basic_string_view<char> v0(s);
        REQUIRE(v0.data() == s);
        REQUIRE(v0.size() == 5);
        REQUIRE(v0.length() == 5);
        basic_string_view<char> v1(v0);
        REQUIRE(v1.data() == s);
        REQUIRE(v1.size() == 5);
        REQUIRE(v1.length() == 5);
        basic_string_view<char> v2;
        v2 = v1;
        REQUIRE(v2.data() == s);
        REQUIRE(v2.size() == 5);
        REQUIRE(v2.length() == 5);
    }
    {
        const char* s = "hello";
        basic_string_view<char> v0(s);

        REQUIRE(v0.begin() == s);
        REQUIRE(v0.cbegin() == s);
        REQUIRE(v0.rend().base() == s);
        REQUIRE(v0.crend().base() == s);

        REQUIRE(v0.rbegin().base() == s + 5);
        REQUIRE(v0.crbegin().base() == s + 5);
        REQUIRE(v0.end() == s + 5);
        REQUIRE(v0.cend() == s + 5);
    }
    {
        basic_string_view<char> v0;
        REQUIRE(v0.empty());
        v0 = basic_string_view<char>("world");
        REQUIRE_FALSE(v0.empty());
    }
    {
        basic_string_view<char> v0("world");
        REQUIRE(v0[0] == 'w');
        REQUIRE(v0[4] == 'd');
        REQUIRE(v0.at(1) == 'o');
        REQUIRE(v0.at(3) == 'l');
        REQUIRE_THROWS_AS(v0.at(5), std::out_of_range);
        REQUIRE(v0.front() == 'w');
        REQUIRE(v0.back() == 'd');
    }
    {
        const char* s0 = "world";
        basic_string_view<char> v0(s0);
        v0.remove_prefix(1);
        REQUIRE(v0.data() == s0 + 1);
        REQUIRE(v0.size() == 4);
        v0.remove_suffix(2);
        REQUIRE(v0.data() == s0 + 1);
        REQUIRE(v0.size() == 2);
    }
    {
        char buf[6] = {0};
        basic_string_view<char>("hello").copy(buf, 5);
        REQUIRE(std::strcpy(buf, "hello"));
        basic_string_view<char>("world").copy(buf, 3, 1);
        REQUIRE(std::memcmp(buf, "orllo\0", 6) == 0);
        basic_string_view<char>("hell").copy(buf, 0);
        REQUIRE(std::memcmp(buf, "orllo\0", 6) == 0);

        basic_string_view<char>().copy(buf, 0);
        REQUIRE(std::memcmp(buf, "orllo\0", 6) == 0);
        basic_string_view<char>().copy(buf, 10);
        REQUIRE(std::memcmp(buf, "orllo\0", 6) == 0);
        basic_string_view<char>("hell").copy(buf, 10, 4);
        REQUIRE(std::memcmp(buf, "orllo\0", 6) == 0);

        REQUIRE_THROWS_AS(basic_string_view<char>("hell").copy(buf, 1, 5), std::out_of_range);
        REQUIRE(std::memcmp(buf, "orllo\0", 6) == 0);
    }
    {
        const char* s0 = "world";
        basic_string_view<char> v0(s0);
        auto v1 = v0.substr(1, 3);
        REQUIRE(v1.data() == v0.data() + 1);
        REQUIRE(v1.size() == 3);
        v1 = basic_string_view<char>(s0).substr(3);
        REQUIRE(v1.data() == v0.data() + 3);
        REQUIRE(v1.size() == 2);
        v1 = basic_string_view<char>(s0).substr(5);
        REQUIRE(v1.data() == v0.data() + 5);
        REQUIRE(v1.size() == 0);
        REQUIRE_THROWS_AS(basic_string_view<char>("hello").substr(6), std::out_of_range);
    }
    {
        using sw = basic_string_view<char>;

        REQUIRE(sw("hello") == sw("hello"));
        REQUIRE(sw("hello") != sw("world"));
        REQUIRE_FALSE(sw("hello") != sw("hello"));
        REQUIRE_FALSE(sw("hello") == sw("world"));

        REQUIRE_FALSE(sw("hello") == sw("hell"));
        REQUIRE_FALSE(sw("hell") == sw("hello"));

        REQUIRE(sw("hello") != sw("hell"));
        REQUIRE(sw("hell") != sw("hello"));

        REQUIRE_FALSE(sw("abc") < sw("abc"));
        REQUIRE(sw("abc") < sw("abcd"));
        REQUIRE_FALSE(sw("abcd") < sw("abc"));
        REQUIRE(sw("abc") < sw("acc"));
        REQUIRE_FALSE(sw("acc") < sw("abc"));

        REQUIRE(sw("world") == "world");
        REQUIRE(("world" == sw("world")));
        REQUIRE_FALSE(sw("world") != "world");
        REQUIRE_FALSE(("world" != sw("world")));

        REQUIRE(sw("hell") < "hello");
        REQUIRE_FALSE(sw("hello") < "hell");

        REQUIRE_FALSE("hello" < sw("hell"));
        REQUIRE("hell" < sw("hello"));

        REQUIRE(sw("world") == str("world"));
        REQUIRE((str("world") == sw("world")));
        REQUIRE_FALSE(sw("world") != str("world"));
        REQUIRE_FALSE((str("world") != sw("world")));

        REQUIRE(sw("hell") < str("hello"));
        REQUIRE_FALSE(sw("hello") < str("hell"));

        REQUIRE_FALSE(str("hello") < sw("hell"));
        REQUIRE(str("hell") < sw("hello"));
    }
}
