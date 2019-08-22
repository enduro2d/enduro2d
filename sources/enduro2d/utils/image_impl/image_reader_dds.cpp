/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"
#include "image_impl_dds.hpp"

namespace
{
    using namespace e2d;
    using namespace e2d::images::impl;
    using namespace e2d::images::impl::dds;

    struct image_info {
        buffer data;
        image_data_format format = image_data_format(-1);
        u32 bytes_per_block = 0;
        v2u block_size;

        image_info() = default;
        image_info(buffer data, image_data_format format, u32 bytes_per_block, v2u block_size)
        : data(std::move(data))
        , format(format)
        , bytes_per_block(bytes_per_block)
        , block_size(block_size) {}

        bool valid() const noexcept {
            return !data.empty()
                && format != image_data_format(-1)
                && bytes_per_block > 0
                && block_size.x > 0
                && block_size.y > 0;
        }
    };

    image_info extract_image_info(
        const dds_header& hdr,
        buffer_view content)
    {
        if ( math::check_all_flags(hdr.pf.flags, dds_pff_fourcc) ) {
            switch ( hdr.pf.fourcc ) {
            case make_fourcc('D', 'X', 'T', '1'):
                return {
                    buffer(content),
                    image_data_format::rgba_dxt1,
                    8,
                    {4,4}};
            case make_fourcc('D', 'X', 'T', '2'):
            case make_fourcc('D', 'X', 'T', '3'):
                return {
                    buffer(content),
                    image_data_format::rgba_dxt3,
                    16,
                    {4,4}};
            case make_fourcc('D', 'X', 'T', '4'):
            case make_fourcc('D', 'X', 'T', '5'):
                return {
                    buffer(content),
                    image_data_format::rgba_dxt5,
                    16,
                    {4,4}};
            }
        }

        if ( math::check_all_flags(hdr.pf.flags, dds_pff_alpha) ) {
            if ( hdr.pf.rgbbit_count == 8 &&
                 hdr.pf.abit_mask == 0x000000FF )
            {
                return {
                    buffer(content),
                    image_data_format::a8,
                    1,
                    {1,1}};
            }
        }

        if ( math::check_all_flags(hdr.pf.flags, dds_pff_luminance | dds_pff_alphapixels) ) {
            if ( hdr.pf.rgbbit_count == 16 &&
                 hdr.pf.rbit_mask == 0x000000FF &&
                 hdr.pf.abit_mask == 0x0000FF00 )
            {
                return {
                    buffer(content),
                    image_data_format::la8,
                    2,
                    {1,1}};
            }
        }

        if ( math::check_all_flags(hdr.pf.flags, dds_pff_luminance) ) {
            if ( hdr.pf.rgbbit_count == 8 &&
                 hdr.pf.rbit_mask == 0x000000FF )
            {
                return {
                    buffer(content),
                    image_data_format::l8,
                    1,
                    {1,1}};
            }
        }

        if ( math::check_all_flags(hdr.pf.flags, dds_pff_rgb | dds_pff_alphapixels) ) {
            if ( hdr.pf.rgbbit_count == 8 ) {
                if ( hdr.pf.rbit_mask == 0x00000000 &&
                     hdr.pf.gbit_mask == 0x00000000 &&
                     hdr.pf.bbit_mask == 0x00000000 &&
                     hdr.pf.abit_mask == 0x000000FF )
                {
                    return {
                        buffer(content),
                        image_data_format::a8,
                        1,
                        {1,1}};
                }
            }

            if ( hdr.pf.rgbbit_count == 32 ) {
                if ( hdr.pf.rbit_mask == 0x000000FF &&
                     hdr.pf.gbit_mask == 0x0000FF00 &&
                     hdr.pf.bbit_mask == 0x00FF0000 &&
                     hdr.pf.abit_mask == 0xFF000000 )
                {
                    return {
                        buffer(content),
                        image_data_format::rgba8,
                        4,
                        {1,1}};
                }

                if ( hdr.pf.rbit_mask == 0x00FF0000 &&
                     hdr.pf.gbit_mask == 0x0000FF00 &&
                     hdr.pf.bbit_mask == 0x000000FF &&
                     hdr.pf.abit_mask == 0xFF000000 )
                {
                    return {
                        bgra8_to_rgba8(content),
                        image_data_format::rgba8,
                        4,
                        {1,1}};
                }
            }
        }

        if ( math::check_all_flags(hdr.pf.flags, dds_pff_rgb) ) {
            if ( hdr.pf.rgbbit_count == 24 ) {
                if ( hdr.pf.rbit_mask == 0x000000FF &&
                     hdr.pf.gbit_mask == 0x0000FF00 &&
                     hdr.pf.bbit_mask == 0x00FF0000 )
                {
                    return {
                        buffer(content),
                        image_data_format::rgb8,
                        3,
                        {1,1}};
                }

                if ( hdr.pf.rbit_mask == 0x00FF0000 &&
                     hdr.pf.gbit_mask == 0x0000FF00 &&
                     hdr.pf.bbit_mask == 0x000000FF )
                {
                    return {
                        bgr8_to_rgb8(content),
                        image_data_format::rgb8,
                        3,
                        {1,1}};
                }
            }

            if ( hdr.pf.rgbbit_count == 32 ) {
                if ( hdr.pf.rbit_mask == 0x00FF0000 &&
                     hdr.pf.gbit_mask == 0x0000FF00 &&
                     hdr.pf.bbit_mask == 0x000000FF )
                {
                    return {
                        bgrx8_to_rgb8(content),
                        image_data_format::rgb8,
                        3,
                        {1,1}};
                }
            }
        }

        return image_info();
    }
}

namespace e2d::images::impl
{
    bool load_image_dds(image& dst, buffer_view src) {
        if ( src.size() < sizeof(dds_header) ) {
            return false;
        }

        const dds_header& hdr = *reinterpret_cast<const dds_header*>(src.data());

        if ( hdr.magic != make_fourcc('D', 'D', 'S', ' ') ) {
            return false;
        }

        if ( hdr.size != sizeof(dds_header) - sizeof(hdr.magic) ) {
            return false;
        }

        if ( hdr.pf.size != sizeof(dds_pixel_format) ) {
            return false;
        }

        if ( math::check_any_flags(hdr.caps, dds_caps_complex | dds_caps_mipmap) ||
             math::check_any_flags(hdr.caps2, dds_caps2_cubemap | dds_caps2_volume) ||
             hdr.depth > 1 ||
             hdr.mipmap_count > 1 )
        {
            return false;
        }

        image_info info = extract_image_info(hdr, {
            static_cast<const u8*>(src.data()) + sizeof(dds_header),
            src.size() - sizeof(dds_header)});

        if ( !info.valid() ) {
            return false;
        }

        const v2u dimension = v2u(hdr.width, hdr.height);
        const std::size_t expected_image_data_size = info.bytes_per_block *
            ((dimension.x + info.block_size.x - 1) / info.block_size.x) *
            ((dimension.y + info.block_size.y - 1) / info.block_size.y);

        if ( info.data.size() != expected_image_data_size ) {
            return false;
        }

        dst = image(dimension, info.format, std::move(info.data));
        return true;
    }
}
