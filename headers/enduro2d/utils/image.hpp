/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "buffer.hpp"
#include "color.hpp"
#include "color32.hpp"
#include "streams.hpp"

namespace e2d
{
    enum class image_file_format : u8 {
        dds,
        jpg,
        png,
        pvr,
        tga,

        unknown
    };

    enum class image_data_format : u8 {
        g8,
        ga8,
        rgb8,
        rgba8,

        dxt1,
        dxt3,
        dxt5,

        rgb_pvrtc2,
        rgb_pvrtc4,

        rgba_pvrtc2,
        rgba_pvrtc4,

        unknown
    };

    class bad_image_access final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad image access";
        }
    };

    class bad_image_data_format final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad image data format";
        }
    };

    class image final {
    public:
        image() = default;

        image(image&& other) noexcept;
        image& operator=(image&& other) noexcept;

        image(const image& other);
        image& operator=(const image& other);

        image(const v2u& size, image_data_format format);
        image(const v2u& size, image_data_format format, buffer&& data);
        image(const v2u& size, image_data_format format, const buffer& data);

        image& assign(image&& other) noexcept;
        image& assign(const image& other);

        image& assign(const v2u& size, image_data_format format);
        image& assign(const v2u& size, image_data_format format, buffer&& data);
        image& assign(const v2u& size, image_data_format format, const buffer& data);

        void swap(image& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        color pixel(u32 u, u32 v) const;
        color pixel(const v2u& uv) const;
        color32 pixel32(u32 u, u32 v) const;
        color32 pixel32(const v2u& uv) const;

        const v2u& size() const noexcept;
        image_data_format format() const noexcept;
        const buffer& data() const noexcept;
    private:
        buffer data_;
        v2u size_;
        image_data_format format_ = image_data_format::unknown;
    };

    void swap(image& l, image& r) noexcept;
    bool operator<(const image& l, const image& r) noexcept;
    bool operator==(const image& l, const image& r) noexcept;
    bool operator!=(const image& l, const image& r) noexcept;
}

namespace e2d { namespace images
{
    bool try_load_image(
        image& dst,
        const buffer& src) noexcept;

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
}}
