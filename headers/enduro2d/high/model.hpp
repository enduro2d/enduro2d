/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "assets/mesh_asset.hpp"
#include "assets/material_asset.hpp"

namespace e2d
{
    class bad_model_access final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad model access";
        }
    };

    class model final {
    public:
        model();
        ~model() noexcept;

        model(model&& other) noexcept;
        model& operator=(model&& other) noexcept;

        model(const model& other);
        model& operator=(const model& other);

        void clear() noexcept;
        void swap(model& other) noexcept;

        model& assign(model&& other) noexcept;
        model& assign(const model& other);

        model& set_mesh(const mesh_asset::ptr& mesh);
        model& set_material(std::size_t index, const material_asset::ptr& material);

        model& set_materials(vector<material_asset::ptr>&& materials) noexcept;
        model& set_materials(const vector<material_asset::ptr>& materials);

        const mesh_asset::ptr& mesh() const noexcept;
        const material_asset::ptr& material(std::size_t index) const;
        std::size_t material_count() const noexcept;

        // It can only be called from the main thread
        void regenerate_geometry();
        const render::geometry& geometry() const noexcept;
    private:
        mesh_asset::ptr mesh_;
        render::geometry geometry_;
        vector<material_asset::ptr> materials_;
    };

    void swap(model& l, model& r) noexcept;
    bool operator==(const model& l, const model& r) noexcept;
    bool operator!=(const model& l, const model& r) noexcept;
}
