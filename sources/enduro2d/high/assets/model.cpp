/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets/model.hpp>

namespace
{
    using namespace e2d;
}

namespace e2d
{
    model::model() = default;

    model::model(model&& other) noexcept = default;
    model& model::operator=(model&& other) noexcept = default;

    model::model(const model& other) = default;
    model& model::operator=(const model& other) = default;

    void model::swap(model& other) noexcept {
        using std::swap;
        swap(mesh_, other.mesh_);
        swap(materials_, other.materials_);
    }

    model& model::assign(model&& other) noexcept {
        return *this = std::move(other);
    }

    model& model::assign(const model& other) {
        return *this = other;
    }

    model& model::set_mesh(const mesh_asset::ptr& mesh) noexcept {
        mesh_ = mesh;
        return *this;
    }

    model& model::set_material(std::size_t index, const material_asset::ptr& material) {
        if ( materials_.size() <= index ) {
            materials_.resize(index + 1);
        }
        materials_[index] = material;
        return *this;
    }

    model& model::set_materials(vector<material_asset::ptr>&& materials) noexcept {
        materials_ = std::move(materials);
        return *this;
    }

    model& model::set_materials(const vector<material_asset::ptr>& materials) {
        materials_ = materials;
        return *this;
    }

    const mesh_asset::ptr& model::mesh() const noexcept {
        return mesh_;
    }

    const material_asset::ptr& model::material(std::size_t index) const {
        if ( index < materials_.size() ) {
            return materials_[index];
        }
        throw bad_model_access();
    }

    std::size_t model::material_count() const noexcept {
        return materials_.size();
    }
}

namespace e2d
{
    void swap(model& l, model& r) noexcept {
        l.swap(r);
    }

    bool operator==(const model& l, const model& r) noexcept {
        if ( l.mesh() != r.mesh() ) {
            return false;
        }
        if ( l.material_count() != r.material_count() ) {
            return false;
        }
        for ( std::size_t i = 0; i < l.material_count(); ++i ) {
            if ( l.material(i) != r.material(i) ) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const model& l, const model& r) noexcept {
        return !(l == r);
    }
}
