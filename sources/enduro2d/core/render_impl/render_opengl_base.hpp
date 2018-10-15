/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "render.hpp"

#if defined(E2D_RENDER_MODE) && E2D_RENDER_MODE == E2D_RENDER_MODE_OPENGL

#if defined(E2D_PLATFORM) && E2D_PLATFORM == E2D_PLATFORM_MACOSX
#  include <OpenGL/gl.h>
#endif

#define GL_FLUSH_ERRORS(dbg)\
    for ( GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError() ) {\
        E2D_ASSERT_MSG(false, "RENDER: GL_FLUSH_ERRORS()");\
        (dbg).error("RENDER: GL_FLUSH_ERRORS():\n"\
            "--> File: %0\n"\
            "--> Line: %1\n"\
            "--> Code: %2",\
            __FILE__, __LINE__, e2d::opengl::gl_error_code_to_cstr(err));\
        if ( err == GL_OUT_OF_MEMORY ) throw std::bad_alloc();\
    }

#define GL_CHECK_CODE(dbg, code)\
    GL_FLUSH_ERRORS(dbg);\
    code;\
    for ( GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError() ) {\
        E2D_ASSERT_MSG(false, #code);\
        (dbg).error(\
            "RENDER: GL_CHECK(%0):\n"\
            "--> File: %1\n"\
            "--> Line: %2\n"\
            "--> Code: %3",\
            #code, __FILE__, __LINE__, e2d::opengl::gl_error_code_to_cstr(err));\
        if ( err == GL_OUT_OF_MEMORY ) throw std::bad_alloc();\
    }

namespace e2d { namespace opengl
{
    class gl_buffer_id final : private noncopyable {
        gl_buffer_id(debug& debug, GLuint id, GLenum target, bool owned) noexcept;
    public:
        static gl_buffer_id create(debug& debug, GLenum target) noexcept;
        static gl_buffer_id current(debug& debug, GLenum target) noexcept;
    public:
        explicit gl_buffer_id(debug& debug) noexcept;
        ~gl_buffer_id() noexcept;
        gl_buffer_id(gl_buffer_id&& other) noexcept;
        bool empty() const noexcept;
        GLuint operator*() const noexcept;
        GLenum target() const noexcept;
    private:
        debug& debug_;
        GLuint id_ = 0;
        GLenum target_ = 0;
        bool owned_ = false;
    };

    class gl_shader_id final : private noncopyable {
        gl_shader_id(debug& debug, GLuint id, GLenum type, bool owned) noexcept;
    public:
        static gl_shader_id create(debug& debug, GLenum type) noexcept;
    public:
        explicit gl_shader_id(debug& debug) noexcept;
        ~gl_shader_id() noexcept;
        gl_shader_id(gl_shader_id&& other) noexcept;
        bool empty() const noexcept;
        GLuint operator*() const noexcept;
        GLenum type() const noexcept;
    private:
        debug& debug_;
        GLuint id_ = 0;
        GLenum type_ = 0;
        bool owned_ = false;
    };

    class gl_program_id final : private noncopyable {
        gl_program_id(debug& debug, GLuint id, bool owned) noexcept;
    public:
        static gl_program_id create(debug& debug) noexcept;
        static gl_program_id current(debug& debug) noexcept;
    public:
        explicit gl_program_id(debug& debug) noexcept;
        ~gl_program_id() noexcept;
        gl_program_id(gl_program_id&& other) noexcept;
        bool empty() const noexcept;
        GLuint operator*() const noexcept;
    private:
        debug& debug_;
        GLuint id_ = 0;
        bool owned_ = false;
    };

    class gl_texture_id final : private noncopyable {
        gl_texture_id(debug& debug, GLuint id, GLenum target, bool owned) noexcept;
    public:
        static gl_texture_id create(debug& debug, GLenum target) noexcept;
        static gl_texture_id current(debug& debug, GLenum target) noexcept;
    public:
        explicit gl_texture_id(debug& debug) noexcept;
        ~gl_texture_id() noexcept;
        gl_texture_id(gl_texture_id&& other) noexcept;
        bool empty() const noexcept;
        GLuint operator*() const noexcept;
        GLenum target() const noexcept;
    private:
        debug& debug_;
        GLuint id_ = 0;
        GLenum target_ = 0;
        bool owned_ = false;
    };

    class gl_framebuffer_id final : private noncopyable {
        gl_framebuffer_id(debug& debug, GLuint id, GLenum target, bool owned) noexcept;
    public:
        static gl_framebuffer_id create(debug& debug, GLenum target) noexcept;
        static gl_framebuffer_id current(debug& debug, GLenum target) noexcept;
    public:
        explicit gl_framebuffer_id(debug& debug) noexcept;
        ~gl_framebuffer_id() noexcept;
        gl_framebuffer_id(gl_framebuffer_id&& other) noexcept;
        bool empty() const noexcept;
        GLuint operator*() const noexcept;
        GLenum target() const noexcept;
    private:
        debug& debug_;
        GLuint id_ = 0;
        GLenum target_ = 0;
        bool owned_ = false;
    };

    bool operator==(const gl_buffer_id& l, const gl_buffer_id& r) noexcept;
    bool operator==(const gl_shader_id& l, const gl_shader_id& r) noexcept;
    bool operator==(const gl_program_id& l, const gl_program_id& r) noexcept;
    bool operator==(const gl_texture_id& l, const gl_texture_id& r) noexcept;
    bool operator==(const gl_framebuffer_id& l, const gl_framebuffer_id& r) noexcept;

    bool operator!=(const gl_buffer_id& l, const gl_buffer_id& r) noexcept;
    bool operator!=(const gl_shader_id& l, const gl_shader_id& r) noexcept;
    bool operator!=(const gl_program_id& l, const gl_program_id& r) noexcept;
    bool operator!=(const gl_texture_id& l, const gl_texture_id& r) noexcept;
    bool operator!=(const gl_framebuffer_id& l, const gl_framebuffer_id& r) noexcept;
}}

namespace e2d { namespace opengl
{
    enum class uniform_type : u8 {
        signed_integer,
        floating_point,

        v2i,
        v3i,
        v4i,

        v2f,
        v3f,
        v4f,

        m2f,
        m3f,
        m4f,

        sampler_2d,
        sampler_cube,

        unknown
    };

    enum class attribute_type : u8 {
        floating_point,

        v2f,
        v3f,
        v4f,

        m2f,
        m3f,
        m4f,

        unknown
    };

    const char* uniform_type_to_cstr(uniform_type ut) noexcept;
    const char* attribute_type_to_cstr(attribute_type at) noexcept;
}}

namespace e2d { namespace opengl
{
    GLint convert_format_to_internal_format(image_data_format idf) noexcept;
    GLenum convert_format_to_external_format(image_data_format idf) noexcept;
    GLenum convert_format_to_external_data_type(image_data_format idf) noexcept;

    GLenum convert_index_type(index_declaration::index_type it) noexcept;
    GLenum convert_attribute_type(vertex_declaration::attribute_type at) noexcept;

    GLint convert_uniform_type(uniform_type ut) noexcept;
    GLint convert_attribute_type(attribute_type at) noexcept;

    GLint convert_sampler_wrap(render::sampler_wrap w) noexcept;
    GLint convert_sampler_filter(render::sampler_min_filter f) noexcept;
    GLint convert_sampler_filter(render::sampler_mag_filter f) noexcept;

    GLenum convert_buffer_usage(index_buffer::usage u) noexcept;
    GLenum convert_buffer_usage(vertex_buffer::usage u) noexcept;
    GLenum convert_topology(render::topology t) noexcept;
    GLenum convert_stencil_op(render::stencil_op sa) noexcept;
    GLenum convert_compare_func(render::compare_func cf) noexcept;
    GLenum convert_culling_mode(render::culling_mode cm) noexcept;
    GLenum convert_culling_face(render::culling_face cf) noexcept;
    GLenum convert_blending_factor(render::blending_factor bf) noexcept;
    GLenum convert_blending_equation(render::blending_equation be) noexcept;

    uniform_type glsl_type_to_uniform_type(GLenum t) noexcept;
    attribute_type glsl_type_to_attribute_type(GLenum t) noexcept;

    const char* glsl_type_to_cstr(GLenum t) noexcept;
    const char* gl_error_code_to_cstr(GLenum e) noexcept;
    GLenum gl_target_to_get_target(GLenum t) noexcept;
}}

namespace e2d { namespace opengl
{
    void gl_trace_info(debug& debug) noexcept;
    gl_shader_id gl_compile_shader(debug& debug, const str& source, GLenum type) noexcept;
    gl_program_id gl_link_program(debug& debug, gl_shader_id vs, gl_shader_id fs) noexcept;
    gl_texture_id gl_compile_texture(debug& debug, const image& image);
    gl_buffer_id gl_compile_index_buffer(debug& debug, const buffer& indices, index_buffer::usage usage);
    gl_buffer_id gl_compile_vertex_buffer(debug& debug, const buffer& vertices, vertex_buffer::usage usage);
}}

namespace e2d { namespace opengl
{
    struct uniform_info {
        str_hash name;
        GLint size = 0;
        GLint location = -1;
        uniform_type type = uniform_type::floating_point;
        u8 _pad[3] = {0};
    public:
        uniform_info(
            str_hash nname,
            GLint nsize,
            GLint nlocation,
            uniform_type ntype) noexcept
        : name(std::move(nname))
        , size(nsize)
        , location(nlocation)
        , type(ntype) {}
    };

    struct attribute_info {
        str_hash name;
        GLint size = 0;
        GLint location = -1;
        attribute_type type = attribute_type::floating_point;
        u8 _pad[3] = {0};
    public:
        attribute_info(
            str_hash nname,
            GLint nsize,
            GLint nlocation,
            attribute_type ntype) noexcept
        : name(std::move(nname))
        , size(nsize)
        , location(nlocation)
        , type(ntype) {}
    };

    void grab_program_uniforms(
        debug& debug,
        GLuint program,
        vector<uniform_info>& uniforms);

    void grab_program_attributes(
        debug& debug,
        GLuint program,
        vector<attribute_info>& attributes);
}}

namespace e2d { namespace opengl
{
    //
    // with_gl_use_program
    //

    template < typename F, typename... Args >
    void with_gl_use_program(debug& debug, GLuint program, F&& f, Args&&... args) {
        GLint prev_program = 0;
        GL_CHECK_CODE(debug, glGetIntegerv(
            GL_CURRENT_PROGRAM, &prev_program));
        GL_CHECK_CODE(debug, glUseProgram(
            program));
        try {
            stdex::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        } catch (...) {
            GL_CHECK_CODE(debug, glUseProgram(
                math::numeric_cast<GLuint>(prev_program)));
            throw;
        }
        GL_CHECK_CODE(debug, glUseProgram(
            math::numeric_cast<GLuint>(prev_program)));
    }

    template < typename F, typename... Args >
    void with_gl_use_program(debug& debug, const gl_program_id& program, F&& f, Args&&... args) {
        with_gl_use_program(debug, *program, std::forward<F>(f), std::forward<Args>(args)...);
    }

    //
    // with_gl_bind_buffer
    //

    template < typename F, typename... Args >
    void with_gl_bind_buffer(debug& debug, GLenum target, GLuint buffer, F&& f, Args&&... args) {
        GLint prev_buffer = 0;
        GL_CHECK_CODE(debug, glGetIntegerv(
            gl_target_to_get_target(target), &prev_buffer));
        GL_CHECK_CODE(debug, glBindBuffer(
            target, buffer));
        try {
            stdex::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        } catch (...) {
            GL_CHECK_CODE(debug, glBindBuffer(
                target, math::numeric_cast<GLuint>(prev_buffer)));
            throw;
        }
        GL_CHECK_CODE(debug, glBindBuffer(
            target, math::numeric_cast<GLuint>(prev_buffer)));
    }

    template < typename F, typename... Args >
    void with_gl_bind_buffer(debug& debug, const gl_buffer_id& buffer, F&& f, Args&&... args) {
        with_gl_bind_buffer(debug, buffer.target(), *buffer, std::forward<F>(f), std::forward<Args>(args)...);
    }

    //
    // with_gl_bind_texture
    //

    template < typename F, typename... Args >
    void with_gl_bind_texture(debug& debug, GLenum target, GLuint texture, F&& f, Args&&... args) {
        GLint prev_texture = 0;
        GL_CHECK_CODE(debug, glGetIntegerv(
            gl_target_to_get_target(target), &prev_texture));
        GL_CHECK_CODE(debug, glBindTexture(
            target, texture));
        try {
            stdex::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        } catch (...) {
            GL_CHECK_CODE(debug, glBindTexture(
                target, math::numeric_cast<GLuint>(prev_texture)));
            throw;
        }
        GL_CHECK_CODE(debug, glBindTexture(
            target, math::numeric_cast<GLuint>(prev_texture)));
    }

    template < typename F, typename... Args >
    void with_gl_bind_texture(debug& debug, const gl_texture_id& texture, F&& f, Args&&... args) {
        with_gl_bind_texture(debug, texture.target(), *texture, std::forward<F>(f), std::forward<Args>(args)...);
    }

    //
    // with_gl_bind_framebuffer
    //

    template < typename F, typename... Args >
    void with_gl_bind_framebuffer(debug& debug, GLenum target, GLuint framebuffer, F&& f, Args&&... args) {
        GLint prev_framebuffer = 0;
        GL_CHECK_CODE(debug, glGetIntegerv(
            gl_target_to_get_target(target), &prev_framebuffer));
        GL_CHECK_CODE(debug, glBindFramebuffer(
            target, framebuffer));
        try {
            stdex::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        } catch (...) {
            GL_CHECK_CODE(debug, glBindFramebuffer(
                target, math::numeric_cast<GLuint>(prev_framebuffer)));
            throw;
        }
        GL_CHECK_CODE(debug, glBindFramebuffer(
            target, math::numeric_cast<GLuint>(prev_framebuffer)));
    }

    template < typename F, typename... Args >
    void with_gl_bind_framebuffer(debug& debug, const gl_framebuffer_id& framebuffer, F&& f, Args&&... args) {
        with_gl_bind_framebuffer(debug, framebuffer.target(), *framebuffer, std::forward<F>(f), std::forward<Args>(args)...);
    }
}}

#endif
