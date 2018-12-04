/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "buffer.hpp"
#include "color32.hpp"
#include "streams.hpp"

namespace e2d
{
    class bad_mesh_access final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad mesh access";
        }
    };

    class mesh final {
    public:
        mesh() = default;

        mesh(mesh&& other) noexcept;
        mesh& operator=(mesh&& other) noexcept;

        mesh(const mesh& other);
        mesh& operator=(const mesh& other);

        mesh& assign(mesh&& other) noexcept;
        mesh& assign(const mesh& other);

        void swap(mesh& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        mesh& set_uvs(std::size_t channel, vector<v2f>&& uvs);
        mesh& set_uvs(std::size_t channel, const vector<v2f>& uvs);
        mesh& set_uvs(std::size_t channel, const v2f* uvs, std::size_t count);

        mesh& set_colors(std::size_t channel, vector<color32>&& colors);
        mesh& set_colors(std::size_t channel, const vector<color32>& colors);
        mesh& set_colors(std::size_t channel, const color32* colors, std::size_t count);

        mesh& set_vertices(vector<v3f>&& vertices) noexcept;
        mesh& set_vertices(const vector<v3f>& vertices);
        mesh& set_vertices(const v3f* vertices, std::size_t count);

        mesh& set_indices(std::size_t submesh, vector<u32>&& indices);
        mesh& set_indices(std::size_t submesh, const vector<u32>& indices);
        mesh& set_indices(std::size_t submesh, const u32* indices, std::size_t count);

        mesh& set_normals(vector<v3f>&& normals) noexcept;
        mesh& set_normals(const vector<v3f>& normals);
        mesh& set_normals(const v3f* normals, std::size_t count);

        mesh& set_tangents(vector<v3f>&& tangents) noexcept;
        mesh& set_tangents(const vector<v3f>& tangents);
        mesh& set_tangents(const v3f* tangents, std::size_t count);

        mesh& set_bitangents(vector<v3f>&& bitangents) noexcept;
        mesh& set_bitangents(const vector<v3f>& bitangents);
        mesh& set_bitangents(const v3f* bitangents, std::size_t count);

        const vector<v2f>& uvs(std::size_t channel) const;
        std::size_t uvs_channel_count() const noexcept;

        const vector<color32>& colors(std::size_t channel) const;
        std::size_t colors_channel_count() const noexcept;

        const vector<v3f>& vertices() const noexcept;

        const vector<u32>& indices(std::size_t submesh) const;
        std::size_t indices_submesh_count() const noexcept;

        const vector<v3f>& normals() const noexcept;
        const vector<v3f>& tangents() const noexcept;
        const vector<v3f>& bitangents() const noexcept;
    private:
        vector<vector<v2f>> uvs_channels_;
        vector<vector<color32>> colors_channels_;

        vector<v3f> vertices_;
        vector<vector<u32>> indices_submeshes_;

        vector<v3f> normals_;
        vector<v3f> tangents_;
        vector<v3f> bitangents_;
    };

    void swap(mesh& l, mesh& r) noexcept;
    bool operator==(const mesh& l, const mesh& r) noexcept;
    bool operator!=(const mesh& l, const mesh& r) noexcept;
}

namespace e2d { namespace meshes
{
    bool try_load_mesh(
        mesh& dst,
        const buffer& src) noexcept;

    bool try_load_mesh(
        mesh& dst,
        const input_stream_uptr& src) noexcept;
}}
