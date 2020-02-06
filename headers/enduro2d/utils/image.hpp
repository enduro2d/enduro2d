/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "buffer.hpp"
#include "buffer_view.hpp"
#include "color32.hpp"
#include "streams.hpp"

namespace e2d
{
    ENUM_HPP_CLASS_DECL(image_file_format, u8,
        (dds)
        (jpg)
        (png)
        (pvr)
        (tga))

    ENUM_HPP_CLASS_DECL(image_data_format, u8,
        (a8)
        (l8)
        (la8)
        (rgb8)
        (rgba8)

        (rgba_dxt1)
        (rgba_dxt3)
        (rgba_dxt5)

        (rgb_etc1)
        (rgb_etc2)
        (rgba_etc2)
        (rgb_a1_etc2)

        (rgba_astc4x4)
        (rgba_astc5x5)
        (rgba_astc6x6)
        (rgba_astc8x8)
        (rgba_astc10x10)
        (rgba_astc12x12)

        (rgb_pvrtc2)
        (rgb_pvrtc4)
        (rgba_pvrtc2)
        (rgba_pvrtc4)

        (rgba_pvrtc2_v2)
        (rgba_pvrtc4_v2))

    class bad_image_access final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad image access";
        }
    };

    class image final {
    public:
        image() = default;

        image(image&& other) noexcept;
        image& operator=(image&& other) noexcept;

        image(const image& other);
        image& operator=(const image& other);

        image(const v2u& size, image_data_format format, buffer&& data) noexcept;
        image(const v2u& size, image_data_format format, const buffer& data);

        image& assign(image&& other) noexcept;
        image& assign(const image& other);

        image& assign(const v2u& size, image_data_format format, buffer&& data) noexcept;
        image& assign(const v2u& size, image_data_format format, const buffer& data);

        void swap(image& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        color32 pixel32(u32 u, u32 v) const;
        color32 pixel32(const v2u& uv) const;

        const v2u& size() const noexcept;
        image_data_format format() const noexcept;
        const buffer& data() const noexcept;
    private:
        buffer data_;
        v2u size_;
        image_data_format format_ = image_data_format::rgba8;
    };

    void swap(image& l, image& r) noexcept;
    bool operator==(const image& l, const image& r) noexcept;
    bool operator!=(const image& l, const image& r) noexcept;
}

ENUM_HPP_REGISTER_TRAITS(e2d::image_file_format)
ENUM_HPP_REGISTER_TRAITS(e2d::image_data_format)

namespace e2d::images
{
    bool try_load_image(
        image& dst,
        buffer_view src) noexcept;

    bool try_load_image(
        image& dst,
        const input_stream_uptr& src) noexcept;

    bool try_save_image(
        const image& src,
        image_file_format format,
        buffer& dst) noexcept;

    bool try_save_image(
        const image& src,
        image_file_format format,
        const output_stream_uptr& dst) noexcept;

    bool check_save_image_support(
        const image& src,
        image_file_format format) noexcept;
}
