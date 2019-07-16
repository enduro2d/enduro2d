/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("images") {
    {
        image i;
        REQUIRE(i.size() == v2u::zero());
        REQUIRE(i.format() == image_data_format::rgba8);
        REQUIRE(i.data().empty());
        REQUIRE(i.empty());
    }
    {
        const u8 img[] = {1,2,3,4,5,6,7,8};
        image i0(v2u(2,2), image_data_format::g8, {img,4});
        image i1(v2u(2,1), image_data_format::ga8, {img,4});
        image i2(v2u(1,2), image_data_format::ga8, {img,4});
        image i3(v2u(2,1), image_data_format::rgb8, {img,6});
        image i4(v2u(1,2), image_data_format::rgb8, {img,6});
        image i5(v2u(2,1), image_data_format::rgba8, {img,8});
        image i6(v2u(1,2), image_data_format::rgba8, {img,8});
        REQUIRE(i0.format() == image_data_format::g8);
        REQUIRE(i1.format() == image_data_format::ga8);
        REQUIRE(i2.format() == image_data_format::ga8);
        REQUIRE(i3.format() == image_data_format::rgb8);
        REQUIRE(i4.format() == image_data_format::rgb8);
        REQUIRE(i5.format() == image_data_format::rgba8);
        REQUIRE(i6.format() == image_data_format::rgba8);
    }
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
    {
        struct img_info {
            const char* name;
            bool can_load;
            image_data_format format;
        };

        const img_info test_images[] = {
            {"bin/images/ship_pvrtc_2bpp_rgba.pvr", true, image_data_format::rgba_pvrtc2},
            {"bin/images/ship_pvrtc_2bpp_rgb.pvr", true, image_data_format::rgb_pvrtc2},
            {"bin/images/ship_pvrtc_4bpp_rgba.pvr", true, image_data_format::rgba_pvrtc4},
            {"bin/images/ship_pvrtc_4bpp_rgb.pvr", true, image_data_format::rgb_pvrtc4},
            {"bin/images/ship_pvrtc_ii_2bpp.pvr", true, image_data_format::rgba_pvrtc2_v2},
            {"bin/images/ship_pvrtc_ii_4bpp.pvr", true, image_data_format::rgba_pvrtc4_v2},
            {"bin/images/ship_etc1.pvr", false, image_data_format(-1)},
            {"bin/images/ship_etc2.pvr", false, image_data_format(-1)},
            {"bin/images/ship_eac_rg11.pvr", false, image_data_format(-1)},
            {"bin/images/ship_rgba8.pvr", true, image_data_format::rgba8},
            {"bin/images/ship_rgba.dds", true, image_data_format::rgba_dxt5},
            {"bin/images/ship_r8.pvr", true, image_data_format::g8},
            {"bin/images/ship_rg8.pvr", true, image_data_format::ga8},
            {"bin/images/ship_rgb8.pvr", true, image_data_format::rgb8}
        };

        str resources;
        REQUIRE(filesystem::extract_predef_path(
            resources,
            filesystem::predef_path::resources));

        for ( const auto& info : test_images ) {
            input_stream_uptr stream = make_read_file(path::combine(resources, info.name));
            REQUIRE(stream);
            image img;
            REQUIRE(images::try_load_image(img, stream) == info.can_load);
            if ( info.can_load ) {
                REQUIRE(img.format() == info.format);
                REQUIRE(img.size().x == 64);
                REQUIRE(img.size().y == 128);
                REQUIRE(img.data().size() > 0);
            }
        }
    }
}
