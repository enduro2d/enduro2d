/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "render.hpp"

#if defined(E2D_RENDER_MODE) && E2D_RENDER_MODE == E2D_RENDER_MODE_OPENGL

#if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX
#  include <OpenGL/gl.h>
#endif

namespace
{
    using namespace e2d;

    GLenum convert_wrap(texture::wrap w) noexcept {
        #define DEFINE_CASE(x,y) case texture::wrap::x: return y;
        switch ( w ) {
            DEFINE_CASE(clamp, GL_CLAMP_TO_EDGE);
            DEFINE_CASE(repeat, GL_REPEAT);
            DEFINE_CASE(mirror, GL_MIRRORED_REPEAT);
            default:
                E2D_ASSERT_MSG(false, "unexpected texture wrap");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_filter(texture::filter f) noexcept {
        #define DEFINE_CASE(x,y) case texture::filter::x: return y;
        switch ( f ) {
            DEFINE_CASE(linear, GL_LINEAR);
            DEFINE_CASE(nearest, GL_NEAREST);
            default:
                E2D_ASSERT_MSG(false, "unexpected texture filter");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_state(render::state s) noexcept {
        #define DEFINE_CASE(x,y) case render::state::x: return y;
        switch ( s ) {
            DEFINE_CASE(blend, GL_BLEND);
            DEFINE_CASE(cull_face, GL_CULL_FACE);
            DEFINE_CASE(depth_test, GL_DEPTH_TEST);
            DEFINE_CASE(stencil_test, GL_STENCIL_TEST);
            default:
                E2D_ASSERT_MSG(false, "unexpected render state");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_cull_face(render::cull_face cf) noexcept {
        #define DEFINE_CASE(x,y) case render::cull_face::x: return y;
        switch ( cf ) {
            DEFINE_CASE(front, GL_FRONT);
            DEFINE_CASE(back, GL_BACK);
            DEFINE_CASE(front_back, GL_FRONT_AND_BACK);
            default:
                E2D_ASSERT_MSG(false, "unexpected render cull face");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_blend_func(render::blend_func bf) noexcept {
        #define DEFINE_CASE(x,y) case render::blend_func::x: return y;
        switch ( bf ) {
            DEFINE_CASE(zero, GL_ZERO);
            DEFINE_CASE(one, GL_ONE);
            DEFINE_CASE(src_color, GL_SRC_COLOR);
            DEFINE_CASE(one_minus_src_color, GL_ONE_MINUS_SRC_COLOR);
            DEFINE_CASE(dst_color, GL_DST_COLOR);
            DEFINE_CASE(one_minus_dst_color, GL_ONE_MINUS_DST_COLOR);
            DEFINE_CASE(src_alpha, GL_SRC_ALPHA);
            DEFINE_CASE(one_minus_src_alpha, GL_ONE_MINUS_SRC_ALPHA);
            DEFINE_CASE(dst_alpha, GL_DST_ALPHA);
            DEFINE_CASE(one_minus_dst_alpha, GL_ONE_MINUS_DST_ALPHA);
            DEFINE_CASE(constant_color, GL_CONSTANT_COLOR);
            DEFINE_CASE(one_minus_constant_color, GL_ONE_MINUS_CONSTANT_COLOR);
            DEFINE_CASE(constant_alpha, GL_CONSTANT_ALPHA);
            DEFINE_CASE(one_minus_constant_alpha, GL_ONE_MINUS_CONSTANT_ALPHA);
            DEFINE_CASE(src_alpha_saturate, GL_SRC_ALPHA_SATURATE);
            default:
                E2D_ASSERT_MSG(false, "unexpected render blend func");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_depth_func(render::depth_func df) noexcept {
        #define DEFINE_CASE(x,y) case render::depth_func::x: return y;
        switch ( df ) {
            DEFINE_CASE(never, GL_NEVER);
            DEFINE_CASE(less, GL_LESS);
            DEFINE_CASE(lequal, GL_LEQUAL);
            DEFINE_CASE(greater, GL_GREATER);
            DEFINE_CASE(gequal, GL_GEQUAL);
            DEFINE_CASE(equal, GL_EQUAL);
            DEFINE_CASE(notequal, GL_NOTEQUAL);
            DEFINE_CASE(always, GL_ALWAYS);
            default:
                E2D_ASSERT_MSG(false, "unexpected render depth func");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_stencil_func(render::stencil_func sf) noexcept {
        #define DEFINE_CASE(x,y) case render::stencil_func::x: return y;
        switch ( sf ) {
            DEFINE_CASE(never, GL_NEVER);
            DEFINE_CASE(less, GL_LESS);
            DEFINE_CASE(lequal, GL_LEQUAL);
            DEFINE_CASE(greater, GL_GREATER);
            DEFINE_CASE(gequal, GL_GEQUAL);
            DEFINE_CASE(equal, GL_EQUAL);
            DEFINE_CASE(notequal, GL_NOTEQUAL);
            DEFINE_CASE(always, GL_ALWAYS);
            default:
                E2D_ASSERT_MSG(false, "unexpected render stencil func");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_stencil_op(render::stencil_op so) noexcept {
        #define DEFINE_CASE(x,y) case render::stencil_op::x: return y;
        switch ( so ) {
            DEFINE_CASE(keep, GL_KEEP);
            DEFINE_CASE(zero, GL_ZERO);
            DEFINE_CASE(replace, GL_REPLACE);
            DEFINE_CASE(incr, GL_INCR);
            DEFINE_CASE(incr_wrap, GL_INCR_WRAP);
            DEFINE_CASE(decr, GL_DECR);
            DEFINE_CASE(decr_wrap, GL_DECR_WRAP);
            DEFINE_CASE(invert, GL_INVERT);
            default:
                E2D_ASSERT_MSG(false, "unexpected render stencil op");
                return 0;
        }
        #undef DEFINE_CASE
    }
}

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
        std::mutex mutex;
        m4f view;
        m4f projection;
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
        E2D_UNUSED(convert_wrap(u), convert_wrap(v));
    }

    void texture::set_filter(filter min, filter mag) noexcept {
        E2D_UNUSED(convert_filter(min), convert_filter(mag));
    }

    //
    // render
    //

    render::render()
    : state_(new internal_state()) {}
    render::~render() noexcept = default;

    void render::clear(bool color, bool depth, bool stencil) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        GLbitfield mask = 0;
        if ( color ) {
            mask |= GL_COLOR_BUFFER_BIT;
        }
        if ( depth ) {
            mask |= GL_DEPTH_BUFFER_BIT;
        }
        if ( stencil ) {
            mask |= GL_STENCIL_BUFFER_BIT;
        }
        glClear(mask);
    }

    void render::set_view(const m4f& view) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->view = view;
    }

    void render::set_projection(const m4f& projection) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->projection = projection;
    }

    void render::set_viewport(f32 x, f32 y, f32 w, f32 h) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glViewport(
            math::numeric_cast<GLint>(x),
            math::numeric_cast<GLint>(y),
            math::numeric_cast<GLsizei>(w),
            math::numeric_cast<GLsizei>(h));
    }

    void render::enable_state(state state) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glEnable(convert_state(state));
    }

    void render::disable_state(state state) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glDisable(convert_state(state));
    }

    void render::set_cull_face(cull_face cull_face) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glCullFace(convert_cull_face(cull_face));
    }

    void render::set_depth_func(depth_func depth_func) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glDepthFunc(convert_depth_func(depth_func));
    }

    void render::set_depth_mask(bool yesno) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glDepthMask(yesno ? GL_TRUE : GL_FALSE);
    }

    void render::set_stencil_func(
        stencil_func stencil_func, i32 ref, u32 mask) noexcept
    {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glStencilFunc(
            convert_stencil_func(stencil_func),
            math::numeric_cast<GLint>(ref),
            math::numeric_cast<GLuint>(mask));
    }

    void render::set_stencil_mask(u32 mask) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glStencilMask(math::numeric_cast<GLuint>(mask));
    }

    void render::set_stencil_op(
        stencil_op fail, stencil_op zfail, stencil_op zpass) noexcept
    {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glStencilOp(
            convert_stencil_op(fail),
            convert_stencil_op(zfail),
            convert_stencil_op(zpass));
    }

    void render::set_clear_color(const color& color) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glClearColor(
            math::numeric_cast<GLclampf>(color.r),
            math::numeric_cast<GLclampf>(color.g),
            math::numeric_cast<GLclampf>(color.b),
            math::numeric_cast<GLclampf>(color.a));
    }

    void render::set_color_mask(bool r, bool g, bool b, bool a) {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glColorMask(
            r ? GL_TRUE : GL_FALSE,
            g ? GL_TRUE : GL_FALSE,
            b ? GL_TRUE : GL_FALSE,
            a ? GL_TRUE : GL_FALSE);
    }

    void render::set_blend_func(blend_func src, blend_func dst) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glBlendFunc(convert_blend_func(src), convert_blend_func(dst));
    }

    void render::set_blend_color(const color& color) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glBlendColor(
            math::numeric_cast<GLclampf>(color.r),
            math::numeric_cast<GLclampf>(color.g),
            math::numeric_cast<GLclampf>(color.b),
            math::numeric_cast<GLclampf>(color.a));
    }
}

#endif
