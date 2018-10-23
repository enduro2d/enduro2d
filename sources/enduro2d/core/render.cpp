/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "render_impl/render.hpp"

namespace
{
    using namespace e2d;

    struct pixel_type_description {
        u32 minimal_size;
        u32 bits_per_pixel;
        bool color;
        bool depth;
        bool stencil;
        pixel_declaration::pixel_type type;
        bool compressed;
        bool must_be_square;
        bool must_be_power_of_two;
    };

    const pixel_type_description pixel_type_descriptions[] = {
        {1, 16, false, true,  false, pixel_declaration::pixel_type::depth16,          false, false, false},
        {1, 24, false, true,  false, pixel_declaration::pixel_type::depth24,          false, false, false},
        {1, 32, false, true,  true,  pixel_declaration::pixel_type::depth24_stencil8, false, false, false},

        {1, 24, true,  false, false, pixel_declaration::pixel_type::rgb8,             false, false, false},
        {1, 32, true,  false, false, pixel_declaration::pixel_type::rgba8,            false, false, false},

        {4,  4, true,  false, false, pixel_declaration::pixel_type::dxt1,             true,  false, true},
        {4,  8, true,  false, false, pixel_declaration::pixel_type::dxt3,             true,  false, true},
        {4,  8, true,  false, false, pixel_declaration::pixel_type::dxt5,             true,  false, true},

        {8,  2, true,  false, false, pixel_declaration::pixel_type::rgb_pvrtc2,       true,  true,  true},
        {8,  4, true,  false, false, pixel_declaration::pixel_type::rgb_pvrtc4,       true,  true,  true},
        {8,  2, true,  false, false, pixel_declaration::pixel_type::rgba_pvrtc2,      true,  true,  true},
        {8,  4, true,  false, false, pixel_declaration::pixel_type::rgba_pvrtc4,      true,  true,  true}
    };

    const pixel_type_description& get_pixel_type_description(pixel_declaration::pixel_type type) noexcept {
        const std::size_t index = math::numeric_cast<std::size_t>(math::enum_to_number(type));
        E2D_ASSERT(index < E2D_COUNTOF(pixel_type_descriptions));
        const pixel_type_description& desc = pixel_type_descriptions[index];
        E2D_ASSERT(desc.type == type);
        return desc;
    }

    std::size_t index_element_size(index_declaration::index_type it) noexcept {
        #define DEFINE_CASE(x,y) case index_declaration::index_type::x: return y;
        switch ( it ) {
            DEFINE_CASE(unsigned_byte, sizeof(u8));
            DEFINE_CASE(unsigned_short, sizeof(u16));
            default:
                E2D_ASSERT_MSG(false, "unexpected index type");
                return 0;
        }
        #undef DEFINE_CASE
    }

    std::size_t attribute_element_size(vertex_declaration::attribute_type at) noexcept {
        #define DEFINE_CASE(x,y) case vertex_declaration::attribute_type::x: return y;
        switch ( at ) {
            DEFINE_CASE(signed_byte, sizeof(u8));
            DEFINE_CASE(unsigned_byte, sizeof(u8));
            DEFINE_CASE(signed_short, sizeof(u16));
            DEFINE_CASE(unsigned_short, sizeof(u16));
            DEFINE_CASE(floating_point, sizeof(u32));
            default:
                E2D_ASSERT_MSG(false, "unexpected attribute type");
                return 0;
        }
        #undef DEFINE_CASE
    }
}

namespace e2d
{
    //
    // pixel_declaration
    //

    pixel_declaration::pixel_declaration(pixel_type type) noexcept
    : type_(type) {}

    pixel_declaration::pixel_type pixel_declaration::type() const noexcept {
        return type_;
    }

    bool pixel_declaration::is_color() const noexcept {
        return get_pixel_type_description(type_).color;
    }

    bool pixel_declaration::is_depth() const noexcept {
        return get_pixel_type_description(type_).depth;
    }

    bool pixel_declaration::is_stencil() const noexcept {
        return get_pixel_type_description(type_).stencil;
    }

    bool pixel_declaration::is_compressed() const noexcept {
        return get_pixel_type_description(type_).compressed;
    }

    std::size_t pixel_declaration::bits_per_pixel() const noexcept {
        return get_pixel_type_description(type_).bits_per_pixel;
    }

    bool operator==(const pixel_declaration& l, const pixel_declaration& r) noexcept {
        return l.type() == r.type();
    }

    bool operator!=(const pixel_declaration& l, const pixel_declaration& r) noexcept {
        return !(l == r);
    }

    //
    // index_declaration
    //

    index_declaration::index_declaration(index_type type) noexcept
    : type_(type) {}

    index_declaration::index_type index_declaration::type() const noexcept {
        return type_;
    }

    std::size_t index_declaration::bytes_per_index() const noexcept {
        return index_element_size(type_);
    }

    bool operator==(const index_declaration& l, const index_declaration& r) noexcept {
        return l.type() == r.type();
    }

    bool operator!=(const index_declaration& l, const index_declaration& r) noexcept {
        return !(l == r);
    }

    //
    // vertex_declaration::attribute_info
    //

    vertex_declaration::attribute_info::attribute_info(
        std::size_t nstride,
        str_hash nname,
        u8 nrows,
        u8 ncolumns,
        attribute_type ntype,
        bool nnormalized) noexcept
    : stride(nstride)
    , name(std::move(nname))
    , rows(nrows)
    , columns(ncolumns)
    , type(ntype)
    , normalized(nnormalized) {}

    std::size_t vertex_declaration::attribute_info::row_size() const noexcept {
        return attribute_element_size(type) * columns;
    }

    //
    // vertex_declaration
    //

    vertex_declaration& vertex_declaration::normalized() noexcept {
        E2D_ASSERT(attribute_count_ > 0);
        attributes_[attribute_count_ - 1].normalized = true;
        return *this;
    }

    vertex_declaration& vertex_declaration::skip_bytes(std::size_t bytes) noexcept {
        bytes_per_vertex_ += bytes;
        return *this;
    }

    vertex_declaration& vertex_declaration::add_attribute(
        str_hash name,
        u8 rows,
        u8 columns,
        attribute_type type,
        bool normalized) noexcept
    {
        E2D_ASSERT(attribute_count_ < attributes_.size());
        const std::size_t stride = bytes_per_vertex_;
        attributes_[attribute_count_] = attribute_info(
            stride,
            name,
            rows,
            columns,
            type,
            normalized);
        bytes_per_vertex_ += attribute_element_size(type) * rows * columns;
        ++attribute_count_;
        return *this;
    }

    const vertex_declaration::attribute_info& vertex_declaration::attribute(std::size_t i) const noexcept {
        E2D_ASSERT(i < attribute_count_);
        return attributes_[i];
    }

    std::size_t vertex_declaration::attribute_count() const noexcept {
        return attribute_count_;
    }

    std::size_t vertex_declaration::bytes_per_vertex() const noexcept {
        return bytes_per_vertex_;
    }

    bool operator==(const vertex_declaration& l, const vertex_declaration& r) noexcept {
        if ( l.bytes_per_vertex() != r.bytes_per_vertex() ) {
            return false;
        }
        if ( l.attribute_count() != r.attribute_count() ) {
            return false;
        }
        for ( std::size_t i = 0, e = l.attribute_count(); i < e; ++i ) {
            if ( l.attribute(i) != r.attribute(i) ) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const vertex_declaration& l, const vertex_declaration& r) noexcept {
        return !(l == r);
    }

    bool operator==(
        const vertex_declaration::attribute_info& l,
        const vertex_declaration::attribute_info& r) noexcept
    {
        return l.stride == r.stride
            && l.name == r.name
            && l.rows == r.rows
            && l.columns == r.columns
            && l.type == r.type
            && l.normalized == r.normalized;
    }

    bool operator!=(
        const vertex_declaration::attribute_info& l,
        const vertex_declaration::attribute_info& r) noexcept
    {
        return !(l == r);
    }

    //
    // depth_state
    //

    render::depth_state& render::depth_state::range(f32 near, f32 far) noexcept {
        range_near_ = near;
        range_far_ = far;
        return *this;
    }

    render::depth_state& render::depth_state::write(bool enable) noexcept {
        write_ = enable;
        return *this;
    }

    render::depth_state& render::depth_state::func(compare_func func) noexcept {
        func_ = func;
        return *this;
    }

    f32 render::depth_state::range_near() const noexcept {
        return range_near_;
    }

    f32 render::depth_state::range_far() const noexcept {
        return range_far_;
    }

    bool render::depth_state::write() const noexcept {
        return write_;
    }

    render::compare_func render::depth_state::func() const noexcept {
        return func_;
    }

    //
    // stencil_state
    //

    render::stencil_state& render::stencil_state::write(u8 mask) noexcept {
        write_ = mask;
        return *this;
    }

    render::stencil_state& render::stencil_state::func(compare_func func, u8 ref, u8 mask) noexcept {
        func_ = func;
        ref_ = ref;
        read_ = mask;
        return *this;
    }

    render::stencil_state& render::stencil_state::op(stencil_op sfail, stencil_op zfail, stencil_op pass) noexcept {
        sfail_ = sfail;
        zfail_ = zfail;
        pass_ = pass;
        return *this;
    }

    u8 render::stencil_state::write() const noexcept {
        return write_;
    }

    render::compare_func render::stencil_state::func() const noexcept {
        return func_;
    }

    u8 render::stencil_state::ref() const noexcept {
        return ref_;
    }

    u8 render::stencil_state::mask() const noexcept {
        return read_;
    }

    render::stencil_op render::stencil_state::pass() const noexcept {
        return pass_;
    }

    render::stencil_op render::stencil_state::sfail() const noexcept {
        return sfail_;
    }

    render::stencil_op render::stencil_state::zfail() const noexcept {
        return zfail_;
    }

    //
    // culling_state
    //

    render::culling_state& render::culling_state::mode(culling_mode mode) noexcept {
        mode_ = mode;
        return *this;
    }

    render::culling_state& render::culling_state::face(culling_face face) noexcept {
        face_ = face;
        return *this;
    }

    render::culling_mode render::culling_state::mode() const noexcept {
        return mode_;
    }

    render::culling_face render::culling_state::face() const noexcept {
        return face_;
    }

    //
    // blending_state
    //

    render::blending_state& render::blending_state::constant_color(const color& c) noexcept {
        constant_color_ = c;
        return *this;
    }

    render::blending_state& render::blending_state::color_mask(blending_color_mask mask) noexcept {
        color_mask_ = mask;
        return *this;
    }

    render::blending_state& render::blending_state::factor(blending_factor src, blending_factor dst) noexcept {
        rgb_factor(src, dst);
        alpha_factor(src, dst);
        return *this;
    }

    render::blending_state& render::blending_state::src_factor(blending_factor src) noexcept {
        src_rgb_factor(src);
        src_alpha_factor(src);
        return *this;
    }

    render::blending_state& render::blending_state::dst_factor(blending_factor dst) noexcept {
        dst_rgb_factor(dst);
        dst_alpha_factor(dst);
        return *this;
    }

    render::blending_state& render::blending_state::rgb_factor(blending_factor src, blending_factor dst) noexcept {
        src_rgb_factor(src);
        dst_rgb_factor(dst);
        return *this;
    }

    render::blending_state& render::blending_state::src_rgb_factor(blending_factor src) noexcept {
        src_rgb_factor_ = src;
        return *this;
    }

    render::blending_state& render::blending_state::dst_rgb_factor(blending_factor dst) noexcept {
        dst_rgb_factor_ = dst;
        return *this;
    }

    render::blending_state& render::blending_state::alpha_factor(blending_factor src, blending_factor dst) noexcept {
        src_alpha_factor(src);
        dst_alpha_factor(dst);
        return *this;
    }

    render::blending_state& render::blending_state::src_alpha_factor(blending_factor src) noexcept {
        src_alpha_factor_ = src;
        return *this;
    }

    render::blending_state& render::blending_state::dst_alpha_factor(blending_factor dst) noexcept {
        dst_alpha_factor_ = dst;
        return *this;
    }

    render::blending_state& render::blending_state::equation(blending_equation equation) noexcept {
        rgb_equation(equation);
        alpha_equation(equation);
        return *this;
    }

    render::blending_state& render::blending_state::rgb_equation(blending_equation equation) noexcept {
        rgb_equation_ = equation;
        return *this;
    }

    render::blending_state& render::blending_state::alpha_equation(blending_equation equation) noexcept {
        alpha_equation_ = equation;
        return *this;
    }

    const color& render::blending_state::constant_color() const noexcept {
        return constant_color_;
    }

    render::blending_color_mask render::blending_state::color_mask() const noexcept {
        return color_mask_;
    }

    render::blending_factor render::blending_state::src_rgb_factor() const noexcept {
        return src_rgb_factor_;
    }

    render::blending_factor render::blending_state::dst_rgb_factor() const noexcept {
        return dst_rgb_factor_;
    }

    render::blending_factor render::blending_state::src_alpha_factor() const noexcept {
        return src_alpha_factor_;
    }

    render::blending_factor render::blending_state::dst_alpha_factor() const noexcept {
        return dst_alpha_factor_;
    }

    render::blending_equation render::blending_state::rgb_equation() const noexcept {
        return rgb_equation_;
    }

    render::blending_equation render::blending_state::alpha_equation() const noexcept {
        return alpha_equation_;
    }

    //
    // capabilities_state
    //

    render::capabilities_state& render::capabilities_state::culling(bool enable) noexcept {
        culling_ = enable;
        return *this;
    }

    render::capabilities_state& render::capabilities_state::blending(bool enable) noexcept {
        blending_ = enable;
        return *this;
    }

    render::capabilities_state& render::capabilities_state::depth_test(bool enable) noexcept {
        depth_test_ = enable;
        return *this;
    }

    render::capabilities_state& render::capabilities_state::stencil_test(bool enable) noexcept {
        stencil_test_ = enable;
        return *this;
    }

    bool render::capabilities_state::culling() const noexcept {
        return culling_;
    }

    bool render::capabilities_state::blending() const noexcept {
        return blending_;
    }

    bool render::capabilities_state::depth_test() const noexcept {
        return depth_test_;
    }

    bool render::capabilities_state::stencil_test() const noexcept {
        return stencil_test_;
    }

    //
    // render_state
    //

    render::state_block& render::state_block::depth(const depth_state& state) noexcept {
        depth_ = state;
        return *this;
    }

    render::state_block& render::state_block::stencil(const stencil_state& state) noexcept {
        stencil_ = state;
        return *this;
    }

    render::state_block& render::state_block::culling(const culling_state& state) noexcept {
        culling_ = state;
        return *this;
    }

    render::state_block& render::state_block::blending(const blending_state& state) noexcept {
        blending_ = state;
        return *this;
    }

    render::state_block& render::state_block::capabilities(const capabilities_state& state) noexcept {
        capabilities_ = state;
        return *this;
    }

    render::depth_state& render::state_block::depth() noexcept {
        return depth_;
    }

    render::stencil_state& render::state_block::stencil() noexcept {
        return stencil_;
    }

    render::culling_state& render::state_block::culling() noexcept {
        return culling_;
    }

    render::blending_state& render::state_block::blending() noexcept {
        return blending_;
    }

    render::capabilities_state& render::state_block::capabilities() noexcept {
        return capabilities_;
    }

    const render::depth_state& render::state_block::depth() const noexcept {
        return depth_;
    }

    const render::stencil_state& render::state_block::stencil() const noexcept {
        return stencil_;
    }

    const render::culling_state& render::state_block::culling() const noexcept {
        return culling_;
    }

    const render::blending_state& render::state_block::blending() const noexcept {
        return blending_;
    }

    const render::capabilities_state& render::state_block::capabilities() const noexcept {
        return capabilities_;
    }

    //
    // render::property_block::sampler
    //

    render::sampler_state& render::sampler_state::texture(const texture_ptr& texture) noexcept {
        texture_ = texture;
        return *this;
    }

    render::sampler_state& render::sampler_state::wrap(sampler_wrap st) noexcept {
        s_wrap(st);
        t_wrap(st);
        return *this;
    }

    render::sampler_state& render::sampler_state::s_wrap(sampler_wrap s) noexcept {
        s_wrap_ = s;
        return *this;
    }

    render::sampler_state& render::sampler_state::t_wrap(sampler_wrap t) noexcept {
        t_wrap_ = t;
        return *this;
    }

    render::sampler_state& render::sampler_state::filter(sampler_min_filter min, sampler_mag_filter mag) noexcept {
        min_filter(min);
        mag_filter(mag);
        return *this;
    }

    render::sampler_state& render::sampler_state::min_filter(sampler_min_filter min) noexcept {
        min_filter_ = min;
        return *this;
    }

    render::sampler_state& render::sampler_state::mag_filter(sampler_mag_filter mag) noexcept {
        mag_filter_ = mag;
        return *this;
    }

    const texture_ptr& render::sampler_state::texture() const noexcept {
        return texture_;
    }

    render::sampler_wrap render::sampler_state::s_wrap() const noexcept {
        return s_wrap_;
    }

    render::sampler_wrap render::sampler_state::t_wrap() const noexcept {
        return t_wrap_;
    }

    render::sampler_min_filter render::sampler_state::min_filter() const noexcept {
        return min_filter_;
    }

    render::sampler_mag_filter render::sampler_state::mag_filter() const noexcept {
        return mag_filter_;
    }

    //
    // render::property_block
    //

    render::property_block& render::property_block::clear() noexcept {
        properties_.clear();
        samplers_.clear();
        return *this;
    }

    render::property_block& render::property_block::merge(const property_block& pb) {
        pb.foreach_by_properties([this](str_hash name, const property_value& v){
            property(name, v);
        });
        pb.foreach_by_samplers([this](str_hash name, const sampler_state& s){
            sampler(name, s);
        });
        return *this;
    }

    render::property_block& render::property_block::sampler(str_hash name, const sampler_state& s) {
        samplers_[name] = s;
        return *this;
    }

    const render::sampler_state* render::property_block::sampler(str_hash name) const noexcept {
        const auto iter = samplers_.find(name);
        return iter != samplers_.end()
            ? &iter->second
            : nullptr;
    }

    render::property_block& render::property_block::property(str_hash name, const property_value& v) {
        properties_[name] = v;
        return *this;
    }

    const render::property_value* render::property_block::property(str_hash name) const noexcept {
        const auto iter = properties_.find(name);
        return iter != properties_.end()
            ? &iter->second
            : nullptr;
    }

    std::size_t render::property_block::sampler_count() const noexcept {
        return samplers_.size();
    }

    std::size_t render::property_block::property_count() const noexcept {
        return properties_.size();
    }

    //
    // pass_state
    //

    render::pass_state& render::pass_state::shader(const shader_ptr& shader) noexcept {
        shader_ = shader;
        return *this;
    }

    render::pass_state& render::pass_state::states(const state_block& render_state) noexcept {
        states_ = render_state;
        return *this;
    }

    render::pass_state& render::pass_state::properties(const property_block& properties) noexcept {
        properties_ = properties;
        return *this;
    }

    shader_ptr& render::pass_state::shader() noexcept {
        return shader_;
    }

    render::state_block& render::pass_state::states() noexcept {
        return states_;
    }

    render::property_block& render::pass_state::properties() noexcept {
        return properties_;
    }

    const shader_ptr& render::pass_state::shader() const noexcept {
        return shader_;
    }

    const render::state_block& render::pass_state::states() const noexcept {
        return states_;
    }

    const render::property_block& render::pass_state::properties() const noexcept {
        return properties_;
    }

    //
    // material
    //

    render::material& render::material::add_pass(const pass_state& pass) noexcept {
        E2D_ASSERT(pass_count_ < max_pass_count);
        passes_[pass_count_] = pass;
        ++pass_count_;
        return *this;
    }

    std::size_t render::material::pass_count() const noexcept {
        return pass_count_;
    }

    render::material& render::material::properties(const property_block& properties) noexcept {
        properties_ = properties;
        return *this;
    }

    render::pass_state& render::material::pass(std::size_t index) noexcept {
        return passes_[index];
    }

    const render::pass_state& render::material::pass(std::size_t index) const noexcept {
        return passes_[index];
    }

    render::property_block& render::material::properties() noexcept {
        return properties_;
    }

    const render::property_block& render::material::properties() const noexcept {
        return properties_;
    }

    //
    // geometry
    //

    render::geometry& render::geometry::add_vertices(const vertex_buffer_ptr& vb) noexcept {
        E2D_ASSERT(vertices_count_ < max_vertices_count);
        vertices_[vertices_count_] = vb;
        ++vertices_count_;
        return *this;
    }

    std::size_t render::geometry::vertices_count() const noexcept {
        return vertices_count_;
    }

    render::geometry& render::geometry::topo(topology tp) noexcept {
        topology_ = tp;
        return *this;
    }

    render::geometry& render::geometry::indices(const index_buffer_ptr& ib) noexcept {
        indices_ = ib;
        return *this;
    }

    render::geometry& render::geometry::vertices(std::size_t index, const vertex_buffer_ptr& vb) noexcept {
        E2D_ASSERT(index < vertices_count_);
        vertices_[index] = vb;
        return *this;
    }

    render::topology& render::geometry::topo() noexcept {
        return topology_;
    }

    index_buffer_ptr& render::geometry::indices() noexcept {
        return indices_;
    }

    vertex_buffer_ptr& render::geometry::vertices(std::size_t index) noexcept {
        E2D_ASSERT(index < vertices_count_);
        return vertices_[index];
    }

    const render::topology& render::geometry::topo() const noexcept {
        return topology_;
    }

    const index_buffer_ptr& render::geometry::indices() const noexcept {
        return indices_;
    }

    const vertex_buffer_ptr& render::geometry::vertices(std::size_t index) const noexcept {
        E2D_ASSERT(index < vertices_count_);
        return vertices_[index];
    }
}
