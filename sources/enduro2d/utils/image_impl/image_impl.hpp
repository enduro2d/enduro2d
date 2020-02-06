/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/utils/image.hpp>
#include <enduro2d/utils/buffer.hpp>
#include <enduro2d/utils/buffer_view.hpp>

namespace e2d::images::impl
{
    bool load_image_dds(image& dst, buffer_view src);
    bool load_image_pvr(image& dst, buffer_view src);
    bool load_image_stb(image& dst, buffer_view src);

    bool save_image_dds(const image& src, buffer& dst);
    bool save_image_jpg(const image& src, buffer& dst);
    bool save_image_png(const image& src, buffer& dst);
    bool save_image_pvr(const image& src, buffer& dst);
    bool save_image_tga(const image& src, buffer& dst);

    bool check_save_image_dds(const image& src) noexcept;
    bool check_save_image_jpg(const image& src) noexcept;
    bool check_save_image_png(const image& src) noexcept;
    bool check_save_image_pvr(const image& src) noexcept;
    bool check_save_image_tga(const image& src) noexcept;
}

namespace e2d::images::impl
{
    inline std::size_t bgr8_to_rgb8(u8* dst, buffer_view src) noexcept {
        E2D_ASSERT(src.size() % 3 == 0);
        const std::size_t dst_size = src.size() / 3 * 3;
        if ( !dst ) {
            return dst_size;
        }
        const u8* data = static_cast<const u8*>(src.data());
        for ( std::size_t i = 0; i < dst_size / 3; ++i ) {
            const u8 b = data[0];
            const u8 g = data[1];
            const u8 r = data[2];
            dst[0] = r;
            dst[1] = g;
            dst[2] = b;
            dst += 3;
            data += 3;
        }
        return dst_size;
    }

    inline std::size_t bgrx8_to_rgb8(u8* dst, buffer_view src) noexcept {
        E2D_ASSERT(src.size() % 4 == 0);
        const std::size_t dst_size = src.size() / 4 * 3;
        if ( !dst ) {
            return dst_size;
        }
        const u8* data = static_cast<const u8*>(src.data());
        for ( std::size_t i = 0; i < dst_size / 3; ++i ) {
            const u8 b = data[0];
            const u8 g = data[1];
            const u8 r = data[2];
            dst[0] = r;
            dst[1] = g;
            dst[2] = b;
            dst += 3;
            data += 4;
        }
        return dst_size;
    }

    inline std::size_t bgra8_to_rgba8(u8* dst, buffer_view src) noexcept {
        E2D_ASSERT(src.size() % 4 == 0);
        const std::size_t dst_size = src.size() / 4 * 4;
        if ( !dst ) {
            return dst_size;
        }
        const u8* data = static_cast<const u8*>(src.data());
        for ( std::size_t i = 0; i < dst_size / 4; ++i ) {
            const u8 b = data[0];
            const u8 g = data[1];
            const u8 r = data[2];
            const u8 a = data[3];
            dst[0] = r;
            dst[1] = g;
            dst[2] = b;
            dst[3] = a;
            dst += 4;
            data += 4;
        }
        return dst_size;
    }

    inline buffer bgr8_to_rgb8(buffer_view src) {
        buffer result(bgr8_to_rgb8(nullptr, src));
        bgr8_to_rgb8(result.data(), src);
        return result;
    }

    inline buffer bgrx8_to_rgb8(buffer_view src) {
        buffer result(bgrx8_to_rgb8(nullptr, src));
        bgrx8_to_rgb8(result.data(), src);
        return result;
    }

    inline buffer bgra8_to_rgba8(buffer_view src) {
        buffer result(bgra8_to_rgba8(nullptr, src));
        bgra8_to_rgba8(result.data(), src);
        return result;
    }
}
