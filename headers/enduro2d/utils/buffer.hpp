/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
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

    void swap(buffer& l, buffer& r) noexcept;
    bool operator<(const buffer& l, const buffer& r) noexcept;
    bool operator==(const buffer& l, const buffer& r) noexcept;
    bool operator!=(const buffer& l, const buffer& r) noexcept;
}
