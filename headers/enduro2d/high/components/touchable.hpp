/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d::touchable_events
{
    class mouse_evt;
    class touch_evt;

    using event = std::variant<std::monostate,
        mouse_evt,
        touch_evt>;
}

namespace e2d
{
    class touchable final {
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
    class factory_loader<events<touchable_events::event>> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            events<touchable_events::event>& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
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
    namespace impl
    {
        template < typename Event >
        class base_evt {
        public:
            base_evt(bool bubbling, bool capturing)
            : bubbling_(bubbling)
            , capturing_(capturing) {}

            base_evt(gobject target, bool bubbling, bool capturing)
            : target_(target)
            , bubbling_(bubbling)
            , capturing_(capturing) {}

            Event& target(gobject value) noexcept {
                target_ = std::move(value);
                return static_cast<Event&>(*this);
            }

            [[nodiscard]] gobject target() const noexcept { return target_; }
            [[nodiscard]] bool bubbling() const noexcept { return bubbling_; }
            [[nodiscard]] bool capturing() const noexcept { return capturing_; }
        private:
            gobject target_;
            bool bubbling_ = true;
            bool capturing_ = true;
        };
    }

    class mouse_evt final : public impl::base_evt<mouse_evt> {
    public:
        ENUM_HPP_CLASS_DECL(types, u8,
            (pressed)
            (released))
    public:
        mouse_evt(types type, mouse_button button)
        : base_evt(true, true)
        , type_(type)
        , button_(button) {}

        mouse_evt(gobject target, types type, mouse_button button)
        : base_evt(target, true, true)
        , type_(type)
        , button_(button) {}

        [[nodiscard]] types type() const noexcept { return type_; }
        [[nodiscard]] mouse_button button() const noexcept { return button_; }
    private:
        types type_ = types::pressed;
        mouse_button button_ = mouse_button::left;
    };

    class touch_evt final : public impl::base_evt<touch_evt> {
    public:
        ENUM_HPP_CLASS_DECL(types, u8,
            (pressed)
            (released))
    public:
        touch_evt(types type, u32 finger)
        : base_evt(true, true)
        , type_(type)
        , finger_(finger) {}

        touch_evt(gobject target, types type, u32 finger)
        : base_evt(target, true, true)
        , type_(type)
        , finger_(finger) {}

        [[nodiscard]] types type() const noexcept { return type_; }
        [[nodiscard]] u32 finger() const noexcept { return finger_; }
    private:
        types type_ = types::pressed;
        u32 finger_ = 0u;
    };
}

ENUM_HPP_REGISTER_TRAITS(e2d::touchable_events::mouse_evt::types)
ENUM_HPP_REGISTER_TRAITS(e2d::touchable_events::touch_evt::types)

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
