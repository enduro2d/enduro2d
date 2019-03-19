/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

#include "buffer.hpp"
#include "color32.hpp"
#include "streams.hpp"

namespace e2d
{
    class bad_shape_access final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad shape access";
        }
    };

    class shape final {
    public:
        shape() = default;

        shape(shape&& other) noexcept;
        shape& operator=(shape&& other) noexcept;

        shape(const shape& other);
        shape& operator=(const shape& other);

        shape& assign(shape&& other) noexcept;
        shape& assign(const shape& other);

        void swap(shape& other) noexcept;
        void clear() noexcept;
        bool empty() const noexcept;

        shape& set_uvs(std::size_t channel, vector<v2f>&& uvs);
        shape& set_uvs(std::size_t channel, const vector<v2f>& uvs);
        shape& set_uvs(std::size_t channel, const v2f* uvs, std::size_t count);

        shape& set_colors(std::size_t channel, vector<color32>&& colors);
        shape& set_colors(std::size_t channel, const vector<color32>& colors);
        shape& set_colors(std::size_t channel, const color32* colors, std::size_t count);

        shape& set_vertices(vector<v2f>&& vertices) noexcept;
        shape& set_vertices(const vector<v2f>& vertices);
        shape& set_vertices(const v2f* vertices, std::size_t count);

        shape& set_indices(std::size_t subshape, vector<u32>&& indices);
        shape& set_indices(std::size_t subshape, const vector<u32>& indices);
        shape& set_indices(std::size_t subshape, const u32* indices, std::size_t count);

        const vector<v2f>& uvs(std::size_t channel) const;
        std::size_t uvs_channel_count() const noexcept;

        const vector<color32>& colors(std::size_t channel) const;
        std::size_t colors_channel_count() const noexcept;

        const vector<v2f>& vertices() const noexcept;

        const vector<u32>& indices(std::size_t subshape) const;
        std::size_t indices_subshape_count() const noexcept;
    private:
        vector<vector<v2f>> uvs_channels_;
        vector<vector<color32>> colors_channels_;

        vector<v2f> vertices_;
        vector<vector<u32>> indices_subshapees_;
    };

    void swap(shape& l, shape& r) noexcept;
    bool operator==(const shape& l, const shape& r) noexcept;
    bool operator!=(const shape& l, const shape& r) noexcept;
}

namespace e2d { namespace shapes
{
    bool try_load_shape(
        shape& dst,
        const buffer& src) noexcept;

    bool try_load_shape(
        shape& dst,
        const input_stream_uptr& src) noexcept;
}}
