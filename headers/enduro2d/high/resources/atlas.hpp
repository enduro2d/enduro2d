/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../assets/texture_asset.hpp"

namespace e2d
{
    class atlas final {
    public:
        atlas() = default;
        ~atlas() noexcept = default;

        atlas(atlas&& other) noexcept;
        atlas& operator=(atlas&& other) noexcept;

        atlas(const atlas& other);
        atlas& operator=(const atlas& other);

        void clear() noexcept;
        void swap(atlas& other) noexcept;

        atlas& assign(atlas&& other) noexcept;
        atlas& assign(const atlas& other);

        atlas& set_texture(const texture_asset::ptr& texture) noexcept;
        const texture_asset::ptr& texture() const noexcept;
    private:
        texture_asset::ptr texture_;
    };

    void swap(atlas& l, atlas& r) noexcept;
    bool operator==(const atlas& l, const atlas& r) noexcept;
    bool operator!=(const atlas& l, const atlas& r) noexcept;
}
