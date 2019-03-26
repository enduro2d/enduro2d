/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "mesh_impl/mesh_impl.hpp"

namespace e2d
{
    mesh::mesh(mesh&& other) noexcept {
        assign(std::move(other));
    }

    mesh& mesh::operator=(mesh&& other) noexcept {
        return assign(std::move(other));
    }

    mesh::mesh(const mesh& other) {
        assign(other);
    }

    mesh& mesh::operator=(const mesh& other) {
        return assign(other);
    }

    mesh& mesh::assign(mesh&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    mesh& mesh::assign(const mesh& other) {
        if ( this != &other ) {
            mesh m;
            m.uvs_channels_ = other.uvs_channels_;
            m.colors_channels_ = other.colors_channels_;
            m.vertices_ = other.vertices_;
            m.indices_submeshes_ = other.indices_submeshes_;
            m.normals_ = other.normals_;
            m.tangents_ = other.tangents_;
            m.bitangents_ = other.bitangents_;
            swap(m);
        }
        return *this;
    }

    void mesh::swap(mesh& other) noexcept {
        using std::swap;
        swap(uvs_channels_, other.uvs_channels_);
        swap(colors_channels_, other.colors_channels_);
        swap(vertices_, other.vertices_);
        swap(indices_submeshes_, other.indices_submeshes_);
        swap(normals_, other.normals_);
        swap(tangents_, other.tangents_);
        swap(bitangents_, other.bitangents_);
    }

    void mesh::clear() noexcept {
        uvs_channels_.clear();
        colors_channels_.clear();
        vertices_.clear();
        indices_submeshes_.clear();
        normals_.clear();
        tangents_.clear();
        bitangents_.clear();
    }

    bool mesh::empty() const noexcept {
        return vertices_.empty();
    }

    mesh& mesh::set_uvs(std::size_t channel, vector<v2f>&& uvs) {
        if ( uvs_channels_.size() <= channel ) {
            uvs_channels_.resize(channel + 1);
        }
        uvs_channels_[channel] = std::move(uvs);
        return *this;
    }

    mesh& mesh::set_uvs(std::size_t channel, const vector<v2f>& uvs) {
        return set_uvs(channel, uvs.data(), uvs.size());
    }

    mesh& mesh::set_uvs(std::size_t channel, const v2f* uvs, std::size_t count) {
        return count
            ? set_uvs(channel, uvs
                ? vector<v2f>(uvs, uvs + count)
                : vector<v2f>(count))
            : set_uvs(channel, vector<v2f>());
    }

    mesh& mesh::set_colors(std::size_t channel, vector<color32>&& colors) {
        if ( colors_channels_.size() <= channel ) {
            colors_channels_.resize(channel + 1);
        }
        colors_channels_[channel] = std::move(colors);
        return *this;
    }

    mesh& mesh::set_colors(std::size_t channel, const vector<color32>& colors) {
        return set_colors(channel, colors.data(), colors.size());
    }

    mesh& mesh::set_colors(std::size_t channel, const color32* colors, std::size_t count) {
        return count
            ? set_colors(channel, colors
                ? vector<color32>(colors, colors + count)
                : vector<color32>(count))
            : set_colors(channel, vector<color32>());
    }

    mesh& mesh::set_vertices(vector<v3f>&& vertices) noexcept {
        vertices_ = std::move(vertices);
        return *this;
    }

    mesh& mesh::set_vertices(const vector<v3f>& vertices) {
        return set_vertices(vertices.data(), vertices.size());
    }

    mesh& mesh::set_vertices(const v3f* vertices, std::size_t count) {
        return count
            ? set_vertices(vertices
                ? vector<v3f>(vertices, vertices + count)
                : vector<v3f>(count))
            : set_vertices(vector<v3f>());
    }

    mesh& mesh::set_indices(std::size_t submesh, vector<u32>&& indices) {
        if ( indices_submeshes_.size() <= submesh ) {
            indices_submeshes_.resize(submesh + 1);
        }
        indices_submeshes_[submesh] = std::move(indices);
        return *this;
    }

    mesh& mesh::set_indices(std::size_t submesh, const vector<u32>& indices) {
        return set_indices(submesh, indices.data(), indices.size());
    }

    mesh& mesh::set_indices(std::size_t submesh, const u32* indices, std::size_t count) {
        return count
            ? set_indices(submesh, indices
                ? vector<u32>(indices, indices + count)
                : vector<u32>(count))
            : set_indices(submesh, vector<u32>());
    }

    mesh& mesh::set_normals(vector<v3f>&& normals) noexcept {
        normals_ = std::move(normals);
        return *this;
    }

    mesh& mesh::set_normals(const vector<v3f>& normals) {
        return set_normals(normals.data(), normals.size());
    }

    mesh& mesh::set_normals(const v3f* normals, std::size_t count) {
        return count
            ? set_normals(normals
                ? vector<v3f>(normals, normals + count)
                : vector<v3f>(count))
            : set_normals(vector<v3f>());
    }

    mesh& mesh::set_tangents(vector<v3f>&& tangents) noexcept {
        tangents_ = std::move(tangents);
        return *this;
    }

    mesh& mesh::set_tangents(const vector<v3f>& tangents) {
        return set_tangents(tangents.data(), tangents.size());
    }

    mesh& mesh::set_tangents(const v3f* tangents, std::size_t count) {
        return count
            ? set_tangents(tangents
                ? vector<v3f>(tangents, tangents + count)
                : vector<v3f>(count))
            : set_tangents(vector<v3f>());
    }

    mesh& mesh::set_bitangents(vector<v3f>&& bitangents) noexcept {
        bitangents_ = std::move(bitangents);
        return *this;
    }

    mesh& mesh::set_bitangents(const vector<v3f>& bitangents) {
        return set_bitangents(bitangents.data(), bitangents.size());
    }

    mesh& mesh::set_bitangents(const v3f* bitangents, std::size_t count) {
        return count
            ? set_bitangents(bitangents
                ? vector<v3f>(bitangents, bitangents + count)
                : vector<v3f>(count))
            : set_bitangents(vector<v3f>());
    }

    const vector<v2f>& mesh::uvs(std::size_t channel) const {
        if ( channel < uvs_channels_.size() ) {
            return uvs_channels_[channel];
        }
        throw bad_mesh_access();
    }

    std::size_t mesh::uvs_channel_count() const noexcept {
        return uvs_channels_.size();
    }

    const vector<color32>& mesh::colors(std::size_t channel) const {
        if ( channel < colors_channels_.size() ) {
            return colors_channels_[channel];
        }
        throw bad_mesh_access();
    }

    std::size_t mesh::colors_channel_count() const noexcept {
        return colors_channels_.size();
    }

    const vector<v3f>& mesh::vertices() const noexcept {
        return vertices_;
    }

    const vector<u32>& mesh::indices(std::size_t submesh) const {
        if ( submesh < indices_submeshes_.size() ) {
            return indices_submeshes_[submesh];
        }
        throw bad_mesh_access();
    }

    std::size_t mesh::indices_submesh_count() const noexcept {
        return indices_submeshes_.size();
    }

    const vector<v3f>& mesh::normals() const noexcept {
        return normals_;
    }

    const vector<v3f>& mesh::tangents() const noexcept {
        return tangents_;
    }

    const vector<v3f>& mesh::bitangents() const noexcept {
        return bitangents_;
    }
}

namespace e2d
{
    void swap(mesh& l, mesh& r) noexcept {
        l.swap(r);
    }

    bool operator==(const mesh& l, const mesh& r) noexcept {
        if ( l.vertices().size() != r.vertices().size() ) {
            return false;
        }
        if ( l.uvs_channel_count() != r.uvs_channel_count() ) {
            return false;
        }
        if ( l.colors_channel_count() != r.colors_channel_count() ) {
            return false;
        }
        if ( l.indices_submesh_count() != r.indices_submesh_count() ) {
            return false;
        }
        for ( std::size_t i = 0; i < l.uvs_channel_count(); ++i ) {
            if ( l.uvs(i) != r.uvs(i) ) {
                return false;
            }
        }
        for ( std::size_t i = 0; i < l.colors_channel_count(); ++i ) {
            if ( l.colors(i) != r.colors(i) ) {
                return false;
            }
        }
        for ( std::size_t i = 0; i < l.indices_submesh_count(); ++i ) {
            if ( l.indices(i) != r.indices(i) ) {
                return false;
            }
        }
        return l.vertices() == r.vertices()
            && l.normals() == r.normals()
            && l.tangents() == r.tangents()
            && l.bitangents() == r.bitangents();
    }

    bool operator!=(const mesh& l, const mesh& r) noexcept {
        return !(l == r);
    }
}

namespace e2d { namespace meshes
{
    bool try_load_mesh(
        mesh& dst,
        const buffer& src) noexcept
    {
        return impl::try_load_mesh_e2d(dst, src);
    }

    bool try_load_mesh(
        mesh& dst,
        const input_stream_uptr& src) noexcept
    {
        buffer file_data;
        return streams::try_read_tail(file_data, src)
            && try_load_mesh(dst, file_data);
    }
}}
