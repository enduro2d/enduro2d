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
    using namespace e2d::images::impl::dds;

    bool is_save_image_dds_supported(image_data_format data_format) noexcept {
        switch ( data_format ) {
            case image_data_format::g8:
            case image_data_format::rgb8:
            case image_data_format::rgba8:
            case image_data_format::rgb_dxt1:
            case image_data_format::rgba_dxt1:
            case image_data_format::rgba_dxt3:
            case image_data_format::rgba_dxt5:
                return true;
            default:
                return false;
        }
    }

    u32 get_pitch_or_linear_size(image_data_format data_format) noexcept {
        switch ( data_format ) {
            case image_data_format::g8:
                break;
            case image_data_format::rgb8:
                break;
            case image_data_format::rgba8:
                break;
            case image_data_format::rgb_dxt1:
                break;
            case image_data_format::rgba_dxt1:
                break;
            case image_data_format::rgba_dxt3:
                break;
            case image_data_format::rgba_dxt5:
                break;
            default:
                E2D_ASSERT_MSG(false, "unused pixel format");
                break;
        }

        return 0;
    }
}

namespace e2d::images::impl
{
    bool save_image_dds(const image& src, buffer& dst) {
        E2D_UNUSED(src, dst);
        if ( is_save_image_dds_supported(src.format()) ) {
            dds_header hdr;
            hdr.dwMagic = 0x20534444;
            hdr.dwSize = 124;
            hdr.dwFlags = ddsd_caps | ddsd_height | ddsd_width | ddsd_pixelformat;
            hdr.dwHeight = src.size().y;
            hdr.dwWidth = src.size().x;
            hdr.dwPitchOrLinearSize = get_pitch_or_linear_size(src.format());
            hdr.dwDepth = 0;
            hdr.dwMipMapCount = 0;
            hdr.ddspf.dwSize = 32;
            hdr.ddspf.dwFlags = 0;
            hdr.ddspf.dwFourCC = 0;
            switch ( src.format() ) {
                case image_data_format::g8:
                    hdr.ddspf.dwRGBBitCount = 8;
                    break;
                case image_data_format::rgb8:
                    hdr.ddspf.dwFlags |= ddsf_rgb;
                    hdr.ddspf.dwRGBBitCount = 24;
                    hdr.ddspf.dwRBitMask = 0x00FF0000;
                    hdr.ddspf.dwGBitMask = 0x0000FF00;
                    hdr.ddspf.dwBBitMask = 0x000000FF;
                    break;
                case image_data_format::rgba8:
                    hdr.ddspf.dwFlags |= ddsf_rgba;
                    hdr.ddspf.dwRGBBitCount = 32;
                    break;
                case image_data_format::rgb_dxt1:
                    hdr.ddspf.dwFlags |= ddsf_fourcc;
                    hdr.ddspf.dwFourCC = fourcc_dxt1;
                    break;
                case image_data_format::rgba_dxt1:
                    hdr.ddspf.dwFlags |= ddsf_fourcc | ddsf_alphapixels;
                    hdr.ddspf.dwFourCC = fourcc_dxt1;
                    break;
                case image_data_format::rgba_dxt3:
                    hdr.ddspf.dwFlags |= ddsf_fourcc;
                    hdr.ddspf.dwFourCC |= fourcc_dxt3;
                    break;
                case image_data_format::rgba_dxt5:
                    hdr.ddspf.dwFlags |= ddsf_fourcc;
                    hdr.ddspf.dwFourCC = fourcc_dxt5;
                    break;
                default:
                    E2D_ASSERT_MSG(false, "unused pixel format");
                    break;;
            }
            hdr.dwCaps = 0x1000;//DDSCAPS_TEXTURE
            hdr.dwCaps2 = 0;
            hdr.dwCaps3 = 0;
            hdr.dwCaps4 = 0;
            hdr.dwReserved2 = 0;

            size_t size = sizeof(hdr) + src.data().size();
            u8 content[size];
            std::memcpy(content, &hdr, sizeof(hdr));
            std::memcpy(content + sizeof(hdr), src.data().data(), src.data().size());
            dst.assign(content, size);
            return true;
        }
        return false;
    }
}
