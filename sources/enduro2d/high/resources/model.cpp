/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/resources/model.hpp>

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

    render::geometry make_geometry(render& render, const mesh& mesh) {
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

                const index_buffer_ptr index_buffer = render.create_index_buffer(
                    indices,
                    index_declaration::index_type::unsigned_int,
                    index_buffer::usage::static_draw);

                if ( index_buffer ) {
                    geo.indices(index_buffer);
                }
            }
        }

        {
            const vector<v3f>& vertices = mesh.vertices();
            const vertex_buffer_ptr vertex_buffer = render.create_vertex_buffer(
                vertices,
                vertex_buffer_decl,
                vertex_buffer::usage::static_draw);
            if ( vertex_buffer ) {
                geo.add_vertices(vertex_buffer);
            }
        }

        {
            const std::size_t uv_count = math::min(
                mesh.uvs_channel_count(),
                std::size(uv_buffer_decls));
            for ( std::size_t i = 0; i < uv_count; ++i ) {
                const vector<v2f>& uvs = mesh.uvs(i);
                const vertex_buffer_ptr uv_buffer = render.create_vertex_buffer(
                    uvs,
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
                std::size(uv_buffer_decls));
            for ( std::size_t i = 0; i < color_count; ++i ) {
                const vector<color32>& colors = mesh.colors(i);
                const vertex_buffer_ptr color_buffer = render.create_vertex_buffer(
                    colors,
                    color_buffer_decls[i],
                    vertex_buffer::usage::static_draw);
                if ( color_buffer ) {
                    geo.add_vertices(color_buffer);
                }
            }
        }

        {
            const vector<v3f>& normals = mesh.normals();
            const vertex_buffer_ptr normal_buffer = render.create_vertex_buffer(
                normals,
                normal_buffer_decl,
                vertex_buffer::usage::static_draw);
            if ( normal_buffer ) {
                geo.add_vertices(normal_buffer);
            }
        }

        {
            const vector<v3f>& tangents = mesh.tangents();
            const vertex_buffer_ptr tangent_buffer = render.create_vertex_buffer(
                tangents,
                tangent_buffer_decl,
                vertex_buffer::usage::static_draw);
            if ( tangent_buffer ) {
                geo.add_vertices(tangent_buffer);
            }
        }

        {
            const vector<v3f>& bitangents = mesh.bitangents();
            const vertex_buffer_ptr bitangent_buffer = render.create_vertex_buffer(
                bitangents,
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
        geometry_.clear();
    }

    void model::swap(model& other) noexcept {
        using std::swap;
        swap(mesh_, other.mesh_);
        swap(geometry_, other.geometry_);
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
            swap(m);
        }
        return *this;
    }

    model& model::set_mesh(const mesh_asset::ptr& mesh) {
        mesh_ = mesh;
        geometry_.clear();
        return *this;
    }

    const mesh_asset::ptr& model::mesh() const noexcept {
        return mesh_;
    }

    void model::regenerate_geometry(render& render) {
        if ( mesh_ ) {
            geometry_ = make_geometry(render, mesh_->content());
        } else {
            geometry_.clear();
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
        return l.mesh() == r.mesh()
            && l.geometry() == r.geometry();
    }

    bool operator!=(const model& l, const model& r) noexcept {
        return !(l == r);
    }
}
