/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
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

    //
    // asset_group
    //

    template < typename Iter >
    asset_group& asset_group::add_assets(Iter first, Iter last) {
        for ( auto iter = first; iter != last; ++iter ) {
            add_asset(iter->first, iter->second);
        }
        return *this;
    }

    inline asset_group& asset_group::add_asset(str_view address, const asset_ptr& asset) {
        str main_address = address::parent(address);
        auto iter = std::upper_bound(
            assets_.begin(), assets_.end(), main_address,
            [](const str& l, const auto& r){
                return l < r.first;
            });
        assets_.insert(iter, std::make_pair(std::move(main_address), asset));
        return *this;
    }

    template < typename Asset, typename Nested >
    typename Nested::load_result asset_group::find_asset(str_view address) const {
        const str main_address = address::parent(address);
        auto iter = std::lower_bound(
            assets_.begin(), assets_.end(), main_address,
            [](const auto& l, const str& r) noexcept {
                return l.first < r;
            });
        for ( ; iter != assets_.end() && iter->first == main_address; ++iter ) {
            asset_ptr main_asset = iter->second;
            typename Asset::load_result typed_main_asset = dynamic_pointer_cast<Asset>(main_asset);
            if ( !typed_main_asset ) {
                continue;
            }

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
        }
        return nullptr;
    }

    //
    // asset_dependency
    //

    template < typename Asset >
    asset_dependency<Asset>::asset_dependency(str_view address)
    : main_address_(address::parent(address)) {}

    template < typename Asset >
    asset_dependency<Asset>::~asset_dependency() noexcept = default;

    template < typename Asset >
    const str& asset_dependency<Asset>::main_address() const noexcept {
        return main_address_;
    }

    template < typename Asset >
    stdex::promise<asset_ptr> asset_dependency<Asset>::load_async(const library& library) {
        return library.load_main_asset_async<Asset>(main_address())
        .then([](const typename Asset::load_result& main_asset){
            return asset_ptr(main_asset);
        });
    }

    //
    // asset_dependencies
    //

    template < typename Asset >
    asset_dependencies& asset_dependencies::add_dependency(str_view address) {
        asset_dependency_base_iptr dep(new asset_dependency<Asset>(address));
        auto iter = std::upper_bound(
            dependencies_.begin(), dependencies_.end(), dep->main_address(),
            [](const str& l, const auto& r){
                return l < r->main_address();
            });
        dependencies_.insert(iter, std::move(dep));
        return *this;
    }

    inline stdex::promise<asset_group> asset_dependencies::load_async(const library& library) {
        vector<stdex::promise<std::pair<str, asset_ptr>>> promises;
        promises.reserve(dependencies_.size());
        std::transform(
            dependencies_.begin(), dependencies_.end(), std::back_inserter(promises),
            [&library](const asset_dependency_base_iptr& dep){
                return dep->load_async(library)
                .then([main_address = dep->main_address()](const asset_ptr& asset){
                    return std::make_pair(main_address, asset);
                });
            });
        return stdex::make_all_promise(std::move(promises))
        .then([](const vector<std::pair<str, asset_ptr>>& results){
            return asset_group()
                .add_assets(results.begin(), results.end());
        });
    }
}

#endif
