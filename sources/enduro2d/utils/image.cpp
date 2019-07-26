/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl/image_impl.hpp"

namespace
{
    using namespace e2d;

    struct data_format_description {
        u32 bits_per_pixel;
        image_data_format format;
        bool compressed;
    };

    const data_format_description data_format_descriptions[] = {
        { 8, image_data_format::g8,             false},
        {16, image_data_format::ga8,            false},
        {24, image_data_format::rgb8,           false},
        {32, image_data_format::rgba8,          false},

        { 4, image_data_format::rgb_dxt1,       true},
        { 4, image_data_format::rgba_dxt1,      true},
        { 8, image_data_format::rgba_dxt3,      true},
        { 8, image_data_format::rgba_dxt5,      true},

        { 2, image_data_format::rgb_pvrtc2,     true},
        { 4, image_data_format::rgb_pvrtc4,     true},
        { 2, image_data_format::rgba_pvrtc2,    true},
        { 4, image_data_format::rgba_pvrtc4,    true},

        { 2, image_data_format::rgba_pvrtc2_v2, true},
        { 4, image_data_format::rgba_pvrtc4_v2, true}
    };

    const data_format_description& get_data_format_description(image_data_format format) noexcept {
        const std::size_t findex = static_cast<std::underlying_type_t<image_data_format>>(format);
        E2D_ASSERT(findex < std::size(data_format_descriptions));
        const data_format_description& fdesc = data_format_descriptions[findex];
        E2D_ASSERT(fdesc.format == format);
        return fdesc;
    }
}

namespace e2d
{
    image::image(image&& other) noexcept {
        assign(std::move(other));
    }

    image& image::operator=(image&& other) noexcept {
        return assign(std::move(other));
    }

    image::image(const image& other) {
        assign(other);
    }

    image& image::operator=(const image& other) {
        return assign(other);
    }

    image::image(const v2u& size, image_data_format format, buffer&& data) noexcept {
        assign(size, format, std::move(data));
    }

    image::image(const v2u& size, image_data_format format, const buffer& data) {
        assign(size, format, data);
    }

    image& image::assign(image&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    image& image::assign(const image& other) {
        if ( this != &other ) {
            data_.assign(other.data_);
            size_ = other.size_;
            format_ = other.format_;
        }
        return *this;
    }

    image& image::assign(const v2u& size, image_data_format format, buffer&& data) noexcept {
        data_.assign(std::move(data));
        size_ = size;
        format_ = format;
        return *this;
    }

    image& image::assign(const v2u& size, image_data_format format, const buffer& data) {
        data_.assign(data);
        size_ = size;
        format_ = format;
        return *this;
    }

    void image::swap(image& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(format_, other.format_);
    }

    void image::clear() noexcept {
        data_.clear();
        size_ = v2u::zero();
        format_ = image_data_format::rgba8;
    }

    bool image::empty() const noexcept {
        return data_.empty();
    }

    color image::pixel(u32 u, u32 v) const {
        return color(pixel32(u, v));
    }

    color image::pixel(const v2u& uv) const {
        return color(pixel32(uv));
    }

    color32 image::pixel32(u32 u, u32 v) const {
        const data_format_description& format_desc =
            get_data_format_description(format_);

        if ( empty() || u >= size_.x || v >= size_.y || format_desc.compressed ) {
            throw bad_image_access();
        }

        const std::size_t bits_per_pixel = format_desc.bits_per_pixel;
        const std::size_t bytes_per_pixel = bits_per_pixel / 8;
        const std::size_t stride_in_bytes = size_.x * bytes_per_pixel;
        E2D_ASSERT(bits_per_pixel % 8 == 0);

        const std::size_t pixel_index = v * stride_in_bytes + u * bytes_per_pixel;
        E2D_ASSERT(pixel_index + bytes_per_pixel <= data_.size());
        const u8* const pixel = data_.data() + pixel_index;

        switch ( format_ ) {
            case image_data_format::g8:
                E2D_ASSERT(bytes_per_pixel == 1);
                return color32(pixel[0], pixel[0], pixel[0]);
            case image_data_format::ga8:
                E2D_ASSERT(bytes_per_pixel == 2);
                return color32(pixel[0], pixel[0], pixel[0], pixel[1]);
            case image_data_format::rgb8:
                E2D_ASSERT(bytes_per_pixel == 3);
                return color32(pixel[0], pixel[1], pixel[2]);
            case image_data_format::rgba8:
                E2D_ASSERT(bytes_per_pixel == 4);
                return color32(pixel[0], pixel[1], pixel[2], pixel[3]);
            default:
                E2D_ASSERT_MSG(false, "unexpected image data format");
                throw bad_image_access();
        }
    }

    color32 image::pixel32(const v2u& uv) const {
        return pixel32(uv.x, uv.y);
    }

    const v2u& image::size() const noexcept {
        return size_;
    }

    image_data_format image::format() const noexcept {
        return format_;
    }

    const buffer& image::data() const noexcept {
        return data_;
    }
}

namespace e2d
{
    void swap(image& l, image& r) noexcept {
        l.swap(r);
    }

    bool operator==(const image& l, const image& r) noexcept {
        return l.format() == r.format()
            && l.size() == r.size()
            && l.data() == r.data();
    }

    bool operator!=(const image& l, const image& r) noexcept {
        return !(l == r);
    }
}

namespace e2d::images
{
    bool try_load_image(
        image& dst,
        const buffer& src) noexcept
    {
        try {
            return impl::load_image_dds(dst, src)
                || impl::load_image_pvr(dst, src)
                || impl::load_image_stb(dst, src);
        } catch (...) {
            return false;
        }
    }

    bool try_load_image(
        image& dst,
        const input_stream_uptr& src) noexcept
    {
        buffer file_data;
        return streams::try_read_tail(file_data, src)
            && try_load_image(dst, file_data);
    }

    bool try_save_image(
        const image& src,
        image_file_format format,
        buffer& dst) noexcept
    {
        try {
            switch ( format ) {
                case image_file_format::dds:
                    return impl::save_image_dds(src, dst);
                case image_file_format::jpg:
                    return impl::save_image_jpg(src, dst);
                case image_file_format::png:
                    return impl::save_image_png(src, dst);
                case image_file_format::pvr:
                    return impl::save_image_pvr(src, dst);
                case image_file_format::tga:
                    return impl::save_image_tga(src, dst);
                default:
                    E2D_ASSERT_MSG(false, "unexpected image file format");
                    return false;
            }
        } catch (...) {
            return false;
        }
    }

    bool try_save_image(
        const image& src,
        image_file_format format,
        const output_stream_uptr& dst) noexcept
    {
        buffer file_data;
        return try_save_image(src, format, file_data)
            && streams::try_write_tail(file_data, dst);
    }
}
