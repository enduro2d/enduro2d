/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/resources/toggle_style.hpp>

namespace e2d
{
    void toggle_style::clear() noexcept {
        normal_tint_ = color32::white();
        normal_pushing_tint_ = color32::white();
        normal_hovering_tint_ = color32::white();
        normal_disabled_tint_ = color32::white();
        pressed_tint_ = color32::white();
        pressed_pushing_tint_ = color32::white();
        pressed_hovering_tint_ = color32::white();
        pressed_disabled_tint_ = color32::white();
        normal_sprite_.reset();
        normal_pushing_sprite_.reset();
        normal_hovering_sprite_.reset();
        normal_disabled_sprite_.reset();
        pressed_sprite_.reset();
        pressed_pushing_sprite_.reset();
        pressed_hovering_sprite_.reset();
        pressed_disabled_sprite_.reset();
    }

    void toggle_style::swap(toggle_style& other) noexcept {
        using std::swap;
        swap(normal_tint_, other.normal_tint_);
        swap(normal_pushing_tint_, other.normal_pushing_tint_);
        swap(normal_hovering_tint_, other.normal_hovering_tint_);
        swap(normal_disabled_tint_, other.normal_disabled_tint_);
        swap(pressed_tint_, other.pressed_tint_);
        swap(pressed_pushing_tint_, other.pressed_pushing_tint_);
        swap(pressed_hovering_tint_, other.pressed_hovering_tint_);
        swap(pressed_disabled_tint_, other.pressed_disabled_tint_);
        swap(normal_sprite_, other.normal_sprite_);
        swap(normal_pushing_sprite_, other.normal_pushing_sprite_);
        swap(normal_hovering_sprite_, other.normal_hovering_sprite_);
        swap(normal_disabled_sprite_, other.normal_disabled_sprite_);
        swap(pressed_sprite_, other.pressed_sprite_);
        swap(pressed_pushing_sprite_, other.pressed_pushing_sprite_);
        swap(pressed_hovering_sprite_, other.pressed_hovering_sprite_);
        swap(pressed_disabled_sprite_, other.pressed_disabled_sprite_);
    }

    toggle_style& toggle_style::assign(toggle_style&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    toggle_style& toggle_style::assign(const toggle_style& other) {
        if ( this != &other ) {
            toggle_style s;
            s.normal_tint_ = other.normal_tint_;
            s.normal_pushing_tint_ = other.normal_pushing_tint_;
            s.normal_hovering_tint_ = other.normal_hovering_tint_;
            s.normal_disabled_tint_ = other.normal_disabled_tint_;
            s.pressed_tint_ = other.pressed_tint_;
            s.pressed_pushing_tint_ = other.pressed_pushing_tint_;
            s.pressed_hovering_tint_ = other.pressed_hovering_tint_;
            s.pressed_disabled_tint_ = other.pressed_disabled_tint_;
            s.normal_sprite_ = other.normal_sprite_;
            s.normal_pushing_sprite_ = other.normal_pushing_sprite_;
            s.normal_hovering_sprite_ = other.normal_hovering_sprite_;
            s.normal_disabled_sprite_ = other.normal_disabled_sprite_;
            s.pressed_sprite_ = other.pressed_sprite_;
            s.pressed_pushing_sprite_ = other.pressed_pushing_sprite_;
            s.pressed_hovering_sprite_ = other.pressed_hovering_sprite_;
            s.pressed_disabled_sprite_ = other.pressed_disabled_sprite_;
            swap(s);
        }
        return *this;
    }

    toggle_style& toggle_style::set_normal_tint(const color32& value) noexcept {
        normal_tint_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_normal_pushing_tint(const color32& value) noexcept {
        normal_pushing_tint_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_normal_hovering_tint(const color32& value) noexcept {
        normal_hovering_tint_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_normal_disabled_tint(const color32& value) noexcept {
        normal_disabled_tint_ = value;
        return *this;
    }

    const color32& toggle_style::normal_tint() const noexcept {
        return normal_tint_;
    }

    const color32& toggle_style::normal_pushing_tint() const noexcept {
        return normal_pushing_tint_;
    }

    const color32& toggle_style::normal_hovering_tint() const noexcept {
        return normal_hovering_tint_;
    }

    const color32& toggle_style::normal_disabled_tint() const noexcept {
        return normal_disabled_tint_;
    }

    toggle_style& toggle_style::set_pressed_tint(const color32& value) noexcept {
        pressed_tint_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_pressed_pushing_tint(const color32& value) noexcept {
        pressed_pushing_tint_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_pressed_hovering_tint(const color32& value) noexcept {
        pressed_hovering_tint_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_pressed_disabled_tint(const color32& value) noexcept {
        pressed_disabled_tint_ = value;
        return *this;
    }

    const color32& toggle_style::pressed_tint() const noexcept {
        return pressed_tint_;
    }

    const color32& toggle_style::pressed_pushing_tint() const noexcept {
        return pressed_pushing_tint_;
    }

    const color32& toggle_style::pressed_hovering_tint() const noexcept {
        return pressed_hovering_tint_;
    }

    const color32& toggle_style::pressed_disabled_tint() const noexcept {
        return pressed_disabled_tint_;
    }

    toggle_style& toggle_style::set_normal_sprite(const sprite_asset::ptr& value) noexcept {
        normal_sprite_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_normal_pushing_sprite(const sprite_asset::ptr& value) noexcept {
        normal_pushing_sprite_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_normal_hovering_sprite(const sprite_asset::ptr& value) noexcept {
        normal_hovering_sprite_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_normal_disabled_sprite(const sprite_asset::ptr& value) noexcept {
        normal_disabled_sprite_ = value;
        return *this;
    }

    const sprite_asset::ptr& toggle_style::normal_sprite() const noexcept {
        return normal_sprite_;
    }

    const sprite_asset::ptr& toggle_style::normal_pushing_sprite() const noexcept {
        return normal_pushing_sprite_;
    }

    const sprite_asset::ptr& toggle_style::normal_hovering_sprite() const noexcept {
        return normal_hovering_sprite_;
    }

    const sprite_asset::ptr& toggle_style::normal_disabled_sprite() const noexcept {
        return normal_disabled_sprite_;
    }

    toggle_style& toggle_style::set_pressed_sprite(const sprite_asset::ptr& value) noexcept {
        pressed_sprite_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_pressed_pushing_sprite(const sprite_asset::ptr& value) noexcept {
        pressed_pushing_sprite_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_pressed_hovering_sprite(const sprite_asset::ptr& value) noexcept {
        pressed_hovering_sprite_ = value;
        return *this;
    }

    toggle_style& toggle_style::set_pressed_disabled_sprite(const sprite_asset::ptr& value) noexcept {
        pressed_disabled_sprite_ = value;
        return *this;
    }

    const sprite_asset::ptr& toggle_style::pressed_sprite() const noexcept {
        return pressed_sprite_;
    }

    const sprite_asset::ptr& toggle_style::pressed_pushing_sprite() const noexcept {
        return pressed_pushing_sprite_;
    }

    const sprite_asset::ptr& toggle_style::pressed_hovering_sprite() const noexcept {
        return pressed_hovering_sprite_;
    }

    const sprite_asset::ptr& toggle_style::pressed_disabled_sprite() const noexcept {
        return pressed_disabled_sprite_;
    }
}

namespace e2d
{
    void swap(toggle_style& l, toggle_style& r) noexcept {
        l.swap(r);
    }

    bool operator==(const toggle_style& l, const toggle_style& r) noexcept {
        return true
            && l.normal_tint() == r.normal_tint()
            && l.normal_pushing_tint() == r.normal_pushing_tint()
            && l.normal_hovering_tint() == r.normal_hovering_tint()
            && l.normal_disabled_tint() == r.normal_disabled_tint()
            && l.pressed_tint() == r.pressed_tint()
            && l.pressed_pushing_tint() == r.pressed_pushing_tint()
            && l.pressed_hovering_tint() == r.pressed_hovering_tint()
            && l.pressed_disabled_tint() == r.pressed_disabled_tint()
            && l.normal_sprite() == r.normal_sprite()
            && l.normal_pushing_sprite() == r.normal_pushing_sprite()
            && l.normal_hovering_sprite() == r.normal_hovering_sprite()
            && l.normal_disabled_sprite() == r.normal_disabled_sprite()
            && l.pressed_sprite() == r.pressed_sprite()
            && l.pressed_pushing_sprite() == r.pressed_pushing_sprite()
            && l.pressed_hovering_sprite() == r.pressed_hovering_sprite()
            && l.pressed_disabled_sprite() == r.pressed_disabled_sprite();
    }

    bool operator!=(const toggle_style& l, const toggle_style& r) noexcept {
        return !(l == r);
    }
}
