/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class bmfont;

    using bmfont_ptr = std::shared_ptr<bmfont>;

    class bmfont final {
    public:
        struct char_data {
            u32 id;
            rect<i16> rect;
            i16 xoffset;
            i16 yoffset;
            i16 xadvance;
            i16 page;
            i16 chnl;
        };

        struct info_data {
            struct padding_data {
                i16 up;
                i16 right;
                i16 down;
                i16 left;
            };

            u32 size;
            padding_data padding;
        };

        struct kerning_data {
            u32 first;
            u32 second;
            i32 amount;
        };

        struct common_data {
            u32 line_height;
            u32 pages;
        };

        bmfont() = default;
        ~bmfont() noexcept = default;

        static bmfont_ptr create(str_view content);

    private:
        info_data info_;
        common_data common_;
        vector<str> pages_;
        vector<char_data> chars_;
        vector<kerning_data> kerning_;
    };
}
