/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    class url final {
    public:
        url() = default;

        url(url&& other) noexcept;
        url& operator=(url&& other) noexcept;

        url(const url& other);
        url& operator=(const url& other);

        explicit url(str_view schemepath);
        url(str_view scheme, str_view path) noexcept;

        template < typename T, typename U >
        url(T&& scheme, U&& path) noexcept;

        url& assign(url&& other) noexcept;
        url& assign(const url& other);

        url& assign(str_view schemepath);
        url& assign(str_view scheme, str_view path) noexcept;

        template < typename T, typename U >
        url& assign(T&& scheme, U&& path) noexcept;

        url& concat(str_view path);
        url& append(str_view path);

        void swap(url& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        const str& scheme() const noexcept;
        const str& path() const noexcept;

        url& operator+=(str_view path);
        url& operator/=(str_view path);
    private:
        str scheme_;
        str path_;
    };
}

namespace e2d
{
    template < typename T, typename U >
    url::url(T&& scheme, U&& path) noexcept {
        assign(std::forward<T>(scheme), std::forward<U>(path));
    }

    template < typename T, typename U >
    url& url::assign(T&& scheme, U&& path) noexcept {
        scheme_ = std::forward<T>(scheme);
        path_ = std::forward<U>(path);
        return *this;
    }
}

namespace e2d
{
    void swap(url& l, url& r) noexcept;
    bool operator==(const url& l, const url& r) noexcept;
    bool operator!=(const url& l, const url& r) noexcept;

    url operator+(const url& l, str_view r);
    url operator/(const url& l, str_view r);
}

namespace std
{
    template <>
    struct hash<e2d::url>
        : std::unary_function<e2d::url, std::size_t>
    {
        std::size_t operator()(const e2d::url& u) const noexcept {
            return e2d::math::numeric_cast<std::size_t>(
                e2d::utils::sdbm_hash(
                    e2d::utils::sdbm_hash(u.scheme().c_str()), u.path().c_str()));
        }
    };
}
