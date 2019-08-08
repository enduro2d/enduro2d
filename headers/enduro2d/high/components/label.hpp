/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"
#include "../assets/font_asset.hpp"

namespace e2d
{
    class label final {
    public:
        class dirty final {
        };
    public:
        enum class haligns {
            left,
            center,
            right
        };

        enum class valigns {
            top,
            center,
            bottom,
            baseline
        };
    public:
        label() = default;
        label(const font_asset::ptr& font);

        label& text(str32 value) noexcept;
        [[nodiscard]] const str32& text() const noexcept;

        label& font(const font_asset::ptr& value) noexcept;
        [[nodiscard]] const font_asset::ptr& font() const noexcept;

        label& tint(const color32& value) noexcept;
        [[nodiscard]] const color32& tint() const noexcept;

        label& width(f32 value) noexcept;
        [[nodiscard]] f32 width() const noexcept;

        label& haligh(haligns value) noexcept;
        [[nodiscard]] haligns halign() const noexcept;

        label& valigh(valigns value) noexcept;
        [[nodiscard]] valigns valign() const noexcept;

        label& char_width(f32 value) noexcept;
        f32 char_width() const noexcept;

        label& char_edge(f32 value) noexcept;
        f32 char_edge() const noexcept;

        label& outline_width(f32 value) noexcept;
        f32 outline_width() const noexcept;

        label& outline_edge(f32 value) noexcept;
        f32 outline_edge() const noexcept;

        label& outline_color(const color32& value) noexcept;
        color32 outline_color() const noexcept;

        label& shadow_width(f32 value) noexcept;
        f32 shadow_width() const noexcept;

        label& shadow_edge(f32 value) noexcept;
        f32 shadow_edge() const noexcept;

        label& shadow_offset(const v2f& value) noexcept;
        v2f shadow_offset() const noexcept;

        label& shadow_color(const color32& value) noexcept;
        color32 shadow_color() const noexcept;
    private:
        str32 text_;
        font_asset::ptr font_;
        color32 tint_ = color32::white();
        f32 width_ = 0.f;
        haligns halign_ = haligns::left;
        valigns valign_ = valigns::baseline;
        f32 char_width_ = 0.f;
        f32 char_edge_ = 0.f;
        f32 outline_width_ = 0.5f;
        f32 outline_edge_ = 0.1f;
        color32 outline_color_ = color32::white();
        f32 shadow_width_ = 0.f;
        f32 shadow_edge_ = 0.f;
        v2f shadow_offset_ = v2f::zero();
        color32 shadow_color_ = color32::white();
    };

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
    inline label::label(const font_asset::ptr& value)
    : font_(value) {}

    inline label& label::text(str32 value) noexcept {
        text_ = std::move(value);
        return *this;
    }

    inline const str32& label::text() const noexcept {
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

    inline label& label::width(f32 value) noexcept {
        width_ = value;
        return *this;
    }

    inline f32 label::width() const noexcept {
        return width_;
    }

    inline label& label::haligh(haligns value) noexcept {
        halign_ = value;
        return *this;
    }

    inline label::haligns label::halign() const noexcept {
        return halign_;
    }

    inline label& label::valigh(valigns value) noexcept {
        valign_ = value;
        return *this;
    }

    inline label::valigns label::valign() const noexcept {
        return valign_;
    }

    inline label& label::char_width(f32 value) noexcept {
        char_width_ = value;
        return *this;
    }

    inline f32 label::char_width() const noexcept {
        return char_width_;
    }

    inline label& label::char_edge(f32 value) noexcept {
        char_edge_ = value;
        return *this;
    }

    inline f32 label::char_edge() const noexcept {
        return char_edge_;
    }

    inline label& label::outline_width(f32 value) noexcept {
        outline_width_ = value;
        return *this;
    }

    inline f32 label::outline_width() const noexcept {
        return outline_width_;
    }

    inline label& label::outline_edge(f32 value) noexcept {
        outline_edge_ = value;
        return *this;
    }

    inline f32 label::outline_edge() const noexcept {
        return outline_edge_;
    }

    inline label& label::outline_color(const color32& value) noexcept {
        outline_color_ = value;
        return *this;
    }

    inline color32 label::outline_color() const noexcept {
        return outline_color_;
    }

    inline label& label::shadow_width(f32 value) noexcept {
        shadow_width_ = value;
        return *this;
    }

    inline f32 label::shadow_width() const noexcept {
        return shadow_width_;
    }

    inline label& label::shadow_edge(f32 value) noexcept {
        shadow_edge_ = value;
        return *this;
    }

    inline f32 label::shadow_edge() const noexcept {
        return shadow_edge_;
    }

    inline label& label::shadow_offset(const v2f& value) noexcept {
        shadow_offset_ = value;
        return *this;
    }

    inline v2f label::shadow_offset() const noexcept {
        return shadow_offset_;
    }

    inline label& label::shadow_color(const color32& value) noexcept {
        shadow_color_ = value;
        return *this;
    }

    inline color32 label::shadow_color() const noexcept {
        return shadow_color_;
    }
}
