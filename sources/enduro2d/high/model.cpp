/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/model.hpp>

namespace
{
    using namespace e2d;

    const vertex_declaration vertex_buffer_decl = vertex_declaration()
        .add_attribute<v3f>("a_vertex");

    const vertex_declaration uv_buffer_decls[] = {
        vertex_declaration().add_attribute<v2f>("a_st0"),
        vertex_declaration().add_attribute<v2f>("a_st1"),
        vertex_declaration().add_attribute<v2f>("a_st2"),
        vertex_declaration().add_attribute<v2f>("a_st3")};

    const vertex_declaration color_buffer_decls[] = {
        vertex_declaration().add_attribute<color32>("a_color0").normalized(),
        vertex_declaration().add_attribute<color32>("a_color1").normalized(),
        vertex_declaration().add_attribute<color32>("a_color2").normalized(),
        vertex_declaration().add_attribute<color32>("a_color3").normalized()};

    const vertex_declaration normal_buffer_decl = vertex_declaration()
        .add_attribute<v3f>("a_normal");

    const vertex_declaration tangent_buffer_decl = vertex_declaration()
        .add_attribute<v3f>("a_tangent");

    const vertex_declaration bitangent_buffer_decl = vertex_declaration()
        .add_attribute<v3f>("a_bitangent");

    render::geometry make_geometry(const mesh& mesh) {
        render::geometry geo;

        {
            std::size_t index_count{0u};
            for ( std::size_t i = 0; i < mesh.indices_submesh_count(); ++i ) {
                index_count += mesh.indices(i).size();
            }

            if ( index_count > 0 ) {
                vector<u32> indices;
                indices.reserve(index_count);

                for ( std::size_t i = 0; i < mesh.indices_submesh_count(); ++i ) {
                    indices.insert(indices.end(), mesh.indices(i).begin(), mesh.indices(i).end());
                }

                const index_buffer_ptr index_buffer = the<render>().create_index_buffer(
                    buffer(indices.data(), indices.size() * sizeof(indices[0])),
                    index_declaration::index_type::unsigned_int,
                    index_buffer::usage::static_draw);

                if ( index_buffer ) {
                    geo.indices(index_buffer);
                }
            }
        }

        {
            const vector<v3f>& vertices = mesh.vertices();
            const vertex_buffer_ptr vertex_buffer = the<render>().create_vertex_buffer(
                buffer(vertices.data(), vertices.size() * sizeof(vertices[0])),
                vertex_buffer_decl,
                vertex_buffer::usage::static_draw);
            if ( vertex_buffer ) {
                geo.add_vertices(vertex_buffer);
            }
        }

        {
            const std::size_t uv_count = math::min(
                mesh.uvs_channel_count(),
                E2D_COUNTOF(uv_buffer_decls));
            for ( std::size_t i = 0; i < uv_count; ++i ) {
                const vector<v2f>& uvs = mesh.uvs(i);
                const vertex_buffer_ptr uv_buffer = the<render>().create_vertex_buffer(
                    buffer(uvs.data(), uvs.size() * sizeof(uvs[0])),
                    uv_buffer_decls[i],
                    vertex_buffer::usage::static_draw);
                if ( uv_buffer ) {
                    geo.add_vertices(uv_buffer);
                }
            }
        }

        {
            const std::size_t color_count = math::min(
                mesh.colors_channel_count(),
                E2D_COUNTOF(uv_buffer_decls));
            for ( std::size_t i = 0; i < color_count; ++i ) {
                const vector<color32>& colors = mesh.colors(i);
                const vertex_buffer_ptr color_buffer = the<render>().create_vertex_buffer(
                    buffer(colors.data(), colors.size() * sizeof(colors[0])),
                    color_buffer_decls[i],
                    vertex_buffer::usage::static_draw);
                if ( color_buffer ) {
                    geo.add_vertices(color_buffer);
                }
            }
        }

        {
            const vector<v3f>& normals = mesh.normals();
            const vertex_buffer_ptr normal_buffer = the<render>().create_vertex_buffer(
                buffer(normals.data(), normals.size() * sizeof(normals[0])),
                normal_buffer_decl,
                vertex_buffer::usage::static_draw);
            if ( normal_buffer ) {
                geo.add_vertices(normal_buffer);
            }
        }

        {
            const vector<v3f>& tangents = mesh.tangents();
            const vertex_buffer_ptr tangent_buffer = the<render>().create_vertex_buffer(
                buffer(tangents.data(), tangents.size() * sizeof(tangents[0])),
                tangent_buffer_decl,
                vertex_buffer::usage::static_draw);
            if ( tangent_buffer ) {
                geo.add_vertices(tangent_buffer);
            }
        }

        {
            const vector<v3f>& bitangents = mesh.bitangents();
            const vertex_buffer_ptr bitangent_buffer = the<render>().create_vertex_buffer(
                buffer(bitangents.data(), bitangents.size() * sizeof(bitangents[0])),
                bitangent_buffer_decl,
                vertex_buffer::usage::static_draw);
            if ( bitangent_buffer ) {
                geo.add_vertices(bitangent_buffer);
            }
        }

        return geo;
    }
}

namespace e2d
{
    model::model() = default;
    model::~model() noexcept = default;

    model::model(model&& other) noexcept {
        assign(std::move(other));
    }

    model& model::operator=(model&& other) noexcept {
        return assign(std::move(other));
    }

    model::model(const model& other) {
        assign(other);
    }

    model& model::operator=(const model& other) {
        return assign(other);
    }

    void model::clear() noexcept {
        mesh_.reset();
        geometry_.reset();
        materials_.clear();
    }

    void model::swap(model& other) noexcept {
        using std::swap;
        swap(mesh_, other.mesh_);
        swap(geometry_, other.geometry_);
        swap(materials_, other.materials_);
    }

    model& model::assign(model&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    model& model::assign(const model& other) {
        if ( this != &other ) {
            model m;
            m.mesh_ = other.mesh_;
            m.geometry_ = other.geometry_;
            m.materials_ = other.materials_;
            swap(m);
        }
        return *this;
    }

    model& model::set_mesh(const mesh_asset::ptr& mesh) {
        mesh_ = mesh;
        geometry_.reset();
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

    void model::regenerate_geometry() {
        if ( mesh_ ) {
            geometry_ = make_geometry(mesh_->content());
        } else {
            geometry_.reset();
        }
    }

    const render::geometry& model::geometry() const noexcept {
        return geometry_;
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
        if ( l.geometry() != r.geometry() ) {
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
