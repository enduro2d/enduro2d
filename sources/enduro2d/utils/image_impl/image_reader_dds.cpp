/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"

namespace
{
    // DDS format specification:
    // https://docs.microsoft.com/en-us/windows/desktop/direct3ddds/dx-graphics-dds-pguide

    using namespace e2d;

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
        u32 dwCaps1;
        u32 dwCaps2;
        u32 dwReserved2[3];
    };

    static_assert(sizeof(dds_pixel_format) == 32, "invalid dds_pixel_format structure size");
    static_assert(sizeof(dds_header) == 128, "invalid dds_header structure size");

    bool is_dds(const void* data, std::size_t byte_size) noexcept {
        if ( byte_size > sizeof(dds_header) ) {
            const auto* hdr = reinterpret_cast<const dds_header*>(data);
            return hdr->dwMagic == 0x20534444; // 'DDS '
        }
        return false;
    }

    bool get_dds_format(
        const dds_header& hdr,
        image_data_format& out_format,
        u32& out_bytes_per_block,
        v2u& out_block_size) noexcept
    {
        if ( math::check_all_flags(hdr.ddspf.dwFlags, ddsf_fourcc) ) {
            switch ( hdr.ddspf.dwFourCC ) {
            case fourcc_dxt1:
                out_format = math::check_all_flags(hdr.ddspf.dwFlags, ddsf_alphapixels)
                    ? image_data_format::rgba_dxt1
                    : image_data_format::rgb_dxt1;
                out_bytes_per_block = 8;
                out_block_size = v2u(4,4);
                break;
            case fourcc_dxt3:
                out_format = image_data_format::rgba_dxt3;
                out_bytes_per_block = 16;
                out_block_size = v2u(4,4);
                break;
            case fourcc_dxt5:
                out_format = image_data_format::rgba_dxt5;
                out_bytes_per_block = 16;
                out_block_size = v2u(4,4);
                break;
            default:
                return false;
            }
        } else if ( math::check_all_flags(hdr.ddspf.dwFlags, ddsf_rgba) && (hdr.ddspf.dwRGBBitCount == 32) ) {
            out_format = image_data_format::rgba8;
            out_bytes_per_block = 4;
            out_block_size = v2u(1,1);
            if ( hdr.ddspf.dwRBitMask == 0x00FF0000 ) {
                return false; // BGRA format is not supported
            }
        } else if ( math::check_all_flags(hdr.ddspf.dwFlags, ddsf_rgb) && (hdr.ddspf.dwRGBBitCount == 24) ) {
            out_format = image_data_format::rgb8;
            out_bytes_per_block = 3;
            out_block_size = v2u(1,1);
            if (hdr.ddspf.dwRBitMask == 0x00FF0000) {
                return false; // BGRA format is not supported
            }
        } else if ( hdr.ddspf.dwRGBBitCount == 16 ) {
            return false; // 16-bit format is not supported by engine
        } else if ( hdr.ddspf.dwRGBBitCount == 8 ) {
            out_format = image_data_format::g8;
            out_bytes_per_block = 1;
            out_block_size = v2u(1,1);
        } else {
            return false;
        }
        return true;
    }
}

namespace e2d::images::impl
{
    bool load_image_dds(image& dst, const buffer& src) {
        if ( !is_dds(src.data(), src.size()) ) {
            return false;
        }

        const dds_header& hdr = *reinterpret_cast<const dds_header*>(src.data());
        const u8* content = src.data() + sizeof(dds_header);
        const std::size_t content_size = static_cast<std::size_t>(src.data() + src.size() - content);

        if ( math::check_all_flags(hdr.dwCaps2, ddsf_cubemap) ||
             math::check_all_flags(hdr.dwCaps2, ddsf_volume) ||
             (hdr.dwDepth > 0) )
        {
            return false;
        }

        image_data_format format = image_data_format(-1);
        u32 bytes_per_block = 0;
        v2u block_size = v2u(1,1);
        if ( !get_dds_format(hdr, format, bytes_per_block, block_size) ) {
            return false;
        }

        const v2u dimension = v2u(hdr.dwWidth, hdr.dwHeight);
        const std::size_t size = bytes_per_block *
            ((dimension.x + block_size.x - 1) / block_size.x) *
            ((dimension.y + block_size.y - 1) / block_size.y);

        if ( content_size != size ) {
            return false;
        }

        dst = image(dimension, format, buffer(content, size));
        return true;
    }
}
