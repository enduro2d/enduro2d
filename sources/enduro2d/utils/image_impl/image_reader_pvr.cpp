/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"
#include "image_impl_structures.hpp"

namespace
{
    // PVR format specification:
    // https://cdn.imgtec.com/sdk-documentation/PVR+File+Format.Specification.pdf

    using namespace e2d;
    using namespace e2d::images::impl::pvr;

    bool is_pvr(const void* data, std::size_t byte_size) noexcept {
        if ( byte_size > sizeof(pvr_header) ) {
            const pvr_header* hdr = reinterpret_cast<const pvr_header*>(data);
            return hdr->version == 0x03525650;
        }
        return false;
    }

    bool get_pvr_format(
        const pvr_header& hdr,
        image_data_format& out_format,
        u32& out_bytes_per_block,
        v2u& out_block_size) noexcept
    {
        if ( pvr_color_space(hdr.colorSpace) != pvr_color_space::linear ) {
            return false;
        }
        const pvr_pixel_format fmt = pvr_pixel_format(
            hdr.pixelFormat0 | (u64(hdr.pixelFormat1) << 32));
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
    bool load_image_pvr(image& dst, const buffer& src) {
        if ( !is_pvr(src.data(), src.size()) ) {
            return false;
        }

        const pvr_header& hdr = *reinterpret_cast<const pvr_header*>(src.data());
        const u8* content = src.data() + sizeof(pvr_header) + hdr.metaDataSize;
        const std::size_t content_size = static_cast<std::size_t>(src.data() + src.size() - content);

        if ( hdr.numSurfaces != 1 || hdr.numFaces != 1 || hdr.depth > 1 ) {
            return false; // cubemap and volume textures are not supported
        }

        image_data_format format = image_data_format(-1);
        u32 bytes_per_block = 0;
        v2u block_size = v2u(1,1);
        if ( !get_pvr_format(hdr, format, bytes_per_block, block_size) ) {
            return false;
        }

        const v2u dimension = v2u(hdr.width, hdr.height);
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
