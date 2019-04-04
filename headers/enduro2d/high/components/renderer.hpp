/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../assets/material_asset.hpp"

namespace e2d
{
    class renderer final {
    public:
        renderer() = default;

        renderer& enabled(bool value) noexcept;
        bool enabled() const noexcept;

        renderer& properties(render::property_block&& value) noexcept;
        renderer& properties(const render::property_block& value);

        render::property_block& properties() noexcept;
        const render::property_block& properties() const noexcept;

        renderer& materials(vector<material_asset::ptr>&& value) noexcept;
        renderer& materials(const vector<material_asset::ptr>& value);

        vector<material_asset::ptr>& materials() noexcept;
        const vector<material_asset::ptr>& materials() const noexcept;
    private:
        bool enabled_ = true;
        render::property_block properties_;
        vector<material_asset::ptr> materials_;
    };
}

namespace e2d
{
    inline renderer& renderer::enabled(bool value) noexcept {
        enabled_ = value;
        return *this;
    }

    inline bool renderer::enabled() const noexcept {
        return enabled_;
    }

    inline renderer& renderer::properties(render::property_block&& value) noexcept {
        properties_ = std::move(value);
        return *this;
    }

    inline renderer& renderer::properties(const render::property_block& value) {
        properties_ = value;
        return *this;
    }

    inline render::property_block& renderer::properties() noexcept {
        return properties_;
    }

    inline const render::property_block& renderer::properties() const noexcept {
        return properties_;
    }

    inline renderer& renderer::materials(vector<material_asset::ptr>&& value) noexcept {
        materials_ = std::move(value);
        return *this;
    }

    inline renderer& renderer::materials(const vector<material_asset::ptr>& value) {
        materials_ = value;
        return *this;
    }

    inline vector<material_asset::ptr>& renderer::materials() noexcept {
        return materials_;
    }

    inline const vector<material_asset::ptr>& renderer::materials() const noexcept {
        return materials_;
    }
}
