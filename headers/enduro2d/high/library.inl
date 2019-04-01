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
    typename Asset::load_result library::load_asset(str_view address) const {
        auto p = load_asset_async<Asset>(address);

        if ( modules::is_initialized<deferrer>() ) {
            the<deferrer>().active_safe_wait_promise(p);
        }

        return p.get_or_default(nullptr);
    }

    template < typename Asset >
    typename Asset::load_async_result library::load_asset_async(str_view address) const {
        if ( !modules::is_initialized<asset_cache<Asset>>() ) {
            return Asset::load_async(*this, address);
        }

        auto& cache = the<asset_cache<Asset>>();
        if ( auto cached_asset = cache.find(address) ) {
            return stdex::make_resolved_promise(std::move(cached_asset));
        }

        return Asset::load_async(*this, address)
            .then([
                &cache,
                address_hash = make_hash(address)
            ](const typename Asset::load_result& new_asset){
                cache.store(address_hash, new_asset);
                return new_asset;
            });
    }
}

#endif
