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
    //
    // library
    //

    template < typename T >
    std::shared_ptr<T> library::load_asset(str_view address) {
        const auto asset_url = root_ / address;

        if ( !modules::is_initialized<asset_cache<T>>() ) {
            return T::load(*this, asset_url);
        }

        auto& cache = the<asset_cache<T>>();

        const auto cached_asset = cache.find(address);
        if ( cached_asset ) {
            return cached_asset;
        }

        const auto new_asset = T::load(*this, asset_url);
        if ( new_asset ) {
            cache.store(address, new_asset);
            return new_asset;
        }

        return nullptr;
    }

    //
    // asset_cache
    //

    template < typename T >
    asset_cache<T>::asset_cache(library& l)
    : library_(l) {}

    template < typename T >
    asset_cache<T>::~asset_cache() noexcept = default;

    template < typename T >
    std::shared_ptr<T> asset_cache<T>::find(str_hash address) const {
        std::lock_guard<std::mutex> guard(mutex_);
        const auto iter = assets_.find(address);
        return iter != assets_.end()
            ? iter->second
            : nullptr;
    }

    template < typename T >
    void asset_cache<T>::store(str_hash address, const std::shared_ptr<T>& asset) {
        std::lock_guard<std::mutex> guard(mutex_);
        assets_[address] = asset;
    }

    template < typename T >
    void asset_cache<T>::clear() {
        std::lock_guard<std::mutex> guard(mutex_);
        assets_.clear();
    }

    template < typename T >
    void asset_cache<T>::unload_unused_assets() {
        std::lock_guard<std::mutex> guard(mutex_);
        for ( auto iter = assets_.begin(); iter != assets_.end(); ) {
            if ( iter->second.unique() ) {
                iter = assets_.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    template < typename T >
    std::size_t asset_cache<T>::asset_count() const noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        return assets_.size();
    }
}

#endif
