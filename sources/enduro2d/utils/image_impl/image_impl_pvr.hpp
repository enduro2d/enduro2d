/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/utils/_utils.hpp>

namespace e2d::images::impl::pvr
{
    // PVR format specification:
    // https://cdn.imgtec.com/sdk-documentation/PVR+File+Format.Specification.pdf

    enum class pvr_flags : u32 {
        none = 0,
        premultiplied = 0x02,
    };

    enum class pvr_pixel_format : u64 {
        pvrtc_2bpp_rgb = 0,
        pvrtc_2bpp_rgba = 1,
        pvrtc_4bpp_rgb = 2,
        pvrtc_4bpp_rgba = 3,
        pvrtc2_2bpp = 4,
        pvrtc2_4bpp = 5,
        etc1 = 6,
        dxt1 = 7,
        dxt2 = 8,
        dxt3 = 9,
        dxt4 = 10,
        dxt5 = 11,
        bc1 = 7,
        bc2 = 9,
        bc3 = 11,
        bc4 = 12,
        bc5 = 13,
        bc6 = 14,
        bc7 = 15,
        uyvy = 16,
        yuy2 = 17,
        bw_1bpp = 18,
        r9g9b9e5 = 19,
        rgbg8888 = 20,
        grgb8888 = 21,
        etc2_rgb = 22,
        etc2_rgba = 23,
        etc2_rgb_a1 = 24,
        eac_r11 = 25,
        eac_rg11 = 26,
        astc_4x4 = 27,
        astc_5x4 = 28,
        astc_5x5 = 29,
        astc_6x5 = 30,
        astc_6x6 = 31,
        astc_8x5 = 32,
        astc_8x6 = 33,
        astc_8x8 = 34,
        astc_10x5 = 35,
        astc_10x6 = 36,
        astc_10x8 = 37,
        astc_10x10 = 38,
        astc_12x10 = 39,
        astc_12x12 = 40,
        astc_3x3x3 = 41,
        astc_4x3x3 = 42,
        astc_4x4x3 = 43,
        astc_4x4x4 = 44,
        astc_5x4x4 = 45,
        astc_5x5x4 = 46,
        astc_5x5x5 = 47,
        astc_6x5x5 = 48,
        astc_6x6x5 = 49,
        astc_6x6x6 = 50,
        a8 = 0x00000008'00000061ull,
        l8 = 0x00000008'0000006Cull,
        la8 = 0x00000808'0000616CuLL,
        rgb8 = 0x00080808'00626772ull,
        rgba8 = 0x08080808'61626772ull,
        bgr8 = 0x08080808'00726762ull,
        bgra8 = 0x08080808'61726762ull,
    };

    enum class pvr_color_space : u32 {
        linear = 0,
        srgb = 1,
    };

    enum class pvr_channel_type : u32 {
        byte_unorm = 0,
        byte_snorm = 1,
        ubyte = 2,
        sbyte = 3,
        short_unorm = 4,
        short_snorm = 5,
        ushort = 6,
        sshort = 7,
        int_unorm = 8,
        int_snorm = 9,
        uint = 10,
        sint = 11,
        sfloat = 12,
    };

    struct pvr_header {
        u32 version;
        u32 flags;
        u32 pixel_format0;
        u32 pixel_format1;
        u32 color_space;
        u32 channel_type;
        u32 height;
        u32 width;
        u32 depth;
        u32 num_surfaces;
        u32 num_faces;
        u32 mipmap_count;
        u32 meta_data_size;
    };

    struct pvr_meta_data {
        u32 fourcc;
        u32 key;
        u32 data_size;
    };

    static_assert(sizeof(pvr_header) == 52, "invalid pvr_header size");

    inline constexpr u32 make_fourcc(u8 a, u8 b, u8 c, u8 d) noexcept {
        return
            (static_cast<u32>(a)) |
            (static_cast<u32>(b) << 8) |
            (static_cast<u32>(c) << 16) |
            (static_cast<u32>(d) << 24);
    }
}
