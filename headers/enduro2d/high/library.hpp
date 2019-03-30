/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_C3A242D805B8481AB9F778BA8F272D72
#define E2D_INCLUDE_GUARD_C3A242D805B8481AB9F778BA8F272D72
#pragma once

#include "_high.hpp"

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
    // asset_loading_exception
    //

    class asset_loading_exception : public exception {
        const char* what() const noexcept override = 0;
    };

    //
    // asset
    //

    class asset
        : private noncopyable
        , public ref_counter<asset> {
    public:
        asset() = default;
        virtual ~asset() noexcept = default;
    };

    using asset_ptr = intrusive_ptr<asset>;
    using nested_content = hash_map<str_hash, asset_ptr>;

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
        static load_result create(Content content) {
            return load_result(new Asset(std::move(content)));
        }

        static load_result create(Content content, nested_content nested_content) {
            return load_result(new Asset(std::move(content), std::move(nested_content)));
        }

        content_asset(Content content)
        : content_(std::move(content)) {}

        content_asset(Content content, nested_content nested_content)
        : content_(std::move(content))
        , nested_content_(std::move(nested_content)) {}

        const Content& content() const noexcept {
            return content_;
        }

        asset_ptr find_nested_asset(str_view name) const noexcept {
            const auto iter = nested_content_.find(name);
            return iter != nested_content_.end()
                ? iter->second
                : nullptr;
        }

        template < typename T >
        intrusive_ptr<T> find_nested_asset(str_view name) const noexcept {
            return dynamic_pointer_cast<T>(find_nested_asset(name));
        }
    private:
        Content content_;
        nested_content nested_content_;
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
        typename Asset::load_result load_asset(str_view address);

        template < typename Asset >
        typename Asset::load_async_result load_asset_async(str_view address);
    private:
        url root_;
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

        asset_ptr find(str_hash address) const;
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

#include "library.inl"
#endif
