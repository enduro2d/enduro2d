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

        label& filtering(bool value) noexcept;
        bool filtering() const noexcept;
    private:
        str32 text_;
        font_asset::ptr font_;
        color32 tint_ = color32::white();
        f32 width_ = 0.f;
        haligns haligh_ = haligns::left;
        valigns valign_ = valigns::baseline;
        bool filtering_ = true;
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
        haligh_ = value;
        return *this;
    }

    inline label::haligns label::halign() const noexcept {
        return haligh_;
    }

    inline label& label::valigh(valigns value) noexcept {
        valign_ = value;
        return *this;
    }

    inline label::valigns label::valign() const noexcept {
        return valign_;
    }

    inline label& label::filtering(bool value) noexcept {
        filtering_ = value;
        return *this;
    }

    inline bool label::filtering() const noexcept {
        return filtering_;
    }
}
