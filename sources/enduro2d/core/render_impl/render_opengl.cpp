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

    GLenum convert_usage(index_buffer::usage u) noexcept {
        #define DEFINE_CASE(x,y) case index_buffer::usage::x: return y;
        switch ( u ) {
            DEFINE_CASE(static_draw, GL_STATIC_DRAW);
            DEFINE_CASE(stream_draw, GL_STREAM_DRAW);
            DEFINE_CASE(dynamic_draw, GL_DYNAMIC_DRAW);
            default:
                E2D_ASSERT_MSG(false, "unexpected index buffer usage");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_usage(vertex_buffer::usage u) noexcept {
        #define DEFINE_CASE(x,y) case vertex_buffer::usage::x: return y;
        switch ( u ) {
            DEFINE_CASE(static_draw, GL_STATIC_DRAW);
            DEFINE_CASE(stream_draw, GL_STREAM_DRAW);
            DEFINE_CASE(dynamic_draw, GL_DYNAMIC_DRAW);
            default:
                E2D_ASSERT_MSG(false, "unexpected vertex buffer usage");
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

    const char* glsl_type_to_cstr(GLenum t) noexcept {
        #define DEFINE_CASE(x) case x: return #x
        switch ( t ) {
            DEFINE_CASE(GL_INT);
            DEFINE_CASE(GL_FLOAT);

            DEFINE_CASE(GL_INT_VEC2);
            DEFINE_CASE(GL_INT_VEC3);
            DEFINE_CASE(GL_INT_VEC4);

            DEFINE_CASE(GL_FLOAT_VEC2);
            DEFINE_CASE(GL_FLOAT_VEC3);
            DEFINE_CASE(GL_FLOAT_VEC4);

            DEFINE_CASE(GL_FLOAT_MAT2);
            DEFINE_CASE(GL_FLOAT_MAT3);
            DEFINE_CASE(GL_FLOAT_MAT4);

            DEFINE_CASE(GL_SAMPLER_2D);
            default:
                return "GL_UNKNOWN";
        }
        #undef DEFINE_CASE
    }

    const char* gl_error_code_to_cstr(GLenum e) noexcept {
        #define DEFINE_CASE(x) case x: return #x;
        switch ( e ) {
            DEFINE_CASE(GL_INVALID_ENUM);
            DEFINE_CASE(GL_INVALID_VALUE);
            DEFINE_CASE(GL_INVALID_OPERATION);
            DEFINE_CASE(GL_INVALID_FRAMEBUFFER_OPERATION);
            DEFINE_CASE(GL_OUT_OF_MEMORY);
            default:
                return "GL_UNKNOWN";
        }
        #undef DEFINE_CASE
    }

    void gl_create_shader_wrapper(GLenum type, GLuint* id) noexcept {
        E2D_ASSERT(id);
        *id = glCreateShader(type);
    }

    void gl_create_program_wrapper(GLuint* id) noexcept {
        E2D_ASSERT(id);
        *id = glCreateProgram();
    }

    void gl_get_uniform_location(GLuint program, const GLchar* name, GLint* loc) noexcept {
        E2D_ASSERT(loc);
        *loc = glGetUniformLocation(program, name);
    }

    void gl_get_attribute_location(GLuint program, const GLchar* name, GLint* loc) noexcept {
        E2D_ASSERT(loc);
        *loc = glGetAttribLocation(program, name);
    }

    #define GL_FLUSH_ERRORS(dbg)\
        for ( GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError() ) {\
            E2D_ASSERT_MSG(false, "RENDER: GL_FLUSH_ERRORS()");\
            dbg.error("RENDER: GL_FLUSH_ERRORS():\n"\
                "--> File: %0\n"\
                "--> Line: %1\n"\
                "--> Code: %2",\
                __FILE__, __LINE__, gl_error_code_to_cstr(err));\
            if ( err == GL_OUT_OF_MEMORY ) throw std::bad_alloc();\
        }

    #define GL_CHECK_CODE(dbg, code)\
        GL_FLUSH_ERRORS(dbg);\
        code;\
        for ( GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError() ) {\
            E2D_ASSERT_MSG(false, #code);\
            dbg.error(\
                "RENDER: GL_CHECK(%0):\n"\
                "--> File: %1\n"\
                "--> Line: %2\n"\
                "--> Code: %3",\
                #code, __FILE__, __LINE__, gl_error_code_to_cstr(err));\
            if ( err == GL_OUT_OF_MEMORY ) throw std::bad_alloc();\
        }

    class gl_buffer_id final : private noncopyable {
    public:
        static gl_buffer_id create_and_bind(debug& debug, GLenum target) {
            E2D_ASSERT(
                target == GL_ARRAY_BUFFER ||
                target == GL_ELEMENT_ARRAY_BUFFER);
            GLuint id = 0;
            GL_CHECK_CODE(debug, glGenBuffers(1, &id));
            if ( !id ) {
                debug.error("RENDER: Failed to generate buffer id");
                return gl_buffer_id(debug);
            }
            GL_CHECK_CODE(debug, glBindBuffer(target, id));
            return gl_buffer_id(debug, id);
        }
    private:
        gl_buffer_id(debug& debug, GLuint id) noexcept
        : debug_(debug)
        , id_(id) {
            E2D_ASSERT(!id || glIsBuffer(id));
        }
    public:
        gl_buffer_id(debug& debug) noexcept
        : debug_(debug) {}

        ~gl_buffer_id() noexcept {
            if ( id_ ) {
                GL_CHECK_CODE(debug_, glDeleteBuffers(1, &id_));
                id_ = 0;
            }
        }

        gl_buffer_id(gl_buffer_id&& other) noexcept
        : debug_(other.debug_)
        , id_(other.release()) {}

        GLuint release() noexcept {
            GLuint result = 0;
            std::swap(result, id_);
            return result;
        }

        bool empty() const noexcept {
            return id_ == 0;
        }

        GLuint operator*() const noexcept {
            return id_;
        }
    private:
        debug& debug_;
        GLuint id_ = 0;
    };

    class gl_shader_id final : private noncopyable {
    public:
        static gl_shader_id create(debug& debug, GLenum type) noexcept {
            E2D_ASSERT(
                type == GL_VERTEX_SHADER ||
                type == GL_FRAGMENT_SHADER);
            GLuint id = 0;
            GL_CHECK_CODE(debug, gl_create_shader_wrapper(type, &id));
            if ( !id ) {
                debug.error("RENDER: Failed to generate shader id");
                return gl_shader_id(debug);
            }
            return gl_shader_id(debug, id);
        }
    private:
        gl_shader_id(debug& debug, GLuint id) noexcept
        : debug_(debug)
        , id_(id) {
            E2D_ASSERT(!id || glIsShader(id));
        }
    public:
        gl_shader_id(debug& debug) noexcept
        : debug_(debug) {}

        ~gl_shader_id() noexcept {
            if ( id_ ) {
                GL_CHECK_CODE(debug_, glDeleteShader(id_));
                id_ = 0;
            }
        }

        gl_shader_id(gl_shader_id&& other) noexcept
        : debug_(other.debug_)
        , id_(other.release()) {}

        GLuint release() noexcept {
            GLuint result = 0;
            std::swap(result, id_);
            return result;
        }

        bool empty() const noexcept {
            return id_ == 0;
        }

        GLuint operator*() const noexcept {
            return id_;
        }
    private:
        debug& debug_;
        GLuint id_ = 0;
    };

    class gl_program_id final : private noncopyable {
    public:
        static gl_program_id create(debug& debug) noexcept {
            GLuint id = 0;
            GL_CHECK_CODE(debug, gl_create_program_wrapper(&id));
            if ( !id ) {
                debug.error("RENDER: Failed to generate program id");
                return gl_program_id(debug);
            }
            return gl_program_id(debug, id);
        }
    private:
        gl_program_id(debug& debug, GLuint id) noexcept
        : debug_(debug)
        , id_(id) {
            E2D_ASSERT(!id || glIsProgram(id));
        }
    public:
        gl_program_id(debug& debug) noexcept
        : debug_(debug) {}

        ~gl_program_id() noexcept {
            if ( id_ ) {
                GL_CHECK_CODE(debug_, glDeleteProgram(id_));
                id_ = 0;
            }
        }

        gl_program_id(gl_program_id&& other) noexcept
        : debug_(other.debug_)
        , id_(other.release()) {}

        GLuint release() noexcept {
            GLuint result = 0;
            std::swap(result, id_);
            return result;
        }

        bool empty() const noexcept {
            return id_ == 0;
        }

        GLuint operator*() const noexcept {
            return id_;
        }
    private:
        debug& debug_;
        GLuint id_ = 0;
    };

    class gl_texture_id final : private noncopyable {
    public:
        static gl_texture_id create_and_bind(debug& debug, GLenum target) noexcept {
            E2D_ASSERT(
                target == GL_TEXTURE_2D ||
                target == GL_TEXTURE_CUBE_MAP);
            GLuint id = 0;
            GL_CHECK_CODE(debug, glGenTextures(1, &id));
            if ( !id ) {
                debug.error("RENDER: Failed to generate texture id");
                return gl_texture_id(debug);
            }
            GL_CHECK_CODE(debug, glBindTexture(target, id));
            return gl_texture_id(debug, id);
        }
    private:
        gl_texture_id(debug& debug, GLuint id) noexcept
        : debug_(debug)
        , id_(id) {
            E2D_ASSERT(!id || glIsTexture(id));
        }
    public:
        gl_texture_id(debug& debug) noexcept
        : debug_(debug) {}

        ~gl_texture_id() noexcept {
            if ( id_ ) {
                GL_CHECK_CODE(debug_, glDeleteTextures(1, &id_));
                id_ = 0;
            }
        }

        gl_texture_id(gl_texture_id&& other) noexcept
        : debug_(other.debug_)
        , id_(other.release()) {}

        GLuint release() noexcept {
            GLuint result = 0;
            std::swap(result, id_);
            return result;
        }

        bool empty() const noexcept {
            return id_ == 0;
        }

        GLuint operator*() const noexcept {
            return id_;
        }
    private:
        debug& debug_;
        GLuint id_ = 0;
    };

    gl_shader_id compile_shader(debug& debug, const str& source, GLenum type) noexcept {
        gl_shader_id id = gl_shader_id::create(debug, type);
        if ( id.empty() ) {
            return id;
        }
        const char* source_cstr = source.c_str();
        GL_CHECK_CODE(debug, glShaderSource(*id, 1, &source_cstr, nullptr));
        GL_CHECK_CODE(debug, glCompileShader(*id));
        GLint success = GL_FALSE;
        GL_CHECK_CODE(debug, glGetShaderiv(*id, GL_COMPILE_STATUS, &success));
        GLint log_len = 0;
        GL_CHECK_CODE(debug, glGetShaderiv(*id, GL_INFO_LOG_LENGTH, &log_len));
        if ( log_len > 0 ) {
            vector<char> log_buffer(math::numeric_cast<std::size_t>(log_len) + 1, '\0');
            GL_CHECK_CODE(debug, glGetShaderInfoLog(*id, log_len, nullptr, log_buffer.data()));
            debug.log(success ? debug::level::trace : debug::level::error,
                "RENDER: shader compilation info:\n--> %0", log_buffer.data());
        }
        return success
            ? std::move(id)
            : gl_shader_id(debug);
    }

    gl_program_id link_program(debug& debug, gl_shader_id vs, gl_shader_id fs) noexcept {
        E2D_ASSERT(!vs.empty() && !fs.empty());
        gl_program_id id = gl_program_id::create(debug);
        if ( id.empty() ) {
            return id;
        }
        GL_CHECK_CODE(debug, glAttachShader(*id, *vs));
        GL_CHECK_CODE(debug, glAttachShader(*id, *fs));

        GL_CHECK_CODE(debug, glBindAttribLocation(*id, 0, "in_pos"));
        GL_CHECK_CODE(debug, glBindAttribLocation(*id, 1, "in_uv0"));
        GL_CHECK_CODE(debug, glBindAttribLocation(*id, 2, "in_uv1"));
        GL_CHECK_CODE(debug, glBindAttribLocation(*id, 3, "in_color"));

        GL_CHECK_CODE(debug, glLinkProgram(*id));
        GLint success = GL_FALSE;
        GL_CHECK_CODE(debug, glGetProgramiv(*id, GL_LINK_STATUS, &success));
        GLint log_len = 0;
        GL_CHECK_CODE(debug, glGetProgramiv(*id, GL_INFO_LOG_LENGTH, &log_len));
        if ( log_len > 0 ) {
            vector<char> log_buffer(math::numeric_cast<std::size_t>(log_len) + 1, '\0');
            GL_CHECK_CODE(debug, glGetProgramInfoLog(*id, log_len, nullptr, log_buffer.data()));
            debug.log(success ? debug::level::trace : debug::level::error,
                "RENDER: program linking info:\n--> %0", log_buffer.data());
        }
        if ( success ) {
            GLint uniforms = 0;
            GLint attributes = 0;
            GL_CHECK_CODE(debug, glGetProgramiv(*id, GL_ACTIVE_UNIFORMS, &uniforms));
            GL_CHECK_CODE(debug, glGetProgramiv(*id, GL_ACTIVE_ATTRIBUTES, &attributes));

            GLint uniform_max_len = 0;
            GLint attribute_max_len = 0;
            GL_CHECK_CODE(debug, glGetProgramiv(*id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_max_len));
            GL_CHECK_CODE(debug, glGetProgramiv(*id, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attribute_max_len));

            debug.trace("Shader info:\n"
                "--> uniforms: %0\n"
                "--> attributes: %1\n"
                "--> uniform_max_len: %2\n"
                "--> attribute_max_len: %3",
                uniforms,
                attributes,
                uniform_max_len,
                attribute_max_len);

            vector<char> name_buffer(
                math::numeric_cast<std::size_t>(
                    math::max(uniform_max_len, attribute_max_len)), '\0');

            for ( GLuint i = 0; i < math::numeric_cast<GLuint>(uniforms); ++i ) {
                GLint size = 0;
                GLenum type = 0;
                GL_CHECK_CODE(debug, glGetActiveUniform(
                    *id,
                    i,
                    math::numeric_cast<GLsizei>(name_buffer.size()),
                    nullptr,
                    &size,
                    &type,
                    name_buffer.data()));

                GLint location = 0;
                GL_CHECK_CODE(debug, gl_get_uniform_location(
                    *id, name_buffer.data(), &location));

                debug.trace(
                    "uniform: %0, size: %1, type: %2, location: %3",
                    name_buffer.data(), size, glsl_type_to_cstr(type), location);
            }

            for ( GLuint i = 0; i < math::numeric_cast<GLuint>(attributes); ++i ) {
                GLint size = 0;
                GLenum type = 0;
                GL_CHECK_CODE(debug, glGetActiveAttrib(
                    *id,
                    i,
                    math::numeric_cast<GLsizei>(name_buffer.size()),
                    nullptr,
                    &size,
                    &type,
                    name_buffer.data()));

                GLint location = 0;
                GL_CHECK_CODE(debug, gl_get_attribute_location(
                    *id, name_buffer.data(), &location));

                debug.trace(
                    "attribute: %0, size: %1, type: %2, location: %3",
                    name_buffer.data(), size, glsl_type_to_cstr(type), location);
            }
        }
        return success
            ? std::move(id)
            : gl_program_id(debug);
    }

    bool validate_program(debug& debug, const gl_program_id& ps) noexcept {
        E2D_ASSERT(!ps.empty());
        GL_CHECK_CODE(debug, glValidateProgram(*ps));
        GLint success = GL_FALSE;
        GL_CHECK_CODE(debug, glGetProgramiv(*ps, GL_VALIDATE_STATUS, &success));
        GLint log_len = 0;
        GL_CHECK_CODE(debug, glGetProgramiv(*ps, GL_INFO_LOG_LENGTH, &log_len));
        if ( log_len > 0 ) {
            vector<char> log_buffer(math::numeric_cast<std::size_t>(log_len) + 1, '\0');
            GL_CHECK_CODE(debug, glGetProgramInfoLog(*ps, log_len, nullptr, log_buffer.data()));
            debug.log(success ? debug::level::trace : debug::level::error,
                "RENDER: program validation info:\n--> %0", log_buffer.data());
        }
        return success == GL_TRUE;
    }
}

namespace e2d
{
    //
    // shader::internal_state
    //

    class shader::internal_state final : private e2d::noncopyable {
    public:
        debug& debug;
        gl_program_id id;
    public:
        internal_state(class debug& debug, gl_program_id nid)
        : debug(debug)
        , id(std::move(nid)) {
            E2D_ASSERT(!id.empty());
        }
        ~internal_state() noexcept = default;
    };

    //
    // texture::internal_state
    //

    class texture::internal_state final : private e2d::noncopyable {
    public:
        debug& debug;
        gl_texture_id id;
    public:
        internal_state(class debug& debug, gl_texture_id nid)
        : debug(debug)
        , id(std::move(nid)) {
            E2D_ASSERT(!id.empty());
        }
        ~internal_state() noexcept = default;
    };

    //
    // index_buffer::internal_state
    //

    class index_buffer::internal_state final : private e2d::noncopyable {
    public:
        debug& debug;
        gl_buffer_id id;
        std::size_t count = 0;
    public:
        internal_state(class debug& debug, gl_buffer_id nid, std::size_t ncount)
        : debug(debug)
        , id(std::move(nid))
        , count(ncount) {
            E2D_ASSERT(!id.empty());
        }
        ~internal_state() noexcept = default;
    };

    //
    // vertex_buffer::internal_state
    //

    class vertex_buffer::internal_state final : private e2d::noncopyable {
    public:
        debug& debug;
        gl_buffer_id id;
        std::size_t count = 0;
    public:
        internal_state(class debug& debug, gl_buffer_id nid, std::size_t ncount)
        : debug(debug)
        , id(std::move(nid))
        , count(ncount) {
            E2D_ASSERT(!id.empty());
        }
        ~internal_state() noexcept = default;
    };

    //
    // render::internal_state
    //

    class render::internal_state final : private e2d::noncopyable {
    public:
        debug& debug;
        window& window;
        m4f view;
        m4f projection;
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
        GL_CHECK_CODE(state_->debug, glBindTexture(
            GL_TEXTURE_2D, *state_->id));
        GL_CHECK_CODE(state_->debug, glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convert_wrap(u)));
        GL_CHECK_CODE(state_->debug, glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, convert_wrap(v)));
        GL_CHECK_CODE(state_->debug, glBindTexture(
            GL_TEXTURE_2D, 0));
    }

    void texture::set_filter(filter min, filter mag) noexcept {
        GL_CHECK_CODE(state_->debug, glBindTexture(
            GL_TEXTURE_2D, *state_->id));
        GL_CHECK_CODE(state_->debug, glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convert_filter(min)));
        GL_CHECK_CODE(state_->debug, glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convert_filter(mag)));
        GL_CHECK_CODE(state_->debug, glBindTexture(
            GL_TEXTURE_2D, 0));
    }

    //
    // index_buffer
    //

    index_buffer::index_buffer(internal_state_uptr state)
    : state_(std::move(state)) {}
    index_buffer::~index_buffer() noexcept = default;

    void index_buffer::update(
        const u16* indices, std::size_t count, std::size_t offset) noexcept
    {
        GL_CHECK_CODE(state_->debug, glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER, *state_->id));
        GL_CHECK_CODE(state_->debug, glBufferSubData(
            GL_ELEMENT_ARRAY_BUFFER,
            math::numeric_cast<GLintptr>(sizeof(u16) * offset),
            math::numeric_cast<GLsizeiptr>(sizeof(u16) * count),
            indices));
        GL_CHECK_CODE(state_->debug, glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    std::size_t index_buffer::count() const noexcept {
        return state_->count;
    }

    //
    // vertex_buffer
    //

    vertex_buffer::vertex_buffer(internal_state_uptr state)
    : state_(std::move(state)) {}
    vertex_buffer::~vertex_buffer() noexcept = default;

    void vertex_buffer::update(
        const vertex* vertices, std::size_t count, std::size_t offset) noexcept
    {
        GL_CHECK_CODE(state_->debug, glBindBuffer(
            GL_ARRAY_BUFFER,
            *state_->id));
        GL_CHECK_CODE(state_->debug, glBufferSubData(
            GL_ARRAY_BUFFER,
            math::numeric_cast<GLintptr>(sizeof(vertex) * offset),
            math::numeric_cast<GLsizeiptr>(sizeof(vertex) * count),
            vertices));
        GL_CHECK_CODE(state_->debug, glBindBuffer(
            GL_ARRAY_BUFFER, 0));
    }

    std::size_t vertex_buffer::count() const noexcept {
        return state_->count;
    }

    //
    // render
    //

    render::render(debug& d, window& w)
    : state_(new internal_state(d, w)) {}
    render::~render() noexcept = default;

    shader_ptr render::create_shader(input_stream_uptr vertex, input_stream_uptr fragment) {
        str vertex_str;
        gl_shader_id vs = streams::try_read_tail(vertex_str, vertex)
            ? compile_shader(state_->debug, vertex_str.c_str(), GL_VERTEX_SHADER)
            : gl_shader_id(state_->debug);
        if ( vs.empty() ) {
            return nullptr;
        }
        str fragment_str;
        gl_shader_id fs = streams::try_read_tail(fragment_str, fragment)
            ? compile_shader(state_->debug, fragment_str.c_str(), GL_FRAGMENT_SHADER)
            : gl_shader_id(state_->debug);
        if ( fs.empty() ) {
            return nullptr;
        }
        gl_program_id ps = link_program(state_->debug, std::move(vs), std::move(fs));
        if ( ps.empty() || !validate_program(state_->debug, ps) ) {
            return nullptr;
        }
        return std::make_shared<shader>(
            std::make_unique<shader::internal_state>(state_->debug, std::move(ps)));
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
        const u16* indices, std::size_t count, index_buffer::usage usage)
    {
        gl_buffer_id id = gl_buffer_id::create_and_bind(
            state_->debug, GL_ELEMENT_ARRAY_BUFFER);
        if ( id.empty() ) {
            return nullptr;
        }
        GL_CHECK_CODE(state_->debug, glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            math::numeric_cast<GLsizeiptr>(sizeof(u16) * count),
            indices,
            convert_usage(usage)));
        GL_CHECK_CODE(state_->debug, glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER, 0));
        return std::make_shared<index_buffer>(
            std::make_unique<index_buffer::internal_state>(
                state_->debug, std::move(id), count));
    }

    vertex_buffer_ptr render::create_vertex_buffer(
        const vertex* vertices, std::size_t count, vertex_buffer::usage usage)
    {
        gl_buffer_id id = gl_buffer_id::create_and_bind(
            state_->debug, GL_ARRAY_BUFFER);
        if ( id.empty() ) {
            return nullptr;
        }
        GL_CHECK_CODE(state_->debug, glBufferData(
            GL_ARRAY_BUFFER,
            math::numeric_cast<GLsizeiptr>(sizeof(vertex) * count),
            vertices,
            convert_usage(usage)));
        GL_CHECK_CODE(state_->debug, glBindBuffer(
            GL_ARRAY_BUFFER, 0));
        return std::make_shared<vertex_buffer>(
            std::make_unique<vertex_buffer::internal_state>(
                state_->debug, std::move(id), count));
    }

    void render::clear(bool color, bool depth, bool stencil) noexcept {
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
        GL_CHECK_CODE(state_->debug, glClear(mask));
    }

    void render::draw(
        const shader_ptr& ps,
        const index_buffer_ptr& ib,
        const vertex_buffer_ptr& vb) noexcept
    {
        E2D_ASSERT(ps && ib && vb);

        GL_CHECK_CODE(state_->debug, glUseProgram(*ps->state_->id));
        GL_CHECK_CODE(state_->debug, glEnableVertexAttribArray(0));
        GL_CHECK_CODE(state_->debug, glEnableVertexAttribArray(1));
        GL_CHECK_CODE(state_->debug, glEnableVertexAttribArray(2));
        GL_CHECK_CODE(state_->debug, glEnableVertexAttribArray(3));

        GL_CHECK_CODE(state_->debug, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ib->state_->id));
        GL_CHECK_CODE(state_->debug, glBindBuffer(GL_ARRAY_BUFFER, *vb->state_->id));

        GL_CHECK_CODE(state_->debug, glVertexAttribPointer(
            0, 3, GL_FLOAT, GL_FALSE,
            sizeof(vertex), reinterpret_cast<const GLvoid*>(offsetof(vertex, pos))));

        GL_CHECK_CODE(state_->debug, glVertexAttribPointer(
            1, 2, GL_FLOAT, GL_FALSE,
            sizeof(vertex), reinterpret_cast<const GLvoid*>(offsetof(vertex, uv[0]))));

        GL_CHECK_CODE(state_->debug, glVertexAttribPointer(
            2, 2, GL_FLOAT, GL_FALSE,
            sizeof(vertex), reinterpret_cast<const GLvoid*>(offsetof(vertex, uv[1]))));

        GL_CHECK_CODE(state_->debug, glVertexAttribPointer(
            3, 4, GL_UNSIGNED_BYTE, GL_TRUE,
            sizeof(vertex), reinterpret_cast<const GLvoid*>(offsetof(vertex, color))));

        GL_CHECK_CODE(state_->debug, glDrawElements(
            GL_TRIANGLES,
            math::numeric_cast<GLsizei>(ib->count()),
            GL_UNSIGNED_SHORT,
            nullptr));

        GL_CHECK_CODE(state_->debug, glDisableVertexAttribArray(3));
        GL_CHECK_CODE(state_->debug, glDisableVertexAttribArray(2));
        GL_CHECK_CODE(state_->debug, glDisableVertexAttribArray(1));
        GL_CHECK_CODE(state_->debug, glDisableVertexAttribArray(0));
        GL_CHECK_CODE(state_->debug, glUseProgram(0));
    }

    void render::set_view(const m4f& view) noexcept {
        state_->view = view;
    }

    void render::set_projection(const m4f& projection) noexcept {
        state_->projection = projection;
    }

    void render::set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept {
        GL_CHECK_CODE(state_->debug, glViewport(
            math::numeric_cast<GLint>(x),
            math::numeric_cast<GLint>(y),
            math::numeric_cast<GLsizei>(w),
            math::numeric_cast<GLsizei>(h)));
    }

    void render::enable_state(state state) noexcept {
        GL_CHECK_CODE(state_->debug, glEnable(convert_state(state)));
    }

    void render::disable_state(state state) noexcept {
        GL_CHECK_CODE(state_->debug, glDisable(convert_state(state)));
    }

    void render::set_blend_func(blend_func src, blend_func dst) noexcept {
        GL_CHECK_CODE(state_->debug, glBlendFunc(
            convert_blend_func(src), convert_blend_func(dst)));
    }

    void render::set_blend_color(const color& color) noexcept {
        GL_CHECK_CODE(state_->debug, glBlendColor(
            math::numeric_cast<GLclampf>(color.r),
            math::numeric_cast<GLclampf>(color.g),
            math::numeric_cast<GLclampf>(color.b),
            math::numeric_cast<GLclampf>(color.a)));
    }

    void render::set_blend_equation(blend_equation blend_equation) noexcept {
        GL_CHECK_CODE(state_->debug, glBlendEquation(
            convert_blend_equation(blend_equation)));
    }

    void render::set_cull_face(cull_face cull_face) noexcept {
        GL_CHECK_CODE(state_->debug, glCullFace(
            convert_cull_face(cull_face)));
    }

    void render::set_depth_func(depth_func depth_func) noexcept {
        GL_CHECK_CODE(state_->debug, glDepthFunc(
            convert_depth_func(depth_func)));
    }

    void render::set_depth_mask(bool yesno) noexcept {
        GL_CHECK_CODE(state_->debug, glDepthMask(
            yesno ? GL_TRUE : GL_FALSE));
    }

    void render::set_clear_depth(f32 value) noexcept {
        GL_CHECK_CODE(state_->debug, glClearDepth(
            math::numeric_cast<GLclampd>(value)));
    }

    void render::set_stencil_func(stencil_func stencil_func, u32 ref, u32 mask) noexcept {
        GL_CHECK_CODE(state_->debug, glStencilFunc(
            convert_stencil_func(stencil_func),
            math::numeric_cast<GLint>(ref),
            math::numeric_cast<GLuint>(mask)));
    }

    void render::set_stencil_mask(u32 mask) noexcept {
        GL_CHECK_CODE(state_->debug, glStencilMask(
            math::numeric_cast<GLuint>(mask)));
    }

    void render::set_stencil_op(
        stencil_op fail, stencil_op zfail, stencil_op zpass) noexcept
    {
        GL_CHECK_CODE(state_->debug, glStencilOp(
            convert_stencil_op(fail),
            convert_stencil_op(zfail),
            convert_stencil_op(zpass)));
    }

    void render::set_clear_stencil(u32 value) noexcept {
        GL_CHECK_CODE(state_->debug, glClearStencil(
            math::numeric_cast<GLint>(value)));
    }

    void render::set_clear_color(const color& color) noexcept {
        GL_CHECK_CODE(state_->debug, glClearColor(
            math::numeric_cast<GLclampf>(color.r),
            math::numeric_cast<GLclampf>(color.g),
            math::numeric_cast<GLclampf>(color.b),
            math::numeric_cast<GLclampf>(color.a)));
    }

    void render::set_color_mask(bool r, bool g, bool b, bool a) {
        GL_CHECK_CODE(state_->debug, glColorMask(
            r ? GL_TRUE : GL_FALSE,
            g ? GL_TRUE : GL_FALSE,
            b ? GL_TRUE : GL_FALSE,
            a ? GL_TRUE : GL_FALSE));
    }
}

#endif
