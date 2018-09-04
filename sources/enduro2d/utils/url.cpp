/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/utils/url.hpp>
#include <enduro2d/utils/path.hpp>

namespace
{
    using namespace e2d;

    const str_view scheme_separator = "://";

    str str_view_concat(str_view v1, str_view v2) {
        str result;
        result.reserve(v1.size() + v2.size());
        result.append(v1.cbegin(), v1.cend());
        result.append(v2.cbegin(), v2.cend());
        return result;
    }

    str_view::size_type str_view_search(
        str_view str, str_view pattern, str_view::size_type index) noexcept
    {
        if ( str.empty() ) {
            return index;
        }
        if ( index > str.size() ) {
            return str_view::npos;
        }
        str_view::const_iterator iter = std::search(
             str.cbegin() + index, str.cend(),
             pattern.cbegin(), pattern.cend());
        return iter != str.cend()
            ? math::numeric_cast<std::size_t>(std::distance(str.cbegin(), iter))
            : str_view::npos;
    }

    std::pair<str, str> separate_schemepath(str_view schemepath) {
        const str_view::size_type sep_pos =
            str_view_search(schemepath, scheme_separator, 0);
        if ( str_view::npos == sep_pos ) {
            return std::make_pair(str(), schemepath);
        }
        return std::make_pair(
            schemepath.substr(0, sep_pos),
            schemepath.substr(sep_pos + scheme_separator.length()));
    }
}

namespace e2d
{
    url::url(url&& other) noexcept {
        assign(std::move(other));
    }

    url& url::operator=(url&& other) noexcept {
        return assign(std::move(other));
    }

    url::url(const url& other) {
        assign(other);
    }

    url& url::operator=(const url& other) {
        return assign(other);
    }

    url::url(str_view schemepath) {
        assign(schemepath);
    }

    url::url(str_view scheme, str_view path) noexcept {
        assign(scheme, path);
    }

    url& url::assign(url&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    url& url::assign(const url& other) {
        return this != &other
            ? assign(other.scheme_, other.path_)
            : *this;
    }

    url& url::assign(str_view schemepath) {
        str nscheme, npath;
        std::tie(nscheme, npath) = separate_schemepath(schemepath);
        return assign(std::move(nscheme), std::move(npath));
    }

    url& url::assign(str_view scheme, str_view path) noexcept {
        scheme_ = scheme;
        path_ = path;
        return *this;
    }

    url& url::concat(str_view path) {
        path_.append(path.cbegin(), path.cend());
        return *this;
    }

    url& url::append(str_view path) {
        path_ = path::combine(path_, path);
        return *this;
    }

    void url::swap(url& other) noexcept {
        using std::swap;
        swap(scheme_, other.scheme_);
        swap(path_, other.path_);
    }

    void url::clear() noexcept {
        scheme_.clear();
        path_.clear();
    }

    bool url::empty() const noexcept {
        return path_.empty();
    }

    const str& url::scheme() const noexcept {
        return scheme_;
    }

    const str& url::path() const noexcept {
        return path_;
    }

    url& url::operator+=(str_view path) {
        return concat(path);
    }

    url& url::operator/=(str_view path) {
        return append(path);
    }
}

namespace e2d
{
    void swap(url& l, url& r) noexcept {
        l.swap(r);
    }

    bool operator==(const url& l, const url& r) noexcept {
        return
            l.scheme() == r.scheme() &&
            l.path() == r.path();
    }

    bool operator!=(const url& l, const url& r) noexcept {
        return !(l == r);
    }

    url operator+(const url& l, str_view r) {
        return url(l.scheme(), str_view_concat(l.path(), r));
    }

    url operator/(const url& l, str_view r) {
        return url(l.scheme(), path::combine(l.path(), r));
    }
}
