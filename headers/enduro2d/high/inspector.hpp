/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "gobject.hpp"

namespace e2d
{
    //
    // bad_inspector_operation
    //

    class bad_inspector_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad inspector operation";
        }
    };

    //
    // component_inspector
    //

    template < typename Component = void >
    class component_inspector;

    template <>
    class component_inspector<> {
    public:
    };

    //
    // inspector_drawer
    //

    namespace impl
    {
        class inspector_drawer;
        using inspector_drawer_iptr = intrusive_ptr<inspector_drawer>;

        class inspector_drawer
            : private noncopyable
            , public ref_counter<inspector_drawer> {
        public:
            inspector_drawer() = default;
            virtual ~inspector_drawer() noexcept = default;

            virtual void operator()(gobject& go) const = 0;
        };

        template < typename Component >
        class typed_inspector_drawer final : public inspector_drawer {
        public:
            typed_inspector_drawer() = default;
            ~typed_inspector_drawer() noexcept final = default;

            void operator()(gobject& go) const final;
        private:
            component_inspector<Component> inspector_;
        };
    }

    //
    // inspector
    //

    class inspector : public module<inspector> {
    public:
        inspector() = default;
        ~inspector() noexcept final = default;

        template < typename Component >
        inspector& register_component(str_hash type);

        void show_inspector_for(gobject& go);
    private:
        mutable std::mutex mutex_;
        hash_map<str_hash, impl::inspector_drawer_iptr> drawers_;
    };
}

#include "inspector.inl"
