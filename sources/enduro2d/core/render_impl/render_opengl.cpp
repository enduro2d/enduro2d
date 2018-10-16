/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "render.hpp"
#include "render_opengl_base.hpp"
#include "render_opengl_impl.hpp"

#if defined(E2D_RENDER_MODE) && E2D_RENDER_MODE == E2D_RENDER_MODE_OPENGL

namespace
{
    using namespace e2d;
    using namespace e2d::opengl;

    class property_block_value_visitor : private noncopyable {
    public:
        property_block_value_visitor(debug& debug, uniform_info ui)
        : debug_(debug)
        , ui_(std::move(ui)) {}

        void operator()(i32 v) const noexcept {
            if ( check_property_type(uniform_type::signed_integer) ) {
                GL_CHECK_CODE(debug_, glUniform1i(ui_.location, v));
            }
        }

        void operator()(f32 v) const noexcept {
            if ( check_property_type(uniform_type::floating_point) ) {
                GL_CHECK_CODE(debug_, glUniform1f(ui_.location, v));
            }
        }

        void operator()(const v2i& v) const noexcept {
            if ( check_property_type(uniform_type::v2i) ) {
                GL_CHECK_CODE(debug_, glUniform2iv(ui_.location, 1, v.data()));
            }
        }

        void operator()(const v3i& v) const noexcept {
            if ( check_property_type(uniform_type::v3i) ) {
                GL_CHECK_CODE(debug_, glUniform3iv(ui_.location, 1, v.data()));
            }
        }

        void operator()(const v4i& v) const noexcept {
            if ( check_property_type(uniform_type::v4i) ) {
                GL_CHECK_CODE(debug_, glUniform4iv(ui_.location, 1, v.data()));
            }
        }

        void operator()(const v2f& v) const noexcept {
            if ( check_property_type(uniform_type::v2f) ) {
                GL_CHECK_CODE(debug_, glUniform2fv(ui_.location, 1, v.data()));
            }
        }

        void operator()(const v3f& v) const noexcept {
            if ( check_property_type(uniform_type::v3f) ) {
                GL_CHECK_CODE(debug_, glUniform3fv(ui_.location, 1, v.data()));
            }
        }

        void operator()(const v4f& v) const noexcept {
            if ( check_property_type(uniform_type::v4f) ) {
                GL_CHECK_CODE(debug_, glUniform4fv(ui_.location, 1, v.data()));
            }
        }

        void operator()(const m2f& v) const noexcept {
            if ( check_property_type(uniform_type::m2f) ) {
                GL_CHECK_CODE(debug_, glUniformMatrix2fv(ui_.location, 1, GL_TRUE, v.data()));
            }
        }

        void operator()(const m3f& v) const noexcept {
            if ( check_property_type(uniform_type::m3f) ) {
                GL_CHECK_CODE(debug_, glUniformMatrix3fv(ui_.location, 1, GL_TRUE, v.data()));
            }
        }

        void operator()(const m4f& v) const noexcept {
            if ( check_property_type(uniform_type::m4f) ) {
                GL_CHECK_CODE(debug_, glUniformMatrix4fv(ui_.location, 1, GL_TRUE, v.data()));
            }
        }
    private:
        bool check_property_type(uniform_type type) const noexcept {
            if ( type == ui_.type ) {
                return true;
            }
            E2D_ASSERT_MSG(false, "unexpected property type");
            debug_.error("RENDER: unexpected property type:\n"
                "--> Type: %0\n"
                "--> Expected: %1",
                uniform_type_to_cstr(type),
                uniform_type_to_cstr(ui_.type));
            return false;
        }
    private:
        debug& debug_;
        uniform_info ui_;
    };

    void bind_property_block(
        debug& debug,
        const shader_ptr& ps,
        const render::property_block& pb) noexcept
    {
        E2D_ASSERT(ps && gl_program_id::current(debug) == ps->state().id());
        pb.foreach_by_properties([&debug, &ps](str_hash name, const render::property_value& value) noexcept {
            ps->state().with_uniform_location(name, [&debug, &value](const uniform_info& ui) noexcept {
                E2D_ASSERT(!value.valueless_by_exception());
                stdex::visit(property_block_value_visitor(debug, ui), value);
            });
        });
        GLint unit = 0;
        pb.foreach_by_samplers([&debug, &ps, &unit](str_hash name, const render::sampler_state& sampler) noexcept {
            ps->state().with_uniform_location(name, [&debug, &sampler, &unit](const uniform_info& ui) noexcept {
                GL_CHECK_CODE(debug, glActiveTexture(
                    math::numeric_cast<GLenum>(GL_TEXTURE0 + unit)));
                GL_CHECK_CODE(debug, glUniform1i(
                    ui.location, math::numeric_cast<GLint>(unit)));
                if ( sampler.texture() ) {
                    const gl_texture_id& texture_id = sampler.texture()->state().id();
                    GL_CHECK_CODE(debug, glBindTexture(
                        texture_id.target(), *texture_id));
                    GL_CHECK_CODE(debug, glTexParameteri(
                        texture_id.target(),
                        GL_TEXTURE_WRAP_S,
                        convert_sampler_wrap(sampler.s_wrap())));
                    GL_CHECK_CODE(debug, glTexParameteri(
                        texture_id.target(),
                        GL_TEXTURE_WRAP_T,
                        convert_sampler_wrap(sampler.t_wrap())));
                    GL_CHECK_CODE(debug, glTexParameteri(
                        texture_id.target(),
                        GL_TEXTURE_MIN_FILTER,
                        convert_sampler_filter(sampler.min_filter())));
                    GL_CHECK_CODE(debug, glTexParameteri(
                        texture_id.target(),
                        GL_TEXTURE_MAG_FILTER,
                        convert_sampler_filter(sampler.mag_filter())));
                } else {
                    GL_CHECK_CODE(debug, glBindTexture(GL_TEXTURE_2D, 0));
                    GL_CHECK_CODE(debug, glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
                }
                ++unit;
            });
        });
    }

    void unbind_property_block(
        debug& debug,
        const shader_ptr& ps,
        const render::property_block& pb) noexcept
    {
        E2D_ASSERT(ps && gl_program_id::current(debug) == ps->state().id());
        GLint unit = 0;
        pb.foreach_by_samplers([&debug, &ps, &unit](str_hash name, const render::sampler_state& sampler) noexcept {
            E2D_UNUSED(sampler);
            ps->state().with_uniform_location(name, [&debug, &unit](const uniform_info& ui) noexcept {
                E2D_UNUSED(ui);
                GL_CHECK_CODE(debug, glActiveTexture(
                    math::numeric_cast<GLenum>(GL_TEXTURE0 + unit)));
                GL_CHECK_CODE(debug, glBindTexture(GL_TEXTURE_2D, 0));
                GL_CHECK_CODE(debug, glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
                ++unit;
            });
        });
    }

    void bind_vertex_declaration(
        debug& debug,
        const shader_ptr& ps,
        const vertex_buffer_ptr& vb) noexcept
    {
        E2D_ASSERT(ps && vb);
        opengl::with_gl_bind_buffer(debug, vb->state().id(), [&debug, &ps, &vb]() noexcept {
            const vertex_declaration& decl = vb->decl();
            for ( std::size_t i = 0; i < decl.attribute_count(); ++i ) {
                const vertex_declaration::attribute_info& vai = decl.attribute(i);
                ps->state().with_attribute_location(vai.name, [&debug, &decl, &vai](const attribute_info& ai) noexcept {
                    const GLuint rows = math::numeric_cast<GLuint>(vai.rows);
                    for ( GLuint row = 0; row < rows; ++row ) {
                        GL_CHECK_CODE(debug, glEnableVertexAttribArray(
                            math::numeric_cast<GLuint>(ai.location) + row));
                        GL_CHECK_CODE(debug, glVertexAttribPointer(
                            math::numeric_cast<GLuint>(ai.location) + row,
                            math::numeric_cast<GLint>(vai.columns),
                            convert_attribute_type(vai.type),
                            vai.normalized ? GL_TRUE : GL_FALSE,
                            math::numeric_cast<GLsizei>(decl.vertex_size()),
                            reinterpret_cast<const GLvoid*>(vai.stride + row * vai.row_size())));
                    }
                });
            }
        });
    }

    void unbind_vertex_declaration(
        debug& debug,
        const shader_ptr& ps,
        const vertex_buffer_ptr& vb) noexcept
    {
        E2D_ASSERT(ps && vb);
        const vertex_declaration& decl = vb->decl();
        for ( std::size_t i = 0; i < decl.attribute_count(); ++i ) {
            const vertex_declaration::attribute_info& vai = decl.attribute(i);
            ps->state().with_attribute_location(vai.name, [&debug, &vai](const attribute_info& ai) noexcept {
                const GLuint rows = math::numeric_cast<GLuint>(vai.rows);
                for ( GLuint row = 0; row < rows; ++row ) {
                    GL_CHECK_CODE(debug, glDisableVertexAttribArray(
                        math::numeric_cast<GLuint>(ai.location) + row));
                }
            });
        }
    }

    void draw_indexed_primitive(debug& debug, render::topology tp, const index_buffer_ptr& ib) noexcept {
        with_gl_bind_buffer(debug, ib->state().id(), [&debug, &tp, &ib]() noexcept {
            const index_declaration& decl = ib->decl();
            GL_CHECK_CODE(debug, glDrawElements(
                convert_topology(tp),
                math::numeric_cast<GLsizei>(ib->index_count()),
                convert_index_type(decl.index().type),
                nullptr));
        });
    }

    template < typename F, typename... Args >
    void with_material_shader(
        debug& debug,
        const shader_ptr& ps,
        const render::property_block& pb,
        F&& f,
        Args&&... args)
    {
        with_gl_use_program(debug, ps->state().id(),
            [&debug, &ps, &pb](auto&& nf, auto&&... nargs) noexcept {
                bind_property_block(debug, ps, pb);
                try {
                    stdex::invoke(
                        std::forward<decltype(nf)>(nf),
                        std::forward<decltype(nargs)>(nargs)...);
                } catch (...) {
                    unbind_property_block(debug, ps, pb);
                    throw;
                }
                unbind_property_block(debug, ps, pb);
            }, std::forward<F>(f), std::forward<Args>(args)...);
    }

    template < typename F, typename... Args >
    void with_geometry_vertices(
        debug& debug,
        const shader_ptr& ps,
        const render::geometry& geo,
        F&& f,
        Args&&... args)
    {
        for ( std::size_t i = 0; i < geo.vertices_count(); ++i ) {
            const vertex_buffer_ptr& vb = geo.vertices(i);
            if ( vb ) {
                bind_vertex_declaration(debug, ps, vb);
            }
        }
        try {
            stdex::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        } catch (...) {
            for ( std::size_t i = 0; i < geo.vertices_count(); ++i ) {
                const vertex_buffer_ptr& vb = geo.vertices(i);
                if ( vb ) {
                    unbind_vertex_declaration(debug, ps, vb);
                }
            }
            throw;
        }
        for ( std::size_t i = 0; i < geo.vertices_count(); ++i ) {
            const vertex_buffer_ptr& vb = geo.vertices(i);
            if ( vb ) {
                unbind_vertex_declaration(debug, ps, vb);
            }
        }
    }
}

namespace e2d
{
    //
    // shader
    //

    const shader::internal_state& shader::state() const noexcept {
        return *state_;
    }

    shader::shader(internal_state_uptr state)
    : state_(std::move(state)) {
        E2D_ASSERT(state_);
    }
    shader::~shader() noexcept = default;

    //
    // texture
    //

    const texture::internal_state& texture::state() const noexcept {
        return *state_;
    }

    texture::texture(internal_state_uptr state)
    : state_(std::move(state)) {
        E2D_ASSERT(state_);
    }
    texture::~texture() noexcept = default;

    const v2u& texture::size() const noexcept {
        return state_->size();
    }

    image_data_format texture::format() const noexcept {
        return state_->format();
    }

    //
    // index_buffer
    //

    const index_buffer::internal_state& index_buffer::state() const noexcept {
        return *state_;
    }

    index_buffer::index_buffer(internal_state_uptr state)
    : state_(std::move(state)) {
        E2D_ASSERT(state_);
    }
    index_buffer::~index_buffer() noexcept = default;

    void index_buffer::update(const buffer& indices, std::size_t offset) noexcept {
        const std::size_t buffer_offset = offset * state_->decl().index_size();
        E2D_ASSERT(indices.size() + buffer_offset <= state_->size());
        E2D_ASSERT(indices.size() % state_->decl().index_size() == 0);
        opengl::with_gl_bind_buffer(state_->dbg(), state_->id(),
            [this, &indices, &buffer_offset]() noexcept {
                GL_CHECK_CODE(state_->dbg(), glBufferSubData(
                    state_->id().target(),
                    math::numeric_cast<GLintptr>(buffer_offset),
                    math::numeric_cast<GLsizeiptr>(indices.size()),
                    indices.data()));
            });
    }

    const index_declaration& index_buffer::decl() const noexcept {
        return state_->decl();
    }

    std::size_t index_buffer::buffer_size() const noexcept {
        return state_->size();
    }

    std::size_t index_buffer::index_count() const noexcept {
        E2D_ASSERT(state_->size() % state_->decl().index_size() == 0);
        return state_->size() / state_->decl().index_size();
    }

    //
    // vertex_buffer
    //

    const vertex_buffer::internal_state& vertex_buffer::state() const noexcept {
        return *state_;
    }

    vertex_buffer::vertex_buffer(internal_state_uptr state)
    : state_(std::move(state)) {
        E2D_ASSERT(state_);
    }
    vertex_buffer::~vertex_buffer() noexcept = default;

    void vertex_buffer::update(const buffer& vertices, std::size_t offset) noexcept {
        const std::size_t buffer_offset = offset * state_->decl().vertex_size();
        E2D_ASSERT(vertices.size() + buffer_offset <= state_->size());
        E2D_ASSERT(vertices.size() % state_->decl().vertex_size() == 0);
        opengl::with_gl_bind_buffer(state_->dbg(), state_->id(),
            [this, &vertices, &buffer_offset]() noexcept {
                GL_CHECK_CODE(state_->dbg(), glBufferSubData(
                    state_->id().target(),
                    math::numeric_cast<GLintptr>(buffer_offset),
                    math::numeric_cast<GLsizeiptr>(vertices.size()),
                    vertices.data()));
            });
    }

    const vertex_declaration& vertex_buffer::decl() const noexcept {
        return state_->decl();
    }

    std::size_t vertex_buffer::buffer_size() const noexcept {
        return state_->size();
    }

    std::size_t vertex_buffer::vertex_count() const noexcept {
        E2D_ASSERT(state_->size() % state_->decl().vertex_size() == 0);
        return state_->size() / state_->decl().vertex_size();
    }

    //
    // render
    //

    render::render(debug& debug, window& window)
    : state_(new internal_state(debug, window)) {
        opengl::gl_trace_info(debug);
        opengl::gl_trace_limits(debug);
        GL_CHECK_CODE(state_->dbg(), glPixelStorei(GL_PACK_ALIGNMENT, 1));
        GL_CHECK_CODE(state_->dbg(), glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    }
    render::~render() noexcept = default;

    shader_ptr render::create_shader(
        input_stream_uptr vertex,
        input_stream_uptr fragment)
    {
        str vertex_str;
        gl_shader_id vs = streams::try_read_tail(vertex_str, vertex)
            ? gl_compile_shader(state_->dbg(), vertex_str, GL_VERTEX_SHADER)
            : gl_shader_id(state_->dbg());
        if ( vs.empty() ) {
            return nullptr;
        }
        str fragment_str;
        gl_shader_id fs = streams::try_read_tail(fragment_str, fragment)
            ? gl_compile_shader(state_->dbg(), fragment_str, GL_FRAGMENT_SHADER)
            : gl_shader_id(state_->dbg());
        if ( fs.empty() ) {
            return nullptr;
        }
        gl_program_id ps = gl_link_program(
            state_->dbg(), std::move(vs), std::move(fs));
        if ( ps.empty() ) {
            return nullptr;
        }
        return std::make_shared<shader>(
            std::make_unique<shader::internal_state>(
                state_->dbg(), std::move(ps)));
    }

    texture_ptr render::create_texture(const image& image) {
        gl_texture_id id = gl_compile_texture(state_->dbg(), image);
        if ( id.empty() ) {
            return nullptr;
        }
        return std::make_shared<texture>(
            std::make_unique<texture::internal_state>(
                state_->dbg(), std::move(id), image.size(), image.format()));
    }

    texture_ptr render::create_texture(const input_stream_uptr& image_stream) {
        image image;
        if ( !images::try_load_image(image, image_stream) ) {
            return nullptr;
        }
        return create_texture(image);
    }

    index_buffer_ptr render::create_index_buffer(
        const buffer& indices,
        const index_declaration& decl,
        index_buffer::usage usage)
    {
        E2D_ASSERT(indices.size() % decl.index_size() == 0);
        gl_buffer_id id = gl_compile_index_buffer(state_->dbg(), indices, usage);
        if ( id.empty() ) {
            return nullptr;
        }
        return std::make_shared<index_buffer>(
            std::make_unique<index_buffer::internal_state>(
                state_->dbg(), std::move(id), indices.size(), decl));
    }

    vertex_buffer_ptr render::create_vertex_buffer(
        const buffer& vertices,
        const vertex_declaration& decl,
        vertex_buffer::usage usage)
    {
        E2D_ASSERT(vertices.size() % decl.vertex_size() == 0);
        gl_buffer_id id = gl_compile_vertex_buffer(state_->dbg(), vertices, usage);
        if ( id.empty() ) {
            return nullptr;
        }
        return std::make_shared<vertex_buffer>(
            std::make_unique<vertex_buffer::internal_state>(
                state_->dbg(), std::move(id), vertices.size(), decl));
    }

    void render::draw(
        const material& mat,
        const geometry& geo)
    {
        draw(mat, geo, property_block());
    }

    void render::draw(
        const material& mat,
        const geometry& geo,
        const property_block& props)
    {
        for ( std::size_t i = 0; i < mat.pass_count(); ++i ) {
            const pass_state& pass = mat.pass(i);
            const property_block main_props = property_block()
                .merge(mat.properties())
                .merge(pass.properties())
                .merge(props);
            state_->set_states(pass.states());
            with_material_shader(state_->dbg(), pass.shader(), main_props, [this, &pass, &geo]() noexcept {
                with_geometry_vertices(state_->dbg(), pass.shader(), geo, [this, &geo]() noexcept {
                    draw_indexed_primitive(state_->dbg(), geo.topo(), geo.indices());
                });
            });
        }
    }

    render& render::clear_depth_buffer(f32 value) noexcept {
        GL_CHECK_CODE(state_->dbg(), glClearDepth(
            math::numeric_cast<GLclampd>(math::saturate(value))));
        GL_CHECK_CODE(state_->dbg(), glClear(GL_DEPTH_BUFFER_BIT));
        return *this;
    }

    render& render::clear_stencil_buffer(u8 value) noexcept {
        GL_CHECK_CODE(state_->dbg(), glClearStencil(
            math::numeric_cast<GLint>(value)));
        GL_CHECK_CODE(state_->dbg(), glClear(GL_STENCIL_BUFFER_BIT));
        return *this;
    }

    render& render::clear_color_buffer(const color& value) noexcept {
        GL_CHECK_CODE(state_->dbg(), glClearColor(
            math::numeric_cast<GLclampf>(math::saturate(value.r)),
            math::numeric_cast<GLclampf>(math::saturate(value.g)),
            math::numeric_cast<GLclampf>(math::saturate(value.b)),
            math::numeric_cast<GLclampf>(math::saturate(value.a))));
        GL_CHECK_CODE(state_->dbg(), glClear(GL_COLOR_BUFFER_BIT));
        return *this;
    }

    render& render::set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept {
        GL_CHECK_CODE(state_->dbg(), glViewport(
            math::numeric_cast<GLint>(x),
            math::numeric_cast<GLint>(y),
            math::numeric_cast<GLsizei>(w),
            math::numeric_cast<GLsizei>(h)));
        return *this;
    }
}

#endif
