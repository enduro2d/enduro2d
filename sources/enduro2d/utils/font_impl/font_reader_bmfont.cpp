/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "font_impl.hpp"

namespace
{
    using namespace e2d;

    bool read_line(str_view content, std::size_t& pos, str_view& line) {
        if ( pos > content.size() ) {
            return false;
        }

        auto end_of_line = content.find('\n', pos);
        if ( end_of_line == str_view::npos ) {
            end_of_line = content.size();
        }

        line = content.substr(pos, end_of_line - pos);
        if ( !line.empty() && line.back() == '\r' ) {
            line.remove_suffix(1u);
        }

        pos = end_of_line + 1u;
        return true;
    }

    bool read_tag(str_view line, std::size_t& pos, str_view& tag) {
        const auto start = line.find_first_not_of(' ', pos);
        if ( start == str_view::npos ) {
            return false;
        }
        const auto end = line.find(' ', start);
        if ( end == str_view::npos || end == line.size() ) {
            return false;
        }
        tag = line.substr(start, end - start);
        pos = end + 1;
        return true;
    }

    bool read_key(str_view line, std::size_t& pos, str_view& key) {
        const auto eq = line.find('=', pos);
        if ( eq == str_view::npos || eq == 0u || eq == line.size() ) {
            return false;
        }
        const auto last = line.find_last_not_of(' ', eq - 1u);
        if ( last == str_view::npos ) {
            return false;
        }
        const auto prestart = line.rfind(' ', last);
        const auto start = prestart == str_view::npos
            ? 0u
            : prestart + 1u;
        key = line.substr(start, last - prestart);
        pos = eq + 1;
        return true;
    }

    template < typename T >
    bool read_value(str_view line, std::size_t& pos, T& value) {
        const auto start = line.find_first_not_of(' ', pos);
        if ( start == str_view::npos ) {
            return false;
        }
        auto end = line.find(' ', start);
        if ( end == str_view::npos ) {
            end = line.size();
        }
        if ( !strings::try_parse(line.substr(start, end - start), value) ) {
            return false;
        }
        pos = end + 1;
        return true;
    }

    bool read_value(str_view line, std::size_t& pos, str& value) {
        const auto start = line.find("\"", pos);
        if ( start == str_view::npos || start == line.size() ) {
            return false;
        }
        const auto end = line.find("\"", start + 1u);
        if ( end == str_view::npos ) {
            return false;
        }
        value = line.substr(start + 1, end - start - 1u);
        pos = end + 1;
        return true;
    }

    // info face="Arial-Black" size=32 bold=0 italic=0 charset=""
    //      unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=2,2
    bool parse_info_line(str_view line, font::content& font_content) {
        str_view key;
        std::size_t pos = 0u;
        while ( read_key(line, pos, key) ) {
            if ( key == "size" ) {
                if ( !read_value(line, pos, font_content.info.font_size) ) {
                    return false;
                }
            }
        }
        return true;
    }

    // common lineHeight=54 base=35 scaleW=512 scaleH=512 pages=1 packed=0
    bool parse_common_line(str_view line, font::content& font_content) {
        str_view key;
        std::size_t pos = 0u;
        while ( read_key(line, pos, key) ) {
            if ( key == "lineHeight" ) {
                if ( !read_value(line, pos, font_content.info.line_height) ) {
                    return false;
                }
            } else if ( key == "base" ) {
                if ( !read_value(line, pos, font_content.info.glyph_ascent) ) {
                    return false;
                }
            } else if ( key == "scaleW" ) {
                if ( !read_value(line, pos, font_content.info.atlas_size.x) ) {
                    return false;
                }
            } else if ( key == "scaleH" ) {
                if ( !read_value(line, pos, font_content.info.atlas_size.y) ) {
                    return false;
                }
            } else if ( key == "pages" ) {
                u32 pages = 0u;
                if ( !read_value(line, pos, pages) || pages > 1u ) {
                    return false;
                }
            }
        }
        return true;
    }

    // page id=0 file="arial.png"
    bool parse_page_line(str_view line, font::content& font_content) {
        str_view key;
        std::size_t pos = 0u;
        while ( read_key(line, pos, key) ) {
            if ( key == "id" ) {
                u32 id = 0u;
                if ( !read_value(line, pos, id) || id > 0 ) {
                    return false;
                }
            } else if ( key == "file" ) {
                if ( !read_value(line, pos, font_content.info.atlas_file) ) {
                    return false;
                }
            }
        }
        return true;
    }

    // chars count=95
    bool parse_chars_line(str_view line, font::content& font_content) {
        str_view key;
        std::size_t pos = 0u;
        while ( read_key(line, pos, key) ) {
            if ( key == "count" ) {
                std::size_t count = 0u;
                if ( !read_value(line, pos, count) ) {
                    return false;
                }
                font_content.glyphs.reserve(count);
            }
        }
        return true;
    }

    // char id=123 x=2 y=2 width=38 height=54
    //      xoffset=0 yoffset=-3 xadvance=12 page=0 chnl=0
    bool parse_char_line(str_view line, font::content& font_content) {
        u32 code_point = 0;
        font::glyph_info glyph;

        str_view key;
        std::size_t pos = 0u;
        while ( read_key(line, pos, key) ) {
            if ( key == "id" ) {
                if ( !read_value(line, pos, code_point) ) {
                    return false;
                }
            } else if ( key == "x" ) {
                if ( !read_value(line, pos, glyph.tex_rect.position.x) ) {
                    return false;
                }
            } else if ( key == "y" ) {
                if ( !read_value(line, pos, glyph.tex_rect.position.y) ) {
                    return false;
                }
            } else if ( key == "width" ) {
                if ( !read_value(line, pos, glyph.tex_rect.size.x) ) {
                    return false;
                }
            } else if ( key == "height" ) {
                if ( !read_value(line, pos, glyph.tex_rect.size.y) ) {
                    return false;
                }
            } else if ( key == "xoffset" ) {
                if ( !read_value(line, pos, glyph.offset.x) ) {
                    return false;
                }
            } else if ( key == "yoffset" ) {
                if ( !read_value(line, pos, glyph.offset.y) ) {
                    return false;
                }
            } else if ( key == "xadvance" ) {
                if ( !read_value(line, pos, glyph.advance) ) {
                    return false;
                }
            } else if ( key == "page" ) {
                u32 page = 0;
                if ( !read_value(line, pos, page) || page > 0 ) {
                    return false;
                }
            }
        }

        font_content.glyphs.insert_or_assign(
            code_point,
            std::move(glyph));

        return true;
    }

    // kernings count=343
    bool parse_kernings_line(str_view line, font::content& font_content) {
        str_view key;
        std::size_t pos = 0u;
        while ( read_key(line, pos, key) ) {
            if ( key == "count" ) {
                std::size_t count = 0u;
                if ( !read_value(line, pos, count) ) {
                    return false;
                }
                font_content.kernings.reserve(count);
            }
        }
        return true;
    }

    // kerning first=1043 second=1071 amount=-1
    bool parse_kerning_line(str_view line, font::content& font_content) {
        u32 first = 0;
        u32 second = 0;
        i32 amount = 0;

        str_view key;
        std::size_t pos = 0u;
        while ( read_key(line, pos, key) ) {
            if ( key == "first" ) {
                if ( !read_value(line, pos, first) ) {
                    return false;
                }
            } else if ( key == "second" ) {
                if ( !read_value(line, pos, second) ) {
                    return false;
                }
            } else if ( key == "amount" ) {
                if ( !read_value(line, pos, amount) ) {
                    return false;
                }
            }
        }

        font_content.kernings.insert_or_assign(
            (static_cast<u64>(first) << 32) | static_cast<u64>(second),
            amount);

        return true;
    }
}

namespace e2d::fonts::impl
{
    bool load_font_bmfont(font& dst, buffer_view src) {
        const auto text_src = str_view{
            reinterpret_cast<const char*>(src.data()),
            src.size()};

        font::content font_content;

        str_view line;
        std::size_t pos = 0u;
        while ( read_line(text_src, pos, line) ) {
            str_view tag;
            std::size_t tag_pos = 0u;
            if ( !read_tag(line, tag_pos, tag) ) {
                continue;
            }

            line.remove_prefix(tag_pos);

            if ( tag == "info" ) {
                if ( !parse_info_line(line, font_content) ) {
                    return false;
                }
            } else if ( tag == "common" ) {
                if ( !parse_common_line(line, font_content) ) {
                    return false;
                }
            } else if ( tag == "page" ) {
                if ( !parse_page_line(line, font_content) ) {
                    return false;
                }
            } else if ( tag == "chars" ) {
                if ( !parse_chars_line(line, font_content) ) {
                    return false;
                }
            } else if ( tag == "char" ) {
                if ( !parse_char_line(line, font_content) ) {
                    return false;
                }
            } else if ( tag == "kernings" ) {
                if ( !parse_kernings_line(line, font_content) ) {
                    return false;
                }
            } else if ( tag == "kerning" ) {
                if ( !parse_kerning_line(line, font_content) ) {
                    return false;
                }
            }
        }

        for ( auto& glyph : font_content.glyphs ) {
            glyph.second.offset.y =
                0 -
                glyph.second.offset.y -
                glyph.second.tex_rect.size.y;
            glyph.second.tex_rect.position.y =
                font_content.info.atlas_size.y -
                glyph.second.tex_rect.position.y -
                glyph.second.tex_rect.size.y;
        }

        bool info_valid = !font_content.info.atlas_file.empty()
            && font_content.info.atlas_size.x > 0u
            && font_content.info.atlas_size.y > 0u
            && font_content.info.font_size > 0u
            && font_content.info.line_height > 0u
            && font_content.info.font_size > 0u
            && font_content.info.glyph_ascent > 0u;

        bool glyphs_valid = true;
        for ( const auto& glyph : font_content.glyphs ) {
            const v2u& mst = glyph.second.tex_rect.position + glyph.second.tex_rect.size;
            glyphs_valid = glyphs_valid && math::inside(b2u(font_content.info.atlas_size), mst);
        }

        if ( !info_valid || !glyphs_valid ) {
            return false;
        }

        dst = font(font_content);
        return true;
    }
}
