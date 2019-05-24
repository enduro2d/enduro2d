/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    class buffer final {
    public:
        buffer() = default;

        buffer(buffer&& other) noexcept;
        buffer& operator=(buffer&& other) noexcept;

        buffer(const buffer& other);
        buffer& operator=(const buffer& other);

        explicit buffer(std::size_t size);
        buffer(const void* src, std::size_t size);

        buffer& fill(u8 ch) noexcept;
        buffer& resize(std::size_t nsize);

        buffer& assign(buffer&& other) noexcept;
        buffer& assign(const buffer& other);
        buffer& assign(const void* src, std::size_t nsize);

        void swap(buffer& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        u8* data() noexcept;
        const u8* data() const noexcept;
        std::size_t size() const noexcept;
    private:
        using data_t = std::unique_ptr<u8[]>;
        data_t data_;
        std::size_t size_ = 0;
    };

    class buffer_view {
    public:
        buffer_view() noexcept = default;
        buffer_view(const buffer_view&) noexcept = default;
        buffer_view& operator=(const buffer_view&) noexcept = default;

        buffer_view(std::nullptr_t) noexcept = delete;
        buffer_view(std::nullptr_t, std::size_t) noexcept = delete;

        buffer_view(const void* data, std::size_t size) noexcept
        : data_(data)
        , size_(size){
            E2D_ASSERT(!size || data);
        }

        buffer_view(const buffer& buffer) noexcept
        : data_(buffer.data())
        , size_(buffer.size()) {}

        template < typename T >
        buffer_view(const vector<T>& buffer) noexcept
        : data_(buffer.data())
        , size_(buffer.size() * sizeof(T)) {}

        template < typename T, std::size_t N >
        buffer_view(const std::array<T,N>& buffer) noexcept
        : data_(buffer.data())
        , size_(buffer.size() * sizeof(T)) {}

        const void* data() const noexcept {
            return data_;
        }

        std::size_t size() const noexcept {
            return size_;
        }

        bool empty() const noexcept {
            return size_ == 0;
        }

        void swap(buffer_view& other) noexcept {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
        }
    private:
        const void* data_ = nullptr;
        std::size_t size_ = 0;
    };

    void swap(buffer& l, buffer& r) noexcept;
    bool operator<(const buffer& l, const buffer& r) noexcept;
    bool operator==(const buffer& l, const buffer& r) noexcept;
    bool operator!=(const buffer& l, const buffer& r) noexcept;

    void swap(buffer_view& l, buffer_view& r) noexcept;
    bool operator<(const buffer_view& l, const buffer_view& r) noexcept;
    bool operator==(const buffer_view& l, const buffer_view& r) noexcept;
    bool operator!=(const buffer_view& l, const buffer_view& r) noexcept;
}
