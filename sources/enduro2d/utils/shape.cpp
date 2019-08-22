/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "shape_impl/shape_impl.hpp"

namespace e2d
{
    shape::shape(shape&& other) noexcept {
        assign(std::move(other));
    }

    shape& shape::operator=(shape&& other) noexcept {
        return assign(std::move(other));
    }

    shape::shape(const shape& other) {
        assign(other);
    }

    shape& shape::operator=(const shape& other) {
        return assign(other);
    }

    shape& shape::assign(shape&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    shape& shape::assign(const shape& other) {
        if ( this != &other ) {
            shape m;
            m.uvs_channels_ = other.uvs_channels_;
            m.colors_channels_ = other.colors_channels_;
            m.vertices_ = other.vertices_;
            m.indices_subshapees_ = other.indices_subshapees_;
            swap(m);
        }
        return *this;
    }

    void shape::swap(shape& other) noexcept {
        using std::swap;
        swap(uvs_channels_, other.uvs_channels_);
        swap(colors_channels_, other.colors_channels_);
        swap(vertices_, other.vertices_);
        swap(indices_subshapees_, other.indices_subshapees_);
    }

    void shape::clear() noexcept {
        uvs_channels_.clear();
        colors_channels_.clear();
        vertices_.clear();
        indices_subshapees_.clear();
    }

    bool shape::empty() const noexcept {
        return vertices_.empty();
    }

    shape& shape::set_uvs(std::size_t channel, vector<v2f>&& uvs) {
        if ( uvs_channels_.size() <= channel ) {
            uvs_channels_.resize(channel + 1);
        }
        uvs_channels_[channel] = std::move(uvs);
        return *this;
    }

    shape& shape::set_uvs(std::size_t channel, const vector<v2f>& uvs) {
        return set_uvs(channel, uvs.data(), uvs.size());
    }

    shape& shape::set_uvs(std::size_t channel, const v2f* uvs, std::size_t count) {
        return count
            ? set_uvs(channel, uvs
                ? vector<v2f>(uvs, uvs + count)
                : vector<v2f>(count))
            : set_uvs(channel, vector<v2f>());
    }

    shape& shape::set_colors(std::size_t channel, vector<color32>&& colors) {
        if ( colors_channels_.size() <= channel ) {
            colors_channels_.resize(channel + 1);
        }
        colors_channels_[channel] = std::move(colors);
        return *this;
    }

    shape& shape::set_colors(std::size_t channel, const vector<color32>& colors) {
        return set_colors(channel, colors.data(), colors.size());
    }

    shape& shape::set_colors(std::size_t channel, const color32* colors, std::size_t count) {
        return count
            ? set_colors(channel, colors
                ? vector<color32>(colors, colors + count)
                : vector<color32>(count))
            : set_colors(channel, vector<color32>());
    }

    shape& shape::set_vertices(vector<v2f>&& vertices) noexcept {
        vertices_ = std::move(vertices);
        return *this;
    }

    shape& shape::set_vertices(const vector<v2f>& vertices) {
        return set_vertices(vertices.data(), vertices.size());
    }

    shape& shape::set_vertices(const v2f* vertices, std::size_t count) {
        return count
            ? set_vertices(vertices
                ? vector<v2f>(vertices, vertices + count)
                : vector<v2f>(count))
            : set_vertices(vector<v2f>());
    }

    shape& shape::set_indices(std::size_t subshape, vector<u32>&& indices) {
        if ( indices_subshapees_.size() <= subshape ) {
            indices_subshapees_.resize(subshape + 1);
        }
        indices_subshapees_[subshape] = std::move(indices);
        return *this;
    }

    shape& shape::set_indices(std::size_t subshape, const vector<u32>& indices) {
        return set_indices(subshape, indices.data(), indices.size());
    }

    shape& shape::set_indices(std::size_t subshape, const u32* indices, std::size_t count) {
        return count
            ? set_indices(subshape, indices
                ? vector<u32>(indices, indices + count)
                : vector<u32>(count))
            : set_indices(subshape, vector<u32>());
    }

    const vector<v2f>& shape::uvs(std::size_t channel) const {
        if ( channel < uvs_channels_.size() ) {
            return uvs_channels_[channel];
        }
        throw bad_shape_access();
    }

    std::size_t shape::uvs_channel_count() const noexcept {
        return uvs_channels_.size();
    }

    const vector<color32>& shape::colors(std::size_t channel) const {
        if ( channel < colors_channels_.size() ) {
            return colors_channels_[channel];
        }
        throw bad_shape_access();
    }

    std::size_t shape::colors_channel_count() const noexcept {
        return colors_channels_.size();
    }

    const vector<v2f>& shape::vertices() const noexcept {
        return vertices_;
    }

    const vector<u32>& shape::indices(std::size_t subshape) const {
        if ( subshape < indices_subshapees_.size() ) {
            return indices_subshapees_[subshape];
        }
        throw bad_shape_access();
    }

    std::size_t shape::indices_subshape_count() const noexcept {
        return indices_subshapees_.size();
    }
}

namespace e2d
{
    void swap(shape& l, shape& r) noexcept {
        l.swap(r);
    }

    bool operator==(const shape& l, const shape& r) noexcept {
        if ( l.vertices().size() != r.vertices().size() ) {
            return false;
        }
        if ( l.uvs_channel_count() != r.uvs_channel_count() ) {
            return false;
        }
        if ( l.colors_channel_count() != r.colors_channel_count() ) {
            return false;
        }
        if ( l.indices_subshape_count() != r.indices_subshape_count() ) {
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
        for ( std::size_t i = 0; i < l.indices_subshape_count(); ++i ) {
            if ( l.indices(i) != r.indices(i) ) {
                return false;
            }
        }
        return l.vertices() == r.vertices();
    }

    bool operator!=(const shape& l, const shape& r) noexcept {
        return !(l == r);
    }
}

namespace e2d::shapes
{
    bool try_load_shape(
        shape& dst,
        buffer_view src) noexcept
    {
        try {
            return impl::load_shape_e2d(dst, src);
        } catch (...) {
            return false;
        }
    }

    bool try_load_shape(
        shape& dst,
        const input_stream_uptr& src) noexcept
    {
        buffer file_data;
        return streams::try_read_tail(file_data, src)
            && try_load_shape(dst, file_data);
    }
}
