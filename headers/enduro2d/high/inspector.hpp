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
        class gizmos_context : private noncopyable {
        public:
            virtual void draw_line(
                const v2f& p1,
                const v2f& p2,
                const color32& color) = 0;

            virtual void draw_rect(
                const v2f& p1,
                const v2f& p2,
                const v2f& p3,
                const v2f& p4,
                const color32& color) = 0;

            virtual void draw_wire_rect(
                const v2f& p1,
                const v2f& p2,
                const v2f& p3,
                const v2f& p4,
                const color32& color) = 0;

            virtual void draw_circle(
                const v2f& center,
                f32 radius,
                u32 segments,
                const color32& color) = 0;

            virtual void draw_wire_circle(
                const v2f& center,
                f32 radius,
                u32 segments,
                const color32& color) = 0;

            virtual bool selected() const noexcept = 0;
        public:
            void draw_rect(
                const v2f& center,
                const v2f& size,
                const color32& color);

            void draw_wire_rect(
                const v2f& center,
                const v2f& size,
                const color32& color);

            template < typename Iter >
            void draw_wire_polygon(
                Iter begin,
                Iter end,
                const v2f& center,
                const color32& color);

            template < typename Container >
            void draw_wire_polygon(
                Container&& container,
                const v2f& center,
                const color32& color);
        };
    };

    namespace impl
    {
        template < typename Component >
        using has_component_inspector =
            decltype(std::declval<component_inspector<Component>>()(
                std::declval<gcomponent<Component>&>()));

        template < typename Component >
        using has_component_inspector_gizmos =
            decltype(std::declval<component_inspector<Component>>()(
                std::declval<gcomponent<Component>&>(),
                std::declval<component_inspector<>::gizmos_context&>()));
    }

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

            virtual void operator()(
                gobject& go) const = 0;

            virtual void operator()(
                gobject& go,
                component_inspector<>::gizmos_context& ctx) const = 0;
        };

        template < typename Component >
        class typed_inspector_drawer final : public inspector_drawer {
        public:
            typed_inspector_drawer() = default;
            ~typed_inspector_drawer() noexcept final = default;

            void operator()(
                gobject& go) const final;

            void operator()(
                gobject& go,
                component_inspector<>::gizmos_context& ctx) const final;
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

        void show_for(
            gobject& go);

        void show_for(
            gobject& go,
            component_inspector<>::gizmos_context& ctx);
    private:
        mutable std::mutex mutex_;
        hash_map<str_hash, impl::inspector_drawer_iptr> drawers_;
    };
}

#include "inspector.inl"
