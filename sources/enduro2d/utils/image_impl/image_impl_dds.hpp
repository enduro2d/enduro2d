/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/utils/_utils.hpp>

namespace e2d::images::impl::dds
{
    // DDS format specification:
    // https://docs.microsoft.com/en-us/windows/desktop/direct3ddds/dx-graphics-dds-pguide

    // header flags
    const u32 dds_hf_caps = 0x00000001;
    const u32 dds_hf_height = 0x00000002;
    const u32 dds_hf_width = 0x00000004;
    const u32 dds_hf_pitch = 0x00000008;
    const u32 dds_hf_pixel_format = 0x00001000;
    const u32 dds_hf_mipmap_count = 0x00020000;
    const u32 dds_hf_linear_size = 0x00080000;
    const u32 dds_hf_depth = 0x00800000;

    // pixel format flags
    const u32 dds_pff_alphapixels = 0x1;
    const u32 dds_pff_alpha = 0x2;
    const u32 dds_pff_fourcc = 0x4;
    const u32 dds_pff_rgb = 0x40;
    const u32 dds_pff_yuv = 0x200;
    const u32 dds_pff_luminance = 0x20000;

    // caps flags
    const u32 dds_caps_complex = 0x8;
    const u32 dds_caps_mipmap = 0x400000;
    const u32 dds_caps_texture = 0x1000;

    // caps2 flags
    const u32 dds_caps2_cubemap = 0x200;
    const u32 dds_caps2_volume = 0x200000;

    struct dds_pixel_format {
        u32 size{0};
        u32 flags{0};
        u32 fourcc{0};
        u32 rgbbit_count{0};
        u32 rbit_mask{0};
        u32 gbit_mask{0};
        u32 bbit_mask{0};
        u32 abit_mask{0};
    };

    struct dds_header {
        u32 magic{0};
        u32 size{0};
        u32 flags{0};
        u32 height{0};
        u32 width{0};
        u32 pitch_or_linear_size{0};
        u32 depth{0};
        u32 mipmap_count{0};
        u32 reserved1[11] = {0};
        dds_pixel_format pf;
        u32 caps{0};
        u32 caps2{0};
        u32 reserved2[3] = {0};
    };

    static_assert(sizeof(dds_pixel_format) == 32, "invalid dds_pixel_format structure size");
    static_assert(sizeof(dds_header) == 128, "invalid dds_header structure size");

    inline constexpr u32 make_fourcc(u8 a, u8 b, u8 c, u8 d) noexcept {
        return
            (static_cast<u32>(a)) |
            (static_cast<u32>(b) << 8) |
            (static_cast<u32>(c) << 16) |
            (static_cast<u32>(d) << 24);
    }
}
