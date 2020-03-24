/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "asset.hpp"

namespace e2d
{
    //
    // content_asset
    //

    template < typename Asset, typename Content >
    typename content_asset<Asset, Content>::load_result
    content_asset<Asset, Content>::create() {
        return load_result(new Asset());
    }

    template < typename Asset, typename Content >
    typename content_asset<Asset, Content>::load_result
    content_asset<Asset, Content>::create(Content content) {
        auto result = create();
        result->fill(std::move(content));
        return result;
    }

    template < typename Asset, typename Content >
    typename content_asset<Asset, Content>::load_result
    content_asset<Asset, Content>::create(Content content, nested_content nested_content) {
        auto result = create();
        result->fill(std::move(content), std::move(nested_content));
        return result;
    }

    template < typename Asset, typename Content >
    void content_asset<Asset, Content>::fill(Content content) {
        content_ = std::move(content);
    }

    template < typename Asset, typename Content >
    void content_asset<Asset, Content>::fill(Content content, nested_content nested_content) {
        content_ = std::move(content);
        nested_content_ = std::move(nested_content);
    }

    template < typename Asset, typename Content >
    const Content& content_asset<Asset, Content>::content() const noexcept {
        return content_;
    }

    template < typename Asset, typename Content >
    template < typename NestedAsset >
    typename NestedAsset::ptr content_asset<Asset, Content>::find_nested_asset(str_view nested_address) const noexcept {
        return dynamic_pointer_cast<NestedAsset>(find_nested_asset(nested_address));
    }

    template < typename Asset, typename Content >
    asset_ptr content_asset<Asset, Content>::find_nested_asset(str_view address) const noexcept {
        const auto iter = nested_content_.find(make_hash(address::parent(address)));
        if ( iter == nested_content_.end() ) {
            return nullptr;
        }
        const str nested_asset = address::nested(address);
        return nested_asset.empty()
            ? iter->second
            : iter->second->find_nested_asset(nested_asset);
    }

    //
    // typed_asset_cache
    //

    namespace impl
    {
        template < typename T >
        typename typed_asset_cache<T>::asset_ptr typed_asset_cache<T>::find(str_hash address) const noexcept {
            const auto iter = assets_.find(address);
            return iter != assets_.end()
                ? iter->second
                : nullptr;
        }

        template < typename T >
        void typed_asset_cache<T>::store(str_hash address, const asset_ptr& asset) {
            assets_[address] = asset;
        }

        template < typename T >
        std::size_t typed_asset_cache<T>::asset_count() const noexcept {
            return assets_.size();
        }

        template < typename T >
        std::size_t typed_asset_cache<T>::unload_unused_assets() noexcept {
            std::size_t result = 0u;
            for ( auto iter = assets_.begin(); iter != assets_.end(); ) {
                if ( !iter->second || 1 == iter->second->use_count() ) {
                    iter = assets_.erase(iter);
                    ++result;
                } else {
                    ++iter;
                }
            }
            return result;
        }
    }

    //
    // asset_store
    //

    template < typename Asset >
    void asset_store::store(str_hash address, const typename Asset::ptr& asset) {
        const auto family = utils::type_family<Asset>::id();
        const auto iter = caches_.find(family);
        impl::typed_asset_cache<Asset>* cache = iter != caches_.end() && iter->second
            ? static_cast<impl::typed_asset_cache<Asset>*>(iter->second.get())
            : nullptr;
        if ( !cache ) {
            cache = static_cast<impl::typed_asset_cache<Asset>*>(caches_.emplace(
                family,
                make_intrusive<impl::typed_asset_cache<Asset>>()).first->second.get());
        }
        cache->store(address, asset);
    }

    template < typename Asset >
    typename Asset::ptr asset_store::find(str_hash address) const noexcept {
        const auto iter = caches_.find(utils::type_family<Asset>::id());
        const impl::typed_asset_cache<Asset>* cache = iter != caches_.end() && iter->second
            ? static_cast<const impl::typed_asset_cache<Asset>*>(iter->second.get())
            : nullptr;
        return cache
            ? cache->find(address)
            : nullptr;
    }

    template < typename Asset >
    std::size_t asset_store::asset_count() const noexcept {
        const auto iter = caches_.find(utils::type_family<Asset>::id());
        return iter != caches_.end() && iter->second
            ? iter->second->asset_count()
            : 0u;
    }

    inline std::size_t asset_store::asset_count() const noexcept {
        return std::accumulate(
            caches_.begin(), caches_.end(), std::size_t(0),
            [](std::size_t acc, const auto& p){
                return p.second
                    ? acc + p.second->asset_count()
                    : acc;
            });
    }

    inline std::size_t asset_store::unload_unused_assets() noexcept {
        return std::accumulate(
            caches_.begin(), caches_.end(), std::size_t(0),
            [](std::size_t acc, const auto& p){
                return p.second
                    ? acc + p.second->unload_unused_assets()
                    : acc;
            });
    }
}
