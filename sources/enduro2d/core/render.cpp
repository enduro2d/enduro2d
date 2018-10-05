/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "render_impl/render.hpp"

namespace
{
    using namespace e2d;

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
    // index_declaration::index_info
    //

    index_declaration::index_info::index_info() noexcept = default;
    index_declaration::index_info::~index_info() noexcept = default;

    index_declaration::index_info::index_info(
        const index_info&) noexcept = default;
    index_declaration::index_info& index_declaration::index_info::operator=(
        const index_info&) noexcept = default;

    index_declaration::index_info::index_info(index_type ntype) noexcept
    : type(ntype) {}

    //
    // index_declaration
    //

    index_declaration::index_declaration() noexcept = default;
    index_declaration::~index_declaration() noexcept = default;

    index_declaration::index_declaration(
        const index_declaration&) noexcept = default;
    index_declaration& index_declaration::operator=(
        const index_declaration&) noexcept = default;

    index_declaration::index_declaration(index_type type) noexcept
    : index_(type) {}

    const index_declaration::index_info& index_declaration::index() const noexcept {
        return index_;
    }

    std::size_t index_declaration::index_size() const noexcept {
        return index_element_size(index_.type);
    }

    bool operator==(const index_declaration& l, const index_declaration& r) noexcept {
        return l.index_size() == r.index_size()
            && l.index() == r.index();
    }

    bool operator!=(const index_declaration& l, const index_declaration& r) noexcept {
        return !(l == r);
    }

    bool operator==(
        const index_declaration::index_info& l,
        const index_declaration::index_info& r) noexcept
    {
        return l.type == r.type;
    }

    bool operator!=(
        const index_declaration::index_info& l,
        const index_declaration::index_info& r) noexcept
    {
        return !(l == r);
    }

    //
    // vertex_declaration::attribute_info
    //

    vertex_declaration::attribute_info::attribute_info() noexcept = default;
    vertex_declaration::attribute_info::~attribute_info() noexcept = default;

    vertex_declaration::attribute_info::attribute_info(
        const attribute_info&) noexcept = default;
    vertex_declaration::attribute_info& vertex_declaration::attribute_info::operator=(
        const attribute_info&) noexcept = default;

    vertex_declaration::attribute_info::attribute_info(
        str_view nname,
        std::size_t nrows,
        std::size_t ncolumns,
        std::size_t nstride,
        attribute_type ntype,
        bool nnormalized)
    : rows(nrows)
    , columns(ncolumns)
    , stride(nstride)
    , type(ntype)
    , normalized(nnormalized) {
        bool name_format_success = strings::format_nothrow(
            name, E2D_COUNTOF(name), nullptr, "%0", nname);
        E2D_UNUSED(name_format_success);
        E2D_ASSERT(name_format_success);
    }

    std::size_t vertex_declaration::attribute_info::row_size() const noexcept {
        return attribute_element_size(type) * columns;
    }

    //
    // vertex_declaration
    //

    vertex_declaration::vertex_declaration() noexcept = default;
    vertex_declaration::~vertex_declaration() noexcept = default;

    vertex_declaration::vertex_declaration(
        const vertex_declaration&) noexcept = default;
    vertex_declaration& vertex_declaration::operator=(
        const vertex_declaration&) noexcept = default;

    vertex_declaration& vertex_declaration::normalized() noexcept {
        E2D_ASSERT(attribute_count_ > 0);
        attributes_[attribute_count_ - 1].normalized = true;
        return *this;
    }

    vertex_declaration& vertex_declaration::skip_bytes(std::size_t bytes) noexcept {
        vertex_size_ += bytes;
        return *this;
    }

    vertex_declaration& vertex_declaration::add_attribute(
        str_view name,
        std::size_t rows,
        std::size_t columns,
        attribute_type type,
        bool normalized) noexcept
    {
        E2D_ASSERT(attribute_count_ < attributes_.size());
        const std::size_t stride = vertex_size_;
        attributes_[attribute_count_] = attribute_info(
            name,
            rows,
            columns,
            stride,
            type,
            normalized);
        vertex_size_ += attribute_element_size(type) * rows * columns;
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

    std::size_t vertex_declaration::vertex_size() const noexcept {
        return vertex_size_;
    }

    bool operator==(const vertex_declaration& l, const vertex_declaration& r) noexcept {
        if ( l.vertex_size() != r.vertex_size() ) {
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
        return !std::strcmp(l.name, r.name)
            && l.rows == r.rows
            && l.columns == r.columns
            && l.stride == r.stride
            && l.type == r.type
            && l.normalized == r.normalized;
    }

    bool operator!=(
        const vertex_declaration::attribute_info& l,
        const vertex_declaration::attribute_info& r) noexcept
    {
        return !(l == r);
    }
}
