/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "image_impl/image_impl.hpp"

namespace
{
    using namespace e2d;

    struct data_format_description {
        u32 minimal_size;
        u32 bits_per_pixel;
        image_data_format format;
        bool compressed;
        bool must_be_square;
        bool must_be_power_of_two;
    };

    const data_format_description data_format_descriptions[] = {
        {1,  8, image_data_format::g8,          false, false, false},
        {1, 16, image_data_format::ga8,         false, false, false},
        {1, 24, image_data_format::rgb8,        false, false, false},
        {1, 32, image_data_format::rgba8,       false, false, false},

        {4,  4, image_data_format::dxt1,        true,  false, true},
        {4,  8, image_data_format::dxt3,        true,  false, true},
        {4,  8, image_data_format::dxt5,        true,  false, true},

        {8,  2, image_data_format::rgb_pvrtc2,  true,  true,  true},
        {8,  4, image_data_format::rgb_pvrtc4,  true,  true,  true},
        {8,  2, image_data_format::rgba_pvrtc2, true,  true,  true},
        {8,  4, image_data_format::rgba_pvrtc4, true,  true,  true}
    };

    const data_format_description& get_data_format_description(image_data_format format) noexcept {
        const std::size_t findex = static_cast<std::underlying_type_t<image_data_format>>(format);
        E2D_ASSERT(findex < E2D_COUNTOF(data_format_descriptions));
        const data_format_description& fdesc = data_format_descriptions[findex];
        E2D_ASSERT(fdesc.format == format);
        return fdesc;
    }

    v2u adjust_image_size(const v2u& size, image_data_format format) noexcept {
        const data_format_description& format_desc = get_data_format_description(format);
        v2u nsize = math::maximized(size, v2u(format_desc.minimal_size));
        if ( format_desc.must_be_square ) {
            nsize = v2u(math::maximum(nsize));
        }
        if ( format_desc.must_be_power_of_two ) {
            nsize = v2u(math::next_power_of_2(nsize.x),
                        math::next_power_of_2(nsize.y));
        }
        return nsize;
    }

    std::size_t buffer_size_for_image(const v2u& size, image_data_format format) noexcept {
        const v2u nsize = adjust_image_size(size, format);
        const std::size_t bpp = get_data_format_description(format).bits_per_pixel;
        const std::size_t bits = nsize.x * nsize.y * bpp;
        E2D_ASSERT(bits % 8 == 0);
        return bits / 8;
    }

    bool check_image_format(const v2u& size, image_data_format format) noexcept {
        return size == adjust_image_size(size, format);
    }

    bool check_image_format(const v2u& size, image_data_format format, const buffer& data) noexcept {
        return check_image_format(size, format)
            && data.size() == buffer_size_for_image(size, format);
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

    image::image(const v2u& size, image_data_format format) {
        assign(size, format);
    }

    image::image(const v2u& size, image_data_format format, buffer&& data) {
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

    image& image::assign(const v2u& size, image_data_format format) {
        if ( !check_image_format(size, format) ) {
            throw bad_image_data_format();
        }
        data_.resize(buffer_size_for_image(size, format));
        size_ = size;
        format_ = format;
        return *this;
    }

    image& image::assign(const v2u& size, image_data_format format, buffer&& data) {
        if ( !check_image_format(size, format, data) ) {
            throw bad_image_data_format();
        }
        data_.assign(std::move(data));
        size_ = size;
        format_ = format;
        return *this;
    }

    image& image::assign(const v2u& size, image_data_format format, const buffer& data) {
        if ( !check_image_format(size, format, data) ) {
            throw bad_image_data_format();
        }
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
                return color32(pixel[0],
                               pixel[0],
                               pixel[0]);
            case image_data_format::ga8:
                E2D_ASSERT(bytes_per_pixel == 2);
                return color32(pixel[0],
                               pixel[0],
                               pixel[0],
                               pixel[1]);
            case image_data_format::rgb8:
                E2D_ASSERT(bytes_per_pixel == 3);
                return color32(pixel[0],
                               pixel[1],
                               pixel[2]);
            case image_data_format::rgba8:
                E2D_ASSERT(bytes_per_pixel == 4);
                return color32(pixel[0],
                               pixel[1],
                               pixel[2],
                               pixel[3]);
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

    bool operator<(const image& l, const image& r) noexcept {
        return l.format() < r.format()
            || (l.format() == r.format() && l.size() < r.size())
            || (l.format() == r.format() && l.size() == r.size() && l.data() < r.data());
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

namespace e2d { namespace images
{
    bool try_load_image(
        image& dst,
        const buffer& src) noexcept
    {
        return impl::try_load_image_dds(dst, src)
            || impl::try_load_image_pvr(dst, src)
            || impl::try_load_image_stb(dst, src);
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
        switch ( format ) {
            case image_file_format::dds:
                return impl::try_save_image_dds(src, dst);
            case image_file_format::jpg:
                return impl::try_save_image_jpg(src, dst);
            case image_file_format::png:
                return impl::try_save_image_png(src, dst);
            case image_file_format::pvr:
                return impl::try_save_image_pvr(src, dst);
            case image_file_format::tga:
                return impl::try_save_image_tga(src, dst);
            default:
                E2D_ASSERT_MSG(false, "unexpected image file format");
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
}}
