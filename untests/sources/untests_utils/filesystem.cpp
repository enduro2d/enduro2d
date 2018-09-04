/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("filesystem") {
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

            REQUIRE_FALSE(filesystem::remove(parent_dir_path));

            REQUIRE(filesystem::remove_directory(child_dir_path));
            REQUIRE(filesystem::remove_directory(parent_dir_path));
            REQUIRE_FALSE(filesystem::exists(child_dir_path));
            REQUIRE_FALSE(filesystem::exists(parent_dir_path));
        }
    }
}
