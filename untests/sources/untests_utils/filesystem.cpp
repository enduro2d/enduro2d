/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("filesystem") {
    SECTION("path") {
        {
            const vector<str> absolute_paths = {
                "/",
                "/usr",
                "/usr/",
                "/usr/local",

                "C:",
                "C:\\",
                "C:\\windows",
                "C:\\windows\\",
                "C:\\windows\\system32",

                "\\\\.\\COM1",
                "\\\\?\\C:",
                "\\\\?\\UNC",
                "\\\\server",
            };
            const vector<str> relative_paths = {
                "",

                "usr",
                "usr/",
                "usr/local",

                "windows",
                "windows\\",
                "windows\\system32",
            };
            for ( const str& s : absolute_paths ) {
                REQUIRE(path::is_absolute(s));
                REQUIRE_FALSE(path::is_relative(s));
            }
            for ( const str& s : relative_paths ) {
                REQUIRE(path::is_relative(s));
                REQUIRE_FALSE(path::is_absolute(s));
            }
        }
        {
            // [lsh, rhs, result]
            using t = std::tuple<str, str, str>;
            const vector<t> combinations = {
                t{"", "", ""},

                t{".", ".", "./."},
                t{"..", "..", "../.."},

                t{".", "..", "./.."},
                t{"..", ".", "../."},

                t{"file", "", "file"},
                t{"", "file", "file"},

                t{"folder", "file", "folder/file"},
                t{"folder/", "file", "folder/file"},
                t{"folder\\", "file", "folder\\file"},

                t{"folder", "other/file", "folder/other/file"},
                t{"folder/", "other/file", "folder/other/file"},
                t{"folder\\", "other/file", "folder\\other/file"},

                t{"folder", "/file", "/file"},
                t{"folder/", "\\file", "\\file"},
                t{"folder\\", "C:\\file", "C:\\file"},
                t{"folder\\", "C:/file", "C:/file"},
            };
            for ( const auto& combination : combinations ) {
                str lhs, rhs, res;
                std::tie(lhs, rhs, res) = combination;
                REQUIRE(path::combine(lhs, rhs) == res);
            }
        }
        {
            // [path, stem, filename, extension]
            using t = std::tuple<str, str, str, str>;
            const vector<t> combinations = {
                t{"", "", "", ""},

                t{".", ".", ".", ""},
                t{"/.", ".", ".", ""},
                t{"\\.", ".", ".", ""},
                t{"folder.jpg/.", ".", ".", ""},
                t{"folder.jpg\\.", ".", ".", ""},

                t{"..", "..", "..", ""},
                t{"/..", "..", "..", ""},
                t{"\\..", "..", "..", ""},
                t{"folder.jpg/..", "..", "..", ""},
                t{"folder.jpg\\..", "..", "..", ""},

                t{"/", "", "", ""},
                t{"file.jpg/", "", "", ""},

                t{"\\", "", "", ""},
                t{"file.jpg\\", "", "", ""},

                t{"C:", "C:", "C:", ""},
                t{"C:/", "", "", ""},
                t{"C:\\", "", "", ""},

                t{"file", "file", "file", ""},
                t{"file.png", "file", "file.png", ".png"},
                t{"file.jpg.png", "file.jpg", "file.jpg.png", ".png"},

                t{"/file", "file", "file", ""},
                t{"/file.png", "file", "file.png", ".png"},
                t{"/file.jpg.png", "file.jpg", "file.jpg.png", ".png"},

                t{"\\file", "file", "file", ""},
                t{"\\file.png", "file", "file.png", ".png"},
                t{"\\file.jpg.png", "file.jpg", "file.jpg.png", ".png"},

                t{"folder.jpg/file", "file", "file", ""},
                t{"folder.jpg/file.png", "file", "file.png", ".png"},
                t{"folder.jpg/file.jpg.png", "file.jpg", "file.jpg.png", ".png"},

                t{"/folder.jpg/file", "file", "file", ""},
                t{"/folder.jpg/file.png", "file", "file.png", ".png"},
                t{"/folder.jpg/file.jpg.png", "file.jpg", "file.jpg.png", ".png"},

                t{"\\folder.jpg\\file", "file", "file", ""},
                t{"\\folder.jpg\\file.png", "file", "file.png", ".png"},
                t{"\\folder.jpg/file.jpg.png", "file.jpg", "file.jpg.png", ".png"},
            };
            for ( const auto& combination : combinations ) {
                const str path = std::get<0>(combination);
                REQUIRE(path::stem(path) == std::get<1>(combination));
                REQUIRE(path::filename(path) == std::get<2>(combination));
                REQUIRE(path::extension(path) == std::get<3>(combination));
            }
        }
        {
            // [path, remove_filename, remove_extension]
            using t = std::tuple<str, str, str>;
            const vector<t> combinations = {
                t{"", "", ""},
                t{"/", "/", "/"},
                t{"\\", "\\", "\\"},

                t{".", "", "."},
                t{"/.", "/", "/."},
                t{"\\.", "\\", "\\."},

                t{"..", "", ".."},
                t{"/..", "/", "/.."},
                t{"\\..", "\\", "\\.."},

                t{"usr", "", "usr"},
                t{"usr.", "", "usr"},
                t{"usr.png", "", "usr"},
                t{"usr.jpg.png", "", "usr.jpg"},

                t{"/usr", "/", "/usr"},
                t{"/usr.", "/", "/usr"},
                t{"/usr.png", "/", "/usr"},
                t{"/usr.jpg.png", "/", "/usr.jpg"},

                t{"\\usr", "\\", "\\usr"},
                t{"\\usr.", "\\", "\\usr"},
                t{"\\usr.png", "\\", "\\usr"},
                t{"\\usr.jpg.png", "\\", "\\usr.jpg"},

                t{"folder.jpg/usr", "folder.jpg/", "folder.jpg/usr"},
                t{"folder.jpg/usr.", "folder.jpg/", "folder.jpg/usr"},
                t{"folder.jpg/usr.png", "folder.jpg/", "folder.jpg/usr"},
                t{"folder.jpg/usr.jpg.png", "folder.jpg/", "folder.jpg/usr.jpg"},

                t{"/folder.jpg/usr", "/folder.jpg/", "/folder.jpg/usr"},
                t{"/folder.jpg/usr.", "/folder.jpg/", "/folder.jpg/usr"},
                t{"/folder.jpg/usr.png", "/folder.jpg/", "/folder.jpg/usr"},
                t{"/folder.jpg/usr.jpg.png", "/folder.jpg/", "/folder.jpg/usr.jpg"},

                t{"\\folder.jpg/usr", "\\folder.jpg/", "\\folder.jpg/usr"},
                t{"\\folder.jpg/usr.", "\\folder.jpg/", "\\folder.jpg/usr"},
                t{"\\folder.jpg/usr.png", "\\folder.jpg/", "\\folder.jpg/usr"},
                t{"\\folder.jpg/usr.jpg.png", "\\folder.jpg/", "\\folder.jpg/usr.jpg"},
            };
            for ( const auto& combination : combinations ) {
                const str path = std::get<0>(combination);
                REQUIRE(path::remove_filename(path) == std::get<1>(combination));
                REQUIRE(path::remove_extension(path) == std::get<2>(combination));
            }
        }
        {
            // [path, filename, result]
            using t = std::tuple<str, str, str>;
            const vector<t> combinations = {
                t{ "", "", "" },
                t{ "/", "", "/" },
                t{ "\\", "", "\\" },

                t{ "", "file", "file" },
                t{ "/", "file", "/file" },
                t{ "\\", "file", "\\file" },

                t{ ".", "file", "file" },
                t{ "/.", "file", "/file" },
                t{ "\\.", "file", "\\file" },
                t{ ".", "", "" },
                t{ "/.", "", "/" },
                t{ "\\.", "", "\\" },

                t{ "..", "file", "file" },
                t{ "/..", "file", "/file" },
                t{ "\\..", "file", "\\file" },
                t{ "..", "", "" },
                t{ "/..", "", "/" },
                t{ "\\..", "", "\\" },

                t{ "usr", "file", "file" },
                t{ "/usr", "file", "/file" },
                t{ "\\usr", "file", "\\file" },
                t{ "usr", "", "" },
                t{ "/usr", "", "/" },
                t{ "\\usr", "", "\\" },

                t{ "usr/", "file", "usr/file" },
                t{ "/usr/", "file", "/usr/file" },
                t{ "\\usr/", "file", "\\usr/file" },
                t{ "usr/", "", "usr/" },
                t{ "/usr/", "", "/usr/" },
                t{ "\\usr/", "", "\\usr/" },

                t{ "folder/usr", "file", "folder/file" },
                t{ "/folder/usr", "file", "/folder/file" },
                t{ "\\folder/usr", "file", "\\folder/file" },
                t{ "folder/usr", "", "folder/" },
                t{ "/folder/usr", "", "/folder/" },
                t{ "\\folder/usr", "", "\\folder/" },

                t{ "folder/usr/", "file", "folder/usr/file" },
                t{ "/folder/usr/", "file", "/folder/usr/file" },
                t{ "\\folder/usr/", "file", "\\folder/usr/file" },
                t{ "folder/usr/", "", "folder/usr/" },
                t{ "/folder/usr/", "", "/folder/usr/" },
                t{ "\\folder/usr/", "", "\\folder/usr/" },
            };
            for ( const auto& combination : combinations ) {
                str path, name, result;
                std::tie(path, name, result) = combination;
                REQUIRE(path::replace_filename(path, name) == result);
            }
        }
        {
            // [path, extension, result]
            using t = std::tuple<str, str, str>;
            const vector<t> combinations = {
                t{"", "", ""},
                t{"/", "", "/"},
                t{"\\", "", "\\"},
                t{"C:", "", "C:"},
                t{".", "", "."},
                t{"..", "", ".."},

                t{".jpg", "png", ".png"},
                t{".jpg", ".png", ".png"},

                t{".jpg/", "png", ".jpg/.png"},
                t{".jpg/", ".png", ".jpg/.png"},

                t{"file", "png", "file.png"},
                t{"file.", "png", "file.png"},
                t{"file.jpg", "png", "file.png"},

                t{"file/", "png", "file/.png"},
                t{"file./", "png", "file./.png"},
                t{"file.jpg/", "png", "file.jpg/.png"},

                t{"/file", "png", "/file.png"},
                t{"/file.", "png", "/file.png"},
                t{"/file.jpg", "png", "/file.png"},

                t{"/file/", "png", "/file/.png"},
                t{"/file./", "png", "/file./.png"},
                t{"/file.jpg/", "png", "/file.jpg/.png"},

                t{"folder.jpg/file", "png", "folder.jpg/file.png"},
                t{"folder.jpg/file.", "png", "folder.jpg/file.png"},
                t{"folder.jpg/file.jpg", "png", "folder.jpg/file.png"},
            };
            for ( const auto& combination : combinations ) {
                str path, extension, result;
                std::tie(path, extension, result) = combination;
                REQUIRE(path::replace_extension(path, extension) == result);
            }
        }
        {
            // [path, result]
            using t = std::tuple<str, str>;
            const vector<t> combinations = {
                t{"usr/local/", "usr/local"},
                t{"usr///local///", "usr///local"},

                t{"usr/local", "usr"},
                t{"usr///local", "usr"},

                t{"usr/", "usr"},
                t{"usr///", "usr"},

                t{"usr", ""},
                t{"", ""},

                t{"/usr/local/", "/usr/local"},
                t{"/usr/local///", "/usr/local"},

                t{"/usr/local", "/usr"},
                t{"/usr///local", "/usr"},

                t{"/usr/", "/usr"},
                t{"/usr///", "/usr"},

                t{"/usr", ""},
                t{"///usr", ""},

                t{"/", ""},
                t{"//", ""},
                t{"///", ""},

                //
                // incorrect but expected
                //

                t{"C:/windows", "C:"},
                t{"C:/", "C:"},
                t{"C:", ""},

                t{"\\\\.\\COM", "\\\\."},
                t{"\\\\.", ""},

                t{"\\\\?\\UNC", "\\\\?"},
                t{"\\\\?", ""},

                t{"\\\\?\\C:\\", "\\\\?\\C:"},
                t{"\\\\?\\C:", "\\\\?"},
            };
            for ( const auto& combination : combinations ) {
                str path, result;
                std::tie(path, result) = combination;
                REQUIRE(path::parent_path(path) == result);
            }
        }
    }
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
