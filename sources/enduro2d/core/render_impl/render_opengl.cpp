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

    GLint convert_uniform_type(shader::uniform_type ut) noexcept {
        #define DEFINE_CASE(x,y) case shader::uniform_type::x: return y;
        switch ( ut ) {
            DEFINE_CASE(i, GL_INT);
            DEFINE_CASE(f, GL_FLOAT);

            DEFINE_CASE(v2i, GL_INT_VEC2);
            DEFINE_CASE(v3i, GL_INT_VEC3);
            DEFINE_CASE(v4i, GL_INT_VEC4);

            DEFINE_CASE(v2f, GL_FLOAT_VEC2);
            DEFINE_CASE(v3f, GL_FLOAT_VEC3);
            DEFINE_CASE(v4f, GL_FLOAT_VEC4);

            DEFINE_CASE(m2f, GL_FLOAT_MAT2);
            DEFINE_CASE(m3f, GL_FLOAT_MAT3);
            DEFINE_CASE(m4f, GL_FLOAT_MAT4);

            DEFINE_CASE(s2d, GL_SAMPLER_2D);
            default:
                E2D_ASSERT_MSG(false, "unexpected uniform type");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLint convert_attribute_type(shader::attribute_type ut) noexcept {
        #define DEFINE_CASE(x,y) case shader::attribute_type::x: return y;
        switch ( ut ) {
            DEFINE_CASE(f, GL_FLOAT);

            DEFINE_CASE(v2f, GL_FLOAT_VEC2);
            DEFINE_CASE(v3f, GL_FLOAT_VEC3);
            DEFINE_CASE(v4f, GL_FLOAT_VEC4);

            DEFINE_CASE(m2f, GL_FLOAT_MAT2);
            DEFINE_CASE(m3f, GL_FLOAT_MAT3);
            DEFINE_CASE(m4f, GL_FLOAT_MAT4);
            default:
                E2D_ASSERT_MSG(false, "unexpected attribute type");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLint convert_wrap(texture::wrap w) noexcept {
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

    GLint convert_filter(texture::filter f) noexcept {
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

    GLenum convert_blend_equation(render::blend_equation bf) noexcept {
        #define DEFINE_CASE(x,y) case render::blend_equation::x: return y;
        switch ( bf ) {
            DEFINE_CASE(add, GL_FUNC_ADD);
            DEFINE_CASE(subtract, GL_FUNC_SUBTRACT);
            DEFINE_CASE(reverse_subtract, GL_FUNC_REVERSE_SUBTRACT);
            default:
                E2D_ASSERT_MSG(false, "unexpected render blend equation");
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

    class gl_shader_id final : private noncopyable {
    public:
        gl_shader_id() noexcept
        : id_(0) {}

        gl_shader_id(GLuint id) noexcept
        : id_(id) {
            E2D_ASSERT(glIsShader(id));
        }

        ~gl_shader_id() noexcept {
            reset();
        }

        gl_shader_id(gl_shader_id&& other) noexcept {
            reset();
            std::swap(id_, other.id_);
        }

        void reset() noexcept {
            if ( id_ ) {
                glDeleteShader(id_);
                id_ = 0;
            }
        }

        bool empty() const noexcept {
            return id_ == 0;
        }

        GLuint operator*() const noexcept {
            return id_;
        }
    private:
        GLuint id_;
    };

    class gl_program_id final : private noncopyable {
    public:
        gl_program_id() noexcept
        : id_(0) {}

        gl_program_id(GLuint id) noexcept
        : id_(id) {
            E2D_ASSERT(glIsProgram(id));
        }

        ~gl_program_id() noexcept {
            reset();
        }

        gl_program_id(gl_program_id&& other) noexcept {
            reset();
            std::swap(id_, other.id_);
        }

        void reset() noexcept {
            if ( id_ ) {
                glDeleteProgram(id_);
                id_ = 0;
            }
        }

        bool empty() const noexcept {
            return id_ == 0;
        }

        GLuint operator*() const noexcept {
            return id_;
        }
    private:
        GLuint id_;
    };

    class gl_texture_id final : private noncopyable {
    public:
        gl_texture_id() noexcept
        : id_(0) {}

        gl_texture_id(GLuint id) noexcept
        : id_(id) {
            E2D_ASSERT(glIsTexture(id));
        }

        ~gl_texture_id() noexcept {
            reset();
        }

        gl_texture_id(gl_texture_id&& other) noexcept {
            reset();
            std::swap(id_, other.id_);
        }

        void reset() noexcept {
            if ( id_ ) {
                glDeleteTextures(1, &id_);
                id_ = 0;
            }
        }

        bool empty() const noexcept {
            return id_ == 0;
        }

        GLuint operator*() const noexcept {
            return id_;
        }
    private:
        GLuint id_;
    };

    gl_shader_id compile_shader(debug& d, const char* const s, GLenum t) {
        E2D_ASSERT(s);
        E2D_ASSERT(t == GL_VERTEX_SHADER || t == GL_FRAGMENT_SHADER);
        gl_shader_id id(glCreateShader(t));
        glShaderSource(*id, 1, &s, nullptr);
        glCompileShader(*id);
        GLint success = GL_FALSE;
        glGetShaderiv(*id, GL_COMPILE_STATUS, &success);
        if ( success ) {
            return id;
        }
        GLint log_len = 0;
        glGetShaderiv(*id, GL_INFO_LOG_LENGTH, &log_len);
        vector<char> error_buffer(math::numeric_cast<std::size_t>(log_len) + 1, '\0');
        glGetShaderInfoLog(*id, log_len, nullptr, error_buffer.data());
        d.error("RENDER: vertex shader compilation failed:\n--> %0", error_buffer.data());
        return gl_shader_id();
    }

    gl_program_id link_program(debug& d, gl_shader_id vs, gl_shader_id fs) {
        E2D_ASSERT(!vs.empty() && !fs.empty());
        gl_program_id id(glCreateProgram());
        glAttachShader(*id, *vs);
        glAttachShader(*id, *fs);
        glLinkProgram(*id);
        GLint success = GL_FALSE;
        glGetProgramiv(*id, GL_LINK_STATUS, &success);
        if ( success ) {
            return id;
        }
        GLint log_len = 0;
        glGetProgramiv(*id, GL_INFO_LOG_LENGTH, &log_len);
        vector<char> error_buffer(math::numeric_cast<std::size_t>(log_len) + 1, '\0');
        glGetProgramInfoLog(*id, log_len, nullptr, error_buffer.data());
        d.error("RENDER: shader program linking failed:\n--> %0", error_buffer.data());
        return gl_program_id();
    }
}

namespace e2d
{
    //
    // shader::internal_state
    //

    class shader::internal_state final : private e2d::noncopyable {
    public:
        gl_program_id id;
    public:
        internal_state(gl_program_id id)
        : id(std::move(id)) {}
        ~internal_state() noexcept = default;
    };

    //
    // texture::internal_state
    //

    class texture::internal_state final : private e2d::noncopyable {
    public:
        gl_texture_id id;
        v2u native_size;
        v2u original_size;
    public:
        internal_state(gl_texture_id id)
        : id(std::move(id)) {}
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
        E2D_ASSERT(!state_->id.empty());
        glBindTexture(GL_TEXTURE_2D, *state_->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convert_wrap(u));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, convert_wrap(v));
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void texture::set_filter(filter min, filter mag) noexcept {
        E2D_ASSERT(!state_->id.empty());
        glBindTexture(GL_TEXTURE_2D, *state_->id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convert_filter(min));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convert_filter(mag));
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    const v2u& texture::native_size() const noexcept {
        return state_->native_size;
    }

    const v2u& texture::original_size() const noexcept {
        return state_->original_size;
    }

    //
    // render
    //

    render::render(debug& debug)
    : debug_(debug)
    , state_(new internal_state()) {}
    render::~render() noexcept = default;

    shader_ptr render::create_shader(input_stream_uptr vertex, input_stream_uptr fragment) {
        str vertex_str;
        gl_shader_id vs = streams::try_read_tail(vertex_str, vertex)
            ? compile_shader(debug_, vertex_str.c_str(), GL_VERTEX_SHADER)
            : gl_shader_id();
        if ( vs.empty() ) {
            return nullptr;
        }
        str fragment_str;
        gl_shader_id fs = streams::try_read_tail(fragment_str, fragment)
            ? compile_shader(debug_, fragment_str.c_str(), GL_FRAGMENT_SHADER)
            : gl_shader_id();
        if ( fs.empty() ) {
            return nullptr;
        }
        gl_program_id ps = link_program(debug_, std::move(vs), std::move(fs));
        if ( ps.empty() ) {
            return nullptr;
        }
        return std::make_unique<shader>(
            std::make_unique<shader::internal_state>(std::move(ps)));
    }

    texture_ptr render::create_texture(const image& image) {
        E2D_UNUSED(image);
        return nullptr;
    }

    texture_ptr render::create_texture(const v2u& size, image_data_format format) {
        E2D_UNUSED(size, format);
        return nullptr;
    }

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

    void render::set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept {
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

    void render::set_blend_equation(blend_equation blend_equation) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glBlendEquation(convert_blend_equation(blend_equation));
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

    void render::set_clear_depth(f32 value) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glClearDepth(math::numeric_cast<GLclampd>(value));
    }

    void render::set_stencil_func(stencil_func stencil_func, u32 ref, u32 mask) noexcept {
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

    void render::set_clear_stencil(u32 value) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        glClearStencil(math::numeric_cast<GLint>(value));
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
}

#endif
