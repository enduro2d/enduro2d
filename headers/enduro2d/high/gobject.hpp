/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class gobject final {
    public:
        class state;
        using state_iptr = intrusive_ptr<state>;
        const state_iptr& internal_state() const noexcept;
    public:
        gobject() = default;
        explicit gobject(state_iptr state);

        bool alive() const noexcept;
        bool valid() const noexcept;
        explicit operator bool() const noexcept;

        void destroy() noexcept;

        template < typename T >
        gcomponent<T> component() noexcept;

        template < typename T >
        const_gcomponent<T> component() const noexcept;
    public:
        ecs::entity raw_entity() noexcept;
        ecs::const_entity raw_entity() const noexcept;

        template < typename T >
        ecs::component<T> raw_component() noexcept;
        template < typename T >
        ecs::const_component<T> raw_component() const noexcept;
    private:
        state_iptr state_;
    };

    template < typename T >
    class gcomponent final {
    public:
        gcomponent() = default;
        gcomponent(gobject owner);

        bool valid() const noexcept;
        bool exists() const noexcept;

        template < typename... Args >
        T& assign(Args&&... args);

        template < typename... Args >
        T& ensure(Args&&... args);

        bool remove() noexcept;

        T& get();
        const T& get() const;

        T* find() noexcept;
        const T* find() const noexcept;

        T& operator*() noexcept;
        const T& operator*() const noexcept;

        T* operator->() noexcept;
        const T* operator->() const noexcept;

        const gobject& owner() const noexcept;
        explicit operator bool() const noexcept;
    private:
        gobject owner_;
    };

    template < typename T >
    class const_gcomponent final {
    public:
        const_gcomponent() = default;
        const_gcomponent(gobject owner);
        const_gcomponent(const gcomponent<T>& comp);

        bool valid() const noexcept;
        bool exists() const noexcept;

        const T& get() const;
        const T* find() const noexcept;

        const T& operator*() const noexcept;
        const T* operator->() const noexcept;

        const gobject& owner() const noexcept;
        explicit operator bool() const noexcept;
    private:
        gobject owner_;
    };
}

namespace e2d
{
    class gobject_destroying_state_ilist_tag;
    using gobject_destroying_states = intrusive_list<
        gobject::state,
        gobject_destroying_state_ilist_tag>;

    class gobject::state
        : private noncopyable
        , public ref_counter<state>
        , public intrusive_list_hook<gobject_destroying_state_ilist_tag> {
    public:
        virtual ~state() noexcept {}
        virtual void destroy() noexcept = 0;
        virtual bool destroyed() const noexcept = 0;
        virtual bool invalided() const noexcept = 0;
        virtual ecs::entity raw_entity() noexcept = 0;
        virtual ecs::const_entity raw_entity() const noexcept = 0;
    };

    template < typename T >
    gcomponent<T> gobject::component() noexcept {
        return gcomponent<T>(*this);
    }

    template < typename T >
    const_gcomponent<T> gobject::component() const noexcept {
        return const_gcomponent<T>(*this);
    }

    template < typename T >
    ecs::component<T> gobject::raw_component() noexcept {
        E2D_ASSERT(valid());
        return ecs::component<T>(raw_entity());
    }

    template < typename T >
    ecs::const_component<T> gobject::raw_component() const noexcept {
        E2D_ASSERT(valid());
        return ecs::const_component<T>(raw_entity());
    }
}

namespace e2d
{
    template < typename T >
    gcomponent<T>::gcomponent(gobject owner)
    : owner_(std::move(owner)) {}

    template < typename T >
    bool gcomponent<T>::valid() const noexcept {
        return owner_.valid();
    }

    template < typename T >
    bool gcomponent<T>::exists() const noexcept {
        return owner_.valid()
            && owner_.raw_component<T>().exists();
    }

    template < typename T >
    template < typename... Args >
    T& gcomponent<T>::assign(Args&&... args) {
        E2D_ASSERT(owner_.valid());
        return owner_.raw_component<T>().assign(std::forward<Args>(args)...);
    }

    template < typename T >
    template < typename... Args >
    T& gcomponent<T>::ensure(Args&&... args) {
        E2D_ASSERT(owner_.valid());
        return owner_.raw_component<T>().ensure(std::forward<Args>(args)...);
    }

    template < typename T >
    bool gcomponent<T>::remove() noexcept {
        return owner_.valid()
            ? owner_.raw_component<T>().remove()
            : false;
    }

    template < typename T >
    T& gcomponent<T>::get() {
        E2D_ASSERT(owner_.valid());
        return owner_.raw_component<T>().get();
    }

    template < typename T >
    const T& gcomponent<T>::get() const {
        E2D_ASSERT(owner_.valid());
        return owner_.raw_component<T>().get();
    }

    template < typename T >
    T* gcomponent<T>::find() noexcept {
        return owner_.valid()
            ? owner_.raw_component<T>().find()
            : nullptr;
    }

    template < typename T >
    const T* gcomponent<T>::find() const noexcept {
        return owner_.valid()
            ? owner_.raw_component<T>().find()
            : nullptr;
    }

    template < typename T >
    T& gcomponent<T>::operator*() noexcept {
        return get();
    }

    template < typename T >
    const T& gcomponent<T>::operator*() const noexcept {
        return get();
    }

    template < typename T >
    T* gcomponent<T>::operator->() noexcept {
        return find();
    }

    template < typename T >
    const T* gcomponent<T>::operator->() const noexcept {
        return find();
    }

    template < typename T >
    const gobject& gcomponent<T>::owner() const noexcept {
        return owner_;
    }

    template < typename T >
    gcomponent<T>::operator bool() const noexcept {
        return exists();
    }
}

namespace e2d
{
    template < typename T >
    const_gcomponent<T>::const_gcomponent(gobject owner)
    : owner_(std::move(owner)) {}

    template < typename T >
    const_gcomponent<T>::const_gcomponent(const gcomponent<T>& comp)
    : owner_(comp.owner()) {}

    template < typename T >
    bool const_gcomponent<T>::valid() const noexcept {
        return owner_.valid();
    }

    template < typename T >
    bool const_gcomponent<T>::exists() const noexcept {
        return owner_.valid()
            && owner_.raw_component<T>().exists();
    }

    template < typename T >
    const T& const_gcomponent<T>::get() const {
        E2D_ASSERT(owner_.valid());
        return owner_.raw_component<T>().get();
    }

    template < typename T >
    const T* const_gcomponent<T>::find() const noexcept {
        return owner_.valid()
            ? owner_.raw_component<T>().find()
            : nullptr;
    }

    template < typename T >
    const T& const_gcomponent<T>::operator*() const noexcept {
        return get();
    }

    template < typename T >
    const T* const_gcomponent<T>::operator->() const noexcept {
        return find();
    }

    template < typename T >
    const gobject& const_gcomponent<T>::owner() const noexcept {
        return owner_;
    }

    template < typename T >
    const_gcomponent<T>::operator bool() const noexcept {
        return exists();
    }
}
