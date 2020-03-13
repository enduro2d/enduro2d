/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "assets/sprite_asset.hpp"

namespace e2d
{
    class button_style final {
    public:
        button_style() = default;
        ~button_style() noexcept = default;

        button_style(button_style&& other) noexcept = default;
        button_style& operator=(button_style&& other) noexcept = default;

        button_style(const button_style& other) = default;
        button_style& operator=(const button_style& other) = default;

        void clear() noexcept;
        void swap(button_style& other) noexcept;

        button_style& assign(button_style&& other) noexcept;
        button_style& assign(const button_style& other);

        button_style& set_normal_tint(const color32& value) noexcept;
        button_style& set_pushing_tint(const color32& value) noexcept;
        button_style& set_hovering_tint(const color32& value) noexcept;
        button_style& set_disabled_tint(const color32& value) noexcept;

        [[nodiscard]] const color32& normal_tint() const noexcept;
        [[nodiscard]] const color32& pushing_tint() const noexcept;
        [[nodiscard]] const color32& hovering_tint() const noexcept;
        [[nodiscard]] const color32& disabled_tint() const noexcept;

        button_style& set_normal_sprite(const sprite_asset::ptr& value) noexcept;
        button_style& set_pushing_sprite(const sprite_asset::ptr& value) noexcept;
        button_style& set_hovering_sprite(const sprite_asset::ptr& value) noexcept;
        button_style& set_disabled_sprite(const sprite_asset::ptr& value) noexcept;

        [[nodiscard]] const sprite_asset::ptr& normal_sprite() const noexcept;
        [[nodiscard]] const sprite_asset::ptr& pushing_sprite() const noexcept;
        [[nodiscard]] const sprite_asset::ptr& hovering_sprite() const noexcept;
        [[nodiscard]] const sprite_asset::ptr& disabled_sprite() const noexcept;
    private:
        color32 normal_tint_ = color32::white();
        color32 pushing_tint_ = color32::white();
        color32 hovering_tint_ = color32::white();
        color32 disabled_tint_ = color32::white();

        sprite_asset::ptr normal_sprite_;
        sprite_asset::ptr pushing_sprite_;
        sprite_asset::ptr hovering_sprite_;
        sprite_asset::ptr disabled_sprite_;
    };

    void swap(button_style& l, button_style& r) noexcept;
    bool operator==(const button_style& l, const button_style& r) noexcept;
    bool operator!=(const button_style& l, const button_style& r) noexcept;
}

namespace e2d
{
    class toggle_style final {
    public:
        toggle_style() = default;
        ~toggle_style() noexcept = default;

        toggle_style(toggle_style&& other) noexcept = default;
        toggle_style& operator=(toggle_style&& other) noexcept = default;

        toggle_style(const toggle_style& other) = default;
        toggle_style& operator=(const toggle_style& other) = default;

        void clear() noexcept;
        void swap(toggle_style& other) noexcept;

        toggle_style& assign(toggle_style&& other) noexcept;
        toggle_style& assign(const toggle_style& other);

        toggle_style& set_normal_tint(const color32& value) noexcept;
        toggle_style& set_normal_pushing_tint(const color32& value) noexcept;
        toggle_style& set_normal_hovering_tint(const color32& value) noexcept;
        toggle_style& set_normal_disabled_tint(const color32& value) noexcept;

        [[nodiscard]] const color32& normal_tint() const noexcept;
        [[nodiscard]] const color32& normal_pushing_tint() const noexcept;
        [[nodiscard]] const color32& normal_hovering_tint() const noexcept;
        [[nodiscard]] const color32& normal_disabled_tint() const noexcept;

        toggle_style& set_pressed_tint(const color32& value) noexcept;
        toggle_style& set_pressed_pushing_tint(const color32& value) noexcept;
        toggle_style& set_pressed_hovering_tint(const color32& value) noexcept;
        toggle_style& set_pressed_disabled_tint(const color32& value) noexcept;

        [[nodiscard]] const color32& pressed_tint() const noexcept;
        [[nodiscard]] const color32& pressed_pushing_tint() const noexcept;
        [[nodiscard]] const color32& pressed_hovering_tint() const noexcept;
        [[nodiscard]] const color32& pressed_disabled_tint() const noexcept;

        toggle_style& set_normal_sprite(const sprite_asset::ptr& value) noexcept;
        toggle_style& set_normal_pushing_sprite(const sprite_asset::ptr& value) noexcept;
        toggle_style& set_normal_hovering_sprite(const sprite_asset::ptr& value) noexcept;
        toggle_style& set_normal_disabled_sprite(const sprite_asset::ptr& value) noexcept;

        [[nodiscard]] const sprite_asset::ptr& normal_sprite() const noexcept;
        [[nodiscard]] const sprite_asset::ptr& normal_pushing_sprite() const noexcept;
        [[nodiscard]] const sprite_asset::ptr& normal_hovering_sprite() const noexcept;
        [[nodiscard]] const sprite_asset::ptr& normal_disabled_sprite() const noexcept;

        toggle_style& set_pressed_sprite(const sprite_asset::ptr& value) noexcept;
        toggle_style& set_pressed_pushing_sprite(const sprite_asset::ptr& value) noexcept;
        toggle_style& set_pressed_hovering_sprite(const sprite_asset::ptr& value) noexcept;
        toggle_style& set_pressed_disabled_sprite(const sprite_asset::ptr& value) noexcept;

        [[nodiscard]] const sprite_asset::ptr& pressed_sprite() const noexcept;
        [[nodiscard]] const sprite_asset::ptr& pressed_pushing_sprite() const noexcept;
        [[nodiscard]] const sprite_asset::ptr& pressed_hovering_sprite() const noexcept;
        [[nodiscard]] const sprite_asset::ptr& pressed_disabled_sprite() const noexcept;
    private:
        color32 normal_tint_ = color32::white();
        color32 normal_pushing_tint_ = color32::white();
        color32 normal_hovering_tint_ = color32::white();
        color32 normal_disabled_tint_ = color32::white();

        color32 pressed_tint_ = color32::white();
        color32 pressed_pushing_tint_ = color32::white();
        color32 pressed_hovering_tint_ = color32::white();
        color32 pressed_disabled_tint_ = color32::white();

        sprite_asset::ptr normal_sprite_;
        sprite_asset::ptr normal_pushing_sprite_;
        sprite_asset::ptr normal_hovering_sprite_;
        sprite_asset::ptr normal_disabled_sprite_;
        sprite_asset::ptr pressed_sprite_;

        sprite_asset::ptr pressed_pushing_sprite_;
        sprite_asset::ptr pressed_hovering_sprite_;
        sprite_asset::ptr pressed_disabled_sprite_;
    };

    void swap(toggle_style& l, toggle_style& r) noexcept;
    bool operator==(const toggle_style& l, const toggle_style& r) noexcept;
    bool operator!=(const toggle_style& l, const toggle_style& r) noexcept;
}
