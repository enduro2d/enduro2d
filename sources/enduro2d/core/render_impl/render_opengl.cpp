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

    GLenum convert_index_type(index_declaration::index_type it) noexcept {
        #define DEFINE_CASE(x,y) case index_declaration::index_type::x: return y;
        switch ( it ) {
            DEFINE_CASE(unsigned_byte, GL_UNSIGNED_BYTE);
            DEFINE_CASE(unsigned_short, GL_UNSIGNED_SHORT);
            default:
                E2D_ASSERT_MSG(false, "unexpected index type");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_attribute_type(vertex_declaration::attribute_type at) noexcept {
        #define DEFINE_CASE(x,y) case vertex_declaration::attribute_type::x: return y;
        switch ( at ) {
            DEFINE_CASE(signed_byte, GL_BYTE);
            DEFINE_CASE(unsigned_byte, GL_UNSIGNED_BYTE);
            DEFINE_CASE(signed_short, GL_SHORT);
            DEFINE_CASE(unsigned_short, GL_UNSIGNED_SHORT);
            DEFINE_CASE(floating_point, GL_FLOAT);
            default:
                E2D_ASSERT_MSG(false, "unexpected attribute type");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLint convert_uniform_type(shader::uniform_type ut) noexcept {
        #define DEFINE_CASE(x,y) case shader::uniform_type::x: return y;
        switch ( ut ) {
            DEFINE_CASE(signed_integer, GL_INT);
            DEFINE_CASE(floating_point, GL_FLOAT);

            DEFINE_CASE(v2i, GL_INT_VEC2);
            DEFINE_CASE(v3i, GL_INT_VEC3);
            DEFINE_CASE(v4i, GL_INT_VEC4);

            DEFINE_CASE(v2f, GL_FLOAT_VEC2);
            DEFINE_CASE(v3f, GL_FLOAT_VEC3);
            DEFINE_CASE(v4f, GL_FLOAT_VEC4);

            DEFINE_CASE(m2f, GL_FLOAT_MAT2);
            DEFINE_CASE(m3f, GL_FLOAT_MAT3);
            DEFINE_CASE(m4f, GL_FLOAT_MAT4);

            DEFINE_CASE(sampler_2d, GL_SAMPLER_2D);
            DEFINE_CASE(sampler_cube, GL_SAMPLER_CUBE);
            default:
                E2D_ASSERT_MSG(false, "unexpected uniform type");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLint convert_attribute_type(shader::attribute_type at) noexcept {
        #define DEFINE_CASE(x,y) case shader::attribute_type::x: return y;
        switch ( at ) {
            DEFINE_CASE(floating_point, GL_FLOAT);

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

    GLenum convert_topology(render::topology t) noexcept {
        #define DEFINE_CASE(x,y) case render::topology::x: return y;
        switch ( t ) {
            DEFINE_CASE(triangles, GL_TRIANGLES);
            DEFINE_CASE(triangles_fan, GL_TRIANGLE_FAN);
            DEFINE_CASE(triangles_strip, GL_TRIANGLE_STRIP);
            default:
                E2D_ASSERT_MSG(false, "unexpected render topology");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_stencil_op(render::stencil_op sa) noexcept {
        #define DEFINE_CASE(x,y) case render::stencil_op::x: return y;
        switch ( sa ) {
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

    GLenum convert_compare_func(render::compare_func cf) noexcept {
        #define DEFINE_CASE(x,y) case render::compare_func::x: return y;
        switch ( cf ) {
            DEFINE_CASE(never, GL_NEVER);
            DEFINE_CASE(less, GL_LESS);
            DEFINE_CASE(lequal, GL_LEQUAL);
            DEFINE_CASE(greater, GL_GREATER);
            DEFINE_CASE(gequal, GL_GEQUAL);
            DEFINE_CASE(equal, GL_EQUAL);
            DEFINE_CASE(notequal, GL_NOTEQUAL);
            DEFINE_CASE(always, GL_ALWAYS);
            default:
                E2D_ASSERT_MSG(false, "unexpected render compare func");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_culling_mode(render::culling_mode cm) noexcept {
        #define DEFINE_CASE(x,y) case render::culling_mode::x: return y;
        switch ( cm ) {
            DEFINE_CASE(cw, GL_CW);
            DEFINE_CASE(ccw, GL_CCW);
            default:
                E2D_ASSERT_MSG(false, "unexpected render culling mode");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_culling_face(render::culling_face cf) noexcept {
        #define DEFINE_CASE(x,y) case render::culling_face::x: return y;
        switch ( cf ) {
            DEFINE_CASE(back, GL_BACK);
            DEFINE_CASE(front, GL_FRONT);
            DEFINE_CASE(back_and_front, GL_FRONT_AND_BACK);
            default:
                E2D_ASSERT_MSG(false, "unexpected render culling face");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_blending_factor(render::blending_factor bf) noexcept {
        #define DEFINE_CASE(x,y) case render::blending_factor::x: return y;
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
                E2D_ASSERT_MSG(false, "unexpected render blending factor");
                return 0;
        }
        #undef DEFINE_CASE
    }

    GLenum convert_blending_equation(render::blending_equation be) noexcept {
        #define DEFINE_CASE(x,y) case render::blending_equation::x: return y;
        switch ( be ) {
            DEFINE_CASE(add, GL_FUNC_ADD);
            DEFINE_CASE(subtract, GL_FUNC_SUBTRACT);
            DEFINE_CASE(reverse_subtract, GL_FUNC_REVERSE_SUBTRACT);
            default:
                E2D_ASSERT_MSG(false, "unexpected render blending equation");
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
            DEFINE_CASE(GL_SAMPLER_CUBE);
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

    void gl_get_string(GLenum name, const char** str) noexcept {
        E2D_ASSERT(str);
        *str = reinterpret_cast<const char*>(glGetString(name));
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

    void gl_enable_or_disable(GLenum cap, bool enable) noexcept {
        if ( enable ) {
            glEnable(cap);
        } else {
            glDisable(cap);
        }
    }

    #define GL_FLUSH_ERRORS(dbg)\
        for ( GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError() ) {\
            E2D_ASSERT_MSG(false, "RENDER: GL_FLUSH_ERRORS()");\
            (dbg).error("RENDER: GL_FLUSH_ERRORS():\n"\
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
            (dbg).error(\
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
        explicit gl_buffer_id(debug& debug) noexcept
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
        explicit gl_shader_id(debug& debug) noexcept
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
        explicit gl_program_id(debug& debug) noexcept
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
        explicit gl_texture_id(debug& debug) noexcept
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

    void trace_opengl_info(debug& debug) noexcept {
        const char* vendor = nullptr;
        GL_CHECK_CODE(debug, gl_get_string(GL_VENDOR, &vendor));
        const char* renderer = nullptr;
        GL_CHECK_CODE(debug, gl_get_string(GL_RENDERER, &renderer));
        const char* version = nullptr;
        GL_CHECK_CODE(debug, gl_get_string(GL_VERSION, &version));
        const char* language = nullptr;
        GL_CHECK_CODE(debug, gl_get_string(GL_SHADING_LANGUAGE_VERSION, &language));
        const char* extensions = nullptr;
        GL_CHECK_CODE(debug, gl_get_string(GL_EXTENSIONS, &extensions));
        const auto cstr_or_undefined = [](const char* cstr) noexcept {
            return (cstr && *cstr)
                ? cstr
                : "(undefined)";
        };
        debug.trace("RENDER: opengl info:\n"
            "--> VENDOR: %0\n"
            "--> RENDERER: %1\n"
            "--> VERSION: %2\n"
            "--> LANGUAGE: %3\n"
            "--> EXTENSIONS: %4",
            cstr_or_undefined(vendor),
            cstr_or_undefined(renderer),
            cstr_or_undefined(version),
            cstr_or_undefined(language),
            cstr_or_undefined(extensions));
    }

    void trace_current_program_info(debug& debug, GLuint program) noexcept {
        E2D_ASSERT(glIsProgram(program));

        GLint uniforms = 0;
        GLint attributes = 0;
        GL_CHECK_CODE(debug, glGetProgramiv(
            program, GL_ACTIVE_UNIFORMS, &uniforms));
        GL_CHECK_CODE(debug, glGetProgramiv(
            program, GL_ACTIVE_ATTRIBUTES, &attributes));

        debug.trace("RENDER: Program info:\n"
            "--> active uniforms: %0\n"
            "--> active attributes: %1",
            uniforms,
            attributes);

        GLint uniform_max_len = 0;
        GLint attribute_max_len = 0;
        GL_CHECK_CODE(debug, glGetProgramiv(
            program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniform_max_len));
        GL_CHECK_CODE(debug, glGetProgramiv(
            program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &attribute_max_len));

        GLint max_name_len = math::max(uniform_max_len, attribute_max_len);
        GLchar* name_buffer = static_cast<GLchar*>(E2D_ALLOCA(
            sizeof(GLchar) * math::numeric_cast<std::size_t>(max_name_len)));
        E2D_ASSERT(name_buffer);

        for ( GLuint i = 0; i < math::numeric_cast<GLuint>(uniforms); ++i ) {
            GLint size = 0;
            GLenum type = 0;
            GL_CHECK_CODE(debug, glGetActiveUniform(
                program, i, max_name_len,
                nullptr, &size, &type, name_buffer));
            GLint location = 0;
            GL_CHECK_CODE(debug, gl_get_uniform_location(
                program, name_buffer, &location));
            debug.trace(
                "uniform: %0, size: %1, type: %2, location: %3",
                name_buffer, size, glsl_type_to_cstr(type), location);
        }

        for ( GLuint i = 0; i < math::numeric_cast<GLuint>(attributes); ++i ) {
            GLint size = 0;
            GLenum type = 0;
            GL_CHECK_CODE(debug, glGetActiveAttrib(
                program, i, max_name_len,
                nullptr, &size, &type, name_buffer));
            GLint location = 0;
            GL_CHECK_CODE(debug, gl_get_attribute_location(
                program, name_buffer, &location));
            debug.trace(
                "attribute: %0, size: %1, type: %2, location: %3",
                name_buffer, size, glsl_type_to_cstr(type), location);
        }
    }

    bool process_shader_compilation_result(debug& debug, GLuint shader) noexcept {
        E2D_ASSERT(glIsShader(shader));
        GLint success = GL_FALSE;
        GL_CHECK_CODE(debug, glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        GLint log_len = 0;
        GL_CHECK_CODE(debug, glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len));
        if ( log_len > 0 ) {
            GLchar* log_buffer = static_cast<GLchar*>(E2D_ALLOCA(
                sizeof(GLchar) * math::numeric_cast<std::size_t>(log_len)));
            GL_CHECK_CODE(debug, glGetShaderInfoLog(
                shader, log_len, nullptr, log_buffer));
            debug.log(success ? debug::level::warning : debug::level::error,
                "RENDER: shader compilation info:\n--> %0", log_buffer);
        }
        return success == GL_TRUE;
    }

    bool process_program_linking_result(debug& debug, GLuint program) noexcept {
        E2D_ASSERT(glIsProgram(program));
        GLint success = GL_FALSE;
        GL_CHECK_CODE(debug, glGetProgramiv(program, GL_LINK_STATUS, &success));
        GLint log_len = 0;
        GL_CHECK_CODE(debug, glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len));
        if ( log_len > 0 ) {
            GLchar* log_buffer = static_cast<GLchar*>(E2D_ALLOCA(
                sizeof(GLchar) * math::numeric_cast<std::size_t>(log_len)));
            GL_CHECK_CODE(debug, glGetProgramInfoLog(
                program, log_len, nullptr, log_buffer));
            debug.log(success ? debug::level::warning : debug::level::error,
                "RENDER: program linking info:\n--> %0", log_buffer);
        }
        return success == GL_TRUE;
    }

    gl_shader_id compile_shader(
        debug& debug, const str& source, GLenum type) noexcept
    {
        gl_shader_id id = gl_shader_id::create(debug, type);
        if ( id.empty() ) {
            return id;
        }
        const char* source_cstr = source.c_str();
        GL_CHECK_CODE(debug, glShaderSource(*id, 1, &source_cstr, nullptr));
        GL_CHECK_CODE(debug, glCompileShader(*id));
        return process_shader_compilation_result(debug, *id)
            ? std::move(id)
            : gl_shader_id(debug);
    }

    bool rebind_program_attributes(
        debug& debug, GLuint program, const vertex_declaration& decl) noexcept
    {
        E2D_ASSERT(glIsProgram(program));
        GLuint location = 0;
        for ( std::size_t i = 0, e = decl.attribute_count(); i < e; ++i ) {
            const vertex_declaration::attribute_info& ai = decl.attribute(i);
            GL_CHECK_CODE(debug, glBindAttribLocation(
                program, location, ai.name.c_str()));
            location += math::numeric_cast<GLuint>(ai.rows);
        }
        return true;
    }

    gl_program_id link_program(
        debug& debug,
        gl_shader_id vs,
        gl_shader_id fs,
        const vertex_declaration& decl) noexcept
    {
        E2D_ASSERT(!vs.empty() && !fs.empty());
        gl_program_id id = gl_program_id::create(debug);
        if ( id.empty() ) {
            return id;
        }
        GL_CHECK_CODE(debug, glAttachShader(*id, *vs));
        GL_CHECK_CODE(debug, glAttachShader(*id, *fs));
        if ( !rebind_program_attributes(debug, *id, decl) ) {
            return gl_program_id(debug);
        }
        GL_CHECK_CODE(debug, glLinkProgram(*id));
        if ( !process_program_linking_result(debug, *id) ) {
            return gl_program_id(debug);
        }
        trace_current_program_info(debug, *id);
        return id;
    }

    bool validate_program(debug& debug, const gl_program_id& ps) noexcept {
        E2D_ASSERT(!ps.empty());
        GL_CHECK_CODE(debug, glValidateProgram(*ps));
        GLint success = GL_FALSE;
        GL_CHECK_CODE(debug, glGetProgramiv(*ps, GL_VALIDATE_STATUS, &success));
        GLint log_len = 0;
        GL_CHECK_CODE(debug, glGetProgramiv(*ps, GL_INFO_LOG_LENGTH, &log_len));
        if ( log_len > 0 ) {
            GLchar* log_buffer = static_cast<GLchar*>(E2D_ALLOCA(
                sizeof(GLchar) * math::numeric_cast<std::size_t>(log_len)));
            GL_CHECK_CODE(debug, glGetProgramInfoLog(*ps, log_len, nullptr, log_buffer));
            debug.log(success ? debug::level::warning : debug::level::error,
                "RENDER: program validation info:\n--> %0", log_buffer);
        }
        return success == GL_TRUE;
    }

    void bind_vertex_declaration(debug& debug, const vertex_declaration& decl) noexcept {
        GLuint location = 0;
        for ( std::size_t i = 0; i < decl.attribute_count(); ++i ) {
            const vertex_declaration::attribute_info& ai = decl.attribute(i);
            const GLuint rows = math::numeric_cast<GLuint>(ai.rows);
            for ( GLuint row = 0; row < rows; ++row ) {
                GL_CHECK_CODE(debug, glEnableVertexAttribArray(
                    location + row));
                GL_CHECK_CODE(debug, glVertexAttribPointer(
                    location + row,
                    math::numeric_cast<GLint>(ai.columns),
                    convert_attribute_type(ai.type),
                    ai.normalized ? GL_TRUE : GL_FALSE,
                    math::numeric_cast<GLsizei>(decl.vertex_size()),
                    reinterpret_cast<const GLvoid*>(ai.stride + row * ai.row_size())));
            }
            location += rows;
        }
    }

    void unbind_vertex_declaration(debug& debug, const vertex_declaration& decl) noexcept {
        GLuint location = 0;
        for ( std::size_t i = 0; i < decl.attribute_count(); ++i ) {
            const vertex_declaration::attribute_info& ai = decl.attribute(i);
            const GLuint rows = math::numeric_cast<GLuint>(ai.rows);
            for ( GLuint row = 0; row < rows; ++row ) {
                GL_CHECK_CODE(debug, glDisableVertexAttribArray(
                    location + row));
            }
            location += rows;
        }
    }
}

namespace e2d
{
    //
    // shader::internal_state
    //

    class shader::internal_state final : private e2d::noncopyable {
    public:
        debug& debug_;
        gl_program_id id_;
        vertex_declaration decl_;
    public:
        internal_state(
            debug& debug,
            gl_program_id id,
            const vertex_declaration& decl)
        : debug_(debug)
        , id_(std::move(id))
        , decl_(decl) {
            E2D_ASSERT(!id_.empty());
        }
        ~internal_state() noexcept = default;
    };

    //
    // texture::internal_state
    //

    class texture::internal_state final : private e2d::noncopyable {
    public:
        debug& debug_;
        gl_texture_id id_;
    public:
        internal_state(debug& debug, gl_texture_id id)
        : debug_(debug)
        , id_(std::move(id)) {
            E2D_ASSERT(!id_.empty());
        }
        ~internal_state() noexcept = default;
    };

    //
    // index_buffer::internal_state
    //

    class index_buffer::internal_state final : private e2d::noncopyable {
    public:
        debug& debug_;
        gl_buffer_id id_;
        std::size_t size_ = 0;
        index_declaration decl_;
        usage usage_;
    public:
        internal_state(
            debug& debug,
            gl_buffer_id id,
            std::size_t size,
            const index_declaration& decl,
            usage usage)
        : debug_(debug)
        , id_(std::move(id))
        , size_(size)
        , decl_(decl)
        , usage_(usage) {
            E2D_ASSERT(!id_.empty());
        }
        ~internal_state() noexcept = default;
    };

    //
    // vertex_buffer::internal_state
    //

    class vertex_buffer::internal_state final : private e2d::noncopyable {
    public:
        debug& debug_;
        gl_buffer_id id_;
        std::size_t size_ = 0;
        vertex_declaration decl_;
        usage usage_;
    public:
        internal_state(
            debug& debug,
            gl_buffer_id id,
            std::size_t size,
            const vertex_declaration& decl,
            usage usage)
        : debug_(debug)
        , id_(std::move(id))
        , size_(size)
        , decl_(decl)
        , usage_(usage) {
            E2D_ASSERT(!id_.empty());
        }
        ~internal_state() noexcept = default;
    };

    //
    // render::internal_state
    //

    class render::internal_state final : private e2d::noncopyable {
    public:
        debug& debug_;
        window& window_;
        m4f model_;
        m4f view_;
        m4f projection_;
    public:
        internal_state(debug& debug, window& window)
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
        const std::size_t name_buffer_size = name.length() + 1;
        GLchar* name_buffer = static_cast<GLchar*>(E2D_ALLOCA(name_buffer_size));
        strings::format_nothrow(name_buffer, name_buffer_size, nullptr, "%0", name);
        GLint location = 0;
        GL_CHECK_CODE(state_->debug_, gl_get_uniform_location(
            *state_->id_, name_buffer, &location));
        if ( location != -1 ) {
            GL_CHECK_CODE(state_->debug_, glUseProgram(*state_->id_));
            GL_CHECK_CODE(state_->debug_, glUniform1i(
                location, math::numeric_cast<GLint>(value)));
            GL_CHECK_CODE(state_->debug_, glUseProgram(0));
        }
    }

    void shader::set_uniform(str_view name, f32 value) const noexcept {
        const std::size_t name_buffer_size = name.length() + 1;
        GLchar* name_buffer = static_cast<GLchar*>(E2D_ALLOCA(name_buffer_size));
        strings::format_nothrow(name_buffer, name_buffer_size, nullptr, "%0", name);
        GLint location = 0;
        GL_CHECK_CODE(state_->debug_, gl_get_uniform_location(
            *state_->id_, name_buffer, &location));
        if ( location != -1 ) {
            GL_CHECK_CODE(state_->debug_, glUseProgram(*state_->id_));
            GL_CHECK_CODE(state_->debug_, glUniform1f(
                location, math::numeric_cast<GLfloat>(value)));
            GL_CHECK_CODE(state_->debug_, glUseProgram(0));
        }
    }

    //
    // texture
    //

    texture::texture(internal_state_uptr state)
    : state_(std::move(state)) {}
    texture::~texture() noexcept = default;

    void texture::set_wrap(wrap u, wrap v) noexcept {
        GL_CHECK_CODE(state_->debug_, glBindTexture(
            GL_TEXTURE_2D, *state_->id_));
        GL_CHECK_CODE(state_->debug_, glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, convert_wrap(u)));
        GL_CHECK_CODE(state_->debug_, glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, convert_wrap(v)));
        GL_CHECK_CODE(state_->debug_, glBindTexture(
            GL_TEXTURE_2D, 0));
    }

    void texture::set_filter(filter min, filter mag) noexcept {
        GL_CHECK_CODE(state_->debug_, glBindTexture(
            GL_TEXTURE_2D, *state_->id_));
        GL_CHECK_CODE(state_->debug_, glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, convert_filter(min)));
        GL_CHECK_CODE(state_->debug_, glTexParameteri(
            GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, convert_filter(mag)));
        GL_CHECK_CODE(state_->debug_, glBindTexture(
            GL_TEXTURE_2D, 0));
    }

    //
    // index_buffer
    //

    index_buffer::index_buffer(internal_state_uptr state)
    : state_(std::move(state)) {}
    index_buffer::~index_buffer() noexcept = default;

    void index_buffer::update(const buffer& indices, std::size_t offset) noexcept {
        const std::size_t buffer_offset = offset * state_->decl_.index_size();
        E2D_ASSERT(indices.size() + buffer_offset <= state_->size_);
        E2D_ASSERT(indices.size() % state_->decl_.index_size() == 0);
        GL_CHECK_CODE(state_->debug_, glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER, *state_->id_));
        GL_CHECK_CODE(state_->debug_, glBufferSubData(
            GL_ELEMENT_ARRAY_BUFFER,
            math::numeric_cast<GLintptr>(buffer_offset),
            math::numeric_cast<GLsizeiptr>(indices.size()),
            indices.data()));
        GL_CHECK_CODE(state_->debug_, glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER, 0));
    }

    const index_declaration& index_buffer::decl() const noexcept {
        return state_->decl_;
    }

    index_buffer::usage index_buffer::buffer_usage() const noexcept {
        return state_->usage_;
    }

    std::size_t index_buffer::buffer_size() const noexcept {
        return state_->size_;
    }

    std::size_t index_buffer::index_count() const noexcept {
        E2D_ASSERT(state_->size_ % state_->decl_.index_size() == 0);
        return state_->size_ / state_->decl_.index_size();
    }

    //
    // vertex_buffer
    //

    vertex_buffer::vertex_buffer(internal_state_uptr state)
    : state_(std::move(state)) {}
    vertex_buffer::~vertex_buffer() noexcept = default;

    void vertex_buffer::update(const buffer& vertices, std::size_t offset) noexcept {
        const std::size_t buffer_offset = offset * state_->decl_.vertex_size();
        E2D_ASSERT(vertices.size() + buffer_offset <= state_->size_);
        E2D_ASSERT(vertices.size() % state_->decl_.vertex_size() == 0);
        GL_CHECK_CODE(state_->debug_, glBindBuffer(
            GL_ARRAY_BUFFER,
            *state_->id_));
        GL_CHECK_CODE(state_->debug_, glBufferSubData(
            GL_ARRAY_BUFFER,
            math::numeric_cast<GLintptr>(buffer_offset),
            math::numeric_cast<GLsizeiptr>(vertices.size()),
            vertices.data()));
        GL_CHECK_CODE(state_->debug_, glBindBuffer(
            GL_ARRAY_BUFFER, 0));
    }

    const vertex_declaration& vertex_buffer::decl() const noexcept {
        return state_->decl_;
    }

    vertex_buffer::usage vertex_buffer::buffer_usage() const noexcept {
        return state_->usage_;
    }

    std::size_t vertex_buffer::buffer_size() const noexcept {
        return state_->size_;
    }

    std::size_t vertex_buffer::vertex_count() const noexcept {
        E2D_ASSERT(state_->size_ % state_->decl_.vertex_size() == 0);
        return state_->size_ / state_->decl_.vertex_size();
    }

    //
    // render
    //

    render::render(debug& debug, window& window)
    : state_(new internal_state(debug, window)) {
        trace_opengl_info(debug);
    }
    render::~render() noexcept = default;

    shader_ptr render::create_shader(
        input_stream_uptr vertex,
        input_stream_uptr fragment,
        const vertex_declaration& decl)
    {
        str vertex_str;
        gl_shader_id vs = streams::try_read_tail(vertex_str, vertex)
            ? compile_shader(state_->debug_, vertex_str, GL_VERTEX_SHADER)
            : gl_shader_id(state_->debug_);
        if ( vs.empty() ) {
            return nullptr;
        }
        str fragment_str;
        gl_shader_id fs = streams::try_read_tail(fragment_str, fragment)
            ? compile_shader(state_->debug_, fragment_str, GL_FRAGMENT_SHADER)
            : gl_shader_id(state_->debug_);
        if ( fs.empty() ) {
            return nullptr;
        }
        gl_program_id ps = link_program(
            state_->debug_, std::move(vs), std::move(fs), decl);
        if ( ps.empty() || !validate_program(state_->debug_, ps) ) {
            return nullptr;
        }
        return std::make_shared<shader>(
            std::make_unique<shader::internal_state>(
                state_->debug_, std::move(ps), decl));
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
        gl_buffer_id id = gl_buffer_id::create_and_bind(
            state_->debug_, GL_ELEMENT_ARRAY_BUFFER);
        if ( id.empty() ) {
            return nullptr;
        }
        E2D_ASSERT(indices.size() % decl.index_size() == 0);
        GL_CHECK_CODE(state_->debug_, glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            math::numeric_cast<GLsizeiptr>(indices.size()),
            indices.data(),
            convert_usage(usage)));
        GL_CHECK_CODE(state_->debug_, glBindBuffer(
            GL_ELEMENT_ARRAY_BUFFER, 0));
        return std::make_shared<index_buffer>(
            std::make_unique<index_buffer::internal_state>(
                state_->debug_, std::move(id), indices.size(), decl, usage));
    }

    vertex_buffer_ptr render::create_vertex_buffer(
        const buffer& vertices,
        const vertex_declaration& decl,
        vertex_buffer::usage usage)
    {
        gl_buffer_id id = gl_buffer_id::create_and_bind(
            state_->debug_, GL_ARRAY_BUFFER);
        if ( id.empty() ) {
            return nullptr;
        }
        E2D_ASSERT(vertices.size() % decl.vertex_size() == 0);
        GL_CHECK_CODE(state_->debug_, glBufferData(
            GL_ARRAY_BUFFER,
            math::numeric_cast<GLsizeiptr>(vertices.size()),
            vertices.data(),
            convert_usage(usage)));
        GL_CHECK_CODE(state_->debug_, glBindBuffer(
            GL_ARRAY_BUFFER, 0));
        return std::make_shared<vertex_buffer>(
            std::make_unique<vertex_buffer::internal_state>(
                state_->debug_, std::move(id), vertices.size(), decl, usage));
    }

    void render::draw(
        topology tp,
        const shader_ptr& ps,
        const index_buffer_ptr& ib,
        const vertex_buffer_ptr& vb) noexcept
    {
        E2D_ASSERT(ps && ib && vb);
        E2D_ASSERT(ps->decl() == vb->decl());

        const index_declaration& id = ib->decl();
        const vertex_declaration& vd = vb->decl();

        GL_CHECK_CODE(state_->debug_, glUseProgram(*ps->state_->id_));
        GL_CHECK_CODE(state_->debug_, glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ib->state_->id_));
        GL_CHECK_CODE(state_->debug_, glBindBuffer(GL_ARRAY_BUFFER, *vb->state_->id_));
        bind_vertex_declaration(state_->debug_, vd);

        GL_CHECK_CODE(state_->debug_, glDrawElements(
            convert_topology(tp),
            math::numeric_cast<GLsizei>(ib->index_count()),
            convert_index_type(id.index().type),
            nullptr));

        unbind_vertex_declaration(state_->debug_, vd);
        GL_CHECK_CODE(state_->debug_, glUseProgram(0));
    }

    render& render::clear_depth_buffer(f32 value) noexcept {
        GL_CHECK_CODE(state_->debug_, glClearDepth(
            math::numeric_cast<GLclampd>(math::saturate(value))));
        GL_CHECK_CODE(state_->debug_, glClear(GL_DEPTH_BUFFER_BIT));
        return *this;
    }

    render& render::clear_stencil_buffer(u8 value) noexcept {
        GL_CHECK_CODE(state_->debug_, glClearStencil(
            math::numeric_cast<GLint>(value)));
        GL_CHECK_CODE(state_->debug_, glClear(GL_STENCIL_BUFFER_BIT));
        return *this;
    }

    render& render::clear_color_buffer(const color& value) noexcept {
        GL_CHECK_CODE(state_->debug_, glClearColor(
            math::numeric_cast<GLclampf>(math::saturate(value.r)),
            math::numeric_cast<GLclampf>(math::saturate(value.g)),
            math::numeric_cast<GLclampf>(math::saturate(value.b)),
            math::numeric_cast<GLclampf>(math::saturate(value.a))));
        GL_CHECK_CODE(state_->debug_, glClear(GL_COLOR_BUFFER_BIT));
        return *this;
    }

    render& render::set_model(const m4f& model) noexcept {
        state_->model_ = model;
        return *this;
    }

    render& render::set_view(const m4f& view) noexcept {
        state_->view_ = view;
        return *this;
    }

    render& render::set_projection(const m4f& projection) noexcept {
        state_->projection_ = projection;
        return *this;
    }

    render& render::set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept {
        GL_CHECK_CODE(state_->debug_, glViewport(
            math::numeric_cast<GLint>(x),
            math::numeric_cast<GLint>(y),
            math::numeric_cast<GLsizei>(w),
            math::numeric_cast<GLsizei>(h)));
        return *this;
    }

    render& render::set_depth_state(const depth_state& ds) noexcept {
        GL_CHECK_CODE(state_->debug_, glDepthRange(
            math::numeric_cast<GLclampd>(math::saturate(ds.near_)),
            math::numeric_cast<GLclampd>(math::saturate(ds.far_))));
        GL_CHECK_CODE(state_->debug_, glDepthMask(
            ds.write_ ? GL_TRUE : GL_FALSE));
        GL_CHECK_CODE(state_->debug_, glDepthFunc(
            convert_compare_func(ds.func_)));
        return *this;
    }

    render& render::set_stencil_state(const stencil_state& ss) noexcept {
        GL_CHECK_CODE(state_->debug_, glStencilMask(
            math::numeric_cast<GLuint>(ss.write_)));
        GL_CHECK_CODE(state_->debug_, glStencilFunc(
            convert_compare_func(ss.func_),
            math::numeric_cast<GLint>(ss.ref_),
            math::numeric_cast<GLuint>(ss.read_)));
        GL_CHECK_CODE(state_->debug_, glStencilOp(
            convert_stencil_op(ss.sfail_),
            convert_stencil_op(ss.zfail_),
            convert_stencil_op(ss.pass_)));
        return *this;
    }

    render& render::set_culling_state(const culling_state& cs) noexcept {
        GL_CHECK_CODE(state_->debug_, glFrontFace(
            convert_culling_mode(cs.mode_)));
        GL_CHECK_CODE(state_->debug_, glCullFace(
            convert_culling_face(cs.face_)));
        return *this;
    }

    render& render::set_blending_state(const blending_state& bs) noexcept {
        GL_CHECK_CODE(state_->debug_, glBlendColor(
            math::numeric_cast<GLclampf>(math::saturate(bs.constant_color_.r)),
            math::numeric_cast<GLclampf>(math::saturate(bs.constant_color_.g)),
            math::numeric_cast<GLclampf>(math::saturate(bs.constant_color_.b)),
            math::numeric_cast<GLclampf>(math::saturate(bs.constant_color_.a))));
        GL_CHECK_CODE(state_->debug_, glBlendFuncSeparate(
            convert_blending_factor(bs.src_rgb_factor_),
            convert_blending_factor(bs.dst_rgb_factor_),
            convert_blending_factor(bs.src_alpha_factor_),
            convert_blending_factor(bs.dst_alpha_factor_)));
        GL_CHECK_CODE(state_->debug_, glBlendEquationSeparate(
            convert_blending_equation(bs.rgb_equation_),
            convert_blending_equation(bs.alpha_equation_)));
        GL_CHECK_CODE(state_->debug_, glColorMask(
            (math::enum_to_number(bs.color_mask_) & math::enum_to_number(blending_color_mask::r)) != 0,
            (math::enum_to_number(bs.color_mask_) & math::enum_to_number(blending_color_mask::g)) != 0,
            (math::enum_to_number(bs.color_mask_) & math::enum_to_number(blending_color_mask::b)) != 0,
            (math::enum_to_number(bs.color_mask_) & math::enum_to_number(blending_color_mask::a)) != 0));
        return *this;
    }

    render& render::set_capabilities_state(const capabilities_state& cs) noexcept {
        GL_CHECK_CODE(state_->debug_, gl_enable_or_disable(
            GL_CULL_FACE, cs.culling_));
        GL_CHECK_CODE(state_->debug_, gl_enable_or_disable(
            GL_BLEND, cs.blending_));
        GL_CHECK_CODE(state_->debug_, gl_enable_or_disable(
            GL_DEPTH_TEST, cs.depth_test_));
        GL_CHECK_CODE(state_->debug_, gl_enable_or_disable(
            GL_STENCIL_TEST, cs.stencil_test_));
        return *this;
    }
}

#endif
