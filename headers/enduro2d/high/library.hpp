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
    // asset
    //

    class asset : private noncopyable {
    public:
        asset();
        virtual ~asset() noexcept;
    };
    using asset_ptr = std::shared_ptr<asset>;

    //
    // library
    //

    class library final : public module<library> {
    public:
        library(const url& root);
        ~library() noexcept final;

        const url& root() const noexcept;

        template < typename T >
        std::shared_ptr<T> load_asset(str_view address);

        template < typename T >
        stdex::promise<std::shared_ptr<T>> load_asset_async(str_view address);
    private:
        url root_;
    };

    //
    // asset_cache
    //

    template < typename T >
    class asset_cache : public module<asset_cache<T>> {
    public:
        asset_cache(library& l);
        ~asset_cache() noexcept final;

        std::shared_ptr<T> find(str_hash address) const;
        void store(str_hash address, const std::shared_ptr<T>& asset);

        void clear() noexcept;
        void unload_unused_assets() noexcept;
        std::size_t asset_count() const noexcept;
    private:
        library& library_;
        mutable std::mutex mutex_;
        hash_map<str_hash, std::shared_ptr<T>> assets_;
    };
}

#include "library.inl"
#endif
