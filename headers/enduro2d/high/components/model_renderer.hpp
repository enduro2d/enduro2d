/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../assets/model_asset.hpp"

namespace e2d
{
    class model_renderer final {
    public:
        model_renderer() = default;
        model_renderer(const model_asset::ptr& model);

        model_renderer& model(const model_asset::ptr& value) noexcept;
        const model_asset::ptr& model() const noexcept;
    private:
        model_asset::ptr model_;
    };
}

namespace e2d
{
    inline model_renderer::model_renderer(const model_asset::ptr& model)
    : model_(model) {}

    inline model_renderer& model_renderer::model(const model_asset::ptr& value) noexcept {
        model_ = value;
        return *this;
    }

    inline const model_asset::ptr& model_renderer::model() const noexcept {
        return model_;
    }
}
