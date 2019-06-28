/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"

namespace
{
    // PVR format specification:
    // https://cdn.imgtec.com/sdk-documentation/PVR+File+Format.Specification.pdf

    using namespace e2d;

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

    static_assert(sizeof(pvr_header) == 52, "invalid PVR header size");

    bool is_pvr(const void* data, std::size_t byte_size) {
        if ( byte_size > sizeof(pvr_header) ) {
            const pvr_header* hdr = reinterpret_cast<const pvr_header*>(data);
            return hdr->version == 0x03525650;
        }
        return false;
    }

    bool get_pvr_format(const pvr_header& hdr, image_data_format& out_format, u32& out_bytes_per_block, v2u& out_block_size) {
        if ( pvr_color_space(hdr.colorSpace) != pvr_color_space::linear ) {
            return false;
        }
        const pvr_pixel_format fmt = pvr_pixel_format(hdr.pixelFormat0 | (u64(hdr.pixelFormat1) << 32));
        switch (fmt)
        {
        case pvr_pixel_format::pvrtc_2bpp_rgb:
            out_format = image_data_format::rgb_pvrtc2;
            out_bytes_per_block = 8;
            out_block_size = v2u(8,4);
            break;
        case pvr_pixel_format::pvrtc_2bpp_rgba:
            out_format = image_data_format::rgba_pvrtc2;
            out_bytes_per_block = 8;
            out_block_size = v2u(8,4);
            break;
        case pvr_pixel_format::pvrtc_4bpp_rgb:
            out_format = image_data_format::rgb_pvrtc4;
            out_bytes_per_block = 8;
            out_block_size = v2u(4,4);
            break;
        case pvr_pixel_format::pvrtc_4bpp_rgba:
            out_format = image_data_format::rgba_pvrtc4;
            out_bytes_per_block = 8;
            out_block_size = v2u(4,4);
            break;
        case pvr_pixel_format::pvrtc_ii_2bpp:
            out_format = image_data_format::rgba_pvrtc2_v2;
            out_bytes_per_block = 8;
            out_block_size = v2u(8,4);
            break;
        case pvr_pixel_format::pvrtc_ii_4bpp:
            out_format = image_data_format::rgba_pvrtc4_v2;
            out_bytes_per_block = 8;
            out_block_size = v2u(4,4);
            break;
        case pvr_pixel_format::dxt1:
            out_format = image_data_format::rgb_dxt1;
            out_bytes_per_block = 8;
            out_block_size = v2u(4,4);
            break;
        case pvr_pixel_format::dxt3:
            out_format = image_data_format::rgba_dxt3;
            out_bytes_per_block = 16;
            out_block_size = v2u(4,4);
            break;
        case pvr_pixel_format::dxt5:
            out_format = image_data_format::rgba_dxt5;
            out_bytes_per_block = 16;
            out_block_size = v2u(4,4);
            break;
        case pvr_pixel_format::rgba8:
            out_format = image_data_format::rgba8;
            out_bytes_per_block = 4;
            out_block_size = v2u(1,1);
            break;
        case pvr_pixel_format::r8:
            out_format = image_data_format::g8;
            out_bytes_per_block = 1;
            out_block_size = v2u(1,1);
            break;
        case pvr_pixel_format::rg8:
            out_format = image_data_format::ga8;
            out_bytes_per_block = 2;
            out_block_size = v2u(1,1);
            break;
        case pvr_pixel_format::rgb8:
            out_format = image_data_format::rgb8;
            out_bytes_per_block = 3;
            out_block_size = v2u(1,1);
            break;
        default:
            return false;
        }
        return true;
    }
}

namespace e2d::images::impl
{
    bool try_load_image_pvr(image& dst, const buffer& src) noexcept {
        if ( !is_pvr(src.data(), src.size()) ) {
            return false;
        }
        const pvr_header& hdr = *reinterpret_cast<const pvr_header*>(src.data());
        const u8* content = src.data() + sizeof(pvr_header) + hdr.metaDataSize;
        if ( hdr.numSurfaces != 1 || hdr.numFaces != 1 || hdr.depth > 1 ) {
            return false; // cubemap and volume textures are not supported
        }
        image_data_format format = image_data_format(-1);
        u32 bytes_per_block = 0;
        v2u block_size = v2u(1,1);
        if ( !get_pvr_format(hdr, format, bytes_per_block, block_size) ) {
            return false;
        }
        v2u dimension = v2u(hdr.width, hdr.height);
        std::size_t size = bytes_per_block *
            (dimension.x + block_size.x - 1) / block_size.x *
            (dimension.y + block_size.y - 1) / block_size.y;
        dst = image(dimension, format, buffer(content, size));
        return true;
    }
}
