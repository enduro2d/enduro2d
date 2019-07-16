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
        label(const str_view text);

        label& text(const str_view text) noexcept;
        const str& text() const noexcept;

        label& dirty(bool dirty) noexcept;
        bool dirty() const noexcept;

        label& font(const font_asset::ptr& font) noexcept;
        const font_asset::ptr&  font() const noexcept;
    private:
        bool dirty_ = false;
        str text_;
        font_asset::ptr font_;
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
    inline label::label(const str_view text)
    : text_(text),
    dirty_(false)
    {}

    inline label& label::text(const str_view text) noexcept {
        text_ = text;
        dirty_ = true;
        return *this;
    }

    inline const str& label::text() const noexcept {
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
}
