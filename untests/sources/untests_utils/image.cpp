/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("images") {
    SECTION("image") {
        {
            image i;
            REQUIRE(i.size() == v2u::zero());
            REQUIRE(i.format() == image_data_format::rgba8);
            REQUIRE(i.data().empty());
            REQUIRE(i.empty());
        }
        {
            const u8 img[] = {1,2,3,4,5,6,7,8};
            image i0(v2u(2,2), image_data_format::a8, {img,4});
            image i1(v2u(2,2), image_data_format::l8, {img,4});
            image i2(v2u(2,1), image_data_format::la8, {img,4});
            image i3(v2u(1,2), image_data_format::la8, {img,4});
            image i4(v2u(2,1), image_data_format::rgb8, {img,6});
            image i5(v2u(1,2), image_data_format::rgb8, {img,6});
            image i6(v2u(2,1), image_data_format::rgba8, {img,8});
            image i7(v2u(1,2), image_data_format::rgba8, {img,8});
            REQUIRE(i0.format() == image_data_format::a8);
            REQUIRE(i1.format() == image_data_format::l8);
            REQUIRE(i2.format() == image_data_format::la8);
            REQUIRE(i3.format() == image_data_format::la8);
            REQUIRE(i4.format() == image_data_format::rgb8);
            REQUIRE(i5.format() == image_data_format::rgb8);
            REQUIRE(i6.format() == image_data_format::rgba8);
            REQUIRE(i7.format() == image_data_format::rgba8);
        }
    }

    SECTION("stb") {
        {
            REQUIRE(filesystem::remove_file("image_save_test.jpg"));
            REQUIRE(filesystem::remove_file("image_save_test.png"));
            REQUIRE(filesystem::remove_file("image_save_test.tga"));
            const u8 img_data[] = {255,0,0, 0,255,0, 0,0,255};
            image img(v2u(3,1), image_data_format::rgb8, buffer(img_data, sizeof(img_data)));
            REQUIRE(images::try_save_image(
                img,
                image_file_format::jpg,
                make_write_file("image_save_test.jpg", false)));
            REQUIRE(filesystem::file_exists("image_save_test.jpg"));
            REQUIRE(images::try_save_image(
                img,
                image_file_format::png,
                make_write_file("image_save_test.png", false)));
            REQUIRE(filesystem::file_exists("image_save_test.png"));
            REQUIRE(images::try_save_image(
                img,
                image_file_format::tga,
                make_write_file("image_save_test.tga", false)));
            REQUIRE(filesystem::file_exists("image_save_test.tga"));
        }
        {
            image img;

            REQUIRE(images::try_load_image(img, make_read_file("image_save_test.jpg")));
            REQUIRE(img.size() == v2u(3,1));
            REQUIRE(img.format() == image_data_format::rgb8);
            REQUIRE(math::approximately(img.pixel32(0,0), color32::red(),   10u));
            REQUIRE(math::approximately(img.pixel32(1,0), color32::green(), 10u));
            REQUIRE(math::approximately(img.pixel32(2,0), color32::blue(),  10u));

            REQUIRE(images::try_load_image(img, make_read_file("image_save_test.png")));
            REQUIRE(img.size() == v2u(3,1));
            REQUIRE(img.format() == image_data_format::rgb8);
            REQUIRE(math::approximately(img.pixel32(0,0), color32::red(),   0u));
            REQUIRE(math::approximately(img.pixel32(1,0), color32::green(), 0u));
            REQUIRE(math::approximately(img.pixel32(2,0), color32::blue(),  0u));

            REQUIRE(images::try_load_image(img, make_read_file("image_save_test.tga")));
            REQUIRE(img.size() == v2u(3,1));
            REQUIRE(img.format() == image_data_format::rgb8);
            REQUIRE(math::approximately(img.pixel32(0,0), color32::red(),   0u));
            REQUIRE(math::approximately(img.pixel32(1,0), color32::green(), 0u));
            REQUIRE(math::approximately(img.pixel32(2,0), color32::blue(),  0u));
        }
        {
            REQUIRE(filesystem::remove_file("image_save_test.jpg"));
            REQUIRE(filesystem::remove_file("image_save_test.png"));
            REQUIRE(filesystem::remove_file("image_save_test.tga"));
            const u8 img_data[] = {255,0,0, 0,255,0, 0,0,255};
            image img(v2u(3,1), image_data_format::rgb8, buffer(img_data, sizeof(img_data)));
            buffer buf;
            REQUIRE(images::try_save_image(
                img,
                image_file_format::jpg,
                buf));
            REQUIRE(filesystem::try_write_all(buf, "image_save_test.jpg", false));
            REQUIRE(images::try_save_image(
                img,
                image_file_format::png,
                buf));
            REQUIRE(filesystem::try_write_all(buf, "image_save_test.png", false));
            REQUIRE(images::try_save_image(
                img,
                image_file_format::tga,
                buf));
            REQUIRE(filesystem::try_write_all(buf, "image_save_test.tga", false));
        }
        {
            image img;
            buffer buf;

            REQUIRE(filesystem::try_read_all(buf, "image_save_test.jpg"));
            REQUIRE(images::try_load_image(img, buf));
            REQUIRE(img.size() == v2u(3,1));
            REQUIRE(img.format() == image_data_format::rgb8);
            REQUIRE(math::approximately(img.pixel32(0,0), color32::red(),   10));
            REQUIRE(math::approximately(img.pixel32(1,0), color32::green(), 10));
            REQUIRE(math::approximately(img.pixel32(2,0), color32::blue(),  10));

            REQUIRE(filesystem::try_read_all(buf, "image_save_test.png"));
            REQUIRE(images::try_load_image(img, buf));
            REQUIRE(img.size() == v2u(3,1));
            REQUIRE(img.format() == image_data_format::rgb8);
            REQUIRE(math::approximately(img.pixel32(0,0), color32::red(),   0));
            REQUIRE(math::approximately(img.pixel32(1,0), color32::green(), 0));
            REQUIRE(math::approximately(img.pixel32(2,0), color32::blue(),  0));

            REQUIRE(filesystem::try_read_all(buf, "image_save_test.tga"));
            REQUIRE(images::try_load_image(img, buf));
            REQUIRE(img.size() == v2u(3,1));
            REQUIRE(img.format() == image_data_format::rgb8);
            REQUIRE(math::approximately(img.pixel32(0,0), color32::red(),   0));
            REQUIRE(math::approximately(img.pixel32(1,0), color32::green(), 0));
            REQUIRE(math::approximately(img.pixel32(2,0), color32::blue(),  0));
        }
    }

    str resources;
    REQUIRE(filesystem::extract_predef_path(
        resources,
        filesystem::predef_path::resources));

    SECTION("stb") {
        struct img_info {
            const char* path;
            std::size_t size;
            image_data_format format;
        };
        const img_info test_infos[] = {
            {"bin/images/stb/ship.jpg", 64 * 128 * 3, image_data_format::rgb8},
            {"bin/images/stb/ship.png", 64 * 128 * 4, image_data_format::rgba8},
            {"bin/images/stb/ship.tga", 64 * 128 * 4, image_data_format::rgba8}};
        for ( const auto& info : test_infos ) {
            input_stream_uptr stream = make_read_file(path::combine(resources, info.path));
            REQUIRE(stream);
            image img;
            REQUIRE(images::try_load_image(img, stream));
            REQUIRE(img.format() == info.format);
            REQUIRE(img.data().size() == info.size);
            REQUIRE(img.size().x == 64);
            REQUIRE(img.size().y == 128);
        }
    }

    SECTION("dds") {
        struct img_info {
            const char* path;
            std::size_t size;
            bool can_load;
            image_data_format format;
        };

        const img_info test_infos[] = {
            {"bin/images/dds/ship_a8.dds", 64 * 128 * 1, true, image_data_format::a8},
            {"bin/images/dds/ship_l8.dds", 64 * 128 * 1, true, image_data_format::l8},
            {"bin/images/dds/ship_a8l8.dds", 64 * 128 * 2, true, image_data_format::la8},

            {"bin/images/dds/ship_r8g8b8.dds", 64 * 128 * 3, true, image_data_format::rgb8},
            {"bin/images/dds/ship_x8r8g8b8.dds", 64 * 128 * 3, true, image_data_format::rgb8},

            {"bin/images/dds/ship_rgba8.dds", 64 * 128 * 4, true, image_data_format::rgba8},
            {"bin/images/dds/ship_a8r8g8b8.dds", 64 * 128 * 4, true, image_data_format::rgba8},

            {"bin/images/dds/ship_dxt1.dds", 64 * 128 * 4 / 8, true, image_data_format::rgba_dxt1},
            {"bin/images/dds/ship_dxt3.dds", 64 * 128 * 8 / 8, true, image_data_format::rgba_dxt3},
            {"bin/images/dds/ship_dxt5.dds", 64 * 128 * 8 / 8, true, image_data_format::rgba_dxt5},
        };

        for ( const auto& info : test_infos ) {
            CAPTURE(info.path);

            image img;
            REQUIRE(info.can_load == images::try_load_image(
                img,
                make_read_file(path::combine(resources, info.path))));

            if ( info.can_load ) {
                REQUIRE(img.format() == info.format);
                REQUIRE(img.data().size() == info.size);
                REQUIRE(img.size().x == 64);
                REQUIRE(img.size().y == 128);
            }
        }

        for ( const auto& info : test_infos ) {
            CAPTURE(info.path);

            if ( !info.can_load ) {
                continue;
            }

            image img;
            REQUIRE(images::try_load_image(
                img,
                make_read_file(path::combine(resources, info.path))));

            REQUIRE(filesystem::remove_file("image_save_test.dds"));
            REQUIRE(images::try_save_image(
                img,
                image_file_format::dds, make_write_file("image_save_test.dds", false)));

            image img2;
            REQUIRE(images::try_load_image(
                img2,
                make_read_file("image_save_test.dds")));

            REQUIRE(img == img2);
        }
    }

    SECTION("dds_bgr") {
        {
            image img;
            REQUIRE(images::try_load_image(
                img,
                make_read_file(path::combine(resources, "bin/images/dds/ship_r8g8b8.dds"))));

            image img2;
            REQUIRE(images::try_load_image(
                img2,
                make_read_file(path::combine(resources, "bin/images/dds/ship_r8g8b8.dds"))));

            image img3;
            REQUIRE(images::try_load_image(
                img3,
                make_read_file(path::combine(resources, "bin/images/dds/ship_x8r8g8b8.dds"))));

            REQUIRE(img == img2);
            REQUIRE(img2 == img3);
        }
        {
            image img;
            REQUIRE(images::try_load_image(
                img,
                make_read_file(path::combine(resources, "bin/images/dds/ship_rgba8.dds"))));

            image img2;
            REQUIRE(images::try_load_image(
                img2,
                make_read_file(path::combine(resources, "bin/images/dds/ship_a8r8g8b8.dds"))));

            REQUIRE(img == img2);
        }
    }

    SECTION("pvr") {
        struct img_info {
            const char* path;
            std::size_t size;
            bool can_load;
            image_data_format format;
        };

        const img_info test_infos[] = {
            {"bin/images/pvr/ship_a8.pvr", 64 * 128 * 1, true, image_data_format::a8},
            {"bin/images/pvr/ship_l8.pvr", 64 * 128 * 1, true, image_data_format::l8},
            {"bin/images/pvr/ship_la8.pvr", 64 * 128 * 2, true, image_data_format::la8},

            {"bin/images/pvr/ship_rgb8.pvr", 64 * 128 * 3, true, image_data_format::rgb8},
            {"bin/images/pvr/ship_rgba8.pvr", 64 * 128 * 4, true, image_data_format::rgba8},
            {"bin/images/pvr/ship_bgra8.pvr", 64 * 128 * 4, true, image_data_format::rgba8},

            {"bin/images/pvr/ship_dxt1.pvr", 64 * 128 * 4 / 8, true, image_data_format::rgba_dxt1},
            {"bin/images/pvr/ship_dxt3.pvr", 64 * 128 * 8 / 8, true, image_data_format::rgba_dxt3},
            {"bin/images/pvr/ship_dxt5.pvr", 64 * 128 * 8 / 8, true, image_data_format::rgba_dxt5},

            {"bin/images/pvr/ship_etc1.pvr", 64 * 128 * 4 / 8, true, image_data_format::rgb_etc1},
            {"bin/images/pvr/ship_etc2_rgb.pvr", 64 * 128 * 4 / 8, true, image_data_format::rgb_etc2},
            {"bin/images/pvr/ship_etc2_rgba.pvr", 64 * 128 * 8 / 8, true, image_data_format::rgba_etc2},
            {"bin/images/pvr/ship_etc2_rgb_a1.pvr", 64 * 128 * 4 / 8, true, image_data_format::rgb_a1_etc2},

            {"bin/images/pvr/ship_pvrtc_2bpp_rgb.pvr", 64 * 128 * 2 / 8, true, image_data_format::rgb_pvrtc2},
            {"bin/images/pvr/ship_pvrtc_2bpp_rgba.pvr", 64 * 128 * 2 / 8, true, image_data_format::rgba_pvrtc2},
            {"bin/images/pvr/ship_pvrtc_4bpp_rgb.pvr", 64 * 128 * 4 / 8, true, image_data_format::rgb_pvrtc4},
            {"bin/images/pvr/ship_pvrtc_4bpp_rgba.pvr", 64 * 128 * 4 / 8, true, image_data_format::rgba_pvrtc4},

            {"bin/images/pvr/ship_pvrtc2_2bpp.pvr", 64 * 128 * 2 / 8, true, image_data_format::rgba_pvrtc2_v2},
            {"bin/images/pvr/ship_pvrtc2_4bpp.pvr", 64 * 128 * 4 / 8, true, image_data_format::rgba_pvrtc4_v2},

            {"bin/images/pvr/ship_eac_r11.pvr", 0, false, image_data_format(-1)},
            {"bin/images/pvr/ship_eac_rg11.pvr", 0, false, image_data_format(-1)},
        };

        for ( const auto& info : test_infos ) {
            CAPTURE(info.path);

            image img;
            REQUIRE(info.can_load == images::try_load_image(
                img,
                make_read_file(path::combine(resources, info.path))));

            if ( info.can_load ) {
                REQUIRE(img.format() == info.format);
                REQUIRE(img.data().size() == info.size);
                REQUIRE(img.size().x == 64);
                REQUIRE(img.size().y == 128);
            }
        }

        for ( const auto& info : test_infos ) {
            CAPTURE(info.path);

            if ( !info.can_load ) {
                continue;
            }

            image img;
            REQUIRE(images::try_load_image(
                img,
                make_read_file(path::combine(resources, info.path))));

            REQUIRE(filesystem::remove_file("image_save_test.pvr"));
            REQUIRE(images::try_save_image(
                img,
                image_file_format::pvr, make_write_file("image_save_test.pvr", false)));

            image img2;
            REQUIRE(images::try_load_image(
                img2,
                make_read_file("image_save_test.pvr")));

            REQUIRE(img == img2);
        }
    }

    SECTION("pvr_bgr") {
        {
            image img;
            REQUIRE(images::try_load_image(
                img,
                make_read_file(path::combine(resources, "bin/images/pvr/ship_rgba8.pvr"))));

            image img2;
            REQUIRE(images::try_load_image(
                img2,
                make_read_file(path::combine(resources, "bin/images/pvr/ship_bgra8.pvr"))));

            REQUIRE(img == img2);
        }
    }
}
