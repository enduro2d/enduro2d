/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    //
    // shader
    //

    class shader final : noncopyable {
    private:
        friend class render;
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
    public:
        enum class uniform_type {
            i,
            f,

            v2i,
            v3i,
            v4i,

            v2f,
            v3f,
            v4f,

            m2f,
            m3f,
            m4f,

            s2d
        };
        enum class attribute_type {
            f,

            v2f,
            v3f,
            v4f,

            m2f,
            m3f,
            m4f
        };
    public:
        shader(internal_state_uptr);
        ~shader() noexcept;
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
        enum class wrap {
            clamp,
            repeat,
            mirror
        };
        enum class filter {
            linear,
            nearest
        };
    public:
        texture(internal_state_uptr);
        ~texture() noexcept;

        void set_wrap(wrap u, wrap v) noexcept;
        void set_filter(filter min, filter mag) noexcept;

        const v2u& native_size() const noexcept;
        const v2u& original_size() const noexcept;
    private:
        internal_state_uptr state_;
    };
    using texture_ptr = std::shared_ptr<texture>;

    //
    // render
    //

    class render final : public module<render> {
    public:
        enum class state {
            blend,
            cull_face,
            depth_test,
            stencil_test
        };
        enum class cull_face {
            front,
            back,
            front_back
        };
        enum class blend_func {
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
        enum class blend_equation {
            add,
            subtract,
            reverse_subtract
        };
        enum class depth_func {
            never,
            less,
            lequal,
            greater,
            gequal,
            equal,
            notequal,
            always
        };
        enum class stencil_func {
            never,
            less,
            lequal,
            greater,
            gequal,
            equal,
            notequal,
            always
        };
        enum class stencil_op {
            keep,
            zero,
            replace,
            incr,
            incr_wrap,
            decr,
            decr_wrap,
            invert
        };
    public:
        render(debug& debug);
        ~render() noexcept;

        shader_ptr create_shader(
            input_stream_uptr vertex,
            input_stream_uptr fragment);

        texture_ptr create_texture(const image& image);
        texture_ptr create_texture(const v2u& size, image_data_format format);

        void clear(bool color, bool depth, bool stencil) noexcept;

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
        debug& debug_;
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
