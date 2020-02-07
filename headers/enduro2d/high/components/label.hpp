/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../assets/font_asset.hpp"

namespace e2d
{
    class label final {
    public:
        class dirty final {};
    public:
        ENUM_HPP_CLASS_DECL(haligns, u8,
            (left)
            (center)
            (right))

        ENUM_HPP_CLASS_DECL(valigns, u8,
            (top)
            (center)
            (bottom)
            (baseline))
    public:
        label() = default;
        label(const font_asset::ptr& font);

        label& text(str value) noexcept;
        [[nodiscard]] const str& text() const noexcept;

        label& font(const font_asset::ptr& value) noexcept;
        [[nodiscard]] const font_asset::ptr& font() const noexcept;

        label& tint(const color32& value) noexcept;
        [[nodiscard]] const color32& tint() const noexcept;

        label& halign(haligns value) noexcept;
        [[nodiscard]] haligns halign() const noexcept;

        label& valign(valigns value) noexcept;
        [[nodiscard]] valigns valign() const noexcept;

        label& leading(f32 value) noexcept;
        [[nodiscard]] f32 leading() const noexcept;

        label& tracking(f32 value) noexcept;
        [[nodiscard]] f32 tracking() const noexcept;

        label& text_width(f32 value) noexcept;
        [[nodiscard]] f32 text_width() const noexcept;

        label& glyph_dilate(f32 value) noexcept;
        [[nodiscard]] f32 glyph_dilate() const noexcept;

        label& outline_width(f32 value) noexcept;
        [[nodiscard]] f32 outline_width() const noexcept;

        label& outline_color(const color32& value) noexcept;
        [[nodiscard]] const color32& outline_color() const noexcept;
    private:
        str text_;
        font_asset::ptr font_;
        color32 tint_ = color32::white();
        haligns halign_ = haligns::center;
        valigns valign_ = valigns::baseline;
        f32 leading_ = 1.f;
        f32 tracking_ = 0.f;
        f32 text_width_ = 0.f;
        f32 glyph_dilate_ = 0.f;
        f32 outline_width_ = 0.f;
        color32 outline_color_ = color32::white();
    };
}

ENUM_HPP_REGISTER_TRAITS(e2d::label::haligns)
ENUM_HPP_REGISTER_TRAITS(e2d::label::valigns)

namespace e2d
{
    template <>
    class factory_loader<label> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            label& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<label::dirty> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            label::dirty& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<label> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<label>& c) const;
    };
}

namespace e2d
{
    inline label::label(const font_asset::ptr& value)
    : font_(value) {}

    inline label& label::text(str value) noexcept {
        text_ = std::move(value);
        return *this;
    }

    inline const str& label::text() const noexcept {
        return text_;
    }

    inline label& label::font(const font_asset::ptr& value) noexcept {
        font_ = value;
        return *this;
    }

    inline const font_asset::ptr& label::font() const noexcept {
        return font_;
    }

    inline label& label::tint(const color32& value) noexcept {
        tint_ = value;
        return *this;
    }

    inline const color32& label::tint() const noexcept {
        return tint_;
    }

    inline label& label::halign(haligns value) noexcept {
        halign_ = value;
        return *this;
    }

    inline label::haligns label::halign() const noexcept {
        return halign_;
    }

    inline label& label::valign(valigns value) noexcept {
        valign_ = value;
        return *this;
    }

    inline label::valigns label::valign() const noexcept {
        return valign_;
    }

    inline label& label::leading(f32 value) noexcept {
        leading_ = value;
        return *this;
    }

    inline f32 label::leading() const noexcept {
        return leading_;
    }

    inline label& label::tracking(f32 value) noexcept {
        tracking_ = value;
        return *this;
    }

    inline f32 label::tracking() const noexcept {
        return tracking_;
    }

    inline label& label::text_width(f32 value) noexcept {
        text_width_ = value;
        return *this;
    }

    inline f32 label::text_width() const noexcept {
        return text_width_;
    }

    inline label& label::glyph_dilate(f32 value) noexcept {
        glyph_dilate_ = value;
        return *this;
    }

    inline f32 label::glyph_dilate() const noexcept {
        return glyph_dilate_;
    }

    inline label& label::outline_width(f32 value) noexcept {
        outline_width_ = value;
        return *this;
    }

    inline f32 label::outline_width() const noexcept {
        return outline_width_;
    }

    inline label& label::outline_color(const color32& value) noexcept {
        outline_color_ = value;
        return *this;
    }

    inline const color32& label::outline_color() const noexcept {
        return outline_color_;
    }
}

namespace e2d::labels
{
    gcomponent<label> mark_dirty(gcomponent<label> self);
    gcomponent<label> unmark_dirty(gcomponent<label> self);
    bool is_dirty(const const_gcomponent<label>& self) noexcept;

    gcomponent<label> change_text(gcomponent<label> self, str value);
    gcomponent<label> change_font(gcomponent<label> self, const font_asset::ptr& value);
    gcomponent<label> change_tint(gcomponent<label> self, const color32& value);
    gcomponent<label> change_halign(gcomponent<label> self, label::haligns value);
    gcomponent<label> change_valign(gcomponent<label> self, label::valigns value);
    gcomponent<label> change_leading(gcomponent<label> self, f32 value);
    gcomponent<label> change_tracking(gcomponent<label> self, f32 value);
    gcomponent<label> change_text_width(gcomponent<label> self, f32 value);
    gcomponent<label> change_glyph_dilate(gcomponent<label> self, f32 value);
    gcomponent<label> change_outline_width(gcomponent<label> self, f32 value);
    gcomponent<label> change_outline_color(gcomponent<label> self, const color32& value);
}
