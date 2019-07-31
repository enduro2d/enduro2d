/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/utils/font.hpp>

namespace
{
    using namespace e2d;

    class bmfont_loading_exception : public exception {
        const char* what() const noexcept override {
            return "bmfont loading exception";
        }
    };

    str read_tag(const str& buf, u32& pos) {
        str res;
        auto end = buf.find(' ', pos);
        if ( end == str::npos ) {
            throw bmfont_loading_exception();
        } else {
            res = buf.substr(pos, end - pos);
            pos = end;
        }

        return res;
    }

    bool read_key(const str& buf, u32& pos, str& key) {
        auto end = buf.find('=', pos);
        if ( end != str::npos ) {
            auto start = buf.rfind(' ', end);
            if ( start != str::npos ) {
                start++;
                key = buf.substr(start, end - start);
                pos = end + 1;
                return true;
            }
        }

        return false;
    }

    i32 read_int(const str& buf, u32& pos) {
        i32 res;
        auto end = buf.find(' ', pos);
        if ( end == str::npos ) {
            end = buf.find('\n', pos);
        }

        if ( end == str::npos ) {
            throw bmfont_loading_exception();
        } else {
            res = std::stoi(buf.substr(pos, end - pos));
            pos = end;
        }

        return res;
    }

    str read_string(const str& buf, u32& pos) {
        str res;
        auto start = buf.find('"', pos);
        if ( start == str::npos ) {
            throw bmfont_loading_exception();
        } else {
            start++;
            auto end = buf.find('"', start);
            if ( end == str::npos ) {
                throw bmfont_loading_exception();
            } else {
                res = buf.substr(start, end - start);
                pos = end + 1;
            }
        }

        return res;
    }

    font::data load_font_data(str_view content) {
        str s(content);
        u32 pos{0};
        str line;
        str tag;
        str key;
        font::data data;
        vector<font::char_data> chars;
        vector<font::kerning_data> kernings;

        chars.reserve(120);
        kernings.reserve(120);
        size_t start_line{0};
        size_t end_line = s.find('\n', start_line);
        while ( end_line != str::npos ) {
            pos = 0;
            line = s.substr(start_line, end_line - start_line + 1);
            line[line.size()-1] = '\n';
            tag = read_tag(line, pos);
            if ( tag == "info" ) {
                // info face="Arial-Black" size=32 bold=0 italic=0 charset=""
                //      unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=2,2
                while ( read_key(line, pos, key) ) {
                    if ( key == "face" ) {
                        data.info.face = read_string(line, pos);
                    } else if ( key == "size" ) {
                        data.info.size = read_int(line, pos);
                    }
                }
            } else if ( tag == "common" ) {
                // common lineHeight=54 base=35 scaleW=512 scaleH=512 pages=1 packed=0
                while ( read_key(line, pos, key) ) {
                    if ( key == "lineHeight" ) {
                        data.common.line = read_int(line, pos);
                    } else if ( key == "base" ) {
                        data.common.base = read_int(line, pos);
                    } else if ( key == "scaleW" ) {
                        data.common.atlas_size.x = read_int(line, pos);
                    } else if ( key == "scaleH" ) {
                        data.common.atlas_size.y = read_int(line, pos);
                    } else if ( key == "pages" ) {
                        data.common.pages = read_int(line, pos);
                    }
                }
            } else if ( tag == "page" ) {
                // page id=0 file="arial.png"
                font::page_data page_data;
                while ( read_key(line, pos, key) ) {
                    if ( key == "id" ) {
                        page_data.id = read_int(line, pos);
                    } else if ( key == "file" ) {
                        page_data.file = read_string(line, pos);
                    }
                }
                data.pages.insert(std::move(page_data));
            } else if ( tag == "chars" ) {
                // chars count=95
                while ( read_key(line, pos, key) ) {
                    if ( key == "count" ) {
                        chars.reserve(read_int(line, pos));
                    }
                }
            } else if ( tag == "char" ) {
                // char id=123 x=2 y=2 width=38 height=54
                //      xoffset=0 yoffset=-3 xadvance=12 page=0 chnl=0
                font::char_data c;
                while ( read_key(line, pos, key) ) {
                    if ( key == "id" ) {
                        c.id = read_int(line, pos);
                    } else if ( key == "x" ) {
                        c.rect.position.x = read_int(line, pos);
                    } else if ( key == "y" ) {
                        c.rect.position.y = read_int(line, pos);
                    } else if ( key == "width" ) {
                        c.rect.size.x = read_int(line, pos);
                    } else if ( key == "height" ) {
                        c.rect.size.y = read_int(line, pos);
                        c.rect.position.y =
                            data.common.atlas_size.y -
                            c.rect.position.y -
                            c.rect.size.y;
                    } else if ( key == "xoffset" ) {
                        c.offset.x = read_int(line, pos);
                    } else if ( key == "yoffset" ) {
                        c.offset.y = read_int(line, pos);
                    } else if ( key == "xadvance" ) {
                        c.advance = read_int(line, pos);
                    } else if ( key == "page" ) {
                        c.page = read_int(line, pos);
                    } else if ( key == "chnl" ) {
                        c.chnl = read_int(line, pos);
                    }
                }
                chars.push_back(std::move(c));
            } else if ( tag == "kernings" ) {
                while ( read_key(line, pos, key) ) {
                    if ( key == "count" ) {
                        kernings.reserve(read_int(line, pos));
                    }
                }
            } else if ( tag == "kerning" ) {
                font::kerning_data k;
                while ( read_key(line, pos, key) ) {
                    if ( key == "first" ) {
                        k.chars.first = read_int(line, pos);
                    } else if ( key == "second" ) {
                        k.chars.second = read_int(line, pos);
                    } else if ( key == "amount" ) {
                        k.amount = read_int(line, pos);
                    }
                }
                kernings.push_back(std::move(k));
            }

            start_line = end_line + 1;
            end_line = s.find('\n', start_line);
        }

        data.chars.reserve(chars.size());
        for ( size_t i = 0; i < chars.size(); i++ ) {
            data.chars.insert(std::make_pair(chars[i].id, chars[i]));
        }

        data.kernings.reserve(kernings.size());
        for ( size_t i = 0; i < kernings.size(); i++ ) {
            data.kernings.insert(std::make_pair(kernings[i].chars, kernings[i].amount));
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

    const flat_map<u32, font::char_data>& font::chars() const noexcept {
        return data_.chars;
    }

    const flat_map<std::pair<u32, u32>, i32>& font::kernings() const noexcept {
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
            ? &iter->second
            : nullptr;
    }

    i32 font::get_kerning(u32 first, u32 second) const noexcept {
        const auto iter = data_.kernings.find(std::make_pair(first, second));
        return iter != data_.kernings.end()
            ? iter->second
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
