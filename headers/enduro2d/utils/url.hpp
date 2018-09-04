/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
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

        url& assign(url&& other) noexcept;
        url& assign(const url& other);

        url& assign(str_view schemepath);
        url& assign(str_view scheme, str_view path) noexcept;

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
    void swap(url& l, url& r) noexcept;
    bool operator==(const url& l, const url& r) noexcept;
    bool operator!=(const url& l, const url& r) noexcept;

    url operator+(const url& l, str_view r);
    url operator/(const url& l, str_view r);
}
