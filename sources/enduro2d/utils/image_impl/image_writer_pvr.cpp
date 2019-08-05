/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"
#include "image_impl_structures.hpp"

namespace
{
    using namespace e2d;
    using namespace e2d::images::impl::pvr;

    bool is_save_image_pvr_supported(image_data_format data_format) noexcept {
        switch ( data_format ) {
            case image_data_format::g8:
            case image_data_format::ga8:
            case image_data_format::rgb8:
            case image_data_format::rgba8:
            case image_data_format::rgb_dxt1:
            case image_data_format::rgba_dxt3:
            case image_data_format::rgba_dxt5:
            case image_data_format::rgb_pvrtc2:
            case image_data_format::rgb_pvrtc4:
            case image_data_format::rgba_pvrtc2:
            case image_data_format::rgba_pvrtc4:
            case image_data_format::rgba_pvrtc2_v2:
            case image_data_format::rgba_pvrtc4_v2:
                return true;
            default:
                return false;
        }
    }

    u64 get_pvr_pixel_format(image_data_format data_format) noexcept {
        pvr_pixel_format res;
        switch ( data_format ) {
            case image_data_format::g8:
                res = pvr_pixel_format::r8;
                break;
            case image_data_format::ga8:
                res = pvr_pixel_format::rg8;
                break;
            case image_data_format::rgb8:
                res = pvr_pixel_format::rgb8;
                break;
            case image_data_format::rgba8:
                res = pvr_pixel_format::rgba8;
                break;
            case image_data_format::rgb_dxt1:
                res = pvr_pixel_format::dxt1;
                break;
            case image_data_format::rgba_dxt3:
                res = pvr_pixel_format::dxt3;
                break;
            case image_data_format::rgba_dxt5:
                res = pvr_pixel_format::dxt5;
                break;
            case image_data_format::rgb_pvrtc2:
                res = pvr_pixel_format::pvrtc_2bpp_rgb;
                break;
            case image_data_format::rgb_pvrtc4:
                res = pvr_pixel_format::pvrtc_4bpp_rgb;
                break;
            case image_data_format::rgba_pvrtc2:
                res = pvr_pixel_format::pvrtc_2bpp_rgba;
                break;
            case image_data_format::rgba_pvrtc4:
                res = pvr_pixel_format::pvrtc_4bpp_rgba;
                break;
            case image_data_format::rgba_pvrtc2_v2:
                res = pvr_pixel_format::pvrtc_ii_2bpp;
                break;
            case image_data_format::rgba_pvrtc4_v2:
                res = pvr_pixel_format::pvrtc_ii_4bpp;
                break;
            default:
                E2D_ASSERT_MSG(false, "unused pixel format");
                break;
        }

        return static_cast<u64>(res);
    }
}

namespace e2d::images::impl
{
    bool save_image_pvr(const image& src, buffer& dst) {
        E2D_UNUSED(src, dst);
        if ( is_save_image_pvr_supported(src.format()) ) {
            u64 pixel_format = get_pvr_pixel_format(src.format());
            pvr_header hdr;
            hdr.version = 0x03525650;
            hdr.flags = 0;
            hdr.pixelFormat0 = pixel_format & 0x00000000'FFFFFFFFull;
            hdr.pixelFormat1 = pixel_format >> 32;
            hdr.colorSpace = static_cast<u32>(pvr_color_space::linear);
            hdr.channelType = static_cast<u32>(pvr_channel_type::ubyte);
            hdr.height = src.size().y;
            hdr.width = src.size().x;
            hdr.depth = 0;
            hdr.numSurfaces = 1;
            hdr.numFaces = 1;
            hdr.mipMapCount = 0;
            hdr.metaDataSize = sizeof(pvr_meta_data);

            pvr_meta_data md;

            size_t size = sizeof(hdr) + sizeof(md) + src.data().size();
            u8 content[size];
            std::memcpy(content, &hdr, sizeof(hdr));
            std::memcpy(content + sizeof(hdr), &md, sizeof(md));
            std::memcpy(content + sizeof(hdr) + sizeof(md), src.data().data(), src.data().size());
            dst.assign(content, size);
            return true;
        }
        return false;
    }
}
