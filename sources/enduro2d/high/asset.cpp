/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/asset.hpp>

namespace e2d
{
    //
    // asset
    //

    asset::asset() = default;
    asset::~asset() noexcept = default;

    //
    // asset_cache_base
    //

    std::mutex asset_cache_base::mutex_;
    hash_set<asset_cache_base*> asset_cache_base::caches_;

    asset_cache_base::asset_cache_base() {
        std::lock_guard<std::mutex> guard(mutex_);
        caches_.insert(this);
    }

    asset_cache_base::~asset_cache_base() noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        caches_.erase(this);
    }

    std::size_t asset_cache_base::unload_all_unused_assets() noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        return std::accumulate(caches_.begin(), caches_.end(), std::size_t(0),
            [](std::size_t acc, asset_cache_base* cache){
                return acc + cache->unload_self_unused_assets();
            });
    }

    //
    // asset_factory
    //

    asset_factory::asset_factory() = default;
    asset_factory::~asset_factory() noexcept = default;

    //
    // asset_dependency_base
    //

    asset_dependency_base::asset_dependency_base(str_view address)
    : address_(address) {}

    asset_dependency_base::~asset_dependency_base() noexcept = default;
}
