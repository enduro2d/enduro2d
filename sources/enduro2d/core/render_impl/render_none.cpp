/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "render.hpp"

#if defined(E2D_RENDER_MODE) && E2D_RENDER_MODE == E2D_RENDER_MODE_NONE

namespace e2d
{
    //
    // shader::internal_state
    //

    class shader::internal_state final : private e2d::noncopyable {
    public:
        internal_state() noexcept = default;
        ~internal_state() noexcept = default;
    };

    //
    // texture::internal_state
    //

    class texture::internal_state final : private e2d::noncopyable {
    public:
        internal_state() noexcept = default;
        ~internal_state() noexcept = default;
    };

    //
    // index_buffer::internal_state
    //

    class index_buffer::internal_state final : private e2d::noncopyable {
    public:
        internal_state() noexcept = default;
        ~internal_state() noexcept = default;
    };

    //
    // vertex_buffer::internal_state
    //

    class vertex_buffer::internal_state final : private e2d::noncopyable {
    public:
        internal_state() noexcept = default;
        ~internal_state() noexcept = default;
    };

    //
    // render::internal_state
    //

    class render::internal_state final : private e2d::noncopyable {
    public:
        debug& debug_;
        window& window_;
    public:
        internal_state(debug& debug, window& window) noexcept
        : debug_(debug)
        , window_(window) {}
        ~internal_state() noexcept = default;
    };

    //
    // shader
    //

    const shader::internal_state& shader::state() const noexcept {
        return *state_;
    }

    shader::shader(internal_state_uptr state)
    : state_(std::move(state)) {}
    shader::~shader() noexcept = default;

    //
    // texture
    //

    const texture::internal_state& texture::state() const noexcept {
        return *state_;
    }

    texture::texture(internal_state_uptr state)
    : state_(std::move(state)) {}
    texture::~texture() noexcept = default;

    //
    // index_buffer
    //

    const index_buffer::internal_state& index_buffer::state() const noexcept {
        return *state_;
    }

    index_buffer::index_buffer(internal_state_uptr state)
    : state_(std::move(state)) {}
    index_buffer::~index_buffer() noexcept = default;

    //
    // vertex_buffer
    //

    const vertex_buffer::internal_state& vertex_buffer::state() const noexcept {
        return *state_;
    }

    vertex_buffer::vertex_buffer(internal_state_uptr state)
    : state_(std::move(state)) {}
    vertex_buffer::~vertex_buffer() noexcept = default;

    //
    // render
    //

    render::render(debug& d, window& w)
    : state_(new internal_state(d, w)) {}
    render::~render() noexcept = default;

    shader_ptr render::create_shader(
        input_stream_uptr vertex,
        input_stream_uptr fragment)
    {
        E2D_UNUSED(vertex, fragment);
        return nullptr;
    }

    texture_ptr render::create_texture(const image& image) {
        E2D_UNUSED(image);
        return nullptr;
    }

    texture_ptr render::create_texture(const input_stream_uptr& image_stream) {
        E2D_UNUSED(image_stream);
        return nullptr;
    }

    index_buffer_ptr render::create_index_buffer(
        const buffer& indices,
        const index_declaration& decl,
        index_buffer::usage usage)
    {
        E2D_UNUSED(indices, decl, usage);
        return nullptr;
    }

    vertex_buffer_ptr render::create_vertex_buffer(
        const buffer& vertices,
        const vertex_declaration& decl,
        vertex_buffer::usage usage)
    {
        E2D_UNUSED(vertices, decl, usage);
        return nullptr;
    }

    void render::draw(
        const material& mat,
        const geometry& geo)
    {
        E2D_UNUSED(mat, geo);
    }

    void render::draw(
        const material& mat,
        const geometry& geo,
        const property_block& props)
    {
        E2D_UNUSED(mat, geo, props);
    }

    render& render::clear_depth_buffer(f32 value) noexcept {
        E2D_UNUSED(value);
        return *this;
    }

    render& render::clear_stencil_buffer(u8 value) noexcept {
        E2D_UNUSED(value);
        return *this;
    }

    render& render::clear_color_buffer(const color& value) noexcept {
        E2D_UNUSED(value);
        return *this;
    }

    render& render::set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept {
        E2D_UNUSED(x, y, w, h);
        return *this;
    }
}

#endif
