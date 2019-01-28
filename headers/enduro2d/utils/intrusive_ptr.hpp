/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    template < typename CounterValue = std::uint32_t >
    struct ref_counter_thread_safe_policy;

    template < typename CounterValue = std::uint32_t >
    struct ref_counter_thread_unsafe_policy;

    template < typename Derived
             , typename CounterPolicy = ref_counter_thread_safe_policy<> >
    class ref_counter;

    template < typename Derived, typename CounterPolicy >
    void intrusive_ptr_add_ref(const ref_counter<Derived, CounterPolicy>* rc) noexcept;

    template < typename Derived, typename CounterPolicy >
    void intrusive_ptr_release(const ref_counter<Derived, CounterPolicy>* rc) noexcept;
}

namespace e2d
{
    template < typename CounterValue >
    struct ref_counter_thread_safe_policy {
        using counter_t = std::atomic<CounterValue>;
        using counter_value_t = CounterValue;

        static counter_value_t incr(counter_t& c) noexcept {
            return c.fetch_add(
                counter_value_t(1),
                std::memory_order_acq_rel) + counter_value_t(1);
        }

        static counter_value_t decr(counter_t& c) noexcept {
            return c.fetch_sub(
                counter_value_t(1),
                std::memory_order_acq_rel) - counter_value_t(1);
        }

        static counter_value_t load(const counter_t& c) noexcept {
            return c.load(std::memory_order_acquire);
        }
    };

    template < typename CounterValue >
    struct ref_counter_thread_unsafe_policy {
        using counter_t = CounterValue;
        using counter_value_t = CounterValue;

        static counter_value_t incr(counter_t& c) noexcept {
            return ++c;
        }

        static counter_value_t decr(counter_t& c) noexcept {
            return --c;
        }

        static counter_value_t load(const counter_t& c) noexcept {
            return c;
        }
    };

    template < typename Derived, typename CounterPolicy >
    class ref_counter {
    public:
        using self_type = ref_counter;
        using counter_t = typename CounterPolicy::counter_t;
        using counter_value_t = typename CounterPolicy::counter_value_t;

        friend void intrusive_ptr_add_ref<Derived, CounterPolicy>(const ref_counter* rc) noexcept;
        friend void intrusive_ptr_release<Derived, CounterPolicy>(const ref_counter* rc) noexcept;
    public:
        ref_counter() = default;
        ~ref_counter() noexcept = default;

        ref_counter(const ref_counter& other) noexcept {
            E2D_UNUSED(other);
        }

        ref_counter& operator=(const ref_counter& other) noexcept {
            E2D_UNUSED(other);
            return *this;
        }

        counter_value_t use_count() const noexcept {
            return CounterPolicy::load(counter_);
        }
    private:
        mutable counter_t counter_{0};
    };
}

namespace e2d
{
    template < typename Derived, typename CounterPolicy >
    void intrusive_ptr_add_ref(const ref_counter<Derived, CounterPolicy>* rc) noexcept {
        CounterPolicy::incr(rc->counter_);
    }

    template < typename Derived, typename CounterPolicy >
    void intrusive_ptr_release(const ref_counter<Derived, CounterPolicy>* rc) noexcept {
        if ( 0u == CounterPolicy::decr(rc->counter_) ) {
            delete static_cast<const Derived*>(rc);
        }
    }
}

namespace e2d
{
    template < typename T >
    class intrusive_ptr final {
    public:
        using self_type = intrusive_ptr;
        using element_type = T;

        template < typename U >
        friend class intrusive_ptr;

        constexpr static bool is_nothrow_add_ref_v = noexcept(intrusive_ptr_add_ref(std::declval<T*>()));
        constexpr static bool is_nothrow_release_v = noexcept(intrusive_ptr_release(std::declval<T*>()));
        constexpr static bool is_nothrow_counter_v = is_nothrow_add_ref_v && is_nothrow_release_v;
    public:
        intrusive_ptr() = default;
        ~intrusive_ptr() noexcept(is_nothrow_release_v);

        intrusive_ptr(intrusive_ptr&& other) noexcept;
        intrusive_ptr& operator=(intrusive_ptr&& other) noexcept(is_nothrow_release_v);

        intrusive_ptr(const intrusive_ptr& other) noexcept(is_nothrow_add_ref_v);
        intrusive_ptr& operator=(const intrusive_ptr& other) noexcept(is_nothrow_counter_v);

        template < typename U
                 , typename = std::enable_if_t<std::is_convertible<U,T>::value> >
        intrusive_ptr(intrusive_ptr<U>&& other) noexcept;

        template < typename U
                 , typename = std::enable_if_t<std::is_convertible<U,T>::value> >
        intrusive_ptr& operator=(intrusive_ptr<U>&& other) noexcept(is_nothrow_release_v);

        template < typename U
                 , typename = std::enable_if_t<std::is_convertible<U,T>::value> >
        intrusive_ptr(const intrusive_ptr<U>& other) noexcept(is_nothrow_add_ref_v);

        template < typename U
                 , typename = std::enable_if_t<std::is_convertible<U,T>::value> >
        intrusive_ptr& operator=(const intrusive_ptr<U>& other) noexcept(is_nothrow_counter_v);

        intrusive_ptr(T* ptr, bool add_ref = true) noexcept(is_nothrow_add_ref_v);

        void reset() noexcept(is_nothrow_release_v);
        void reset(T* other) noexcept(is_nothrow_counter_v);
        void reset(T* other, bool add_ref) noexcept(is_nothrow_counter_v);

        T* get() const noexcept;
        T* release() noexcept;

        T& operator*() const noexcept;
        T* operator->() const noexcept;
        explicit operator bool() const noexcept;

        void swap(intrusive_ptr<T>& other) noexcept;
    private:
        T* ptr_{nullptr};
    };
}

namespace e2d
{
    template < typename T >
    intrusive_ptr<T>::~intrusive_ptr()
        noexcept(is_nothrow_release_v)
    {
        if ( ptr_ ) {
            intrusive_ptr_release(ptr_);
        }
    }

    template < typename T >
    intrusive_ptr<T>::intrusive_ptr(intrusive_ptr&& other) noexcept
    : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    template < typename T >
    intrusive_ptr<T>& intrusive_ptr<T>::operator=(intrusive_ptr&& other)
        noexcept(is_nothrow_release_v)
    {
        self_type(std::move(other)).swap(*this);
        return *this;
    }

    template < typename T >
    intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr& other)
        noexcept(is_nothrow_add_ref_v)
    : ptr_(other.ptr_) {
        if ( ptr_ ) {
            intrusive_ptr_add_ref(ptr_);
        }
    }

    template < typename T >
    intrusive_ptr<T>& intrusive_ptr<T>::operator=(const intrusive_ptr& other)
        noexcept(is_nothrow_counter_v)
    {
        self_type(other).swap(*this);
        return *this;
    }

    template < typename T >
    template < typename U, typename >
    intrusive_ptr<T>::intrusive_ptr(intrusive_ptr<U>&& other) noexcept
    : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    template < typename T >
    template < typename U, typename >
    intrusive_ptr<T>& intrusive_ptr<T>::operator=(intrusive_ptr<U>&& other)
        noexcept(is_nothrow_release_v)
    {
        self_type(std::move(other)).swap(*this);
        return *this;
    }

    template < typename T >
    template < typename U, typename >
    intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr<U>& other)
        noexcept(is_nothrow_add_ref_v)
    : ptr_(other.ptr_) {
        if ( ptr_ ) {
            intrusive_ptr_add_ref(ptr_);
        }
    }

    template < typename T >
    template < typename U, typename >
    intrusive_ptr<T>& intrusive_ptr<T>::operator=(const intrusive_ptr<U>& other)
        noexcept(is_nothrow_counter_v)
    {
        self_type(other).swap(*this);
        return *this;
    }

    template < typename T >
    intrusive_ptr<T>::intrusive_ptr(T* ptr, bool add_ref)
        noexcept(is_nothrow_add_ref_v)
    : ptr_(ptr) {
        if ( ptr_ && add_ref ) {
            intrusive_ptr_add_ref(ptr_);
        }
    }

    template < typename T >
    void intrusive_ptr<T>::reset()
        noexcept(is_nothrow_release_v)
    {
        self_type().swap(*this);
    }

    template < typename T >
    void intrusive_ptr<T>::reset(T* other)
        noexcept(is_nothrow_counter_v)
    {
        self_type(other).swap(*this);
    }

    template < typename T >
    void intrusive_ptr<T>::reset(T* other, bool add_ref)
        noexcept(is_nothrow_counter_v)
    {
        self_type(other, add_ref).swap(*this);
    }

    template < typename T >
    T* intrusive_ptr<T>::get() const noexcept {
        return ptr_;
    }

    template < typename T >
    T* intrusive_ptr<T>::release() noexcept {
        T* result = ptr_;
        ptr_ = nullptr;
        return result;
    }

    template < typename T >
    T& intrusive_ptr<T>::operator*() const noexcept {
        E2D_ASSERT(ptr_);
        return *ptr_;
    }

    template < typename T >
    T* intrusive_ptr<T>::operator->() const noexcept {
        E2D_ASSERT(ptr_);
        return ptr_;
    }

    template < typename T >
    intrusive_ptr<T>::operator bool() const noexcept {
        return !!ptr_;
    }

    template < typename T >
    void intrusive_ptr<T>::swap(intrusive_ptr<T>& other) noexcept {
        T* tmp = ptr_;
        ptr_ = other.ptr_;
        other.ptr_ = tmp;
    }
}

namespace e2d
{
    //
    // swap
    //

    template < typename T >
    void swap(intrusive_ptr<T>& l, intrusive_ptr<T>& r) noexcept {
        l.swap(r);
    }

    //
    // make_intrusive
    //

    template < typename T, typename... Args >
    intrusive_ptr<T> make_intrusive(Args&&... args) {
        return intrusive_ptr<T>(new T(std::forward<Args>(args)...));
    }

    //
    // iptr (==,!=) iptr
    //

    template < typename T, typename U >
    bool operator==(const intrusive_ptr<T>& l, const intrusive_ptr<U>& r) noexcept {
        return l.get() == r.get();
    }

    template < typename T, typename U >
    bool operator!=(const intrusive_ptr<T>& l, const intrusive_ptr<U>& r) noexcept {
        return l.get() != r.get();
    }

    //
    // iptr (==,!=) ptr
    //

    template < typename T, typename U >
    bool operator==(const intrusive_ptr<T>& l, const U* r) noexcept {
        return l.get() == r;
    }

    template < typename T, typename U >
    bool operator!=(const intrusive_ptr<T>& l, const U* r) noexcept {
        return l.get() != r;
    }

    //
    // ptr (==,!=) iptr
    //

    template < typename T, typename U >
    bool operator==(const T* l, const intrusive_ptr<U>& r) noexcept {
        return l == r.get();
    }

    template < typename T, typename U >
    bool operator!=(const T* l, const intrusive_ptr<U>& r) noexcept {
        return l != r.get();
    }

    //
    // iptr (==,!=) nullptr
    //

    template < typename T >
    bool operator==(const intrusive_ptr<T>& l, std::nullptr_t) noexcept {
        return l.get() == nullptr;
    }

    template < typename T >
    bool operator!=(const intrusive_ptr<T>& l, std::nullptr_t) noexcept {
        return l.get() != nullptr;
    }

    //
    // nullptr (==,!=) iptr
    //

    template < typename T >
    bool operator==(std::nullptr_t, const intrusive_ptr<T>& r) noexcept {
        return nullptr == r.get();
    }

    template < typename T >
    bool operator!=(std::nullptr_t, const intrusive_ptr<T>& r) noexcept {
        return nullptr != r.get();
    }

    //
    // iptr (<,<=,>,>=) iptr
    //

    template < typename T >
    bool operator<(const intrusive_ptr<T>& l, const intrusive_ptr<T>& r) noexcept {
        return std::less<T*>()(l.get(), r.get());
    }

    template < typename T >
    bool operator<=(const intrusive_ptr<T>& l, const intrusive_ptr<T>& r) noexcept {
        return std::less_equal<T*>()(l.get(), r.get());
    }

    template < typename T >
    bool operator>(const intrusive_ptr<T>& l, const intrusive_ptr<T>& r) noexcept {
        return std::greater<T*>()(l.get(), r.get());
    }

    template < typename T >
    bool operator>=(const intrusive_ptr<T>& l, const intrusive_ptr<T>& r) noexcept {
        return std::greater_equal<T*>()(l.get(), r.get());
    }

    //
    // iptr casts
    //

    template < typename T, typename U >
    intrusive_ptr<T> const_pointer_cast(const intrusive_ptr<U>& p) noexcept {
        return const_cast<T*>(p.get());
    }

    template < typename T, typename U >
    intrusive_ptr<T> static_pointer_cast(const intrusive_ptr<U>& p) noexcept {
        return static_cast<T*>(p.get());
    }

    template < typename T, typename U >
    intrusive_ptr<T> dynamic_pointer_cast(const intrusive_ptr<U>& p) noexcept {
        return dynamic_cast<T*>(p.get());
    }

}

namespace std
{
    template < typename T >
    struct hash<e2d::intrusive_ptr<T>> final
        : std::unary_function<e2d::intrusive_ptr<T>, std::size_t>
    {
        std::size_t operator()(const e2d::intrusive_ptr<T>& p) const noexcept {
            return std::hash<T*>()(p.get());
        }
    };
}
