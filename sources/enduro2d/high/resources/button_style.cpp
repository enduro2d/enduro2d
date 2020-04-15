/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/resources/button_style.hpp>

namespace e2d
{
    void button_style::clear() noexcept {
        normal_tint_ = color32::white();
        pushing_tint_ = color32::white();
        hovering_tint_ = color32::white();
        disabled_tint_ = color32::white();
        normal_sprite_.reset();
        pushing_sprite_.reset();
        hovering_sprite_.reset();
        disabled_sprite_.reset();
    }

    void button_style::swap(button_style& other) noexcept {
        using std::swap;
        swap(normal_tint_, other.normal_tint_);
        swap(pushing_tint_, other.pushing_tint_);
        swap(hovering_tint_, other.hovering_tint_);
        swap(disabled_tint_, other.disabled_tint_);
        swap(normal_sprite_, other.normal_sprite_);
        swap(pushing_sprite_, other.pushing_sprite_);
        swap(hovering_sprite_, other.hovering_sprite_);
        swap(disabled_sprite_, other.disabled_sprite_);
    }

    button_style& button_style::assign(button_style&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    button_style& button_style::assign(const button_style& other) {
        if ( this != &other ) {
            button_style s;
            s.normal_tint_ = other.normal_tint_;
            s.pushing_tint_ = other.pushing_tint_;
            s.hovering_tint_ = other.hovering_tint_;
            s.disabled_tint_ = other.disabled_tint_;
            s.normal_sprite_ = other.normal_sprite_;
            s.pushing_sprite_ = other.pushing_sprite_;
            s.hovering_sprite_ = other.hovering_sprite_;
            s.disabled_sprite_ = other.disabled_sprite_;
            swap(s);
        }
        return *this;
    }

    button_style& button_style::set_normal_tint(const color32& value) noexcept {
        normal_tint_ = value;
        return *this;
    }

    button_style& button_style::set_pushing_tint(const color32& value) noexcept {
        pushing_tint_ = value;
        return *this;
    }

    button_style& button_style::set_hovering_tint(const color32& value) noexcept {
        hovering_tint_ = value;
        return *this;
    }

    button_style& button_style::set_disabled_tint(const color32& value) noexcept {
        disabled_tint_ = value;
        return *this;
    }

    const color32& button_style::normal_tint() const noexcept {
        return normal_tint_;
    }

    const color32& button_style::pushing_tint() const noexcept {
        return pushing_tint_;
    }

    const color32& button_style::hovering_tint() const noexcept {
        return hovering_tint_;
    }

    const color32& button_style::disabled_tint() const noexcept {
        return disabled_tint_;
    }

    button_style& button_style::set_normal_sprite(const sprite_asset::ptr& value) noexcept {
        normal_sprite_ = value;
        return *this;
    }

    button_style& button_style::set_pushing_sprite(const sprite_asset::ptr& value) noexcept {
        pushing_sprite_ = value;
        return *this;
    }

    button_style& button_style::set_hovering_sprite(const sprite_asset::ptr& value) noexcept {
        hovering_sprite_ = value;
        return *this;
    }

    button_style& button_style::set_disabled_sprite(const sprite_asset::ptr& value) noexcept {
        disabled_sprite_ = value;
        return *this;
    }

    const sprite_asset::ptr& button_style::normal_sprite() const noexcept {
        return normal_sprite_;
    }

    const sprite_asset::ptr& button_style::pushing_sprite() const noexcept {
        return pushing_sprite_;
    }

    const sprite_asset::ptr& button_style::hovering_sprite() const noexcept {
        return hovering_sprite_;
    }

    const sprite_asset::ptr& button_style::disabled_sprite() const noexcept {
        return disabled_sprite_;
    }
}

namespace e2d
{
    void swap(button_style& l, button_style& r) noexcept {
        l.swap(r);
    }

    bool operator==(const button_style& l, const button_style& r) noexcept {
        return true
            && l.normal_tint() == r.normal_tint()
            && l.pushing_tint() == r.pushing_tint()
            && l.hovering_tint() == r.hovering_tint()
            && l.disabled_tint() == r.disabled_tint()
            && l.normal_sprite() == r.normal_sprite()
            && l.pushing_sprite() == r.pushing_sprite()
            && l.hovering_sprite() == r.hovering_sprite()
            && l.disabled_sprite() == r.disabled_sprite();
    }

    bool operator!=(const button_style& l, const button_style& r) noexcept {
        return !(l == r);
    }
}
