/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d::touchable_events
{
    class mouse_move_evt;
    class mouse_hover_evt;
    class mouse_button_evt;

    using event = std::variant<std::monostate,
        mouse_move_evt,
        mouse_hover_evt,
        mouse_button_evt>;
}

namespace e2d
{
    class touchable final {
    public:
        class pushing final {};
        class hovering final {};

        class clicked final {};
        class pressed final {};
        class released final {};

        class hover_over final {};
        class hover_out final {};
        class hover_enter final {};
        class hover_leave final {};
    public:
        touchable() = default;

        touchable& bubbling(bool value) noexcept;
        touchable& capturing(bool value) noexcept;

        [[nodiscard]] bool bubbling() const noexcept;
        [[nodiscard]] bool capturing() const noexcept;
    private:
        enum flag_masks : u8 {
            fm_bubbling = 1u << 0,
            fm_capturing = 1u << 1,
        };
    private:
        std::underlying_type_t<flag_masks> flags_{
            fm_bubbling |
            fm_capturing};
    };
}

namespace e2d
{
    template <>
    class factory_loader<touchable> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            touchable& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<touchable::pushing> final
    : public empty_factory_loader<touchable::pushing> {};

    template <>
    class factory_loader<touchable::hovering> final
    : public empty_factory_loader<touchable::hovering> {};

    template <>
    class factory_loader<touchable::clicked> final
    : public empty_factory_loader<touchable::clicked> {};

    template <>
    class factory_loader<touchable::pressed> final
    : public empty_factory_loader<touchable::pressed> {};

    template <>
    class factory_loader<touchable::released> final
    : public empty_factory_loader<touchable::released> {};

    template <>
    class factory_loader<touchable::hover_over> final
    : public empty_factory_loader<touchable::hover_over> {};

    template <>
    class factory_loader<touchable::hover_out> final
    : public empty_factory_loader<touchable::hover_out> {};

    template <>
    class factory_loader<touchable::hover_enter> final
    : public empty_factory_loader<touchable::hover_enter> {};

    template <>
    class factory_loader<touchable::hover_leave> final
    : public empty_factory_loader<touchable::hover_leave> {};

    template <>
    class factory_loader<events<touchable_events::event>> final
    : public empty_factory_loader<events<touchable_events::event>> {};
}

namespace e2d
{
    template <>
    class component_inspector<touchable> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<touchable>& c) const;
    };
}

namespace e2d::touchable_events
{
    //
    // base_evt
    //

    namespace impl
    {
        template < typename Event >
        class base_evt {
        public:
            base_evt(bool bubbling)
            : bubbling_(bubbling) {}

            base_evt(gobject target, bool bubbling)
            : target_(target)
            , bubbling_(bubbling) {}

            Event& target(gobject value) noexcept {
                target_ = std::move(value);
                return static_cast<Event&>(*this);
            }

            [[nodiscard]] gobject target() const noexcept { return target_; }
            [[nodiscard]] bool bubbling() const noexcept { return bubbling_; }
        private:
            gobject target_;
            bool bubbling_ = false;
        };
    }

    //
    // mouse_move_evt
    //

    class mouse_move_evt final : public impl::base_evt<mouse_move_evt> {
    public:
        mouse_move_evt(
            gobject target,
            const v2f& local_point,
            const v2f& world_point)
        : base_evt(target, true)
        , local_point_(local_point)
        , world_point_(world_point) {}

        [[nodiscard]] const v2f& local_point() const noexcept { return local_point_; }
        [[nodiscard]] const v2f& world_point() const noexcept { return world_point_; }
    private:
        v2f local_point_ = v2f::zero();
        v2f world_point_ = v2f::zero();
    };

    //
    // mouse_hover_evt
    //

    class mouse_hover_evt final : public impl::base_evt<mouse_hover_evt> {
    public:
        ENUM_HPP_CLASS_DECL(types, u8,
            (over)
            (out)
            (enter)
            (leave))
    public:
        mouse_hover_evt(gobject target, types type)
        : base_evt(target, type == types::over || type == types::out)
        , type_(type) {}

        [[nodiscard]] types type() const noexcept { return type_; }
    private:
        types type_ = types::over;
    };

    ENUM_HPP_REGISTER_TRAITS(mouse_hover_evt::types)

    //
    // mouse_button_evt
    //

    class mouse_button_evt final : public impl::base_evt<mouse_button_evt> {
    public:
        ENUM_HPP_CLASS_DECL(types, u8,
            (clicked)
            (pressed)
            (released))
    public:
        mouse_button_evt(
            gobject target,
            types type,
            mouse_button button,
            const v2f& local_point,
            const v2f& world_point)
        : base_evt(target, true)
        , type_(type)
        , button_(button)
        , local_point_(local_point)
        , world_point_(world_point) {}

        [[nodiscard]] types type() const noexcept { return type_; }
        [[nodiscard]] mouse_button button() const noexcept { return button_; }
        [[nodiscard]] const v2f& local_point() const noexcept { return local_point_; }
        [[nodiscard]] const v2f& world_point() const noexcept { return world_point_; }
    private:
        types type_ = types::pressed;
        mouse_button button_ = mouse_button::left;
        v2f local_point_ = v2f::zero();
        v2f world_point_ = v2f::zero();
    };

    ENUM_HPP_REGISTER_TRAITS(mouse_button_evt::types)
}

namespace e2d
{
    inline touchable& touchable::bubbling(bool value) noexcept {
        if ( value != bubbling() ) {
            math::flip_flags_inplace(flags_, fm_bubbling);
        }
        return *this;
    }

    inline touchable& touchable::capturing(bool value) noexcept {
        if ( value != capturing() ) {
            math::flip_flags_inplace(flags_, fm_capturing);
        }
        return *this;
    }

    inline bool touchable::bubbling() const noexcept {
        return math::check_any_flags(flags_, fm_bubbling);
    }

    inline bool touchable::capturing() const noexcept {
        return math::check_any_flags(flags_, fm_capturing);
    }
}
