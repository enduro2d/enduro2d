/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/utils/image.hpp>
#include <enduro2d/utils/buffer.hpp>

namespace e2d::images::impl::dds
{
    // pixel format flags
    const u32 ddsf_alphapixels = 0x00000001;
    const u32 ddsf_fourcc = 0x00000004;
    const u32 ddsf_rgb = 0x00000040;
    const u32 ddsf_rgba = 0x00000041;

    // dwCaps2 flags
    const u32 ddsf_cubemap = 0x00000200;
    const u32 ddsf_volume = 0x00200000;

    // compressed texture types
    const u32 fourcc_dxt1 = 0x31545844; // 'DXT1'
    const u32 fourcc_dxt3 = 0x33545844; // 'DXT3'
    const u32 fourcc_dxt5 = 0x35545844; // 'DXT5'

    struct dds_pixel_format {
        u32 dwSize;
        u32 dwFlags;
        u32 dwFourCC;
        u32 dwRGBBitCount;
        u32 dwRBitMask;
        u32 dwGBitMask;
        u32 dwBBitMask;
        u32 dwABitMask;
    };

    struct dds_header {
        u32 dwMagic;
        u32 dwSize;
        u32 dwFlags;
        u32 dwHeight;
        u32 dwWidth;
        u32 dwPitchOrLinearSize;
        u32 dwDepth;
        u32 dwMipMapCount;
        u32 dwReserved1[11];
        dds_pixel_format ddspf;
        u32 dwCaps;
        u32 dwCaps2;
        u32 dwCaps3;
        u32 dwCaps4;
        u32 dwReserved2;
    };

    // pixel format flags
    const u32 ddsd_caps = 0x00000001;
    const u32 ddsd_height = 0x00000002;
    const u32 ddsd_width = 0x00000004;
    const u32 ddsd_pitch = 0x00000008;
    const u32 ddsd_pixelformat = 0x00001000;
    const u32 ddsd_mipmapcount = 0x00020000;
    const u32 ddsd_linearsize = 0x00080000;
    const u32 ddsd_dept = 0x00800000;

    static_assert(sizeof(dds_pixel_format) == 32, "invalid dds_pixel_format structure size");
    static_assert(sizeof(dds_header) == 128, "invalid dds_header structure size");
}

namespace e2d::images::impl::pvr
{
    enum class pvr_flags : u32 {
        none = 0,
        premultiplied = 0x02,
    };

    enum class pvr_pixel_format : u64 {
        pvrtc_2bpp_rgb = 0,
        pvrtc_2bpp_rgba = 1,
        pvrtc_4bpp_rgb = 2,
        pvrtc_4bpp_rgba = 3,
        pvrtc_ii_2bpp = 4,
        pvrtc_ii_4bpp = 5,
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
        rgba8 = 0x08080808'61626772ull,
        r8 = 0x00000008'00000072ull,
        rg8 = 0x00000808'00006772ull,
        rgb8 = 0x00080808'00626772ull,
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
        u32 pixelFormat0;
        u32 pixelFormat1;
        u32 colorSpace;
        u32 channelType;
        u32 height;
        u32 width;
        u32 depth;
        u32 numSurfaces;
        u32 numFaces;
        u32 mipMapCount;
        u32 metaDataSize;
    };

    struct pvr_meta_data {
        u32 four_cc{0};
        u32 key{0};
        u32 data_size{0};
    };

    static_assert(sizeof(pvr_header) == 52, "invalid PVR header size");
}
