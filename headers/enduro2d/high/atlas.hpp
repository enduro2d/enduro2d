/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "assets/texture_asset.hpp"

namespace e2d
{
    class atlas final {
    public:
        struct region {
            str_hash name;
            v2f pivot;
            b2f texrect;
        };

        struct shape_region {
            str_hash name;
            v2f pivot;
            vector<v2f> points;
        };
    public:
        atlas();
        ~atlas() noexcept;

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

        atlas& set_regions(vector<region>&& regions) noexcept;
        atlas& set_regions(const vector<region>& regions);
        const vector<region>& regions() const noexcept;
        const region* find_region(str_hash name) const noexcept;

        atlas& set_shape_regions(vector<shape_region>&& regions) noexcept;
        atlas& set_shape_regions(const vector<shape_region>& regions);
        const vector<shape_region>& shape_regions() const noexcept;
        const shape_region* find_shape_region(str_hash name) const noexcept;
    private:
        texture_asset::ptr texture_;
        vector<region> regions_;
        vector<shape_region> shape_regions_;
    };

    void swap(atlas& l, atlas& r) noexcept;
    bool operator==(const atlas& l, const atlas& r) noexcept;
    bool operator!=(const atlas& l, const atlas& r) noexcept;

    void swap(atlas::region& l, atlas::region& r) noexcept;
    bool operator==(const atlas::region& l, const atlas::region& r) noexcept;
    bool operator!=(const atlas::region& l, const atlas::region& r) noexcept;

    void swap(atlas::shape_region& l, atlas::shape_region& r) noexcept;
    bool operator==(const atlas::shape_region& l, const atlas::shape_region& r) noexcept;
    bool operator!=(const atlas::shape_region& l, const atlas::shape_region& r) noexcept;
}
