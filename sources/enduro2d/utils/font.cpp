/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/utils/font.hpp>

namespace
{
    using namespace e2d;

    font::data load_font_data(str_view content) {
        str s(content);
        font::data data;

        std::replace(s.begin(), s.end(), '=', ' ');
        std::replace(s.begin(), s.end(), ',', ' ');

        str tag, line;
        std::stringstream data_stream(s);

        while ( !data_stream.eof() && std::getline(data_stream, line) ) {
            std::stringstream line_stream(line);
            line_stream.exceptions(std::ios::failbit | std::ios::badbit);

            line_stream >> std::ws;
            if ( line_stream.eof() ) {
                continue;
            }

            line_stream >> tag;
            if ( tag == "info" ) {
                // info face="Arial-Black" size=32 bold=0 italic=0 charset=""
                //      unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=2,2
                line_stream >> tag;
                while ( !line_stream.eof() ) {
                    if ( tag == "face" ) {
                        line_stream >> data.info.face;
                        // remove ""
                        data.info.face.erase(data.info.face.begin());
                        data.info.face.pop_back();
                    } else if ( tag == "size" ) {
                        line_stream >> data.info.size;
                    }
                    if ( !line_stream.eof() ) {
                        line_stream >> tag;
                    }
                }
            } else if ( tag == "common" ) {
                // common lineHeight=54 base=35 scaleW=512 scaleH=512 pages=1 packed=0
                line_stream >> tag;
                while ( !line_stream.eof() ) {
                    if ( tag == "lineHeight" ) {
                        line_stream >> data.common.line;
                    } else if ( tag == "base" ) {
                        line_stream >> data.common.base;
                    } else if ( tag == "pages" ) {
                        line_stream >> data.common.pages;
                        data.pages.reserve(data.common.pages);
                    } else if ( tag == "scaleW" ) {
                        line_stream >> data.common.atlas_size.x;
                    } else if ( tag == "scaleH" ) {
                        line_stream >> data.common.atlas_size.y;
                    }
                    if ( !line_stream.eof() ) {
                        line_stream >> tag;
                    }
                }
            } else if ( tag == "page" ) {
                // page id=0 file="arial.png"
                line_stream >> tag;
                font::page_data page_data;
                while ( !line_stream.eof() ) {
                    if ( tag == "id" ) {
                        line_stream >> page_data.id;
                    } else if ( tag == "file" ) {
                        line_stream >> page_data.file;
                        // remove ""
                        page_data.file.erase(page_data.file.begin());
                        page_data.file.pop_back();
                    }
                    if ( !line_stream.eof() ) {
                        line_stream >> tag;
                    }
                }
                data.pages.insert(std::move(page_data));
            } else if ( tag == "chars" ) {
                // chars count=95
                line_stream >> tag;
                while ( !line_stream.eof() ) {
                    if ( tag == "count" ) {
                        std::size_t count{0u};
                        line_stream >> count;
                        data.chars.reserve(count);
                    }
                    if ( !line_stream.eof() ) {
                        line_stream >> tag;
                    }
                }
            } else if ( tag == "char" ) {
                // char id=123 x=2 y=2 width=38 height=54
                //      xoffset=0 yoffset=-3 xadvance=12 page=0 chnl=0
                line_stream >> tag;
                font::char_data char_data;
                while ( !line_stream.eof() ) {
                    if ( tag == "id" ) {
                        line_stream >> char_data.id;
                    } else if ( tag == "x" ) {
                        line_stream >> char_data.rect.position.x;
                    } else if ( tag == "y" ) {
                        line_stream >> char_data.rect.position.y;
                    } else if ( tag == "width" ) {
                        line_stream >> char_data.rect.size.x;
                    } else if ( tag == "height" ) {
                        line_stream >> char_data.rect.size.y;
                        char_data.rect.position.y =
                            data.common.atlas_size.y -
                            char_data.rect.position.y -
                            char_data.rect.size.y;
                    } else if ( tag == "xoffset" ) {
                        line_stream >> char_data.offset.x;
                    } else if ( tag == "yoffset" ) {
                        line_stream >> char_data.offset.y;
                    } else if ( tag == "xadvance" ) {
                        line_stream >> char_data.advance;
                    } else if ( tag == "page" ) {
                        line_stream >> char_data.page;
                    } else if ( tag == "chnl" ) {
                        line_stream >> char_data.chnl;
                    }
                    if ( !line_stream.eof() ) {
                        line_stream >> tag;
                    }
                }
                data.chars.insert(std::move(char_data));
            } else if ( tag == "kernings" ) {
                // kernings count=243
                line_stream >> tag;
                while ( !line_stream.eof() ) {
                    if ( tag == "count" ) {
                        std::size_t count{0u};
                        line_stream >> count;
                        data.kernings.reserve(count);
                    }
                    if ( !line_stream.eof() ) {
                        line_stream >> tag;
                    }
                }
            } else if ( tag == "kerning" ) {
                // kerning first=86 second=101 amount=-2
                line_stream >> tag;
                font::kerning_data k;
                while ( !line_stream.eof() ) {
                    if ( tag == "first" ) {
                        line_stream >> k.chars.first;
                    } else if ( tag == "second" ) {
                        line_stream >> k.chars.second;
                    } else if ( tag == "amount" ) {
                        line_stream >> k.amount;
                    }
                    if ( !line_stream.eof() ) {
                        line_stream >> tag;
                    }
                }
                data.kernings.insert(std::move(k));
            }
        }

        return data;
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

    font::font(data&& data) noexcept {
        assign(std::move(data));
    }

    font::font(const data& data) {
        assign(data);
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
            data_ = other.data_;
        }
        return *this;
    }

    font& font::assign(data&& data) noexcept {
        data_ = std::move(data);
        return *this;
    }

    font& font::assign(const data& data) {
        data_ = data;
        return *this;
    }

    void font::swap(font& other) noexcept {
        using std::swap;
        swap(data_.info, other.data_.info);
        swap(data_.common, other.data_.common);
        swap(data_.pages, other.data_.pages);
        swap(data_.chars, other.data_.chars);
        swap(data_.kernings, other.data_.kernings);
    }

    void font::clear() noexcept {
        data_ = data();
    }

    bool font::empty() const noexcept {
        return data_.pages.empty();
    }

    const font::info_data& font::info() const noexcept {
        return data_.info;
    }

    const font::common_data& font::common() const noexcept {
        return data_.common;
    }

    const flat_set<font::page_data>& font::pages() const noexcept {
        return data_.pages;
    }

    const flat_set<font::char_data>& font::chars() const noexcept {
        return data_.chars;
    }

    const flat_set<font::kerning_data>& font::kernings() const noexcept {
        return data_.kernings;
    }

    const font::page_data* font::find_page(u32 id) const noexcept {
        const auto iter = data_.pages.find(id);
        return iter != data_.pages.end()
            ? &*iter
            : nullptr;
    }

    const font::char_data* font::find_char(u32 id) const noexcept {
        const auto iter = data_.chars.find(id);
        return iter != data_.chars.end()
            ? &*iter
            : nullptr;
    }

    i32 font::get_kerning(u32 first, u32 second) const noexcept {
        const auto iter = data_.kernings.find(std::make_pair(first, second));
        return iter != data_.kernings.end()
            ? iter->amount
            : 0;
    }
}

namespace e2d
{
    void swap(font& l, font& r) noexcept {
        l.swap(r);
    }

    bool operator==(const font& l, const font& r) noexcept {
        return l.info() == r.info()
            && l.common() == r.common()
            && l.pages() == r.pages()
            && l.chars() == r.chars()
            && l.kernings() == r.kernings();
    }

    bool operator!=(const font& l, const font& r) noexcept {
        return !(l == r);
    }

    bool operator<(u32 l, const font::page_data& r) noexcept {
        return l < r.id;
    }

    bool operator<(const font::page_data& l, u32 r) noexcept {
        return l.id < r;
    }

    bool operator<(const font::page_data& l, const font::page_data& r) noexcept {
        return l.id < r.id;
    }

    bool operator<(u32 l, const font::char_data& r) noexcept {
        return l < r.id;
    }

    bool operator<(const font::char_data& l, u32 r) noexcept {
        return l.id < r;
    }

    bool operator<(const font::char_data& l, const font::char_data& r) noexcept {
        return l.id < r.id;
    }

    bool operator<(const std::pair<u32,u32>& l, const font::kerning_data& r) noexcept {
        return l < r.chars;
    }

    bool operator<(const font::kerning_data& l, const std::pair<u32,u32>& r) noexcept {
        return l.chars < r;
    }

    bool operator<(const font::kerning_data& l, const font::kerning_data& r) noexcept {
        return l.chars < r.chars;
    }

    bool operator==(const font::info_data& l, const font::info_data& r) noexcept {
        return l.face == r.face
            && l.size == r.size;
    }

    bool operator==(const font::common_data& l, const font::common_data& r) noexcept {
        return l.line == r.line
            && l.base == r.base
            && l.pages == r.pages
            && l.atlas_size == r.atlas_size;
    }

    bool operator==(const font::page_data& l, const font::page_data& r) noexcept {
        return l.id == r.id
            && l.file == r.file;
    }

    bool operator==(const font::char_data& l, const font::char_data& r) noexcept {
        return l.id == r.id
            && l.rect == r.rect
            && l.offset == r.offset
            && l.advance == r.advance
            && l.page == r.page
            && l.chnl == r.chnl;
    }

    bool operator==(const font::kerning_data& l, const font::kerning_data& r) noexcept {
        return l.chars == r.chars
            && l.amount == r.amount;
    }
}

namespace e2d::fonts
{
    bool try_load_font(font& dst, const buffer& src) noexcept {
        try {
            dst = load_font_data(str_view{
                reinterpret_cast<const char*>(src.data()),
                src.size()});
            return true;
        } catch (...) {
            return false;
        }
    }

    bool try_load_font(font& dst, const input_stream_uptr& src) noexcept {
        buffer file_data;
        return streams::try_read_tail(file_data, src)
            && try_load_font(dst, file_data);
    }
}
