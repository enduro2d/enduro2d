/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_C3A242D805B8481AB9F778BA8F272D72
#define E2D_INCLUDE_GUARD_C3A242D805B8481AB9F778BA8F272D72
#pragma once

#include "_high.hpp"

#include "asset.hpp"
#include "address.hpp"

namespace e2d
{
    //
    // bad_library_operation
    //

    class bad_library_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad library operation";
        }
    };

    //
    // library
    //

    class library final : public module<library> {
    public:
        library(const url& root);
        ~library() noexcept final;

        const url& root() const noexcept;
        std::size_t unload_unused_assets() noexcept;

        template < typename Asset >
        typename Asset::load_result load_asset(str_view address) const;

        template < typename Asset >
        typename Asset::load_async_result load_asset_async(str_view address) const;

        template < typename Asset, typename Nested >
        typename Nested::load_result load_asset(str_view address) const;

        template < typename Asset, typename Nested >
        typename Nested::load_async_result load_asset_async(str_view address) const;
    private:
        url root_;
    };
}

#include "library.inl"
#endif
