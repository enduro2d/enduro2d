/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "buffer.hpp"
#include "buffer_view.hpp"
#include "streams.hpp"

namespace e2d
{
    class font final {
    public:
        struct font_info {
            str atlas_file;
            v2u atlas_size;
            u32 font_size{0};
            u32 line_height{0};
            u32 glyph_ascent{0};
        };

        struct glyph_info {
            v2i offset;
            b2u tex_rect;
            i32 advance{0};
        };

        struct content {
            font_info info;
            flat_map<u64, i32> kernings;
            flat_map<u32, glyph_info> glyphs;
        };
    public:
        font() = default;

        font(font&& other) noexcept;
        font& operator=(font&& other) noexcept;

        font(const font& other);
        font& operator=(const font& other);

        font(content&& content) noexcept;
        font(const content& content);

        font& assign(font&& other) noexcept;
        font& assign(const font& other);

        font& assign(content&& content) noexcept;
        font& assign(const content& content);

        void swap(font& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        const font_info& info() const noexcept;
        const flat_map<u64, i32>& kernings() const noexcept;
        const flat_map<u32, glyph_info>& glyphs() const noexcept;

        i32 get_kerning(u32 first, u32 second) const noexcept;
        const glyph_info* find_glyph(u32 code_point) const noexcept;
    private:
        content content_;
    };

    void swap(font& l, font& r) noexcept;
    bool operator==(const font& l, const font& r) noexcept;
    bool operator!=(const font& l, const font& r) noexcept;

    bool operator==(const font::font_info& l, const font::font_info& r) noexcept;
    bool operator==(const font::glyph_info& l, const font::glyph_info& r) noexcept;
    bool operator==(const font::content& l, const font::content& r) noexcept;
}

namespace e2d::fonts
{
    bool try_load_font(
        font& dst,
        buffer_view src) noexcept;

    bool try_load_font(
        font& dst,
        const input_stream_uptr& src) noexcept;
}
