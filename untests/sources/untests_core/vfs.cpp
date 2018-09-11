/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_core.hpp"
using namespace e2d;

TEST_CASE("vfs"){
    const str_view file_path = "vfs_file_name";
    const str_view nofile_path = "vfs_file_name2";
    {
        REQUIRE(filesystem::remove_file(nofile_path));
        REQUIRE(filesystem::create_file(file_path));
        REQUIRE(filesystem::try_write_all({"hello", 5}, file_path, false));
    }
    {
        vfs v;
        REQUIRE(v.register_scheme<filesystem_file_source>("file"));
        {
            REQUIRE(v.exists({"file", file_path}));
            REQUIRE_FALSE(v.exists({"file2", file_path}));
            REQUIRE_FALSE(v.exists({"file", nofile_path}));
        }
        {
            buffer b;
            auto r = v.open({"file", file_path});
            REQUIRE(r);
            REQUIRE(streams::try_read_tail(b, r));
            REQUIRE(b == buffer{"hello", 5});
            REQUIRE(v.open({"file2", file_path}) == input_stream_uptr());
            REQUIRE(v.open({"file", nofile_path}) == input_stream_uptr());
        }
        {
            auto r = v.load({"file", file_path});
            REQUIRE(r.second);
            REQUIRE(r.first == buffer{"hello", 5});

            auto r2 = v.load_async({"file", file_path}).get();
            REQUIRE(r2.second);
            REQUIRE(r2.first == buffer{"hello", 5});
        }
    }
    {
        vfs v;
        v.register_scheme_alias("home", url("file://~"));
        v.register_scheme_alias("save", url("home://game/saves"));
        REQUIRE(v.resolve_scheme_aliases({"home", "file.txt"}) == url("file://~/file.txt"));
        REQUIRE(v.resolve_scheme_aliases({"save", "save.txt"}) == url("file://~/game/saves/save.txt"));
    }
    SECTION("archive"){
        vfs v;
        {
            str resources;
            REQUIRE(filesystem::extract_predef_path(resources, filesystem::predef_path::resources));
            REQUIRE(v.register_scheme_alias("resources", {"file", resources}));

            REQUIRE_FALSE(v.register_scheme<archive_file_source>(
                "archive",
                v.open(url("resources://bin/noresources.zip"))));

            REQUIRE(v.register_scheme<filesystem_file_source>("file"));

            REQUIRE_FALSE(v.register_scheme<archive_file_source>(
                "archive",
                v.open(url("resources://bin/noresources.zip"))));

            REQUIRE(v.register_scheme<archive_file_source>(
                "archive",
                v.open(url("resources://bin/resources.zip"))));

            REQUIRE(v.exists({"archive", "test.txt"}));
            REQUIRE_FALSE(v.exists({"archive", "TEst.txt"}));

            REQUIRE(v.exists({"archive", "folder/file.txt"}));
            REQUIRE_FALSE(v.exists({"archive", "FOLder/file.txt"}));

            REQUIRE_FALSE(v.exists({"archive", "test2.txt"}));
            REQUIRE_FALSE(v.exists({"archive", "folder/file2.txt"}));
            {
                auto f = v.open(url("archive://test.txt"));
                REQUIRE(f);
                buffer b;
                REQUIRE(streams::try_read_tail(b, f));
                REQUIRE(b == buffer("hello", 5));
            }
            {
                auto r = v.load(url("archive://test.txt"));
                REQUIRE(r.second);
                REQUIRE(r.first == buffer("hello", 5));

                auto r2 = v.load_async(url("archive://test.txt")).get();
                REQUIRE(r2.second);
                REQUIRE(r2.first == buffer("hello", 5));
            }
            {
                auto f = v.open(url("archive://folder/file.txt"));
                REQUIRE(f);
                buffer b;
                REQUIRE(streams::try_read_tail(b, f));
                REQUIRE(b == buffer("world", 5));
            }
            {
                auto r = v.load(url("archive://folder/file.txt"));
                REQUIRE(r.second);
                REQUIRE(r.first == buffer("world", 5));

                auto r2 = v.load_async(url("archive://folder/file.txt")).get();
                REQUIRE(r2.second);
                REQUIRE(r2.first == buffer("world", 5));
            }
            {
                REQUIRE(v.open(url("archive://TEst.txt")) == input_stream_uptr());

                auto r = v.load(url("archive://TEst.txt"));
                REQUIRE_FALSE(r.second);
                REQUIRE(r.first == buffer());

                auto r2 = v.load_async(url("archive://TEst.txt")).get();
                REQUIRE_FALSE(r2.second);
                REQUIRE(r2.first == buffer());
            }
            {
                auto f = v.open(url("archive://test.txt"));
                REQUIRE(f);
                REQUIRE(v.unregister_scheme("archive"));
                buffer b;
                REQUIRE(streams::try_read_tail(b, f));
                REQUIRE(b == buffer("hello", 5));
            }
        }
    }
}
