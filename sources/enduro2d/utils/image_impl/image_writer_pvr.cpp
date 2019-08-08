/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"
#include "image_impl_pvr.hpp"

namespace
{
    using namespace e2d;
    using namespace e2d::images::impl::pvr;

    pvr_pixel_format get_pvr_pixel_format(image_data_format fmt) noexcept {
        switch ( fmt ) {
        case image_data_format::a8: return pvr_pixel_format::a8;
        case image_data_format::l8: return pvr_pixel_format::l8;
        case image_data_format::la8: return pvr_pixel_format::la8;
        case image_data_format::rgb8: return pvr_pixel_format::rgb8;
        case image_data_format::rgba8: return pvr_pixel_format::rgba8;

        case image_data_format::rgba_dxt1: return pvr_pixel_format::dxt1;
        case image_data_format::rgba_dxt3: return pvr_pixel_format::dxt3;
        case image_data_format::rgba_dxt5: return pvr_pixel_format::dxt5;

        case image_data_format::rgb_pvrtc2: return pvr_pixel_format::pvrtc_2bpp_rgb;
        case image_data_format::rgb_pvrtc4: return pvr_pixel_format::pvrtc_4bpp_rgb;
        case image_data_format::rgba_pvrtc2: return pvr_pixel_format::pvrtc_2bpp_rgba;
        case image_data_format::rgba_pvrtc4: return pvr_pixel_format::pvrtc_4bpp_rgba;

        case image_data_format::rgba_pvrtc2_v2: return pvr_pixel_format::pvrtc2_2bpp;
        case image_data_format::rgba_pvrtc4_v2: return pvr_pixel_format::pvrtc2_4bpp;
        default:
            E2D_ASSERT_MSG(false, "unexpected image data format");
            return pvr_pixel_format(-1);
        }
    }
}

namespace e2d::images::impl
{
    bool check_save_image_pvr(const image& src) noexcept {
        switch ( src.format() ) {
            case image_data_format::a8:
            case image_data_format::l8:
            case image_data_format::la8:
            case image_data_format::rgb8:
            case image_data_format::rgba8:

            case image_data_format::rgba_dxt1:
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
    
    bool save_image_pvr(const image& src, buffer& dst) {
        if ( !check_save_image_pvr(src) ) {
            return false;
        }

        pvr_header hdr;
        hdr.version = make_fourcc('P', 'V', 'R', 3);
        hdr.flags = static_cast<u32>(pvr_flags::none);
        hdr.pixel_format0 = static_cast<u32>(
            static_cast<u64>(get_pvr_pixel_format(src.format())) & 0xFFFFFFFF);
        hdr.pixel_format1 = static_cast<u32>(
            static_cast<u64>(get_pvr_pixel_format(src.format())) >> 32);
        hdr.color_space = static_cast<u32>(pvr_color_space::linear);
        hdr.channel_type = static_cast<u32>(pvr_channel_type::ubyte);
        hdr.height = src.size().y;
        hdr.width = src.size().x;
        hdr.depth = 1;
        hdr.num_surfaces = 1;
        hdr.num_faces = 1;
        hdr.mipmap_count = 1;
        hdr.meta_data_size = 0;

        buffer image_data(sizeof(pvr_header) + src.data().size());

        std::memcpy(
            image_data.data(),
            &hdr,
            sizeof(pvr_header));

        std::memcpy(
            image_data.data() + sizeof(pvr_header),
            src.data().data(),
            src.data().size());

        dst.assign(std::move(image_data));
        return true;
    }
}
