/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/high/_high.hpp>

#include <enduro2d/high/assets/material_asset.hpp>

namespace e2d::render_system_impl
{
    class bad_batcher_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad batcher operation";
        }
    };

    template < typename Index, typename Vertex >
    class batcher : private noncopyable {
    public:
        using index_type = typename Index::type;
        using vertex_type = typename Vertex::type;

        batcher(debug& debug, render& render);

        void batch(
            const material_asset::ptr& material,
            const render::property_block& properties,
            const index_type* indices, std::size_t index_count,
            const vertex_type* vertices, std::size_t vertex_count);

        render::property_block& flush();
        void clear(bool clear_internal_props) noexcept;
    private:
        void update_buffers_();
        void render_buffers_();
        void update_index_buffer_();
        void update_vertex_buffer_();
    private:
        struct batch_type {
            std::size_t start{0u};
            std::size_t count{0u};
            material_asset::ptr material;
            render::property_block properties;

            batch_type(
                std::size_t nstart,
                const material_asset::ptr& nmaterial,
                const render::property_block& nproperties)
            : start(nstart)
            , material(nmaterial)
            , properties(nproperties) {}
        };
    private:
        debug& debug_;
        render& render_;
        vector<batch_type> batches_;
        vector<index_type> indices_;
        vector<vertex_type> vertices_;
        index_declaration index_decl_;
        vertex_declaration vertex_decl_;
        index_buffer_ptr index_buffer_;
        vertex_buffer_ptr vertex_buffer_;
        render::property_block property_cache_;
        render::property_block internal_properties_;
    private:
        static std::size_t calculate_new_buffer_size(
            std::size_t esize, std::size_t osize, std::size_t nsize);
    };
}

namespace e2d::render_system_impl
{
    template < typename Index, typename Vertex >
    batcher<Index, Vertex>::batcher(debug& debug, render& render)
    : debug_(debug)
    , render_(render)
    , index_decl_(Index::decl())
    , vertex_decl_(Vertex::decl()) {
        E2D_ASSERT(sizeof(index_type) == index_decl_.bytes_per_index());
        E2D_ASSERT(sizeof(vertex_type) == vertex_decl_.bytes_per_vertex());
    }

    template < typename Index, typename Vertex >
    void batcher<Index, Vertex>::batch(
        const material_asset::ptr& material,
        const render::property_block& properties,
        const index_type* indices, std::size_t index_count,
        const vertex_type* vertices, std::size_t vertex_count)
    {
        E2D_ASSERT(material);
        E2D_ASSERT(indices || !index_count);
        E2D_ASSERT(vertices || !vertex_count);

        const std::size_t max_vertex_count = std::numeric_limits<index_type>::max();

        if ( vertex_count > max_vertex_count ) {
            throw bad_batcher_operation();
        }

        if ( max_vertex_count - vertices_.size() < vertex_count ) {
            flush();
        }

        try {
            const bool batching_available =
                !batches_.empty() &&
                (batches_.back().material == material || batches_.back().material->content() == material->content()) &&
                batches_.back().properties == properties;

            if ( !batching_available ) {
                const std::size_t start = batches_.empty()
                    ? 0u
                    : batches_.back().start + batches_.back().count;
                batches_.emplace_back(start, material, properties);
            }

            if ( indices && index_count ) {
                auto iter = indices_.insert(
                    indices_.end(),
                    indices, indices + index_count);
                std::transform(
                    iter, indices_.end(), iter,
                    [add = vertices_.size()](index_type v) noexcept {
                        return static_cast<index_type>(v + add);
                    });
                batches_.back().count += index_count;
            }

            if ( vertices && vertex_count ) {
                vertices_.insert(
                    vertices_.end(),
                    vertices, vertices + vertex_count);
            }
        } catch ( ... ) {
            clear(false);
            throw;
        }
    }

    template < typename Index, typename Vertex >
    render::property_block& batcher<Index, Vertex>::flush() {
        try {
            update_buffers_();
            render_buffers_();
        } catch (...) {
            clear(false);
            throw;
        }
        clear(false);
        return internal_properties_;
    }

    template < typename Index, typename Vertex >
    void batcher<Index, Vertex>::clear(bool clear_internal_props) noexcept {
        batches_.clear();
        indices_.clear();
        vertices_.clear();
        if ( clear_internal_props ) {
            internal_properties_.clear();
        }
    }

    template < typename Index, typename Vertex >
    void batcher<Index, Vertex>::update_buffers_() {
        update_index_buffer_();
        update_vertex_buffer_();
    }

    template < typename Index, typename Vertex >
    void batcher<Index, Vertex>::render_buffers_() {
        if ( !index_buffer_ || !vertex_buffer_ ) {
            return;
        }

        const auto geo = render::geometry()
            .indices(index_buffer_)
            .add_vertices(vertex_buffer_);

        try {
            for ( const batch_type& batch : batches_ ) {
                const render::material& mat = batch.material->content();
                render_.execute(render::draw_command(
                    mat,
                    geo,
                    property_cache_
                        .merge(internal_properties_)
                        .merge(batch.properties)
                ).index_range(batch.start, batch.count));
            }
        } catch ( ... ) {
            property_cache_.clear();
            throw;
        }
        property_cache_.clear();
    }

    template < typename Index, typename Vertex >
    void batcher<Index, Vertex>::update_index_buffer_() {
        const std::size_t min_ib_size = indices_.size() * sizeof(indices_[0]);
        if ( index_buffer_ && index_buffer_->buffer_size() >= min_ib_size ) {
            index_buffer_->update(indices_, 0u);
        } else {
            const std::size_t new_ib_size = calculate_new_buffer_size(
                sizeof(Index),
                index_buffer_ ? index_buffer_->buffer_size() : 0u,
                min_ib_size);

            buffer new_ib_data(new_ib_size);
            std::memcpy(new_ib_data.data(), indices_.data(), min_ib_size);

            index_buffer_ = render_.create_index_buffer(
                new_ib_data,
                index_decl_,
                index_buffer::usage::dynamic_draw);

            if ( !index_buffer_ ) {
                debug_.error("BATCHER: Failed to create index buffer:\n"
                    "--> Size: %0",
                    new_ib_size);
            }
        }
    }

    template < typename Index, typename Vertex >
    void batcher<Index, Vertex>::update_vertex_buffer_() {
        const std::size_t min_vb_size = vertices_.size() * sizeof(vertices_[0]);
        if ( vertex_buffer_ && vertex_buffer_->buffer_size() >= min_vb_size ) {
            vertex_buffer_->update(vertices_, 0u);
        } else {
            const std::size_t new_vb_size = calculate_new_buffer_size(
                sizeof(Vertex),
                vertex_buffer_ ? vertex_buffer_->buffer_size() : 0u,
                min_vb_size);

            buffer new_vb_data(new_vb_size);
            std::memcpy(new_vb_data.data(), vertices_.data(), min_vb_size);

            vertex_buffer_ = render_.create_vertex_buffer(
                new_vb_data,
                vertex_decl_,
                vertex_buffer::usage::dynamic_draw);

            if ( !vertex_buffer_ ) {
                debug_.error("BATCHER: Failed to create vertex buffer:\n"
                    "--> Size: %0",
                    new_vb_size);
            }
        }
    }

    template < typename Index, typename Vertex >
    std::size_t batcher<Index, Vertex>::calculate_new_buffer_size(
        std::size_t esize, std::size_t osize, std::size_t nsize)
    {
        std::size_t msize = std::size_t(-1) / esize * esize;
        if ( nsize > msize ) {
            throw bad_batcher_operation();
        }
        if ( osize >= msize / 2 ) {
            return msize;
        }
        return math::max(osize * 2u, nsize);
    }
}
