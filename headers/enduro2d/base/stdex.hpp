/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "macros.hpp"

#define E2D_STDEX_NOEXCEPT_RETURN(...) \
    noexcept(noexcept(__VA_ARGS__)) { return __VA_ARGS__; }

#define E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(...) \
    noexcept(noexcept(__VA_ARGS__)) -> decltype (__VA_ARGS__) { return __VA_ARGS__; }

//
// void_t
//

namespace e2d { namespace stdex
{
    namespace impl
    {
        template < typename... Args >
        struct make_void {
            using type = void;
        };
    }

    template < typename... Args >
    using void_t = typename impl::make_void<Args...>::type;
}}

//
// is_reference_wrapper
//

namespace e2d { namespace stdex
{
    namespace impl
    {
        template < typename T >
        struct is_reference_wrapper_impl
        : std::false_type {};

        template < typename U >
        struct is_reference_wrapper_impl<std::reference_wrapper<U>>
        : std::true_type {};
    }

    template < typename T >
    struct is_reference_wrapper
    : impl::is_reference_wrapper_impl<std::remove_cv_t<T>> {};
}}

//
// invoke
//

namespace e2d { namespace stdex
{
    namespace impl
    {
        //
        // invoke_member_object_impl
        //

        template
        <
            typename Base, typename F, typename Derived,
            typename std::enable_if_t<std::is_base_of<Base, std::decay_t<Derived>>::value, int> = 0
        >
        constexpr auto invoke_member_object_impl(F Base::* f, Derived&& ref)
        E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(
            std::forward<Derived>(ref).*f)

        template
        <
            typename Base, typename F, typename RefWrap,
            typename std::enable_if_t<is_reference_wrapper<std::decay_t<RefWrap>>::value, int> = 0
        >
        constexpr auto invoke_member_object_impl(F Base::* f, RefWrap&& ref)
        E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(
            ref.get().*f)

        template
        <
            typename Base, typename F, typename Pointer,
            typename std::enable_if_t<
                !std::is_base_of<Base, std::decay_t<Pointer>>::value &&
                !is_reference_wrapper<std::decay_t<Pointer>>::value
            , int> = 0
        >
        constexpr auto invoke_member_object_impl(F Base::* f, Pointer&& ptr)
        E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(
            (*std::forward<Pointer>(ptr)).*f)

        //
        // invoke_member_function_impl
        //

        template
        <
            typename Base, typename F, typename Derived, typename... Args,
            typename std::enable_if_t<std::is_base_of<Base, std::decay_t<Derived>>::value, int> = 0
        >
        constexpr auto invoke_member_function_impl(F Base::* f, Derived&& ref, Args&&... args)
        E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(
            (std::forward<Derived>(ref).*f)(std::forward<Args>(args)...))

        template
        <
            typename Base, typename F, typename RefWrap, typename... Args,
            typename std::enable_if_t<is_reference_wrapper<std::decay_t<RefWrap>>::value, int> = 0
        >
        constexpr auto invoke_member_function_impl(F Base::* f, RefWrap&& ref, Args&&... args)
        E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(
            (ref.get().*f)(std::forward<Args>(args)...))

        template
        <
            typename Base, typename F, typename Pointer, typename... Args,
            typename std::enable_if_t<
                !std::is_base_of<Base, std::decay_t<Pointer>>::value &&
                !is_reference_wrapper<std::decay_t<Pointer>>::value
            , int> = 0
        >
        constexpr auto invoke_member_function_impl(F Base::* f, Pointer&& ptr, Args&&... args)
        E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(
            ((*std::forward<Pointer>(ptr)).*f)(std::forward<Args>(args)...))
    }

    template
    <
        typename F, typename... Args,
        typename std::enable_if_t<!std::is_member_pointer<std::decay_t<F>>::value, int> = 0
    >
    constexpr auto invoke(F&& f, Args&&... args)
    E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(
        std::forward<F>(f)(std::forward<Args>(args)...))

    template
    <
        typename F, typename T,
        typename std::enable_if_t<std::is_member_object_pointer<std::decay_t<F>>::value, int> = 0
    >
    constexpr auto invoke(F&& f, T&& t)
    E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(
        impl::invoke_member_object_impl(std::forward<F>(f), std::forward<T>(t)))

    template
    <
        typename F, typename... Args,
        typename std::enable_if_t<std::is_member_function_pointer<std::decay_t<F>>::value, int> = 0
    >
    constexpr auto invoke(F&& f, Args&&... args)
    E2D_STDEX_NOEXCEPT_DECLTYPE_RETURN(
        impl::invoke_member_function_impl(std::forward<F>(f), std::forward<Args>(args)...))
}}

//
// invoke_result
//

namespace e2d { namespace stdex
{
    namespace impl
    {
        struct invoke_result_impl_tag {};

        template < typename Void, typename F, typename... Args >
        struct invoke_result_impl {};

        template < typename F, typename... Args >
        struct invoke_result_impl<void_t<invoke_result_impl_tag, decltype(stdex::invoke(std::declval<F>(), std::declval<Args>()...))>, F, Args...> {
            using type = decltype(stdex::invoke(std::declval<F>(), std::declval<Args>()...));
        };
    }

    template < typename F, typename... Args >
    struct invoke_result
    : impl::invoke_result_impl<void, F, Args...> {};

    template < typename F, typename... Args >
    using invoke_result_t = typename invoke_result<F, Args...>::type;
}}

//
// apply
//

namespace e2d { namespace stdex
{
    namespace impl
    {
        template < typename F, typename Tuple, std::size_t... I >
        constexpr decltype(auto) apply_impl(F&& f, Tuple&& args, std::index_sequence<I...>)
        E2D_STDEX_NOEXCEPT_RETURN(
            stdex::invoke(
                std::forward<F>(f),
                std::get<I>(std::forward<Tuple>(args))...))
    }

    template < typename F, typename Tuple >
    constexpr decltype(auto) apply(F&& f, Tuple&& args)
    E2D_STDEX_NOEXCEPT_RETURN(
        impl::apply_impl(
            std::forward<F>(f),
            std::forward<Tuple>(args),
            std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>()))
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
