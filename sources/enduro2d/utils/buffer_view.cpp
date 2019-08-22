/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/utils/buffer_view.hpp>

namespace e2d
{
    buffer_view::buffer_view(const buffer& buffer) noexcept
    : data_(buffer.data())
    , size_(buffer.size()) {}
    
    buffer_view::buffer_view(const void* data, std::size_t size) noexcept
    : data_(data)
    , size_(size){
        E2D_ASSERT(!size || data);
    }

    const void* buffer_view::data() const noexcept {
        return data_;
    }

    std::size_t buffer_view::size() const noexcept {
        return size_;
    }

    bool buffer_view::empty() const noexcept {
        return size_ == 0;
    }

    void buffer_view::swap(buffer_view& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
    }

    buffer_view::operator buffer() {
        return buffer(data_, size_);
    }
}

namespace e2d
{
    void swap(buffer_view& l, buffer_view& r) noexcept {
        l.swap(r);
    }

    bool operator<(const buffer_view& l, const buffer_view& r) noexcept {
        const void* ld = l.data();
        const void* rd = r.data();
        const std::size_t ls = l.size();
        const std::size_t rs = r.size();
        return
            (ls < rs) ||
            (ls == rs && ls > 0 && std::memcmp(ld, rd, ls) < 0);
    }

    bool operator==(const buffer_view& l, const buffer_view& r) noexcept {
        const void* ld = l.data();
        const void* rd = r.data();
        const std::size_t ls = l.size();
        const std::size_t rs = r.size();
        return
            (ls == rs) &&
            (ls == 0 || std::memcmp(ld, rd, ls) == 0);
    }

    bool operator!=(const buffer_view& l, const buffer_view& r) noexcept {
        return !(l == r);
    }
}
