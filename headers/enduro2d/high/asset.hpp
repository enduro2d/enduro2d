/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_1122A7CA62954AEF9E0A787064D28F73
#define E2D_INCLUDE_GUARD_1122A7CA62954AEF9E0A787064D28F73
#pragma once

#include "_high.hpp"

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
    // bad_asset_factory_operation
    //

    class bad_asset_factory_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad asset factory operation";
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
        asset();
        virtual ~asset() noexcept;
        virtual asset_ptr find_nested_asset(str_view name) const noexcept = 0;
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

        template < typename T >
        intrusive_ptr<T> find_nested_asset(str_view name) const noexcept;
        asset_ptr find_nested_asset(str_view name) const noexcept override;
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
    class asset_cache : public asset_cache_base
                      , public module<asset_cache<Asset>> {
    public:
        using asset_ptr = typename Asset::ptr;
    public:
        asset_cache(library& l);
        ~asset_cache() noexcept final;

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

    //
    // asset_factory
    //

    class asset_factory : public module<asset_factory> {
    public:
        using asset_creator = std::function<
            stdex::promise<asset_ptr>(const library& library, str_view address)>;
    public:
        asset_factory();
        ~asset_factory() noexcept final;

        template < typename Asset >
        asset_factory& register_asset(str_hash type);
        asset_factory& register_creator(str_hash type, asset_creator creator);
    private:
        std::mutex mutex_;
        hash_map<str_hash, asset_creator> creators_;
    };
}

#include "asset.inl"
#endif
