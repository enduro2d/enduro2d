/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
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
    // library
    //

    class library final : public module<library> {
    public:
        library(const url& root);
        ~library() noexcept final;

        const url& root() const noexcept;
        std::size_t unload_unused_assets() noexcept;

        template < typename Asset >
        typename Asset::load_result load_main_asset(str_view address) const;

        template < typename Asset >
        typename Asset::load_async_result load_main_asset_async(str_view address) const;

        template < typename Asset, typename Nested = Asset >
        typename Nested::load_result load_asset(str_view address) const;

        template < typename Asset, typename Nested = Asset >
        typename Nested::load_async_result load_asset_async(str_view address) const;
    private:
        url root_;
    };

    //
    // asset_group
    //

    class asset_group {
    public:
        asset_group() = default;
        ~asset_group() noexcept = default;

        asset_group(asset_group&& other) noexcept = default;
        asset_group& operator=(asset_group&& other) noexcept = default;

        asset_group(const asset_group& other) = default;
        asset_group& operator=(const asset_group& other) = default;

        template < typename Iter >
        asset_group& add_assets(Iter first, Iter last);
        asset_group& add_asset(str_view address, const asset_ptr& asset);

        template < typename Asset, typename Nested = Asset >
        typename Nested::load_result find_asset(str_view address) const;
    private:
        vector<std::pair<str, asset_ptr>> assets_;
    };

    //
    // asset_dependency_base
    //

    class asset_dependency_base;
    using asset_dependency_base_iptr = intrusive_ptr<asset_dependency_base>;

    class asset_dependency_base
        : private noncopyable
        , public ref_counter<asset_dependency_base> {
    public:
        asset_dependency_base() = default;
        virtual ~asset_dependency_base() noexcept = default;

        virtual const str& main_address() const noexcept = 0;
        virtual stdex::promise<asset_ptr> load_async(const library& library) = 0;
    };

    //
    // asset_dependency
    //

    template < typename Asset >
    class asset_dependency : public asset_dependency_base {
    public:
        using asset_type = Asset;
        using load_result = typename Asset::load_result;
    public:
        asset_dependency(str_view address);
        ~asset_dependency() noexcept override;

        const str& main_address() const noexcept override;
        stdex::promise<asset_ptr> load_async(const library& library) override;
    private:
        str main_address_;
    };

    //
    // asset_dependencies
    //

    class asset_dependencies final {
    public:
        asset_dependencies() = default;
        ~asset_dependencies() noexcept = default;

        asset_dependencies(asset_dependencies&& other) noexcept = default;
        asset_dependencies& operator=(asset_dependencies&& other) noexcept = default;

        asset_dependencies(const asset_dependencies& other) = default;
        asset_dependencies& operator=(const asset_dependencies& other) = default;

        template < typename Asset >
        asset_dependencies& add_dependency(str_view address);
        stdex::promise<asset_group> load_async(const library& library);
    private:
        vector<asset_dependency_base_iptr> dependencies_;
    };
}

#include "library.inl"
#endif
