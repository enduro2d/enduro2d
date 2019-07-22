/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/utils/font.hpp>
#include <sstream>

namespace
{
    using namespace e2d;

    u64 make_kerning_key(u32 first, u32 second) noexcept {
        return (static_cast<u64>(first) << 32) | static_cast<u64>(second);
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

    font::font(
        font::info_data&& info,
        font::common_data&& common,
        vector<str>&& pages,
        vector<font::char_data>&& chars,
        flat_map<u64,i32>&& kerning) noexcept {
        assign(info, common, pages, chars, kerning);
    }

    font::font(
        const font::info_data& info,
        const font::common_data& common,
        const vector<str>& pages,
        const vector<font::char_data>& chars,
        const flat_map<u64,i32>& kerning) {
        assign(info, common, pages, chars, kerning);
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
            info_ = other.info_;
            common_ = other.common_;
            pages_ = other.pages_;
            chars_ = other.chars_;
            kerning_ = other.kerning_;
        }
        return *this;
    }

    font& font::assign(
        font::info_data&& info,
        font::common_data&& common,
        vector<str>&& pages,
        vector<font::char_data>&& chars,
        flat_map<u64,i32>&& kerning) noexcept {
        info_ = std::move(info);
        common_ = std::move(common);
        pages_ = std::move(pages);
        chars_ = std::move(chars);
        kerning_ = std::move(kerning);
        return *this;
    }

    font& font::assign(
        const font::info_data& info,
        const font::common_data& common,
        const vector<str>& pages,
        const vector<font::char_data>& chars,
        const flat_map<u64,i32>& kerning) {
        info_ = info;
        common_ = common;
        pages_ = pages;
        chars_ = chars;
        kerning_ = kerning;
        return *this;
    }

    void font::swap(font& other) noexcept {
        using std::swap;
        swap(info_, other.info_);
        swap(common_, other.common_);
        swap(pages_, other.pages_);
        swap(chars_, other.chars_);
        swap(kerning_, other.kerning_);
    }

    void font::clear() noexcept {
        pages_.clear();
        chars_.clear();
        kerning_.clear();
    }

    bool font::empty() const noexcept {
        return pages_.empty();
    }

    const font::info_data& font::info() const noexcept {
        return info_;
    }

    const font::common_data& font::common() const noexcept {
        return common_;
    }

    const vector<str>& font::pages() const noexcept {
        return pages_;
    }

    const vector<font::char_data>& font::chars() const noexcept {
        return chars_;
    }

    const flat_map<u64,i32>& font::kerning() const noexcept {
        return kerning_;
    }

    font font::create(str_view content) {
        font b;
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
                        line_stream >>  b.info_.size;
                    }
                    line_stream >> tag;
                }
            } else if (tag == "common") {
                //common lineHeight=54 base=35 scaleW=512 scaleH=512 pages=1 packed=0
                line_stream >> tag;
                while (!line_stream.eof()) {
                    if (tag == "lineHeight") {
                        line_stream >>  b.common_.line_height;
                    } else if (tag == "base") {
                        line_stream >>  b.common_.base;
                    } else if (tag == "pages") {
                        line_stream >>  b.common_.pages;
                         b.pages_.resize( b.common_.pages);
                    } else if (tag == "scaleW") {
                        line_stream >>  b.common_.atlas_width;
                    } else if (tag == "scaleH") {
                        line_stream >>  b.common_.atlas_height;
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
                        b.pages_[id] = file;
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
                    } else if (tag == "width") {
                        line_stream >> c.rect.size.x;
                    } else if (tag == "height") {
                        line_stream >> c.rect.size.y;
                        c.rect.position.y = b.common_.atlas_height - c.rect.position.y - c.rect.size.y;
                    } else if (tag == "xoffset") {
                        line_stream >> c.xoffset;
                    } else if (tag == "yoffset") {
                        line_stream >> c.yoffset;
                        c.yoffset = b.common_.base - c.yoffset - c.rect.size.y;
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

        b.chars_.insert(b.chars_.begin(), chars.begin(), chars.begin()+chars_counter);
        b.kerning_.reserve(kerning_counter);
        for ( int i = 0; i < kerning_counter; i++ ) {
            b.kerning_.insert_or_assign(make_kerning_key(kerning[i].first,kerning[i].second),
                                         kerning[i].amount);
        }

        return b;
    }

    const font::char_data* font::find_char(u32 charId) const noexcept {
        for ( auto& c: chars_ ) {
            if (c.id == charId) {
                return &c;
            }
        }

        return 0;
    }

    i32 font::find_kerning(u32 first, u32 second) const noexcept {
        auto it = kerning_.find(make_kerning_key(first,second));
        if (it != kerning_.end()) {
            return it->second;
        }

        return 0;
    }
}


namespace e2d
{
    bool operator==(const font::info_data& l, const font::info_data& r) noexcept {
        return l.size == r.size;
    }

    bool operator==(const font::common_data& l, const font::common_data& r) noexcept {
        return l.line_height == r.line_height
        && l.base == r.base
        && l.pages == r.pages
        && l.atlas_width == r.atlas_width
        && l.atlas_height == r.atlas_height;
    }

    bool operator==(const font::char_data& l, const font::char_data& r) noexcept {
        return l.id == r.id
        && l.rect == r.rect
        && l.xoffset == r.xoffset
        && l.yoffset == r.yoffset
        && l.xadvance == r.xadvance
        && l.page == r.page
        && l.chnl == r.chnl;
    }


    void swap(font& l, font& r) noexcept {
        l.swap(r);
    }

    bool operator==(const font& l, const font& r) noexcept {
        return l.info() == r.info()
        && l.common() == r.common()
        && l.pages() == r.pages()
        && l.chars() == r.chars()
        && l.kerning() == r.kerning();
    }

    bool operator!=(const font& l, const font& r) noexcept {
        return !(l == r);
    }
}
