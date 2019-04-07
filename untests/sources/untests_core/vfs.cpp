/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core.hpp"
using namespace e2d;

TEST_CASE("vfs"){
    const str_view file_path = "vfs_file_name";
    const str_view nofile_path = "vfs_file_name2";
    {
        REQUIRE(filesystem::remove_file(nofile_path));
        REQUIRE(filesystem::create_file(file_path));
        REQUIRE(filesystem::try_write_all(buffer{"hello", 5}, file_path, false));
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
            auto r = v.read({"file", file_path});
            REQUIRE(r);
            REQUIRE(streams::try_read_tail(b, r));
            REQUIRE(b == buffer{"hello", 5});
            REQUIRE(v.read({"file2", file_path}) == input_stream_uptr());
            REQUIRE(v.read({"file", nofile_path}) == input_stream_uptr());
        }
        {
            buffer b0;
            REQUIRE(v.load({"file", file_path}, b0));
            REQUIRE(b0 == buffer{"hello", 5});

            auto b1 = v.load_async({"file", file_path}).get();
            REQUIRE(b1 == buffer{"hello", 5});

            str b2;
            REQUIRE(v.load_as_string({"file", file_path}, b2));
            REQUIRE(b2 == "hello");

            auto b3 = v.load_as_string_async({"file", file_path}).get();
            REQUIRE(b3 == "hello");
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
                v.read(url("resources://bin/noresources.zip"))));

            REQUIRE(v.register_scheme<filesystem_file_source>("file"));

            REQUIRE_FALSE(v.register_scheme<archive_file_source>(
                "archive",
                v.read(url("resources://bin/noresources.zip"))));

            REQUIRE(v.register_scheme<archive_file_source>(
                "archive",
                v.read(url("resources://bin/resources.zip"))));

            REQUIRE(v.exists({"archive", "test.txt"}));
            REQUIRE_FALSE(v.exists({"archive", "TEst.txt"}));

            REQUIRE(v.exists({"archive", "folder/file.txt"}));
            REQUIRE_FALSE(v.exists({"archive", "FOLder/file.txt"}));

            REQUIRE_FALSE(v.exists({"archive", "test2.txt"}));
            REQUIRE_FALSE(v.exists({"archive", "folder/file2.txt"}));
            {
                vector<std::pair<str,bool>> result;
                REQUIRE(v.extract(url("archive://"), std::back_inserter(result)));
                REQUIRE(result == vector<std::pair<str, bool>>{
                    {"folder/", true},
                    {"folder/file.txt", false},
                    {"test.txt", false},
                    {"folder/subfolder/", true},
                    {"folder/subfolder/file.txt", false},
                    {"folder2/", true},
                    {"folder2/file.txt", false},
                    {"folder2/subfolder2/", true},
                    {"folder2/subfolder2/file.txt", false}
                });
            }
            {
                vector<std::pair<str,bool>> result;
                REQUIRE(v.extract(url("archive://folder"), std::back_inserter(result)));
                REQUIRE(result == vector<std::pair<str, bool>>{
                    {"folder/file.txt", false},
                    {"folder/subfolder/", true},
                    {"folder/subfolder/file.txt", false}
                });
            }
            {
                vector<std::pair<str,bool>> result;
                REQUIRE(v.extract(url("archive://folder2/subfolder2"), std::back_inserter(result)));
                REQUIRE(result == vector<std::pair<str, bool>>{
                    {"folder2/subfolder2/file.txt", false}
                });
            }
            {
                vector<std::pair<str,bool>> result, result2;
                REQUIRE(v.extract(url("archive://folder"), std::back_inserter(result)));
                REQUIRE(v.extract(url("archive://folder/"), std::back_inserter(result2)));
                REQUIRE(result == result2);
            }
            {
                vector<std::pair<str,bool>> result;
                REQUIRE_FALSE(v.extract(url("archive://fold"), std::back_inserter(result)));
                REQUIRE_FALSE(v.extract(url("archive://folder3"), std::back_inserter(result)));
                REQUIRE_FALSE(v.extract(url("archive://test.txt"), std::back_inserter(result)));
            }
            {
                auto f = v.read(url("archive://test.txt"));
                REQUIRE(f);
                buffer b;
                REQUIRE(streams::try_read_tail(b, f));
                REQUIRE(b == buffer("hello", 5));
            }
            {
                buffer b0;
                REQUIRE(v.load(url("archive://test.txt"), b0));
                REQUIRE(b0 == buffer("hello", 5));

                auto b1 = v.load_async(url("archive://test.txt")).get();
                REQUIRE(b1 == buffer("hello", 5));

                str b2;
                REQUIRE(v.load_as_string(url("archive://test.txt"), b2));
                REQUIRE(b2 == "hello");

                auto b3 = v.load_as_string_async(url("archive://test.txt")).get();
                REQUIRE(b3 == "hello");
            }
            {
                auto f = v.read(url("archive://folder/file.txt"));
                REQUIRE(f);
                buffer b;
                REQUIRE(streams::try_read_tail(b, f));
                REQUIRE(b == buffer("world", 5));
            }
            {
                buffer b0;
                REQUIRE(v.load(url("archive://folder/file.txt"), b0));
                REQUIRE(b0 == buffer("world", 5));

                auto b1 = v.load_async(url("archive://folder/file.txt")).get();
                REQUIRE(b1 == buffer("world", 5));

                str b2;
                REQUIRE(v.load_as_string(url("archive://folder/file.txt"), b2));
                REQUIRE(b2 == "world");

                auto b3 = v.load_as_string_async(url("archive://folder/file.txt")).get();
                REQUIRE(b3 == "world");
            }
            {
                REQUIRE(v.read(url("archive://TEst.txt")) == input_stream_uptr());

                buffer b0;
                REQUIRE_FALSE(v.load(url("archive://TEst.txt"), b0));
                REQUIRE(b0.empty());

                REQUIRE_THROWS_AS(
                    v.load_async(url("archive://TEst.txt")).get(),
                    vfs_load_async_exception);

                str b2;
                REQUIRE_FALSE(v.load_as_string(url("archive://TEst.txt"), b2));
                REQUIRE(b2.empty());

                REQUIRE_THROWS_AS(
                    v.load_as_string_async(url("archive://TEst.txt")).get(),
                    vfs_load_async_exception);
            }
            {
                auto f = v.read(url("archive://test.txt"));
                REQUIRE(f);
                REQUIRE(v.unregister_scheme("archive"));
                buffer b;
                REQUIRE(streams::try_read_tail(b, f));
                REQUIRE(b == buffer("hello", 5));
            }
        }
    }
}
