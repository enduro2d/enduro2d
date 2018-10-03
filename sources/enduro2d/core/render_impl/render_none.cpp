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
        internal_state() {}
        ~internal_state() noexcept = default;
    };

    //
    // texture::internal_state
    //

    class texture::internal_state final : private e2d::noncopyable {
    public:
        internal_state() {}
        ~internal_state() noexcept = default;
    };

    //
    // index_buffer::internal_state
    //

    class index_buffer::internal_state final : private e2d::noncopyable {
    public:
        std::size_t count = 0;
    public:
        internal_state(std::size_t ncount)
        : count(ncount) {}
        ~internal_state() noexcept = default;
    };

    //
    // vertex_buffer::internal_state
    //

    class vertex_buffer::internal_state final : private e2d::noncopyable {
    public:
        std::size_t count = 0;
    public:
        internal_state(std::size_t ncount)
        : count(ncount) {}
        ~internal_state() noexcept = default;
    };

    //
    // render::internal_state
    //

    class render::internal_state final : private e2d::noncopyable {
    public:
        debug& debug;
        window& window;
    public:
        internal_state(class debug& debug, class window& window)
        : debug(debug)
        , window(window) {}
        ~internal_state() noexcept = default;
    };

    //
    // shader
    //

    shader::shader(internal_state_uptr state)
    : state_(std::move(state)) {}
    shader::~shader() noexcept = default;

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

    shader_ptr render::create_shader(input_stream_uptr vertex, input_stream_uptr fragment) {
        E2D_UNUSED(vertex, fragment);
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
        const u16* indices,
        std::size_t count,
        index_buffer::usage usage)
    {
        E2D_UNUSED(indices, count, usage);
        return nullptr;
    }

    vertex_buffer_ptr render::create_vertex_buffer(
        const vertex* vertices,
        std::size_t count,
        vertex_buffer::usage usage)
    {
        E2D_UNUSED(vertices, count, usage);
        return nullptr;
    }

    void render::clear(bool color, bool depth, bool stencil) noexcept {
        E2D_UNUSED(color, depth, stencil);
    }

    void render::draw(
        const shader_ptr& ps,
        const index_buffer_ptr& ib,
        const vertex_buffer_ptr& vb) noexcept
    {
        E2D_UNUSED(ps, ib, vb);
    }

    void render::set_view(const m4f& view) noexcept {
        E2D_UNUSED(view);
    }

    void render::set_projection(const m4f& projection) noexcept {
        E2D_UNUSED(projection);
    }

    void render::set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept {
        E2D_UNUSED(x, y, w, h);
    }

    void render::enable_state(state state) noexcept {
        E2D_UNUSED(state);
    }

    void render::disable_state(state state) noexcept {
        E2D_UNUSED(state);
    }

    void render::set_blend_func(blend_func src, blend_func dst) noexcept {
        E2D_UNUSED(src, dst);
    }

    void render::set_blend_color(const color& color) noexcept {
        E2D_UNUSED(color);
    }

    void render::set_blend_equation(blend_equation blend_equation) noexcept {
        E2D_UNUSED(blend_equation);
    }

    void render::set_cull_face(cull_face cull_face) noexcept {
        E2D_UNUSED(cull_face);
    }

    void render::set_depth_func(depth_func depth_func) noexcept {
        E2D_UNUSED(depth_func);
    }

    void render::set_depth_mask(bool yesno) noexcept {
        E2D_UNUSED(yesno);
    }

    void render::set_clear_depth(f32 value) noexcept {
        E2D_UNUSED(value);
    }

    void render::set_stencil_func(stencil_func stencil_func, u32 ref, u32 mask) noexcept {
        E2D_UNUSED(stencil_func, ref, mask);
    }

    void render::set_stencil_mask(u32 mask) noexcept {
        E2D_UNUSED(mask);
    }

    void render::set_stencil_op(stencil_op fail, stencil_op zfail, stencil_op zpass) noexcept {
        E2D_UNUSED(fail, zfail, zpass);
    }

    void render::set_clear_stencil(u32 value) noexcept {
        E2D_UNUSED(value);
    }

    void render::set_clear_color(const color& color) noexcept {
        E2D_UNUSED(color);
    }

    void render::set_color_mask(bool r, bool g, bool b, bool a) {
        E2D_UNUSED(r, g, b, a);
    }
}

#endif
