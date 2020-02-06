/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("streams") {
    buffer hello_data("hello", 5);
    {
        input_stream_uptr s = make_memory_stream(hello_data);
        REQUIRE_THROWS_AS(s->seek(-10, false), bad_stream_operation);
        REQUIRE(s->tell() == 0);
        REQUIRE_THROWS_AS(s->seek(10, false), bad_stream_operation);
        REQUIRE_THROWS_AS(
            s->seek(std::numeric_limits<std::ptrdiff_t>::min(), false),
            bad_stream_operation);
        REQUIRE_THROWS_AS(
            s->seek(std::numeric_limits<std::ptrdiff_t>::max(), false),
            bad_stream_operation);
        REQUIRE(s->tell() == 0);
        REQUIRE((s->seek(5, false) == 5 && s->tell() == 5));
        REQUIRE((s->seek(3, false) == 3 && s->tell() == 3));
        REQUIRE((s->seek(0, false) == 0 && s->tell() == 0));
    }
    {
        input_stream_uptr s = make_memory_stream(hello_data);
        REQUIRE((s->seek(3, false) == 3 && s->tell() == 3));
        REQUIRE_THROWS_AS(s->seek(3, true), bad_stream_operation);
        REQUIRE(s->tell() == 3);
        REQUIRE((s->seek(2, true) == 5 && s->tell() == 5));
        REQUIRE_THROWS_AS(s->seek(-6, true), bad_stream_operation);
        REQUIRE(s->tell() == 5);
        REQUIRE((s->seek(-5, true) == 0 && s->tell() == 0));
    }
    {
        input_stream_uptr s = make_memory_stream(hello_data);
        REQUIRE((s->seek(3, false) == 3 && s->tell() == 3));
        REQUIRE_THROWS_AS(s->seek(-4, true), bad_stream_operation);
        REQUIRE(s->tell() == 3);
        REQUIRE((s->seek(-3, true) == 0 && s->tell() == 0));
        REQUIRE_THROWS_AS(s->seek(6, true), bad_stream_operation);
        REQUIRE(s->tell() == 0);
        REQUIRE((s->seek(5, true) == 5 && s->tell() == 5));
    }
    {
        input_stream_uptr s = make_memory_stream(hello_data);
        REQUIRE(s->tell() == 0);
        REQUIRE(s->length() == 5);
        char buf[5] = {'\0'};
        REQUIRE(s->read(buf, 5) == 5);
        REQUIRE(std::memcmp(buf, "hello", 5) == 0);
        REQUIRE(s->tell() == 5);
        REQUIRE(s->length() == 5);
    }
    {
        input_stream_uptr s = make_memory_stream(hello_data);
        char buf[10] = {'\0'};
        {
            REQUIRE(s->read(buf, 0) == 0);
            REQUIRE(std::memcmp(buf, "\0\0\0\0\0\0\0\0\0\0", 10) == 0);
            REQUIRE(s->tell() == 0);
            REQUIRE(s->length() == 5);
        }
        {
            REQUIRE(s->read(buf, 10) == 5);
            REQUIRE(std::memcmp(buf, "hello\0\0\0\0\0", 10) == 0);
            REQUIRE(s->tell() == 5);
            REQUIRE(s->length() == 5);
        }
        {
            REQUIRE(s->seek(2, false) == 2);
            REQUIRE(s->read(buf, 10) == 3);
            REQUIRE(std::memcmp(buf, "llolo\0\0\0\0\0", 10) == 0);
            REQUIRE(s->tell() == 5);
            REQUIRE(s->length() == 5);
        }
        {
            REQUIRE(s->seek(0, false) == 0);
            REQUIRE(s->read(buf, 3) == 3);
            REQUIRE(std::memcmp(buf, "hello\0\0\0\0\0", 10) == 0);
            REQUIRE(s->tell() == 3);
            REQUIRE(s->length() == 5);
        }
    }
    {
        input_stream_uptr s = make_memory_stream(hello_data);
        char buf[10] = {'\0'};
        {
            REQUIRE(s->read(buf, 1) == 1);
            REQUIRE(std::memcmp(buf, "h\0\0\0\0\0\0\0\0\0", 10) == 0);
            REQUIRE(s->tell() == 1);
            REQUIRE(s->length() == 5);
        }
        {
            buffer b;
            REQUIRE(streams::try_read_tail(b, s));
            REQUIRE(b.size() == 4);
            REQUIRE(std::memcmp(b.data(), "ello", 4) == 0);
            REQUIRE(s->tell() == 5);
            REQUIRE(s->length() == 5);
        }
    }
}
