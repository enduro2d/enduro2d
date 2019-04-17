/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_1122A7CA62954AEF9E0A787064D28F73
#define E2D_INCLUDE_GUARD_1122A7CA62954AEF9E0A787064D28F73
#pragma once

#include "_high.hpp"

#include "address.hpp"

namespace e2d
{
    //
    // asset_loading_exception
    //

    class asset_loading_exception : public exception {
        const char* what() const noexcept override {
            return "asset loading exception";
        }
    };

    //
    // asset
    //

    class asset;
    using asset_ptr = intrusive_ptr<asset>;
    using nested_content = hash_map<str_hash, asset_ptr>;

    class asset
        : private noncopyable
        , public ref_counter<asset> {
    public:
        asset() = default;
        virtual ~asset() noexcept = default;
        virtual asset_ptr find_nested_asset(str_view nested_address) const noexcept = 0;
    };

    //
    // content_asset
    //

    template < typename Asset, typename Content >
    class content_asset : public asset {
    public:
        using asset_type = Asset;
        using content_type = Content;

        using ptr = intrusive_ptr<Asset>;
        using load_result = intrusive_ptr<Asset>;
        using load_async_result = stdex::promise<load_result>;
    public:
        static load_result create();
        static load_result create(Content content);
        static load_result create(Content content, nested_content nested_content);

        void fill(Content content);
        void fill(Content content, nested_content nested_content);

        const Content& content() const noexcept;

        template < typename NestedAsset >
        typename NestedAsset::ptr find_nested_asset(str_view nested_address) const noexcept;
        asset_ptr find_nested_asset(str_view nested_address) const noexcept override;
    private:
        Content content_;
        nested_content nested_content_;
    };

    //
    // asset_cache_base
    //

    class asset_cache_base : private noncopyable {
    public:
        asset_cache_base();
        virtual ~asset_cache_base() noexcept;

        static std::size_t unload_all_unused_assets() noexcept;
        virtual std::size_t unload_self_unused_assets() noexcept = 0;
    private:
        static std::mutex mutex_;
        static hash_set<asset_cache_base*> caches_;
    };

    //
    // asset_cache
    //

    template < typename Asset >
    class asset_cache
        : public asset_cache_base
        , public module<asset_cache<Asset>> {
    public:
        using asset_ptr = typename Asset::ptr;
    public:
        asset_cache(library& l);
        ~asset_cache() noexcept final = default;

        asset_ptr find(str_hash address) const noexcept;
        void store(str_hash address, const asset_ptr& asset);

        void clear() noexcept;
        std::size_t asset_count() const noexcept;

        std::size_t unload_self_unused_assets() noexcept override;
    private:
        library& library_;
        mutable std::mutex mutex_;
        hash_map<str_hash, asset_ptr> assets_;
    };
}

#include "asset.inl"
#endif
