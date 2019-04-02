/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_C9B4A08B2E2A4A659AB390BDC6EBEFE8
#define E2D_INCLUDE_GUARD_C9B4A08B2E2A4A659AB390BDC6EBEFE8
#pragma once

#include "_high.hpp"
#include "library.hpp"

namespace e2d
{
    template < typename Asset >
    typename Asset::load_result library::load_main_asset(str_view address) const {
        auto p = load_main_asset_async<Asset>(address);

        if ( modules::is_initialized<deferrer>() ) {
            the<deferrer>().active_safe_wait_promise(p);
        }

        return p.get_or_default(nullptr);
    }

    template < typename Asset >
    typename Asset::load_async_result library::load_main_asset_async(str_view address) const {
        const auto main_address = address::parent(address);
        const auto main_address_hash = make_hash(main_address);

        if ( !modules::is_initialized<asset_cache<Asset>>() ) {
            return Asset::load_async(*this, main_address);
        }

        auto& cache = the<asset_cache<Asset>>();
        if ( auto cached_asset = cache.find(main_address_hash) ) {
            return stdex::make_resolved_promise(std::move(cached_asset));
        }

        return Asset::load_async(*this, main_address)
            .then([
                &cache,
                main_address_hash
            ](const typename Asset::load_result& new_asset){
                cache.store(main_address_hash, new_asset);
                return new_asset;
            });
    }

    template < typename Asset, typename Nested >
    typename Nested::load_result library::load_asset(str_view address) const {
        auto p = load_asset_async<Asset, Nested>(address);

        if ( modules::is_initialized<deferrer>() ) {
            the<deferrer>().active_safe_wait_promise(p);
        }

        return p.get_or_default(nullptr);
    }

    template < typename Asset, typename Nested >
    typename Nested::load_async_result library::load_asset_async(str_view address) const {
        return load_main_asset_async<Asset>(address::parent(address))
            .then([
                address = str(address)
            ](const typename Asset::load_result& main_asset){
                asset_ptr nested_asset = main_asset;
                str nested_address = address::nested(address);

                while ( nested_asset && !nested_address.empty() ) {
                    nested_asset = nested_asset->find_nested_asset(address::parent(nested_address));
                    nested_address = address::nested(nested_address);
                }

                using nested_asset_type = typename Nested::asset_type;
                if ( auto result = dynamic_pointer_cast<nested_asset_type>(nested_asset) ) {
                    return result;
                }

                throw asset_loading_exception();
            });
    }
}

#endif
