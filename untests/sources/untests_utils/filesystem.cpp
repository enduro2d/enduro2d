/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("filesystem") {
    E2D_DEFER([](){
        filesystem::remove_file("files_test");
    });
    SECTION("files") {
        {
            auto f = make_write_file("files_test", false);
            REQUIRE(f);
            REQUIRE(f->path() == "files_test");
            REQUIRE(f->tell() == 0);
            REQUIRE(f->write("hello", 5) == 5);
            REQUIRE(f->tell() == 5);
        }
        {
            auto f = make_read_file("files_test");
            REQUIRE(f);
            REQUIRE(f->path() == "files_test");
            REQUIRE(f->tell() == 0);
            REQUIRE(f->length() == 5);
            char buf[5] = {'\0'};
            REQUIRE(f->read(buf, 5) == 5);
            REQUIRE(f->tell() == 5);
            REQUIRE(std::memcmp(buf, "hello", 5) == 0);
        }
        {
            auto f = make_write_file("files_test", true);
            REQUIRE(f);
            REQUIRE(f->path() == "files_test");
            REQUIRE(f->tell() == 5);
            REQUIRE(f->write("world", 5) == 5);
            REQUIRE(f->tell() == 10);
        }
        {
            auto f = make_read_file("files_test");
            REQUIRE(f);
            REQUIRE(f->path() == "files_test");
            REQUIRE(f->tell() == 0);
            REQUIRE(f->length() == 10);
            char buf[10] = {'\0'};
            REQUIRE(f->read(buf, 10) == 10);
            REQUIRE(f->tell() == 10);
            REQUIRE(std::memcmp(buf, "helloworld", 10) == 0);
        }
    }
    SECTION("filesystem") {
        {
            std::printf("-= filesystem::extract_predef_path tests\n");
            str path;
            std::printf("home: %s\n",
                filesystem::extract_predef_path(path, filesystem::predef_path::home) ? path.c_str() : "error");
            std::printf("appdata: %s\n",
                filesystem::extract_predef_path(path, filesystem::predef_path::appdata) ? path.c_str() : "error");
            std::printf("desktop: %s\n",
                filesystem::extract_predef_path(path, filesystem::predef_path::desktop) ? path.c_str() : "error");
            std::printf("working: %s\n",
                filesystem::extract_predef_path(path, filesystem::predef_path::working) ? path.c_str() : "error");
            std::printf("documents: %s\n",
                filesystem::extract_predef_path(path, filesystem::predef_path::documents) ? path.c_str() : "error");
            std::printf("resources: %s\n",
                filesystem::extract_predef_path(path, filesystem::predef_path::resources) ? path.c_str() : "error");
            std::printf("executable: %s\n",
                filesystem::extract_predef_path(path, filesystem::predef_path::executable) ? path.c_str() : "error");
        }
        {
            const str_view child_dir_name = "test_filesystem_file_name";
            REQUIRE(filesystem::remove_file(child_dir_name));
            REQUIRE_FALSE(filesystem::exists(child_dir_name));

            REQUIRE(filesystem::create_file(child_dir_name));
            REQUIRE(filesystem::exists(child_dir_name));
            REQUIRE(filesystem::file_exists(child_dir_name));

            buffer data("hello", 5);
            REQUIRE(filesystem::try_write_all(data, child_dir_name, false));
            {
                buffer d1;
                REQUIRE(filesystem::try_read_all(d1, child_dir_name));
                REQUIRE(data == d1);
            }
            REQUIRE(filesystem::create_file(child_dir_name));
            REQUIRE(filesystem::file_exists(child_dir_name));
            {
                buffer d1;
                REQUIRE(filesystem::try_read_all(d1, child_dir_name));
                REQUIRE(data == d1);
            }
            REQUIRE(filesystem::remove_file(child_dir_name));
            REQUIRE_FALSE(filesystem::exists(child_dir_name));
        }
        {
            const str child_dir_name = "test_filesystem_child_dir";
            const str parent_dir_path = "test_filesystem_parent_dir";
            const str child_dir_path = path::combine(parent_dir_path, child_dir_name);
            REQUIRE(filesystem::remove(child_dir_path));
            REQUIRE(filesystem::remove(parent_dir_path));
            REQUIRE_FALSE(filesystem::exists(child_dir_path));
            REQUIRE_FALSE(filesystem::exists(parent_dir_path));
            REQUIRE_FALSE(filesystem::file_exists(child_dir_path));
            REQUIRE_FALSE(filesystem::file_exists(parent_dir_path));
            REQUIRE_FALSE(filesystem::directory_exists(child_dir_path));
            REQUIRE_FALSE(filesystem::directory_exists(parent_dir_path));

            REQUIRE_FALSE(filesystem::create_directory(child_dir_path));
            REQUIRE_FALSE(filesystem::exists(child_dir_path));

            REQUIRE(filesystem::create_directory(parent_dir_path));

            REQUIRE(filesystem::exists(parent_dir_path));
            REQUIRE(filesystem::directory_exists(parent_dir_path));
            REQUIRE_FALSE(filesystem::file_exists(parent_dir_path));

            REQUIRE(filesystem::create_directory(child_dir_path));

            REQUIRE(filesystem::exists(child_dir_path));
            REQUIRE(filesystem::directory_exists(child_dir_path));
            REQUIRE_FALSE(filesystem::file_exists(child_dir_path));

            REQUIRE(filesystem::remove_directory(child_dir_path));
            REQUIRE(filesystem::remove_directory(parent_dir_path));
            REQUIRE_FALSE(filesystem::exists(child_dir_path));
            REQUIRE_FALSE(filesystem::exists(parent_dir_path));
        }
        {
            const str parent_dir_path = "test_filesystem_parent_dir";
            const str noparent_dir_path = "test_filesystem_noparent_dir";

            const str child1_dir_path = path::combine(parent_dir_path, "child1");
            const str child2_dir_path = path::combine(parent_dir_path, "child2");

            const str subchild1_dir_path_a = path::combine(child1_dir_path, "subchild_a");
            const str subchild1_dir_path_b = path::combine(child1_dir_path, "subchild_b");

            const str subchild2_dir_path_a = path::combine(child2_dir_path, "subchild_a");
            const str subchild2_dir_path_b = path::combine(child2_dir_path, "subchild_b");

            REQUIRE(filesystem::create_file(subchild1_dir_path_a));
            REQUIRE(filesystem::create_file(subchild1_dir_path_b));
            REQUIRE(filesystem::create_file(subchild2_dir_path_a));
            REQUIRE(filesystem::create_file(subchild2_dir_path_b));

            REQUIRE(filesystem::file_exists(subchild1_dir_path_a));
            REQUIRE(filesystem::file_exists(subchild1_dir_path_b));
            REQUIRE(filesystem::file_exists(subchild2_dir_path_a));
            REQUIRE(filesystem::file_exists(subchild2_dir_path_b));

            REQUIRE(filesystem::directory_exists(child1_dir_path));
            REQUIRE(filesystem::directory_exists(child2_dir_path));
            REQUIRE(filesystem::directory_exists(parent_dir_path));

            REQUIRE(filesystem::remove(noparent_dir_path));
            REQUIRE_FALSE(filesystem::exists(noparent_dir_path));
            REQUIRE_FALSE(filesystem::trace_directory(noparent_dir_path, [](str_view relative, bool directory) {
                E2D_UNUSED(relative, directory);
                return true;
            }));

            vector<std::pair<str,bool>> result;
            REQUIRE(filesystem::extract_directory(parent_dir_path, std::back_inserter(result)));
            std::sort(result.begin(), result.end(), [](const auto& a, const auto& b){
                return a.first < b.first;
            });
            REQUIRE(result == vector<std::pair<str,bool>>{
                {"child1", true},
                {"child2", true}
            });

            vector<std::pair<str,bool>> result_recursive;
            REQUIRE(filesystem::extract_directory_recursive(parent_dir_path, std::back_inserter(result_recursive)));
            std::sort(result_recursive.begin(), result_recursive.end(), [](const auto& a, const auto& b){
                return a.first < b.first;
            });
            REQUIRE(result_recursive == vector<std::pair<str,bool>>{
                {"child1", true},
                {"child1/subchild_a", false},
                {"child1/subchild_b", false},
                {"child2", true},
                {"child2/subchild_a", false},
                {"child2/subchild_b", false}
            });

            REQUIRE(filesystem::remove_directory(parent_dir_path));
            REQUIRE_FALSE(filesystem::file_exists(subchild1_dir_path_a));
            REQUIRE_FALSE(filesystem::file_exists(subchild2_dir_path_b));
        }
    }
}
