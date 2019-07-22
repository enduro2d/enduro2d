/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    class font;

    class font final {
    public:
        struct char_data {
            u32 id{0};
            b2hi rect;
            i16 xoffset{0};
            i16 yoffset{0};
            i16 xadvance{0};
            i16 page{0};
            i16 chnl{0};
        };

        struct info_data {
            u32 size{0};
        };

        struct kerning_data {
            u64 first{0};
            u32 second{0};
            i32 amount{0};
        };

        struct common_data {
            u32 line_height{0};
            u32 base{0};
            u32 pages{0};
            u32 atlas_width{0};
            u32 atlas_height{0};
        };

        font() = default;
        ~font() noexcept = default;

        font(font&& other) noexcept;
        font& operator=(font&& other) noexcept;

        font(const font& other);
        font& operator=(const font& other);

        font(
             info_data&& info,
             common_data&& common,
             vector<str>&& pages,
             vector<char_data>&& chars,
             flat_map<u64,i32>&& kerning) noexcept;

        font(
             const info_data& info,
             const common_data& common,
             const vector<str>& pages,
             const vector<char_data>& chars,
             const flat_map<u64,i32>& kerning);

        font& assign(font&& other) noexcept;
        font& assign(const font& other);

        font& assign(
             info_data&& info,
             common_data&& common,
             vector<str>&& pages,
             vector<char_data>&& chars,
             flat_map<u64,i32>&& kerning) noexcept;

        font& assign(
             const info_data& info,
             const common_data& common,
             const vector<str>& pages,
             const vector<char_data>& chars,
             const flat_map<u64,i32>& kerning);

        void swap(font& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        const info_data& info() const noexcept;
        const common_data& common() const noexcept;
        const vector<str>& pages() const noexcept;
        const vector<char_data>& chars() const noexcept;
        const flat_map<u64,i32>& kerning() const noexcept;

        static font create(str_view content);
        const char_data* find_char(u32 charId) const noexcept;

        i32 find_kerning(u32 first, u32 second) const noexcept;
    private:
        info_data info_;
        common_data common_;
        vector<str> pages_;
        vector<char_data> chars_;
        flat_map<u64,i32> kerning_;
    };

    bool operator==(const font::info_data& l, const font::info_data& r) noexcept;
    bool operator==(const font::common_data& l, const font::common_data& r) noexcept;
    bool operator==(const font::char_data& l, const font::char_data& r) noexcept;

    void swap(font& l, font& r) noexcept;
    bool operator==(const font& l, const font& r) noexcept;
    bool operator!=(const font& l, const font& r) noexcept;
}
