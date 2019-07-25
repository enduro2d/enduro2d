/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "buffer.hpp"
#include "streams.hpp"

namespace e2d
{
    class font final {
    public:
        struct info_data {
            str face;
            u32 size{0};
        };

        struct common_data {
            u32 line{0};
            u32 base{0};
            u32 pages{0};
            v2u atlas_size;
        };

        struct page_data {
            u32 id{0};
            str file;
        };

        struct char_data {
            u32 id{0};
            b2hi rect;
            v2hi offset;
            i16 advance{0};
            u16 page{0};
            u16 chnl{0};
        };

        struct kerning_data {
            std::pair<u32,u32> chars{0,0};
            i32 amount{0};
        };

        struct data {
            info_data info;
            common_data common;
            flat_set<page_data> pages;
            flat_set<char_data> chars;
            flat_set<kerning_data> kernings;
        };
    public:
        font() = default;

        font(font&& other) noexcept;
        font& operator=(font&& other) noexcept;

        font(const font& other);
        font& operator=(const font& other);

        font(data&& data) noexcept;
        font(const data& data);

        font& assign(font&& other) noexcept;
        font& assign(const font& other);

        font& assign(data&& data) noexcept;
        font& assign(const data& data);

        void swap(font& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        const info_data& info() const noexcept;
        const common_data& common() const noexcept;
        const flat_set<page_data>& pages() const noexcept;
        const flat_set<char_data>& chars() const noexcept;
        const flat_set<kerning_data>& kernings() const noexcept;

        const page_data* find_page(u32 id) const noexcept;
        const char_data* find_char(u32 id) const noexcept;
        i32 get_kerning(u32 first, u32 second) const noexcept;
    private:
        data data_;
    };

    void swap(font& l, font& r) noexcept;
    bool operator==(const font& l, const font& r) noexcept;
    bool operator!=(const font& l, const font& r) noexcept;

    bool operator<(u32 l, const font::page_data& r) noexcept;
    bool operator<(const font::page_data& l, u32 r) noexcept;
    bool operator<(const font::page_data& l, const font::page_data& r) noexcept;

    bool operator<(u32 l, const font::char_data& r) noexcept;
    bool operator<(const font::char_data& l, u32 r) noexcept;
    bool operator<(const font::char_data& l, const font::char_data& r) noexcept;

    bool operator<(const std::pair<u32,u32>& l, const font::kerning_data& r) noexcept;
    bool operator<(const font::kerning_data& l, const std::pair<u32,u32>& r) noexcept;
    bool operator<(const font::kerning_data& l, const font::kerning_data& r) noexcept;

    bool operator==(const font::info_data& l, const font::info_data& r) noexcept;
    bool operator==(const font::common_data& l, const font::common_data& r) noexcept;
    bool operator==(const font::page_data& l, const font::page_data& r) noexcept;
    bool operator==(const font::char_data& l, const font::char_data& r) noexcept;
    bool operator==(const font::kerning_data& l, const font::kerning_data& r) noexcept;
}

namespace e2d::fonts
{
    bool try_load_font(
        font& dst,
        const buffer& src) noexcept;

    bool try_load_font(
        font& dst,
        const input_stream_uptr& src) noexcept;
}
