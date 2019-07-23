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
        class label_dirty final {};

        label() = default;
        
        label& text(str32_view text);
        [[nodiscard]] const str32& text() const noexcept;

        label& font(const font_asset::ptr& font) noexcept;
        [[nodiscard]] const font_asset::ptr&  font() const noexcept;

        label& tint(const color32& value) noexcept;
        [[nodiscard]] const color32& tint() const noexcept;

        label& pivot(const v2f& pivot) noexcept;
        [[nodiscard]] const v2f& pivot() const noexcept;
    private:
        v2f pivot_ = v2f::zero();
        str32 text_;
        font_asset::ptr font_;
        color32 tint_ = color32::white();
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
    class factory_loader<label::label_dirty> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            label::label_dirty& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    inline label& label::text(str32_view text) {
        text_ = text;
        return *this;
    }

    [[nodiscard]] inline const str32& label::text() const noexcept {
        return text_;
    }

    inline label& label::font(const font_asset::ptr& font) noexcept {
        font_ = font;
        return *this;
    }

    [[nodiscard]] inline const font_asset::ptr& label::font() const noexcept {
        return font_;
    }

    inline label& label::tint(const color32& value) noexcept {
        tint_ = value;
        return *this;
    }

    [[nodiscard]] inline const color32& label::tint() const noexcept {
        return tint_;
    }

    inline label& label::pivot(const v2f& pivot) noexcept {
        pivot_ = pivot;
        return *this;
    }

    [[nodiscard]] inline const v2f& label::pivot() const noexcept {
        return pivot_;
    }
}
