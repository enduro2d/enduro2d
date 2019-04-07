/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_6FD0B1DD12004CF78E7AC5FBBC36E854
#define E2D_INCLUDE_GUARD_6FD0B1DD12004CF78E7AC5FBBC36E854
#pragma once

#include "_high.hpp"
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
    template < typename T >
    intrusive_ptr<T> content_asset<Asset, Content>::find_nested_asset(str_view name) const noexcept {
        return dynamic_pointer_cast<T>(find_nested_asset(name));
    }

    template < typename Asset, typename Content >
    asset_ptr content_asset<Asset, Content>::find_nested_asset(str_view name) const noexcept {
        const auto iter = nested_content_.find(name);
        return iter != nested_content_.end()
            ? iter->second
            : nullptr;
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
    typename asset_cache<T>::asset_ptr asset_cache<T>::find(str_hash address) const noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        const auto iter = assets_.find(address);
        return iter != assets_.end()
            ? iter->second
            : nullptr;
    }

    template < typename T >
    void asset_cache<T>::store(str_hash address, const asset_ptr& asset) {
        std::lock_guard<std::mutex> guard(mutex_);
        assets_[address] = asset;
    }

    template < typename T >
    void asset_cache<T>::clear() noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        assets_.clear();
    }

    template < typename T >
    std::size_t asset_cache<T>::asset_count() const noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
        return assets_.size();
    }

    template < typename T >
    std::size_t asset_cache<T>::unload_self_unused_assets() noexcept {
        std::lock_guard<std::mutex> guard(mutex_);
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

    //
    // asset_factory
    //

    template < typename Asset >
    asset_factory& asset_factory::register_asset(str_hash type) {
        return register_creator(type, [](const library& library, str_view address) {
            return Asset::load_async(library, address)
                .then([](const typename Asset::load_result& result){
                    return static_pointer_cast<asset>(result);
                });
        });
    }

    inline asset_factory& asset_factory::register_creator(str_hash type, asset_creator creator) {
        std::lock_guard<std::mutex> guard(mutex_);
        bool success = creators_.insert(
            std::make_pair(type, std::move(creator))).second;
        if ( !success ) {
            throw bad_asset_factory_operation();
        }
        return *this;
    }
}

#endif
