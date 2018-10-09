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
            floating_point,

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
        enum class topology : u8 {
            triangles,
            triangles_fan,
            triangles_strip
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

        enum class compare_func : u8 {
            never,
            less,
            lequal,
            greater,
            gequal,
            equal,
            notequal,
            always
        };

        enum class culling_mode : u8 {
            cw,
            ccw
        };

        enum class culling_face : u8 {
            back,
            front,
            back_and_front
        };

        enum class blending_factor : u8 {
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

        enum class blending_equation : u8 {
            add,
            subtract,
            reverse_subtract
        };

        enum class blending_color_mask : u8 {
            none = 0,

            r = (u8(1) << u8(0)),
            g = (u8(1) << u8(1)),
            b = (u8(1) << u8(2)),
            a = (u8(1) << u8(3)),

            rg = r | g,
            rb = r | b,
            ra = r | a,
            gb = g | b,
            ga = g | a,
            ba = b | a,

            rgb = r | g | b,
            rga = r | g | a,
            rba = r | b | a,
            gba = g | b | a,

            rgba = r | g | b | a
        };

        class depth_state {
        public:
            depth_state& range(f32 near, f32 far) noexcept;
            depth_state& write(bool enable) noexcept;
            depth_state& func(compare_func func) noexcept;
        private:
            friend class render;
            f32 near_ = 0.0f;
            f32 far_ = 1.0f;
            bool write_ = true;
            compare_func func_ = compare_func::less;
            u8 _pad[2] = {0};
        };

        class render_state {
        public:
            render_state& culling(bool enable) noexcept;
            render_state& blending(bool enable) noexcept;
            render_state& depth_test(bool enable) noexcept;
            render_state& stencil_test(bool enable) noexcept;
        private:
            friend class render;
            bool culling_ = false;
            bool blending_ = false;
            bool depth_test_ = false;
            bool stencil_test_ = false;
        };

        class stencil_state {
        public:
            stencil_state& write(u8 mask) noexcept;
            stencil_state& func(compare_func func, u8 ref, u8 mask) noexcept;
            stencil_state& op(stencil_op sfail, stencil_op zfail, stencil_op pass) noexcept;
        private:
            friend class render;
            u8 write_ = 1;
            u8 ref_ = 0;
            u8 read_ = 1;
            stencil_op pass_ = stencil_op::keep;
            stencil_op sfail_ = stencil_op::keep;
            stencil_op zfail_ = stencil_op::keep;
            compare_func func_ = compare_func::always;
            u8 _pad[1] = {0};
        };

        class culling_state {
        public:
            culling_state& mode(culling_mode mode) noexcept;
            culling_state& face(culling_face face) noexcept;
        private:
            friend class render;
            culling_mode mode_ = culling_mode::ccw;
            culling_face face_ = culling_face::back;
            u8 _pad[2] = {0};
        };

        class blending_state {
        public:
            blending_state& constant_color(const color& c) noexcept;
            blending_state& factor(blending_factor src, blending_factor dst) noexcept;
            blending_state& rgb_factor(blending_factor src, blending_factor dst) noexcept;
            blending_state& alpha_factor(blending_factor src, blending_factor dst) noexcept;
            blending_state& equation(blending_equation equation) noexcept;
            blending_state& rgb_equation(blending_equation equation) noexcept;
            blending_state& alpha_equation(blending_equation equation) noexcept;
            blending_state& color_mask(blending_color_mask mask) noexcept;
        private:
            friend class render;
            color constant_color_ = color::clear();
            blending_factor src_rgb_factor_ = blending_factor::one;
            blending_factor dst_rgb_factor_ = blending_factor::zero;
            blending_equation rgb_equation_ = blending_equation::add;
            blending_factor src_alpha_factor_ = blending_factor::one;
            blending_factor dst_alpha_factor_ = blending_factor::zero;
            blending_equation alpha_equation_ = blending_equation::add;
            blending_color_mask color_mask_ = blending_color_mask::rgba;
            u8 _pad[1] = {0};
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

        void draw(
            topology tp,
            const shader_ptr& ps,
            const index_buffer_ptr& ib,
            const vertex_buffer_ptr& vb) noexcept;

        render& clear_depth_buffer(f32 value) noexcept;
        render& clear_stencil_buffer(u8 value) noexcept;
        render& clear_color_buffer(const color& value) noexcept;

        render& set_model(const m4f& model) noexcept;
        render& set_view(const m4f& view) noexcept;
        render& set_projection(const m4f& projection) noexcept;
        render& set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept;

        render& set_depth_state(const depth_state& ds) noexcept;
        render& set_render_state(const render_state& rs) noexcept;
        render& set_stencil_state(const stencil_state& ss) noexcept;
        render& set_culling_state(const culling_state& cs) noexcept;
        render& set_blending_state(const blending_state& bs) noexcept;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}

#include "render.inl"
#endif
