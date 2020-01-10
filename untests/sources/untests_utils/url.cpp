/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("url") {
    {
        url u;
        REQUIRE(u.empty());
        REQUIRE(u.scheme().empty());
        REQUIRE(u.path().empty());
    }
    {
        url u("://");
        REQUIRE(u.empty());
        REQUIRE(u.scheme().empty());
        REQUIRE(u.path().empty());
        REQUIRE(u.schemepath() == "://");
    }
    {
        url u("://file");
        REQUIRE(!u.empty());
        REQUIRE(u.scheme().empty());
        REQUIRE(u.path() == "file");
        REQUIRE(u.schemepath() == "://file");
    }
    {
        url u("file://");
        REQUIRE(u.empty());
        REQUIRE(u.scheme() == "file");
        REQUIRE(u.path().empty());
        REQUIRE(u.schemepath() == "file://");
    }
    {
        url u("file://test_file");
        REQUIRE(!u.empty());
        REQUIRE(u.scheme() == "file");
        REQUIRE(u.path() == "test_file");
        REQUIRE(u.schemepath() == "file://test_file");
    }
    {
        url u("dir/file");
        REQUIRE(!u.empty());
        REQUIRE(u.scheme().empty());
        REQUIRE(u.path() == "dir/file");
    }
    {
        url u("file", "test_file");
        REQUIRE(!u.empty());
        REQUIRE(u.scheme() == "file");
        REQUIRE(u.path() == "test_file");
    }
    {
        url u;
        u = url("http://www.enduro2d.com/showcase");
        REQUIRE(u.scheme() == "http");
        REQUIRE(u.path() == "www.enduro2d.com/showcase");
        url o("file", "test_file");
        u = o;
        REQUIRE(u.scheme() == "file");
        REQUIRE(u.path() == "test_file");
        url u2(o);
        REQUIRE(u2.scheme() == "file");
        REQUIRE(u2.path() == "test_file");
        REQUIRE(!o.empty());
        url u3(std::move(o));
        REQUIRE(u3.scheme() == "file");
        REQUIRE(u3.path() == "test_file");
        REQUIRE(o.empty());
        u3.clear();
        REQUIRE(u3.empty());
        REQUIRE(u3.scheme().empty());
        REQUIRE(u3.path().empty());
    }
    {
        REQUIRE(url("fi:le://test_file") == url("fi:le", "test_file"));
        REQUIRE_FALSE(url("file://test_file") != url("file", "test_file"));

        REQUIRE_FALSE(url("file://test_file") == url("http", "test_file"));
        REQUIRE(url("file://test_file") != url("http", "test_file"));

        REQUIRE_FALSE(url("file://test_file") == url("file", "test_file2"));
        REQUIRE(url("file://test_file") != url("file", "test_file2"));

        REQUIRE_FALSE(url("file://test_file") == url("http", "test_file2"));
        REQUIRE(url("file://test_file") != url("http", "test_file2"));
    }
    {
        REQUIRE(
            url("http://enduro2d.com/show") + "case" ==
            url("http://enduro2d.com/showcase"));
        url u("http://enduro2d.c");
        REQUIRE(&u == &(u += "om"));
        REQUIRE(&u == &(u /= "showcase"));
        REQUIRE(u == url("http://enduro2d.com/showcase"));
        REQUIRE(&u == &u.concat("2"));
        REQUIRE(u == url("http://enduro2d.com/showcase2"));
        REQUIRE(&u == &u.append("game"));
        REQUIRE(u == url("http://enduro2d.com/showcase2/game"));
    }
    {
        REQUIRE(
            url("http://enduro2d.com") / "showcase" / "game" ==
            url("http://enduro2d.com/showcase/game"));
    }
    {
        REQUIRE(url("http://enduro2d.com") / "showcase" ==
                url("http://enduro2d.com/showcase"));
        REQUIRE(url("http://enduro2d.com/") / "showcase" ==
                url("http://enduro2d.com/showcase"));
        REQUIRE(url("http://enduro2d.com\\") / "showcase" ==
                url("http://enduro2d.com\\showcase"));

        REQUIRE(url() / "" == url());
        REQUIRE(url() / "test_file" == url("", "test_file"));
        REQUIRE(url("http", "") / "test_file" == url("http", "test_file"));
        REQUIRE(url("http", "dir") / "test_file" == url("http", "dir/test_file"));

        REQUIRE(url() / "/test_file" == url("", "/test_file"));
        REQUIRE(url("http", "") / "/test_file" == url("http", "/test_file"));
        REQUIRE(url("http", "dir") / "/test_file" == url("http", "/test_file"));

        REQUIRE(url() / "\\test_file" == url("", "\\test_file"));
        REQUIRE(url("http", "") / "\\test_file" == url("http", "\\test_file"));
        REQUIRE(url("http", "dir") / "\\test_file" == url("http", "\\test_file"));

        REQUIRE(url() / "C:\test_file" == url("", "C:\test_file"));
        REQUIRE(url("http", "") / "C:\test_file" == url("http", "C:\test_file"));
        REQUIRE(url("http", "dir") / "C:\test_file" == url("http", "C:\test_file"));
    }
}
