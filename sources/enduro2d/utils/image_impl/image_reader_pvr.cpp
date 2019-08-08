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
    using namespace e2d::images::impl;
    using namespace e2d::images::impl::pvr;

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
        const pvr_header& hdr,
        buffer_view content) noexcept
    {
        if ( pvr_color_space(hdr.color_space) != pvr_color_space::linear ) {
            return image_info();
        }

        const pvr_pixel_format fmt = pvr_pixel_format(
            (static_cast<u64>(hdr.pixel_format0)) |
            (static_cast<u64>(hdr.pixel_format1) << 32));

        switch ( fmt ) {
        case pvr_pixel_format::pvrtc_2bpp_rgb:
            return {
                buffer(content),
                image_data_format::rgb_pvrtc2,
                8,
                {8,4}};
        case pvr_pixel_format::pvrtc_2bpp_rgba:
            return {
                buffer(content),
                image_data_format::rgba_pvrtc2,
                8,
                {8,4}};
        case pvr_pixel_format::pvrtc_4bpp_rgb:
            return {
                buffer(content),
                image_data_format::rgb_pvrtc4,
                8,
                {4,4}};
        case pvr_pixel_format::pvrtc_4bpp_rgba:
            return {
                buffer(content),
                image_data_format::rgba_pvrtc4,
                8,
                {4,4}};
        case pvr_pixel_format::pvrtc2_2bpp:
            return {
                buffer(content),
                image_data_format::rgba_pvrtc2_v2,
                8,
                {8,4}};
        case pvr_pixel_format::pvrtc2_4bpp:
            return {
                buffer(content),
                image_data_format::rgba_pvrtc4_v2,
                8,
                {4,4}};
        case pvr_pixel_format::dxt1:
            return {
                buffer(content),
                image_data_format::rgba_dxt1,
                8,
                {4,4}};
        case pvr_pixel_format::dxt2:
        case pvr_pixel_format::dxt3:
            return {
                buffer(content),
                image_data_format::rgba_dxt3,
                16,
                {4,4}};
        case pvr_pixel_format::dxt4:
        case pvr_pixel_format::dxt5:
            return {
                buffer(content),
                image_data_format::rgba_dxt5,
                16,
                {4,4}};
        case pvr_pixel_format::a8:
            return {
                buffer(content),
                image_data_format::a8,
                1,
                {1,1}};
        case pvr_pixel_format::l8:
            return {
                buffer(content),
                image_data_format::l8,
                1,
                {1,1}};
        case pvr_pixel_format::la8:
            return {
                buffer(content),
                image_data_format::la8,
                2,
                {1,1}};
        case pvr_pixel_format::rgb8:
            return {
                buffer(content),
                image_data_format::rgb8,
                3,
                {1,1}};
        case pvr_pixel_format::rgba8:
            return {
                buffer(content),
                image_data_format::rgba8,
                4,
                {1,1}};
        case pvr_pixel_format::bgr8:
            return {
                bgr8_to_rgb8(content),
                image_data_format::rgb8,
                3,
                {1,1}};
        case pvr_pixel_format::bgra8:
            return {
                bgra8_to_rgba8(content),
                image_data_format::rgba8,
                4,
                {1,1}};
        default:
            return image_info();
        }
    }
}

namespace e2d::images::impl
{
    bool load_image_pvr(image& dst, buffer_view src) {
        if ( src.size() < sizeof(pvr_header) ) {
            return false;
        }

        const pvr_header& hdr = *reinterpret_cast<const pvr_header*>(src.data());

        if ( hdr.version != make_fourcc('P', 'V', 'R', 3) ) {
            return false;
        }

        if ( src.size() < sizeof(pvr_header) + hdr.meta_data_size ) {
            return false;
        }

        if ( hdr.mipmap_count > 1 ||
             hdr.num_surfaces > 1 ||
             hdr.num_faces > 1 ||
             hdr.depth > 1 )
        {
            return false;
        }

        image_info info = extract_image_info(hdr, {
            static_cast<const u8*>(src.data()) + sizeof(pvr_header) + hdr.meta_data_size,
            src.size() - sizeof(pvr_header) - hdr.meta_data_size});

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
