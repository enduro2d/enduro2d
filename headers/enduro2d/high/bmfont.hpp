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
            u32 id{0};
            rect<i16> rect;
            i16 xoffset{0};
            i16 yoffset{0};
            i16 xadvance{0};
            i16 page{0};
            i16 chnl{0};
        };

        struct info_data {
            struct padding_data {
                i16 up{0};
                i16 right{0};
                i16 down{0};
                i16 left{0};
            };

            u32 size{0};
            padding_data padding;
        };

        struct kerning_data {
            u32 first{0};
            u32 second{0};
            i32 amount{0};
        };

        struct common_data {
            u32 line_height{0};
            u32 pages{0};
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
