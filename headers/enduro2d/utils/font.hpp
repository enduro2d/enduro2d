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

    using font_ptr = std::shared_ptr<font>;

    class font final {
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

        static font_ptr create(str_view content);
        char_data data (u32 charId) const noexcept;
        common_data common() const noexcept;

        i32 kerning (u32 first, u32 second) const noexcept;
    private:
        info_data info_;
        common_data common_;
        vector<str> pages_;
        vector<char_data> chars_;
        flat_map<u64,i32> kerning_;
    };
}
