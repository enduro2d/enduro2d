/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_64EC1BED88C24F57851A315A761E9D48
#define E2D_INCLUDE_GUARD_64EC1BED88C24F57851A315A761E9D48
#pragma once

#include "_core.hpp"

namespace e2d
{
    //
    // index_declaration
    //

    class index_declaration final {
    public:
        enum class index_type : u8 {
            unsigned_byte,
            unsigned_short
        };

        class index_info final {
        public:
            index_type type = index_type::unsigned_short;
        public:
            index_info() noexcept;
            ~index_info() noexcept;

            index_info(const index_info&) noexcept;
            index_info& operator=(const index_info&) noexcept;

            explicit index_info(index_type type) noexcept;
        };
    public:
        index_declaration() noexcept;
        ~index_declaration() noexcept;

        index_declaration(const index_declaration&) noexcept;
        index_declaration& operator=(const index_declaration&) noexcept;

        explicit index_declaration(index_type index_type) noexcept;

        const index_info& index() const noexcept;
        std::size_t index_size() const noexcept;
    private:
        index_info index_;
    };

    bool operator==(
        const index_declaration& l,
        const index_declaration& r) noexcept;
    bool operator!=(
        const index_declaration& l,
        const index_declaration& r) noexcept;
    bool operator==(
        const index_declaration::index_info& l,
        const index_declaration::index_info& r) noexcept;
    bool operator!=(
        const index_declaration::index_info& l,
        const index_declaration::index_info& r) noexcept;

    //
    // vertex_declaration
    //

    class vertex_declaration final {
    public:
        constexpr static std::size_t max_attribute_name = 128;
        constexpr static std::size_t max_attribute_count = 16;

        enum class attribute_type : u8 {
            signed_byte,
            unsigned_byte,
            signed_short,
            unsigned_short,
            floating_point
        };

        class attribute_info final {
        public:
            char name[max_attribute_name] = {0};
            std::size_t rows = 0;
            std::size_t columns = 0;
            std::size_t stride = 0;
            attribute_type type = attribute_type::floating_point;
            bool normalized = false;
        public:
            attribute_info() noexcept;
            ~attribute_info() noexcept;

            attribute_info(const attribute_info&) noexcept;
            attribute_info& operator=(const attribute_info&) noexcept;

            attribute_info(
                str_view name,
                std::size_t rows,
                std::size_t columns,
                std::size_t stride,
                attribute_type type,
                bool normalized);

            std::size_t row_size() const noexcept;
        };
    public:
        vertex_declaration() noexcept;
        ~vertex_declaration() noexcept;

        vertex_declaration(const vertex_declaration&) noexcept;
        vertex_declaration& operator=(const vertex_declaration&) noexcept;

        template < typename T >
        vertex_declaration& add_attribute(str_view name) noexcept;
        vertex_declaration& normalized() noexcept;

        vertex_declaration& skip_bytes(
            std::size_t bytes) noexcept;

        vertex_declaration& add_attribute(
            str_view name,
            std::size_t rows,
            std::size_t columns,
            attribute_type type,
            bool normalized) noexcept;

        const attribute_info& attribute(std::size_t i) const noexcept;
        std::size_t attribute_count() const noexcept;
        std::size_t vertex_size() const noexcept;
    private:
        array<attribute_info, max_attribute_count> attributes_;
        std::size_t attribute_count_ = 0;
        std::size_t vertex_size_ = 0;
    };

    bool operator==(
        const vertex_declaration& l,
        const vertex_declaration& r) noexcept;
    bool operator!=(
        const vertex_declaration& l,
        const vertex_declaration& r) noexcept;
    bool operator==(
        const vertex_declaration::attribute_info& l,
        const vertex_declaration::attribute_info& r) noexcept;
    bool operator!=(
        const vertex_declaration::attribute_info& l,
        const vertex_declaration::attribute_info& r) noexcept;

    //
    // shader
    //

    class shader final : noncopyable {
    private:
        friend class render;
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
    public:
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
            sampler_cube
        };
        enum class attribute_type : u8 {
            f,

            v2f,
            v3f,
            v4f,

            m2f,
            m3f,
            m4f
        };
    public:
        explicit shader(internal_state_uptr);
        ~shader() noexcept;
        const vertex_declaration& decl() const noexcept;
        void set_uniform(str_view name, i32 value) const noexcept;
        void set_uniform(str_view name, f32 value) const noexcept;
    private:
        internal_state_uptr state_;
    };
    using shader_ptr = std::shared_ptr<shader>;

    //
    // texture
    //

    class texture final : noncopyable {
    private:
        friend class render;
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
    public:
        enum class wrap : u8 {
            clamp,
            repeat,
            mirror
        };
        enum class filter : u8 {
            linear,
            nearest
        };
    public:
        explicit texture(internal_state_uptr);
        ~texture() noexcept;

        void set_wrap(wrap u, wrap v) noexcept;
        void set_filter(filter min, filter mag) noexcept;
    private:
        internal_state_uptr state_;
    };
    using texture_ptr = std::shared_ptr<texture>;

    //
    // index buffer
    //

    class index_buffer final : private noncopyable {
    private:
        friend class render;
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
    public:
        enum class usage : u8 {
            static_draw,
            stream_draw,
            dynamic_draw
        };
    public:
        explicit index_buffer(internal_state_uptr);
        ~index_buffer() noexcept;
        void update(const buffer& indices, std::size_t offset) noexcept;
        const index_declaration& decl() const noexcept;
        usage buffer_usage() const noexcept;
        std::size_t buffer_size() const noexcept;
        std::size_t index_count() const noexcept;
    private:
        internal_state_uptr state_;
    };
    using index_buffer_ptr = std::shared_ptr<index_buffer>;

    //
    // vertex buffer
    //

    class vertex_buffer final : private noncopyable {
    private:
        friend class render;
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
    public:
        enum class usage : u8{
            static_draw,
            stream_draw,
            dynamic_draw
        };
    public:
        explicit vertex_buffer(internal_state_uptr);
        ~vertex_buffer() noexcept;
        void update(const buffer& vertices, std::size_t offset) noexcept;
        const vertex_declaration& decl() const noexcept;
        usage buffer_usage() const noexcept;
        std::size_t buffer_size() const noexcept;
        std::size_t vertex_count() const noexcept;
    private:
        internal_state_uptr state_;
    };
    using vertex_buffer_ptr = std::shared_ptr<vertex_buffer>;

    //
    // render
    //

    class render final : public module<render> {
    public:
        enum class state : u8 {
            blend,
            cull_face,
            depth_test,
            stencil_test
        };

        enum class cull_face : u8 {
            front,
            back,
            front_back
        };

        enum class blend_func : u8 {
            zero,
            one,
            src_color,
            one_minus_src_color,
            dst_color,
            one_minus_dst_color,
            src_alpha,
            one_minus_src_alpha,
            dst_alpha,
            one_minus_dst_alpha,
            constant_color,
            one_minus_constant_color,
            constant_alpha,
            one_minus_constant_alpha,
            src_alpha_saturate
        };

        enum class blend_equation : u8 {
            add,
            subtract,
            reverse_subtract
        };

        enum class depth_func : u8 {
            never,
            less,
            lequal,
            greater,
            gequal,
            equal,
            notequal,
            always
        };

        enum class stencil_func : u8 {
            never,
            less,
            lequal,
            greater,
            gequal,
            equal,
            notequal,
            always
        };

        enum class stencil_op : u8 {
            keep,
            zero,
            replace,
            incr,
            incr_wrap,
            decr,
            decr_wrap,
            invert
        };

        enum class topology : u8 {
            triangles,
            triangles_fan,
            triangles_strip
        };
    public:
        render(debug& d, window& w);
        ~render() noexcept final;

        shader_ptr create_shader(
            input_stream_uptr vertex,
            input_stream_uptr fragment,
            const vertex_declaration& decl);

        texture_ptr create_texture(
            const image& image);

        texture_ptr create_texture(
            const v2u& size,
            image_data_format format);

        index_buffer_ptr create_index_buffer(
            const buffer& indices,
            const index_declaration& decl,
            index_buffer::usage usage);

        vertex_buffer_ptr create_vertex_buffer(
            const buffer& vertices,
            const vertex_declaration& decl,
            vertex_buffer::usage usage);

        void clear(
            bool color,
            bool depth,
            bool stencil) noexcept;

        void draw(
            topology tp,
            const shader_ptr& ps,
            const index_buffer_ptr& ib,
            const vertex_buffer_ptr& vb) noexcept;

        void set_model(const m4f& model) noexcept;
        void set_view(const m4f& view) noexcept;
        void set_projection(const m4f& projection) noexcept;
        void set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept;

        void enable_state(state state) noexcept;
        void disable_state(state state) noexcept;

        void set_blend_func(blend_func src, blend_func dst) noexcept;
        void set_blend_color(const color& color) noexcept;
        void set_blend_equation(blend_equation blend_equation) noexcept;

        void set_cull_face(cull_face cull_face) noexcept;

        void set_depth_func(depth_func depth_func) noexcept;
        void set_depth_mask(bool yesno) noexcept;
        void set_clear_depth(f32 value) noexcept;

        void set_stencil_func(stencil_func stencil_func, u32 ref, u32 mask) noexcept;
        void set_stencil_mask(u32 mask) noexcept;
        void set_stencil_op(stencil_op fail, stencil_op zfail, stencil_op zpass) noexcept;
        void set_clear_stencil(u32 value) noexcept;

        void set_clear_color(const color& color) noexcept;
        void set_color_mask(bool r, bool g, bool b, bool a);
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}

#include "render.inl"
#endif
