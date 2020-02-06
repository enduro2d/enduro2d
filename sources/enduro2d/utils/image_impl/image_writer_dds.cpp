/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"
#include "image_impl_dds.hpp"

namespace
{
    using namespace e2d;
    using namespace e2d::images::impl::dds;
}

namespace e2d::images::impl
{
    bool check_save_image_dds(const image& src) noexcept {
        switch ( src.format() ) {
            case image_data_format::a8:
            case image_data_format::l8:
            case image_data_format::la8:
            case image_data_format::rgb8:
            case image_data_format::rgba8:

            case image_data_format::rgba_dxt1:
            case image_data_format::rgba_dxt3:
            case image_data_format::rgba_dxt5:
                return true;
            default:
                return false;
        }
    }

    bool save_image_dds(const image& src, buffer& dst) {
        if ( !check_save_image_dds(src) ) {
            return false;
        }

        dds_header hdr;
        hdr.magic = make_fourcc('D', 'D', 'S', ' ');
        hdr.size = sizeof(dds_header) - sizeof(hdr.magic);
        hdr.flags =
            dds_hf_caps |
            dds_hf_width |
            dds_hf_height |
            dds_hf_linear_size |
            dds_hf_pixel_format;

        hdr.width = src.size().x;
        hdr.height = src.size().y;
        hdr.pitch_or_linear_size = math::numeric_cast<u32>(src.data().size());
        hdr.depth = 0;
        hdr.mipmap_count = 0;
        hdr.caps = dds_caps_texture;
        hdr.caps2 = 0;

        hdr.pf.size = sizeof(dds_pixel_format);
        hdr.pf.flags = 0;
        hdr.pf.fourcc = 0;
        hdr.pf.rgbbit_count = 0;
        hdr.pf.rbit_mask = 0;
        hdr.pf.gbit_mask = 0;
        hdr.pf.bbit_mask = 0;
        hdr.pf.abit_mask = 0;

        switch ( src.format() ) {
        case image_data_format::a8:
            hdr.pf.flags = dds_pff_alpha | dds_pff_alphapixels;
            hdr.pf.rgbbit_count = 8;
            hdr.pf.rbit_mask = 0x00000000;
            hdr.pf.gbit_mask = 0x00000000;
            hdr.pf.bbit_mask = 0x00000000;
            hdr.pf.abit_mask = 0x000000FF;
            break;
        case image_data_format::l8:
            hdr.pf.flags = dds_pff_luminance;
            hdr.pf.rgbbit_count = 8;
            hdr.pf.rbit_mask = 0x000000FF;
            hdr.pf.gbit_mask = 0x00000000;
            hdr.pf.bbit_mask = 0x00000000;
            hdr.pf.abit_mask = 0x00000000;
            break;
        case image_data_format::la8:
            hdr.pf.flags = dds_pff_luminance | dds_pff_alphapixels;
            hdr.pf.rgbbit_count = 16;
            hdr.pf.rbit_mask = 0x000000FF;
            hdr.pf.gbit_mask = 0x00000000;
            hdr.pf.bbit_mask = 0x00000000;
            hdr.pf.abit_mask = 0x0000FF00;
            break;
        case image_data_format::rgb8:
            hdr.pf.flags = dds_pff_rgb;
            hdr.pf.rgbbit_count = 24;
            hdr.pf.rbit_mask = 0x000000FF;
            hdr.pf.gbit_mask = 0x0000FF00;
            hdr.pf.bbit_mask = 0x00FF0000;
            hdr.pf.abit_mask = 0x00000000;
            break;
        case image_data_format::rgba8:
            hdr.pf.flags = dds_pff_rgb | dds_pff_alphapixels;
            hdr.pf.rgbbit_count = 32;
            hdr.pf.rbit_mask = 0x000000FF;
            hdr.pf.gbit_mask = 0x0000FF00;
            hdr.pf.bbit_mask = 0x00FF0000;
            hdr.pf.abit_mask = 0xFF000000;
            break;
        case image_data_format::rgba_dxt1:
            hdr.pf.flags = dds_pff_fourcc;
            hdr.pf.fourcc = make_fourcc('D', 'X', 'T', '1');
            break;
        case image_data_format::rgba_dxt3:
            hdr.pf.flags = dds_pff_fourcc;
            hdr.pf.fourcc = make_fourcc('D', 'X', 'T', '3');
            break;
        case image_data_format::rgba_dxt5:
            hdr.pf.flags = dds_pff_fourcc;
            hdr.pf.fourcc = make_fourcc('D', 'X', 'T', '5');
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected image data format");
            return false;
        }

        buffer image_data(sizeof(dds_header) + src.data().size());

        std::memcpy(
            image_data.data(),
            &hdr,
            sizeof(dds_header));

        std::memcpy(
            image_data.data() + sizeof(dds_header),
            src.data().data(),
            src.data().size());

        dst.assign(std::move(image_data));
        return true;
    }
}
