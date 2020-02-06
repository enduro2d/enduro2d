/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "font_impl/font_impl.hpp"

namespace
{
    using namespace e2d;

    u64 make_kerning_key(u32 first, u32 second) noexcept {
        return (static_cast<u64>(first) << 32) | static_cast<u64>(second);
    }
}

namespace e2d
{
    font::font(font&& other) noexcept {
        assign(std::move(other));
    }

    font& font::operator=(font&& other) noexcept {
        return assign(std::move(other));
    }

    font::font(const font& other) {
        assign(other);
    }

    font& font::operator=(const font& other) {
        return assign(other);
    }

    font::font(content&& content) noexcept {
        assign(std::move(content));
    }

    font::font(const content& content) {
        assign(content);
    }

    font& font::assign(font&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    font& font::assign(const font& other) {
        if ( this != &other ) {
            content_ = other.content_;
        }
        return *this;
    }

    font& font::assign(content&& content) noexcept {
        content_ = std::move(content);
        return *this;
    }

    font& font::assign(const content& content) {
        content_ = content;
        return *this;
    }

    void font::swap(font& other) noexcept {
        using std::swap;
        swap(content_.info, other.content_.info);
        swap(content_.kernings, other.content_.kernings);
        swap(content_.glyphs, other.content_.glyphs);
    }

    void font::clear() noexcept {
        content_ = content();
    }

    bool font::empty() const noexcept {
        return !content_.info.font_size;
    }

    const font::font_info& font::info() const noexcept {
        return content_.info;
    }

    const flat_map<u64, i32>& font::kernings() const noexcept {
        return content_.kernings;
    }

    const flat_map<u32, font::glyph_info>& font::glyphs() const noexcept {
        return content_.glyphs;
    }

    i32 font::get_kerning(u32 first, u32 second) const noexcept {
        const u64 key = make_kerning_key(first, second);
        const auto iter = content_.kernings.find(key);
        return iter != content_.kernings.end()
            ? iter->second
            : 0;
    }

    const font::glyph_info* font::find_glyph(u32 code_point) const noexcept {
        const auto iter = content_.glyphs.find(code_point);
        return iter != content_.glyphs.end()
            ? &iter->second
            : nullptr;
    }
}

namespace e2d
{
    void swap(font& l, font& r) noexcept {
        l.swap(r);
    }

    bool operator==(const font& l, const font& r) noexcept {
        return l.info() == r.info()
            && l.kernings() == r.kernings()
            && l.glyphs() == r.glyphs();
    }

    bool operator!=(const font& l, const font& r) noexcept {
        return !(l == r);
    }

    bool operator==(const font::font_info& l, const font::font_info& r) noexcept {
        return l.atlas_file == r.atlas_file
            && l.atlas_size == r.atlas_size
            && l.font_size == r.font_size
            && l.line_height == r.line_height
            && l.glyph_ascent == r.glyph_ascent;
    }

    bool operator==(const font::glyph_info& l, const font::glyph_info& r) noexcept {
        return l.offset == r.offset
            && l.tex_rect == r.tex_rect
            && l.advance == r.advance;
    }

    bool operator==(const font::content& l, const font::content& r) noexcept {
        return l.info == r.info
            && l.kernings == r.kernings
            && l.glyphs == r.glyphs;
    }
}

namespace e2d::fonts
{
    bool try_load_font(
        font& dst,
        buffer_view src) noexcept
    {
        try {
            return impl::load_font_bmfont(dst, src);
        } catch (...) {
            return false;
        }
    }

    bool try_load_font(
        font& dst,
        const input_stream_uptr& src) noexcept
    {
        buffer file_data;
        return streams::try_read_tail(file_data, src)
            && try_load_font(dst, file_data);
    }
}
