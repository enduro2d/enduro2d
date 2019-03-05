/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d
{
    class renderer final {
    public:
        renderer() = default;

        renderer& enabled(bool value) noexcept;
        bool enabled() const noexcept;

        render::property_block& properties() noexcept;
        const render::property_block& properties() const noexcept;
    private:
        bool enabled_ = true;
        render::property_block properties_;
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

    inline render::property_block& renderer::properties() noexcept {
        return properties_;
    }

    inline const render::property_block& renderer::properties() const noexcept {
        return properties_;
    }
}
