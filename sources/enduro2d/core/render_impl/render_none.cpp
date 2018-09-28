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
    // texture::internal_state
    //

    class texture::internal_state final : private e2d::noncopyable {
    public:
        internal_state() {}
        ~internal_state() noexcept = default;
    };

    //
    // render::internal_state
    //

    class render::internal_state final : private e2d::noncopyable {
    public:
        internal_state() {}
        ~internal_state() noexcept = default;
    };

    //
    // texture
    //

    texture::texture()
    : state_(new internal_state()) {}
    texture::~texture() noexcept = default;

    void texture::set_wrap(wrap u, wrap v) noexcept {
        E2D_UNUSED(u, v);
    }

    void texture::set_filter(filter min, filter mag) noexcept {
        E2D_UNUSED(min, mag);
    }

    //
    // render
    //

    render::render()
    : state_(new internal_state()) {}
    render::~render() noexcept = default;

    void render::clear(bool color, bool depth, bool stencil) noexcept {
        E2D_UNUSED(color, depth, stencil);
    }

    void render::set_view(const m4f& view) noexcept {
        E2D_UNUSED(view);
    }

    void render::set_projection(const m4f& projection) noexcept {
        E2D_UNUSED(projection);
    }

    void render::set_viewport(f32 x, f32 y, f32 w, f32 h) noexcept {
        E2D_UNUSED(x, y, w, h);
    }

    void render::enable_state(state state) noexcept {
        E2D_UNUSED(state);
    }

    void render::disable_state(state state) noexcept {
        E2D_UNUSED(state);
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

    void render::set_stencil_func(stencil_func stencil_func, i32 ref, u32 mask) noexcept {
        E2D_UNUSED(stencil_func, ref, mask);
    }

    void render::set_stencil_mask(u32 mask) noexcept {
        E2D_UNUSED(mask);
    }

    void render::set_stencil_op(stencil_op fail, stencil_op zfail, stencil_op zpass) noexcept {
        E2D_UNUSED(fail, zfail, zpass);
    }

    void render::set_clear_color(const color& color) noexcept {
        E2D_UNUSED(color);
    }

    void render::set_color_mask(bool r, bool g, bool b, bool a) {
        E2D_UNUSED(r, g, b, a);
    }

    void render::set_blend_func(blend_func src, blend_func dst) noexcept {
        E2D_UNUSED(src, dst);
    }

    void render::set_blend_color(const color& color) noexcept {
        E2D_UNUSED(color);
    }
}

#endif
