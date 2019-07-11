/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

//#include "assets/texture_asset.hpp"

namespace e2d
{
    class bmfont;

    using bmfont_ptr = std::shared_ptr<bmfont>;

    class bad_bmfont_access final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad bmfont access";
        }
    };

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
            struct spacing_data {
                i32 horizontal;
                i32 vertical;
            };

            struct padding_data {
                i16 up;
                i16 right;
                i16 down;
                i16 left;
            };

//            str face;    //This is the name of the true type font.
            u32 size;    //The size of the true type font.
//            bool bold;    //The font is bold.
//            bool italic;    //The font is italic.
//            str charset;    //The name of the OEM charset used (when not unicode).
//            bool unicode;    //Set to 1 if it is the unicode charset.
//            u32 stretchH;    //The font height stretch in percentage. 100% means no stretch.
//            bool smooth;    //Set to 1 if smoothing was turned on.
//            u32 aa;    //The supersampling level used. 1 means no supersampling was used.
            padding_data padding;    //The padding for each character (up, right, down, left). TODOPA
//            spacing_data spacing;    //The spacing for each character (horizontal, vertical).
//            u32 outline;    //The outline thickness for the characters.
        };

        struct kerning_data {
            u32 first;  // The first character id.
            u32 second; // The second character id.
            i32 amount; // How much the x position should be adjusted when drawing the second character immediately following the first.
        };

        struct common_data {
            u32 lineHeight;    //This is the distance in pixels between each line of text.
//            u32 base;    //The number of pixels from the absolute top of the line to the base of the characters.
//            u32 scaleW;    //The width of the texture, normally used to scale the x pos of the character image.
//            u32 scaleH;    //The height of the texture, normally used to scale the y pos of the character image.
            u32 pages;    //The number of texture pages included in the font.
//            u32 packed;    //Set to 1 if the monochrome characters have been packed into each of the texture channels. In this case alphaChnl describes what is stored in each channel.
//            u32 alphaChnl;    //Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
//            u32 redChnl;    //Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
//            u32 greenChnl;    //Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
//            u32 blueChnl;    //Set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
        };

        bmfont() = default;
        ~bmfont() noexcept = default;

        static bmfont_ptr create (str_view content);

    protected:
        info_data info_;
        common_data common_;
        std::vector<std::string> pages_;
        std::vector<char_data> chars_;
        std::vector<kerning_data> kerning_;
    };
}
