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
        label() = default;
        label(str32_view text);

        label& text(str32_view text);
        const str32& text() const noexcept;

        label& dirty(bool dirty) noexcept;
        bool dirty() const noexcept;

        label& font(const font_asset::ptr& font) noexcept;
        const font_asset::ptr&  font() const noexcept;

        label& tint(const color32& value) noexcept;
        const color32& tint() const noexcept;
    private:
        bool dirty_ = false;
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
}

namespace e2d
{
    inline label::label(str32_view text)
    : dirty_(false)
    {
        text_ = make_utf32(text);
    }

    inline label& label::text(str32_view text) {
        text_ = text;
        dirty_ = true;
        return *this;
    }

    inline const str32& label::text() const noexcept {
        return text_;
    }

    inline label& label::dirty(bool dirty) noexcept {
        dirty_ = dirty;
        return *this;
    }

    inline bool label::dirty() const noexcept {
        return dirty_;
    }

    inline label& label::font(const font_asset::ptr& font) noexcept {
        font_ = font;
        dirty_ = true;
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
}
