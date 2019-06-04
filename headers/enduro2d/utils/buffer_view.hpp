/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"
#include "buffer.hpp"

namespace e2d
{
    class buffer_view {
    public:
        buffer_view() noexcept = default;

        buffer_view(const buffer_view&) noexcept = default;
        buffer_view& operator=(const buffer_view&) noexcept = default;

        buffer_view(std::nullptr_t) noexcept = delete;
        buffer_view(std::nullptr_t, std::size_t) noexcept = delete;

        buffer_view(const buffer& buffer) noexcept;
        buffer_view(const void* data, std::size_t size) noexcept;

        template < typename T >
        buffer_view(const vector<T>& buffer) noexcept;

        template < typename T, std::size_t N >
        buffer_view(const std::array<T,N>& buffer) noexcept;

        const void* data() const noexcept;
        std::size_t size() const noexcept;
        bool empty() const noexcept;
        void swap(buffer_view& other) noexcept;
    private:
        const void* data_ = nullptr;
        std::size_t size_ = 0;
    };

    void swap(buffer_view& l, buffer_view& r) noexcept;
    bool operator<(const buffer_view& l, const buffer_view& r) noexcept;
    bool operator==(const buffer_view& l, const buffer_view& r) noexcept;
    bool operator!=(const buffer_view& l, const buffer_view& r) noexcept;
}

namespace e2d
{
    template < typename T >
    buffer_view::buffer_view(const vector<T>& buffer) noexcept
    : data_(buffer.data())
    , size_(buffer.size() * sizeof(T)) {}

    template < typename T, std::size_t N >
    buffer_view::buffer_view(const std::array<T,N>& buffer) noexcept
    : data_(buffer.data())
    , size_(buffer.size() * sizeof(T)) {}
}
