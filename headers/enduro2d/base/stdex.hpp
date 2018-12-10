/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "macros.hpp"

//
// variant
//

namespace e2d { namespace stdex
{
    using namespace mpark;
}}

//
// invoke
//

namespace e2d { namespace stdex
{
    using namespace invoke_hpp;
}}

//
// promise
//

namespace e2d { namespace stdex
{
    using namespace promise_hpp;
}}

//
// basic_string_view
//

namespace e2d { namespace stdex
{
    template < typename Char, typename Traits = std::char_traits<Char> >
    class basic_string_view {
    public:
        using value_type = Char;
        using traits_type = Traits;

        using pointer = const Char*;
        using const_pointer = const Char*;

        using reference = const Char&;
        using const_reference = const Char&;

        using const_iterator = const_pointer;
        using iterator = const_iterator;

        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator = const_reverse_iterator;

        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        static const size_type npos = size_type(-1);

        static_assert(
            std::is_pod<value_type>::value,
            "value_type must be a POD");
        static_assert(
            std::is_same<value_type, typename traits_type::char_type>::value,
            "traits_type::char_type must be the same type as value_type");
    public:
        basic_string_view() noexcept = default;
        basic_string_view(const basic_string_view&) noexcept = default;
        basic_string_view& operator=(const basic_string_view&) noexcept = default;

        basic_string_view(std::nullptr_t) noexcept = delete;
        basic_string_view(std::nullptr_t, size_type) noexcept = delete;

        basic_string_view(const Char* str) noexcept {
            E2D_ASSERT(str);
            data_ = str;
            size_ = Traits::length(str);
        }

        basic_string_view(const Char* str, size_type size) noexcept {
            E2D_ASSERT(!size || str);
            data_ = str;
            size_ = size;
        }

        template < typename Alloc >
        basic_string_view(const std::basic_string<Char, Traits, Alloc>& str) noexcept
        : data_(str.data())
        , size_(str.size()) {}

        template < typename Alloc >
        operator std::basic_string<Char, Traits, Alloc>() const {
            return {cbegin(), cend()};
        }

        const_iterator begin() const noexcept {
            return data_;
        }

        const_iterator cbegin() const noexcept {
            return data_;
        }

        const_iterator end() const noexcept {
            return data_ + size_;
        }

        const_iterator cend() const noexcept {
            return data_ + size_;
        }

        const_reverse_iterator rbegin() const noexcept {
            return const_reverse_iterator(end());
        }

        const_reverse_iterator crbegin() const noexcept {
            return const_reverse_iterator(cend());
        }

        const_reverse_iterator rend() const noexcept {
            return const_reverse_iterator(begin());
        }

        const_reverse_iterator crend() const noexcept {
            return const_reverse_iterator(cbegin());
        }

        size_type size() const noexcept {
            return size_;
        }

        size_type length() const noexcept {
            return size_;
        }

        size_type max_size() const noexcept {
            return std::numeric_limits<size_type>::max();
        }

        bool empty() const noexcept {
            return size_ == 0;
        }

        const_reference operator[](size_type index) const noexcept {
            E2D_ASSERT(index < size_);
            return data_[index];
        }

        const_reference at(size_type index) const {
            if ( index < size_ ) {
                return data_[index];
            }
            throw std::out_of_range("basic_string_view::at");
        }

        const_reference front() const noexcept {
            E2D_ASSERT(size_ > 0);
            return data_[0];
        }

        const_reference back() const noexcept {
            E2D_ASSERT(size_ > 0);
            return data_[size_ - 1];
        }

        const_pointer data() const noexcept {
            return data_;
        }

        void swap(basic_string_view& other) noexcept {
            std::swap(data_, other.data_);
            std::swap(size_, other.size_);
        }

        void remove_prefix(size_type n) noexcept {
            E2D_ASSERT(n <= size_);
            data_ += n;
            size_ -= n;
        }

        void remove_suffix(size_type n) noexcept {
            E2D_ASSERT(n <= size_);
            size_ -= n;
        }

        int compare(basic_string_view str) const noexcept {
            const auto ms = std::min(size(), str.size());
            const auto cr = Traits::compare(data(), str.data(), ms);
            return cr == 0
                ? (size() == str.size()
                    ? 0
                    : (size() < str.size() ? -1 : 1))
                : cr;
        }

        int compare(size_type pos1, size_type n1, basic_string_view other) const noexcept {
            return substr(pos1, n1).compare(other);
        }

        int compare(size_type pos1, size_type n1, basic_string_view other, size_type pos2, size_type n2) const noexcept {
            return substr(pos1, n1).compare(other.substr(pos2, n2));
        }

        int compare(const Char* other) const noexcept {
            return compare(basic_string_view(other));
        }

        int compare(size_type pos1, size_type n1, const Char* other) const noexcept {
            return substr(pos1, n1).compare(basic_string_view(other));
        }

        int compare(size_type pos1, size_type n1, const Char* other, size_type n2) const noexcept {
            return substr(pos1, n1).compare(basic_string_view(other, n2));
        }

        bool starts_with(basic_string_view other) const noexcept {
            return size() >= other.size() && 0 == compare(0, other.size(), other);
        }

        bool starts_with(Char other) const noexcept {
            return starts_with(basic_string_view(&other, 1));
        }

        bool starts_with(const Char* other) const noexcept {
            return starts_with(basic_string_view(other));
        }

        bool ends_with(basic_string_view other) const noexcept {
            return size() >= other.size() && 0 == compare(size() - other.size(), npos, other);
        }

        bool ends_with(Char other) const noexcept {
            return ends_with(basic_string_view(&other, 1));
        }

        bool ends_with(const Char* other) const noexcept {
            return ends_with(basic_string_view(other));
        }

        size_type copy(Char* dst, size_type size, size_type index = 0) const {
            if ( index > size_ ) {
                throw std::out_of_range("basic_string_view::copy");
            }
            size_type ms = std::min(size, size_ - index);
            Traits::copy(dst, data_ + index, ms);
            return ms;
        }

        basic_string_view substr(size_type index = 0, size_type size = npos) const {
            if ( index <= size_ ) {
                return basic_string_view(
                    data_ + index,
                    std::min(size, size_ - index));
            }
            throw std::out_of_range("basic_string_view::substr");
        }
    private:
        const Char* data_ = nullptr;
        size_type size_ = 0;
    };

    template < typename Char, typename Traits >
    const typename basic_string_view<Char, Traits>::size_type
        basic_string_view<Char, Traits>::npos;

    template < typename Char, typename Traits >
    bool operator<(
        basic_string_view<Char, Traits> l,
        basic_string_view<Char, Traits> r) noexcept
    {
        return l.compare(r) < 0;
    }

    template < typename Char, typename Traits >
    bool operator<(
        basic_string_view<Char, Traits> l,
        const char* r) noexcept
    {
        return l.compare(basic_string_view<Char, Traits>(r)) < 0;
    }

    template < typename Char, typename Traits >
    bool operator<(
        basic_string_view<Char, Traits> l,
        const std::basic_string<Char, Traits>& r) noexcept
    {
        return l.compare(basic_string_view<Char, Traits>(r)) < 0;
    }

    template < typename Char, typename Traits >
    bool operator<(
        const char* l,
        basic_string_view<Char, Traits> r) noexcept
    {
        return basic_string_view<Char, Traits>(l).compare(r) < 0;
    }

    template < typename Char, typename Traits >
    bool operator<(
        const std::basic_string<Char, Traits>& l,
        basic_string_view<Char, Traits> r) noexcept
    {
        return basic_string_view<Char, Traits>(l).compare(r) < 0;
    }

    template < typename Char, typename Traits >
    bool operator==(
        basic_string_view<Char, Traits> l,
        basic_string_view<Char, Traits> r) noexcept
    {
        const auto ls = l.size();
        const auto rs = r.size();
        return ls == rs && Traits::compare(l.data(), r.data(), ls) == 0;
    }

    template < typename Char, typename Traits >
    bool operator==(
        basic_string_view<Char, Traits> l,
        const char* r) noexcept
    {
        return l.compare(basic_string_view<Char, Traits>(r)) == 0;
    }

    template < typename Char, typename Traits >
    bool operator==(
        basic_string_view<Char, Traits> l,
        const std::basic_string<Char, Traits>& r) noexcept
    {
        return l.compare(basic_string_view<Char, Traits>(r)) == 0;
    }

    template < typename Char, typename Traits >
    bool operator==(
        const char* l,
        basic_string_view<Char, Traits> r) noexcept
    {
        return basic_string_view<Char, Traits>(l).compare(r) == 0;
    }

    template < typename Char, typename Traits >
    bool operator==(
        const std::basic_string<Char, Traits>& l,
        basic_string_view<Char, Traits> r) noexcept
    {
        return basic_string_view<Char, Traits>(l).compare(r) == 0;
    }

    template < typename Char, typename Traits >
    bool operator!=(
        basic_string_view<Char, Traits> l,
        basic_string_view<Char, Traits> r) noexcept
    {
        return !(l == r);
    }

    template < typename Char, typename Traits >
    bool operator!=(
        basic_string_view<Char, Traits> l,
        const char* r) noexcept
    {
        return !(l == r);
    }

    template < typename Char, typename Traits >
    bool operator!=(
        basic_string_view<Char, Traits> l,
        const std::basic_string<Char, Traits>& r) noexcept
    {
        return !(l == r);
    }

    template < typename Char, typename Traits >
    bool operator!=(
        const char* l,
        basic_string_view<Char, Traits> r) noexcept
    {
        return !(l == r);
    }

    template < typename Char, typename Traits >
    bool operator!=(
        const std::basic_string<Char, Traits>& l,
        basic_string_view<Char, Traits> r) noexcept
    {
        return !(l == r);
    }
}}
