/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/bmfont.hpp>
#include <sstream>

namespace e2d
{
    bmfont_ptr bmfont::create (str_view content) {
        bmfont_ptr b = std::make_shared<bmfont>();
        str s{content.data()};
        std::replace(s.begin(), s.end(), '=', ' ');
        std::replace(s.begin(), s.end(), ',', ' ');

        str tag;
        str line;
        std::stringstream data_stream(s);
        vector<char_data> chars(120);
        vector<kerning_data> kerning(120);
        int chars_counter = 0;
        int kerning_counter = 0;
        while (getline(data_stream, line, '\n'))
        {
            std::stringstream line_stream(line);
            line_stream >> tag;
            if (tag == "info") {
                //info face="Arial-Black" size=32 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=2,2
                line_stream >> tag;
                while (!line_stream.eof()) {
                    if (tag == "size") {
                        line_stream >>  b->info_.size;
                    } else if (tag == "padding") {
                        line_stream >>  b->info_.padding.up;
                        line_stream >>  b->info_.padding.right;
                        line_stream >>  b->info_.padding.down;
                        line_stream >>  b->info_.padding.left;
                    }
                    line_stream >> tag;
                }
            } else if (tag == "common") {
                //common lineHeight=54 base=35 scaleW=512 scaleH=512 pages=1 packed=0
                line_stream >> tag;
                while (!line_stream.eof()) {
                    if (tag == "lineHeight") {
                        line_stream >>  b->common_.line_height;
                    } else if (tag == "pages") {
                        line_stream >>  b->common_.pages;
                         b->pages_.resize( b->common_.pages);
                    }
                    line_stream >> tag;
                }
            } else if (tag == "page") {
                //page id=0 file="boundsTestFont.png"
                line_stream >> tag;
                i32 id{-1};
                while (!line_stream.eof()) {
                    if (tag == "id") {
                        line_stream >> id;
                    } else if (tag == "file") {
                        str file;
                        line_stream >> file;
                        //remove ""
                        file.erase(file.begin());
                        file.pop_back();
                        b->pages_[id] = file;
                    }
                    line_stream >> tag;
                }
            } else if (tag == "chars") {
                //chars count=95
                //this field may not exist!
                while (!line_stream.eof()) {
                    if (tag == "count") {
                        size_t count;
                        line_stream >> count;
                        if ( count > chars.size() ) {
                            chars.resize(count);
                        }
                    }
                    line_stream >> tag;
                }
            } else if (tag == "char") {
                //char id=123 x=2 y=2 width=38 height=54 xoffset=0 yoffset=-3 xadvance=12 page=0 chnl=0 letter="{"
                line_stream >> tag;
                char_data& c = chars[chars_counter++];
                while (!line_stream.eof()) {
                    if (tag == "id") {
                        line_stream >> c.id;
                    } else if (tag == "x") {
                        line_stream >> c.rect.position.x;
                    } else if (tag == "y") {
                        line_stream >> c.rect.position.y;
                    } else if (tag == "widht") {
                        line_stream >> c.rect.size.x;
                    } else if (tag == "height") {
                        line_stream >> c.rect.size.y;
                        c.rect.position.y -= c.rect.size.y;
                    } else if (tag == "xoffset") {
                        line_stream >> c.xoffset;
                    } else if (tag == "yoffset") {
                        line_stream >> c.yoffset;
                    } else if (tag == "xadvance") {
                        line_stream >> c.xadvance;
                    } else if (tag == "page") {
                        line_stream >> c.page;
                    }
                    line_stream >> tag;
                }
            } else if (tag == "kernings") {
                //kernings count=243
                //this field may not exist!
                while (!line_stream.eof()) {
                    if (tag == "count") {
                        size_t count;
                        line_stream >> count;
                        if ( count > kerning.size() ) {
                            kerning.resize(count);
                        }
                    }
                    line_stream >> tag;
                }
            } else if (tag == "kerning") {
                //kerning first=86 second=101 amount=-2
                line_stream >> tag;
                kerning_data& k = kerning[kerning_counter++];
                while (!line_stream.eof()) {
                    if (tag == "first") {
                        line_stream >> k.first;
                    } else if (tag == "second") {
                        line_stream >> k.second;
                    } else if (tag == "amount") {
                        line_stream >> k.amount;
                    }
                    line_stream >> tag;
                }
            }
        }

         b->chars_.insert(b->chars_.begin(), chars.begin(), chars.begin()+chars_counter);
         b->kerning_.insert(b->kerning_.begin(), kerning.begin(), kerning.begin()+kerning_counter);

        return b;
    }
}
