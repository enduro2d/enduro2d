/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../assets/flipbook_asset.hpp"

namespace e2d
{
    class flipbook_controller final {
    public:
        flipbook_controller() = default;
        flipbook_controller(const flipbook_asset::ptr& flipbook);

        flipbook_controller& flipbook(const flipbook_asset::ptr& value) noexcept;
        const flipbook_asset::ptr& flipbook() const noexcept;
    private:
        flipbook_asset::ptr flipbook_;
    };
}

namespace e2d
{
    inline flipbook_controller::flipbook_controller(const flipbook_asset::ptr& flipbook)
    : flipbook_(flipbook) {}

    inline flipbook_controller& flipbook_controller::flipbook(const flipbook_asset::ptr& value) noexcept {
        flipbook_ = value;
        return *this;
    }

    inline const flipbook_asset::ptr& flipbook_controller::flipbook() const noexcept {
        return flipbook_;
    }
}
