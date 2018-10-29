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
        property_block_value_visitor(debug& debug, uniform_info ui) noexcept
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
            for ( std::size_t i = 0, e = decl.attribute_count(); i < e; ++i ) {
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
                            math::numeric_cast<GLsizei>(decl.bytes_per_vertex()),
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
        for ( std::size_t i = 0, e = decl.attribute_count(); i < e; ++i ) {
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
                convert_index_type(decl.type()),
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
        bind_property_block(debug, ps, pb);
        try {
            stdex::invoke(
                std::forward<F>(f),
                std::forward<Args>(args)...);
        } catch (...) {
            unbind_property_block(debug, ps, pb);
            throw;
        }
        unbind_property_block(debug, ps, pb);
    }

    template < typename F, typename... Args >
    void with_geometry_vertices(
        debug& debug,
        const shader_ptr& ps,
        const render::geometry& geo,
        F&& f,
        Args&&... args)
    {
        for ( std::size_t i = 0, e = geo.vertices_count(); i < e; ++i ) {
            const vertex_buffer_ptr& vb = geo.vertices(i);
            if ( vb ) {
                bind_vertex_declaration(debug, ps, vb);
            }
        }
        try {
            stdex::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        } catch (...) {
            for ( std::size_t i = 0, e = geo.vertices_count(); i < e; ++i ) {
                const vertex_buffer_ptr& vb = geo.vertices(i);
                if ( vb ) {
                    unbind_vertex_declaration(debug, ps, vb);
                }
            }
            throw;
        }
        for ( std::size_t i = 0, e = geo.vertices_count(); i < e; ++i ) {
            const vertex_buffer_ptr& vb = geo.vertices(i);
            if ( vb ) {
                unbind_vertex_declaration(debug, ps, vb);
            }
        }
    }

    render::property_block& main_property_cache() {
        static render::property_block props;
        return props;
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

    const pixel_declaration& texture::decl() const noexcept {
        return state_->decl();
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
        const std::size_t buffer_offset = offset * state_->decl().bytes_per_index();
        E2D_ASSERT(indices.size() + buffer_offset <= state_->size());
        E2D_ASSERT(indices.size() % state_->decl().bytes_per_index() == 0);
        opengl::with_gl_bind_buffer(state_->dbg(), state_->id(),
            [this, &indices, &buffer_offset]() noexcept {
                GL_CHECK_CODE(state_->dbg(), glBufferSubData(
                    state_->id().target(),
                    math::numeric_cast<GLintptr>(buffer_offset),
                    math::numeric_cast<GLsizeiptr>(indices.size()),
                    indices.data()));
            });
    }

    std::size_t index_buffer::buffer_size() const noexcept {
        return state_->size();
    }

    std::size_t index_buffer::index_count() const noexcept {
        E2D_ASSERT(state_->size() % state_->decl().bytes_per_index() == 0);
        return state_->size() / state_->decl().bytes_per_index();
    }

    const index_declaration& index_buffer::decl() const noexcept {
        return state_->decl();
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
        const std::size_t buffer_offset = offset * state_->decl().bytes_per_vertex();
        E2D_ASSERT(vertices.size() + buffer_offset <= state_->size());
        E2D_ASSERT(vertices.size() % state_->decl().bytes_per_vertex() == 0);
        opengl::with_gl_bind_buffer(state_->dbg(), state_->id(),
            [this, &vertices, &buffer_offset]() noexcept {
                GL_CHECK_CODE(state_->dbg(), glBufferSubData(
                    state_->id().target(),
                    math::numeric_cast<GLintptr>(buffer_offset),
                    math::numeric_cast<GLsizeiptr>(vertices.size()),
                    vertices.data()));
            });
    }

    std::size_t vertex_buffer::buffer_size() const noexcept {
        return state_->size();
    }

    std::size_t vertex_buffer::vertex_count() const noexcept {
        E2D_ASSERT(state_->size() % state_->decl().bytes_per_vertex() == 0);
        return state_->size() / state_->decl().bytes_per_vertex();
    }

    const vertex_declaration& vertex_buffer::decl() const noexcept {
        return state_->decl();
    }

    //
    // render_target
    //

    const render_target::internal_state& render_target::state() const noexcept {
        return *state_;
    }

    render_target::render_target(internal_state_uptr state)
    : state_(std::move(state)) {
        E2D_ASSERT(state_);
    }
    render_target::~render_target() noexcept = default;

    const v2u& render_target::size() const noexcept {
        return state_->size();
    }

    const texture_ptr& render_target::color() const noexcept {
        return state_->color();
    }

    const texture_ptr& render_target::depth() const noexcept {
        return state_->depth();
    }

    //
    // render
    //

    render::render(debug& ndebug, window& nwindow)
    : state_(new internal_state(ndebug, nwindow)) {
        E2D_ASSERT(main_thread() == nwindow.main_thread());
    }
    render::~render() noexcept = default;

    shader_ptr render::create_shader(
        const str& vertex_source,
        const str& fragment_source)
    {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());

        gl_shader_id vs = gl_compile_shader(
            state_->dbg(), vertex_source, GL_VERTEX_SHADER);
        if ( vs.empty() ) {
            return nullptr;
        }

        gl_shader_id fs = gl_compile_shader(
            state_->dbg(), fragment_source, GL_FRAGMENT_SHADER);
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

    shader_ptr render::create_shader(
        const input_stream_uptr& vertex,
        const input_stream_uptr& fragment)
    {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());

        str vertex_source, fragment_source;
        return streams::try_read_tail(vertex_source, vertex)
            && streams::try_read_tail(fragment_source, fragment)
            ? create_shader(vertex_source, fragment_source)
            : nullptr;
    }

    texture_ptr render::create_texture(
        const image& image)
    {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());

        gl_texture_id id = gl_texture_id::create(
            state_->dbg(), GL_TEXTURE_2D);
        if ( id.empty() ) {
            state_->dbg().error("RENDER: Failed to create texture: %0",
                "failed to create texture id");
            return nullptr;
        }

        const pixel_declaration decl =
            convert_image_data_format_to_pixel_declaration(image.format());
        with_gl_bind_texture(state_->dbg(), id, [this, &id, &image, &decl]() noexcept {
            if ( decl.is_compressed() ) {
                GL_CHECK_CODE(state_->dbg(), glCompressedTexImage2D(
                    id.target(),
                    0,
                    convert_pixel_type_to_internal_format_e(decl.type()),
                    math::numeric_cast<GLsizei>(image.size().x),
                    math::numeric_cast<GLsizei>(image.size().y),
                    0,
                    math::numeric_cast<GLsizei>(image.data().size()),
                    image.data().data()));
            } else {
                GL_CHECK_CODE(state_->dbg(), glTexImage2D(
                    id.target(),
                    0,
                    convert_pixel_type_to_internal_format(decl.type()),
                    math::numeric_cast<GLsizei>(image.size().x),
                    math::numeric_cast<GLsizei>(image.size().y),
                    0,
                    convert_image_data_format_to_external_format(image.format()),
                    convert_image_data_format_to_external_data_type(image.format()),
                    image.data().data()));
            }
        #if !defined(GL_ES_VERSION_2_0)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        #endif
        });

        return std::make_shared<texture>(
            std::make_unique<texture::internal_state>(
                state_->dbg(), std::move(id), image.size(), decl));
    }

    texture_ptr render::create_texture(
        const input_stream_uptr& image_stream)
    {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());

        image image;
        if ( !images::try_load_image(image, image_stream) ) {
            return nullptr;
        }
        return create_texture(image);
    }

    texture_ptr render::create_texture(
        const v2u& size,
        const pixel_declaration& decl)
    {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());

        gl_texture_id id = gl_texture_id::create(
            state_->dbg(), GL_TEXTURE_2D);
        if ( id.empty() ) {
            state_->dbg().error("RENDER: Failed to create texture: %0",
                "failed to create texture id");
            return nullptr;
        }

        with_gl_bind_texture(state_->dbg(), id, [this, &id, &size, &decl]() noexcept {
            if ( decl.is_compressed() ) {
                buffer empty_data(decl.bits_per_pixel() * size.x * size.y / 8);
                GL_CHECK_CODE(state_->dbg(), glCompressedTexImage2D(
                    id.target(),
                    0,
                    convert_pixel_type_to_internal_format_e(decl.type()),
                    math::numeric_cast<GLsizei>(size.x),
                    math::numeric_cast<GLsizei>(size.y),
                    0,
                    math::numeric_cast<GLsizei>(empty_data.size()),
                    empty_data.data()));
            } else {
                GL_CHECK_CODE(state_->dbg(), glTexImage2D(
                    id.target(),
                    0,
                    convert_pixel_type_to_internal_format(decl.type()),
                    math::numeric_cast<GLsizei>(size.x),
                    math::numeric_cast<GLsizei>(size.y),
                    0,
                    convert_pixel_type_to_external_format(decl.type()),
                    convert_pixel_type_to_external_data_type(decl.type()),
                    nullptr));
            }
        #if !defined(GL_ES_VERSION_2_0)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
        #endif
        });

        return std::make_shared<texture>(
            std::make_unique<texture::internal_state>(
                state_->dbg(), std::move(id), size, decl));
    }

    index_buffer_ptr render::create_index_buffer(
        const buffer& indices,
        const index_declaration& decl,
        index_buffer::usage usage)
    {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());
        E2D_ASSERT(indices.size() % decl.bytes_per_index() == 0);

        gl_buffer_id id = gl_compile_index_buffer(
            state_->dbg(), indices, usage);
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
        E2D_ASSERT(main_thread() == std::this_thread::get_id());
        E2D_ASSERT(vertices.size() % decl.bytes_per_vertex() == 0);

        gl_buffer_id id = gl_compile_vertex_buffer(
            state_->dbg(), vertices, usage);
        if ( id.empty() ) {
            return nullptr;
        }

        return std::make_shared<vertex_buffer>(
            std::make_unique<vertex_buffer::internal_state>(
                state_->dbg(), std::move(id), vertices.size(), decl));
    }

    render_target_ptr render::create_render_target(
        const v2u& size,
        const pixel_declaration& color_decl,
        const pixel_declaration& depth_decl,
        render_target::external_texture external_texture)
    {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());

        E2D_ASSERT(
            depth_decl.is_depth() &&
            color_decl.is_color() &&
            !color_decl.is_compressed());

        gl_framebuffer_id id = gl_framebuffer_id::create(
            state_->dbg(), GL_FRAMEBUFFER);
        if ( id.empty() ) {
            state_->dbg().error("RENDER: Failed to create framebuffer: %0",
                "failed to create framebuffer id");
            return nullptr;
        }

        bool need_color =
            math::enum_to_number(external_texture)
            & math::enum_to_number(render_target::external_texture::color);

        bool need_depth =
            math::enum_to_number(external_texture)
            & math::enum_to_number(render_target::external_texture::depth);

        texture_ptr color;
        texture_ptr depth;

        gl_renderbuffer_id color_rb(state_->dbg());
        gl_renderbuffer_id depth_rb(state_->dbg());

        if ( need_color ) {
            color = create_texture(size, color_decl);
            if ( !color ) {
                state_->dbg().error("RENDER: Failed to create framebuffer: %0",
                    "failed to create color texture");
                return nullptr;
            }
            gl_attach_texture(state_->dbg(), id, color->state().id(), GL_COLOR_ATTACHMENT0);
        } else {
            color_rb = gl_compile_renderbuffer(
                state_->dbg(),
                size,
                convert_pixel_type_to_internal_format_e(color_decl.type()));
            if ( color_rb.empty() ) {
                state_->dbg().error("RENDER: Failed to create framebuffer: %0",
                    "failed to create color renderbuffer");
                return nullptr;
            }
            gl_attach_renderbuffer(state_->dbg(), id, color_rb, GL_COLOR_ATTACHMENT0);
        }

        if ( need_depth ) {
            depth = create_texture(size, depth_decl);
            if ( !depth ) {
                state_->dbg().error("RENDER: Failed to create framebuffer: %0",
                    "failed to create depth texture");
                return nullptr;
            }
            gl_attach_texture(state_->dbg(), id, depth->state().id(), GL_DEPTH_ATTACHMENT);
            if ( depth_decl.is_stencil() ) {
                gl_attach_texture(state_->dbg(), id, depth->state().id(), GL_STENCIL_ATTACHMENT);
            }
        } else {
            depth_rb = gl_compile_renderbuffer(
                state_->dbg(),
                size,
                convert_pixel_type_to_internal_format_e(depth_decl.type()));
            if ( depth_rb.empty() ) {
                state_->dbg().error("RENDER: Failed to create framebuffer: %0",
                    "failed to create depth renderbuffer");
                return nullptr;
            }
            gl_attach_renderbuffer(state_->dbg(), id, depth_rb, GL_DEPTH_ATTACHMENT);
            if ( depth_decl.is_stencil() ) {
                gl_attach_renderbuffer(state_->dbg(), id, depth_rb, GL_STENCIL_ATTACHMENT);
            }
        }

        GLenum fb_status = GL_FRAMEBUFFER_COMPLETE;
        if ( !gl_check_framebuffer(state_->dbg(), id, &fb_status) ) {
            state_->dbg().error("RENDER: Failed to create framebuffer: %0",
                gl_framebuffer_status_to_cstr(fb_status));
            return nullptr;
        }

        return std::make_shared<render_target>(
            std::make_unique<render_target::internal_state>(
                state_->dbg(),
                std::move(id),
                size,
                std::move(color),
                std::move(depth),
                std::move(color_rb),
                std::move(depth_rb)));
    }

    render& render::execute(const swap_command& command) {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());
        state_->wnd().swap_buffers(command.vsync());
        return *this;
    }

    render& render::execute(const draw_command& command) {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());

        const material& mat = command.material_ref();
        const geometry& geo = command.geometry_ref();
        const property_block& props = command.properties_ref();

        for ( std::size_t i = 0, e = mat.pass_count(); i < e; ++i ) {
            const pass_state& pass = mat.pass(i);
            const property_block& main_props = main_property_cache()
                .merge(mat.properties())
                .merge(pass.properties())
                .merge(props);
            state_->set_states(pass.states());
            state_->set_shader_program(pass.shader());
            with_material_shader(state_->dbg(), pass.shader(), main_props, [this, &pass, &geo]() noexcept {
                with_geometry_vertices(state_->dbg(), pass.shader(), geo, [this, &geo]() noexcept {
                    draw_indexed_primitive(state_->dbg(), geo.topo(), geo.indices());
                });
            });
            main_property_cache().clear();
        }
        return *this;
    }

    render& render::execute(const clear_command& command) {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());

        bool clear_color =
            math::enum_to_number(command.clear_buffer())
            & math::enum_to_number(clear_command::buffer::color);
        bool clear_depth =
            math::enum_to_number(command.clear_buffer())
            & math::enum_to_number(clear_command::buffer::depth);
        bool clear_stencil =
            math::enum_to_number(command.clear_buffer())
            & math::enum_to_number(clear_command::buffer::stencil);

        const render_target_ptr& rt = state_->render_target();
        bool has_color = !rt || rt->state().color() || !rt->state().color_rb().empty();
        bool has_depth = !rt || rt->state().depth() || !rt->state().depth_rb().empty();

        GLbitfield clear_mask = 0;
        if ( has_color && clear_color  ) {
            clear_mask |= GL_COLOR_BUFFER_BIT;
            GL_CHECK_CODE(state_->dbg(), glClearColor(
                math::numeric_cast<GLclampf>(math::saturate(command.color_value().r)),
                math::numeric_cast<GLclampf>(math::saturate(command.color_value().g)),
                math::numeric_cast<GLclampf>(math::saturate(command.color_value().b)),
                math::numeric_cast<GLclampf>(math::saturate(command.color_value().a))));
        }
        if ( has_depth ) {
            if ( clear_depth ) {
                clear_mask |= GL_DEPTH_BUFFER_BIT;
                GL_CHECK_CODE(state_->dbg(), glClearDepth(
                    math::numeric_cast<GLclampd>(math::saturate(command.depth_value()))));
            }
            if ( clear_stencil ) {
                clear_mask |= GL_STENCIL_BUFFER_BIT;
                GL_CHECK_CODE(state_->dbg(), glClearStencil(
                    math::numeric_cast<GLint>(command.stencil_value())));
            }
        }
        GL_CHECK_CODE(state_->dbg(), glClear(clear_mask));
        return *this;
    }

    render& render::execute(const target_command& command) {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());
        state_->set_render_target(command.target());
        return *this;
    }

    render& render::execute(const viewport_command& command) {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());
        const b2u vp = make_minmax_rect(command.rect());
        GL_CHECK_CODE(state_->dbg(), glViewport(
            math::numeric_cast<GLint>(vp.position.x),
            math::numeric_cast<GLint>(vp.position.y),
            math::numeric_cast<GLsizei>(vp.size.x),
            math::numeric_cast<GLsizei>(vp.size.y)));
        return *this;
    }
}

#endif
