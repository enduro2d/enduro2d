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
    class render;
    class shader;
    class texture;
    class index_buffer;
    class vertex_buffer;
    class index_declaration;
    class vertex_declaration;

    using shader_ptr = std::shared_ptr<shader>;
    using texture_ptr = std::shared_ptr<texture>;
    using index_buffer_ptr = std::shared_ptr<index_buffer>;
    using vertex_buffer_ptr = std::shared_ptr<vertex_buffer>;

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
            index_info() = default;
            ~index_info() noexcept = default;

            index_info(const index_info&) noexcept = default;
            index_info& operator=(const index_info&) noexcept = default;

            explicit index_info(index_type type) noexcept;
        };
    public:
        index_declaration() = default;
        ~index_declaration() noexcept = default;

        index_declaration(const index_declaration&) noexcept = default;
        index_declaration& operator=(const index_declaration&) noexcept = default;

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
        enum class attribute_type : u8 {
            signed_byte,
            unsigned_byte,
            signed_short,
            unsigned_short,
            floating_point
        };

        class attribute_info final {
        public:
            std::size_t stride = 0;
            str_hash name;
            u8 rows = 0;
            u8 columns = 0;
            attribute_type type = attribute_type::floating_point;
            bool normalized = false;
        public:
            attribute_info() = default;
            ~attribute_info() noexcept = default;

            attribute_info(const attribute_info&) noexcept = default;
            attribute_info& operator=(const attribute_info&) noexcept = default;

            attribute_info(
                std::size_t stride,
                str_hash name,
                u8 rows,
                u8 columns,
                attribute_type type,
                bool normalized) noexcept;

            std::size_t row_size() const noexcept;
        };
    public:
        vertex_declaration() = default;
        ~vertex_declaration() noexcept = default;

        vertex_declaration(const vertex_declaration&) noexcept = default;
        vertex_declaration& operator=(const vertex_declaration&) noexcept = default;

        template < typename T >
        vertex_declaration& add_attribute(str_hash name) noexcept;
        vertex_declaration& normalized() noexcept;

        vertex_declaration& skip_bytes(
            std::size_t bytes) noexcept;

        vertex_declaration& add_attribute(
            str_hash name,
            u8 rows,
            u8 columns,
            attribute_type type,
            bool normalized) noexcept;

        const attribute_info& attribute(std::size_t i) const noexcept;
        std::size_t attribute_count() const noexcept;
        std::size_t vertex_size() const noexcept;
    private:
        constexpr static std::size_t max_attribute_count = 8;
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
    public:
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
        const internal_state& state() const noexcept;
    public:
        explicit shader(internal_state_uptr);
        ~shader() noexcept;
    private:
        internal_state_uptr state_;
    };

    //
    // texture
    //

    class texture final : noncopyable {
    public:
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
        const internal_state& state() const noexcept;
    public:
        explicit texture(internal_state_uptr);
        ~texture() noexcept;
    private:
        internal_state_uptr state_;
    };

    //
    // index buffer
    //

    class index_buffer final : private noncopyable {
    public:
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
        const internal_state& state() const noexcept;
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
        std::size_t buffer_size() const noexcept;
        std::size_t index_count() const noexcept;
    private:
        internal_state_uptr state_;
    };

    //
    // vertex buffer
    //

    class vertex_buffer final : private noncopyable {
    public:
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
        const internal_state& state() const noexcept;
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
        std::size_t buffer_size() const noexcept;
        std::size_t vertex_count() const noexcept;
    private:
        internal_state_uptr state_;
    };

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

            r = (1 << 0),
            g = (1 << 1),
            b = (1 << 2),
            a = (1 << 3),

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

        enum class sampler_wrap : u8 {
            clamp,
            repeat,
            mirror
        };

        enum class sampler_min_filter : u8 {
            nearest,
            linear,
            nearest_mipmap_nearest,
            linear_mipmap_nearest,
            nearest_mipmap_linear,
            linear_mipmap_linear
        };

        enum class sampler_mag_filter : u8 {
            nearest,
            linear
        };

        class depth_state final {
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

        class stencil_state final {
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

        class culling_state final {
        public:
            culling_state& mode(culling_mode mode) noexcept;
            culling_state& face(culling_face face) noexcept;
        private:
            friend class render;
            culling_mode mode_ = culling_mode::ccw;
            culling_face face_ = culling_face::back;
            u8 _pad[2] = {0};
        };

        class blending_state final {
        public:
            blending_state& constant_color(const color& c) noexcept;
            blending_state& color_mask(blending_color_mask mask) noexcept;

            blending_state& factor(blending_factor src, blending_factor dst) noexcept;
            blending_state& src_factor(blending_factor src) noexcept;
            blending_state& dst_factor(blending_factor dst) noexcept;

            blending_state& rgb_factor(blending_factor src, blending_factor dst) noexcept;
            blending_state& src_rgb_factor(blending_factor src) noexcept;
            blending_state& dst_rgb_factor(blending_factor dst) noexcept;

            blending_state& alpha_factor(blending_factor src, blending_factor dst) noexcept;
            blending_state& src_alpha_factor(blending_factor src) noexcept;
            blending_state& dst_alpha_factor(blending_factor dst) noexcept;

            blending_state& equation(blending_equation equation) noexcept;
            blending_state& rgb_equation(blending_equation equation) noexcept;
            blending_state& alpha_equation(blending_equation equation) noexcept;
        private:
            friend class render;
            color constant_color_ = color::clear();
            blending_color_mask color_mask_ = blending_color_mask::rgba;
            blending_factor src_rgb_factor_ = blending_factor::one;
            blending_factor dst_rgb_factor_ = blending_factor::zero;
            blending_equation rgb_equation_ = blending_equation::add;
            blending_factor src_alpha_factor_ = blending_factor::one;
            blending_factor dst_alpha_factor_ = blending_factor::zero;
            blending_equation alpha_equation_ = blending_equation::add;
            u8 _pad[1] = {0};
        };

        class capabilities_state final {
        public:
            capabilities_state& culling(bool enable) noexcept;
            capabilities_state& blending(bool enable) noexcept;
            capabilities_state& depth_test(bool enable) noexcept;
            capabilities_state& stencil_test(bool enable) noexcept;
        private:
            friend class render;
            bool culling_ = false;
            bool blending_ = false;
            bool depth_test_ = false;
            bool stencil_test_ = false;
        };

        class state_block final {
        public:
            state_block& depth(const depth_state& state_block) noexcept;
            state_block& stencil(const stencil_state& state_block) noexcept;
            state_block& culling(const culling_state& state_block) noexcept;
            state_block& blending(const blending_state& state_block) noexcept;
            state_block& capabilities(const capabilities_state& state_block) noexcept;

            depth_state& depth() noexcept;
            stencil_state& stencil() noexcept;
            culling_state& culling() noexcept;
            blending_state& blending() noexcept;
            capabilities_state& capabilities() noexcept;

            const depth_state& depth() const noexcept;
            const stencil_state& stencil() const noexcept;
            const culling_state& culling() const noexcept;
            const blending_state& blending() const noexcept;
            const capabilities_state& capabilities() const noexcept;
        private:
            depth_state depth_;
            stencil_state stencil_;
            culling_state culling_;
            blending_state blending_;
            capabilities_state capabilities_;
        };

        class sampler_state {
        public:
            sampler_state& texture(const texture_ptr& texture) noexcept;

            sampler_state& wrap(sampler_wrap st) noexcept;
            sampler_state& s_wrap(sampler_wrap s) noexcept;
            sampler_state& t_wrap(sampler_wrap t) noexcept;

            sampler_state& filter(sampler_min_filter min, sampler_mag_filter mag) noexcept;
            sampler_state& min_filter(sampler_min_filter min) noexcept;
            sampler_state& mag_filter(sampler_mag_filter mag) noexcept;

            const texture_ptr& texture() const noexcept;

            sampler_wrap s_wrap() const noexcept;
            sampler_wrap t_wrap() const noexcept;

            sampler_min_filter min_filter() const noexcept;
            sampler_mag_filter mag_filter() const noexcept;
        private:
            texture_ptr texture_;
            sampler_wrap s_wrap_;
            sampler_wrap t_wrap_;
            sampler_min_filter min_filter_;
            sampler_mag_filter mag_filter_;
            u8 _pad[4] = {0};
        };

        using property_value = stdex::variant<
            i32, f32,
            v2i, v3i, v4i,
            v2f, v3f, v4f,
            m2f, m3f, m4f>;

        class property_block final {
        public:
            property_block() = default;
            ~property_block() noexcept = default;

            property_block(const property_block&) = default;
            property_block& operator=(const property_block&) = default;

            property_block& clear() noexcept;
            property_block& merge(const property_block& pb);

            property_block& sampler(str_hash name, const sampler_state& s);
            const sampler_state* sampler(str_hash name) const noexcept;

            template < typename T >
            property_block& property(str_hash name, T&& v);
            template < typename T >
            const T* property(str_hash name) const noexcept;

            property_block& property(str_hash name, const property_value& v);
            const property_value* property(str_hash name) const noexcept;

            template < typename F >
            void foreach_by_samplers(F&& f) const;

            template < typename F >
            void foreach_by_properties(F&& f) const;

            std::size_t sampler_count() const noexcept;
            std::size_t property_count() const noexcept;
        private:
            hash_map<str_hash, sampler_state> samplers_;
            hash_map<str_hash, property_value> properties_;
        };

        class pass_state final {
        public:
            pass_state& shader(const shader_ptr& shader) noexcept;
            pass_state& states(const state_block& states) noexcept;
            pass_state& properties(const property_block& properties) noexcept;

            shader_ptr& shader() noexcept;
            state_block& states() noexcept;
            property_block& properties() noexcept;

            const shader_ptr& shader() const noexcept;
            const state_block& states() const noexcept;
            const property_block& properties() const noexcept;
        private:
            property_block properties_;
            shader_ptr shader_;
            state_block states_;
            u8 _pad[4] = {0};
        };

        class material final {
        public:
            material& add_pass(const pass_state& pass) noexcept;
            std::size_t pass_count() const noexcept;

            material& properties(const property_block& properties) noexcept;

            pass_state& pass(std::size_t index) noexcept;
            const pass_state& pass(std::size_t index) const noexcept;

            property_block& properties() noexcept;
            const property_block& properties() const noexcept;
        private:
            constexpr static std::size_t max_pass_count = 8;
            array<pass_state, max_pass_count> passes_;
            std::size_t pass_count_ = 0;
            property_block properties_;
        };

        class geometry final {
        public:
            geometry& add_vertices(const vertex_buffer_ptr& vb) noexcept;
            std::size_t vertices_count() const noexcept;

            geometry& topo(topology tp) noexcept;
            geometry& indices(const index_buffer_ptr& ib) noexcept;
            geometry& vertices(std::size_t index, const vertex_buffer_ptr& vb) noexcept;

            topology& topo() noexcept;
            index_buffer_ptr& indices() noexcept;
            vertex_buffer_ptr& vertices(std::size_t index) noexcept;

            const topology& topo() const noexcept;
            const index_buffer_ptr& indices() const noexcept;
            const vertex_buffer_ptr& vertices(std::size_t index) const noexcept;
        private:
            constexpr static std::size_t max_vertices_count = 8;
            index_buffer_ptr indices_;
            array<vertex_buffer_ptr, max_vertices_count> vertices_;
            std::size_t vertices_count_ = 0;
            topology topology_ = topology::triangles;
            u8 _pad[7] = {0};
        };
    public:
        render(debug& d, window& w);
        ~render() noexcept final;

        shader_ptr create_shader(
            input_stream_uptr vertex_stream,
            input_stream_uptr fragment_stream);

        texture_ptr create_texture(
            const image& image);

        texture_ptr create_texture(
            const input_stream_uptr& image_stream);

        index_buffer_ptr create_index_buffer(
            const buffer& indices,
            const index_declaration& decl,
            index_buffer::usage usage);

        vertex_buffer_ptr create_vertex_buffer(
            const buffer& vertices,
            const vertex_declaration& decl,
            vertex_buffer::usage usage);

        void draw(
            const material& mat,
            const geometry& geo);

        void draw(
            const material& mat,
            const geometry& geo,
            const property_block& props);

        render& clear_depth_buffer(f32 value) noexcept;
        render& clear_stencil_buffer(u8 value) noexcept;
        render& clear_color_buffer(const color& value) noexcept;
        render& set_viewport(u32 x, u32 y, u32 w, u32 h) noexcept;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}

#include "render.inl"
#endif
