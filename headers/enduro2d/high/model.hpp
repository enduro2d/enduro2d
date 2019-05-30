/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "assets/mesh_asset.hpp"

namespace e2d
{
    class model final {
    public:
        model() = default;
        ~model() noexcept = default;

        model(model&& other) noexcept;
        model& operator=(model&& other) noexcept;

        model(const model& other);
        model& operator=(const model& other);

        void clear() noexcept;
        void swap(model& other) noexcept;

        model& assign(model&& other) noexcept;
        model& assign(const model& other);

        model& set_mesh(const mesh_asset::ptr& mesh);
        const mesh_asset::ptr& mesh() const noexcept;

        // It can only be called from the main thread
        void regenerate_geometry(render& render);
        const render::geometry& geometry() const noexcept;
    private:
        mesh_asset::ptr mesh_;
        render::geometry geometry_;
    };

    void swap(model& l, model& r) noexcept;
    bool operator==(const model& l, const model& r) noexcept;
    bool operator!=(const model& l, const model& r) noexcept;
}
