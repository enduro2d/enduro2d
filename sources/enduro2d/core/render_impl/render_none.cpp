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
        vertex_declaration decl_;
    public:
        explicit internal_state(const vertex_declaration& decl) noexcept
        : decl_(decl){}
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

    shader::shader(internal_state_uptr state)
    : state_(std::move(state)) {}
    shader::~shader() noexcept = default;

    const vertex_declaration& shader::decl() const noexcept {
        return state_->decl_;
    }

    void shader::set_uniform(str_view name, i32 value) const noexcept {
        E2D_UNUSED(name, value);
    }

    void shader::set_uniform(str_view name, f32 value) const noexcept {
        E2D_UNUSED(name, value);
    }

    //
    // texture
    //

    texture::texture(internal_state_uptr state)
    : state_(std::move(state)) {}
    texture::~texture() noexcept = default;

    void texture::set_wrap(wrap u, wrap v) noexcept {
        E2D_UNUSED(u, v);
    }

    void texture::set_filter(filter min, filter mag) noexcept {
        E2D_UNUSED(min, mag);
    }

    //
    // index_buffer
    //

    index_buffer::index_buffer(internal_state_uptr state)
    : state_(std::move(state)) {}
    index_buffer::~index_buffer() noexcept = default;

    //
    // vertex_buffer
    //

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
        input_stream_uptr fragment,
        const vertex_declaration& decl)
    {
        E2D_UNUSED(vertex, fragment, decl);
        return nullptr;
    }

    texture_ptr render::create_texture(const image& image) {
        E2D_UNUSED(image);
        return nullptr;
    }

    texture_ptr render::create_texture(const v2u& size, image_data_format format) {
        E2D_UNUSED(size, format);
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

    void render::draw(
        topology tp,
        const shader_ptr& ps,
        const index_buffer_ptr& ib,
        const vertex_buffer_ptr& vb) noexcept
    {
        E2D_UNUSED(tp, ps, ib, vb);
    }

    render& render::set_model(const m4f& model) noexcept {
        E2D_UNUSED(model);
        return *this;
    }

    render& render::set_view(const m4f& view) noexcept {
        E2D_UNUSED(view);
        return *this;
    }

    render& render::set_projection(const m4f& projection) noexcept {
        E2D_UNUSED(projection);
        return *this;
    }

    render& render::set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept {
        E2D_UNUSED(x, y, w, h);
        return *this;
    }

    render& render::set_depth_state(const depth_state& ds) noexcept {
        E2D_UNUSED(ds);
        return *this;
    }

    render& render::set_stencil_state(const stencil_state& ss) noexcept {
        E2D_UNUSED(ss);
        return *this;
    }

    render& render::set_culling_state(const culling_state& cs) noexcept {
        E2D_UNUSED(cs);
        return *this;
    }

    render& render::set_blending_state(const blending_state& bs) noexcept {
        E2D_UNUSED(bs);
        return *this;
    }

    render& render::set_capabilities_state(const capabilities_state& cs) noexcept {
        E2D_UNUSED(cs);
        return *this;
    }
}

#endif
