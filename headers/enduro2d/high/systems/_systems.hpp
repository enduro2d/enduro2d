/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d::systems
{
    struct update_event {
        f32 dt{0.f};
        f32 time{0.f};
    };

    struct pre_update_event {
        f32 dt{0.f};
        f32 time{0.f};
    };

    struct post_update_event {
        f32 dt{0.f};
        f32 time{0.f};
    };

    struct render_event {};
    struct pre_render_event {};
    struct post_render_event {};

    struct frame_finalize_event {};

    using update_system = ecs::system<update_event>;
    using post_update_system = ecs::system<post_update_event>;

    using render_system = ecs::system<render_event>;
    using post_render_system = ecs::system<post_render_event>;
}

namespace e2d::systems
{
    template < typename... Ts, typename Iter >
    std::size_t extract_components(ecs::registry& owner, Iter iter) {
        std::size_t count{0u};
        owner.for_joined_components<Ts...>(
        [&iter, &count](const ecs::entity& e, Ts&... cs){
            iter++ = std::make_tuple(e, std::ref(cs)...);
            ++count;
        });
        return count;
    }

    template < typename... Ts, typename Iter >
    std::size_t extract_components(const ecs::registry& owner, Iter iter) {
        std::size_t count{0u};
        owner.for_joined_components<Ts...>(
        [&iter, &count](const ecs::const_entity& e, const Ts&... cs){
            iter++ = std::make_tuple(e, std::cref(cs)...);
            ++count;
        });
        return count;
    }
}

namespace e2d::systems
{
    template < typename... Ts, typename F >
    void for_extracted_components(ecs::registry& owner, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<std::tuple<
            ecs::entity,
            std::reference_wrapper<Ts>...>> components;
        try {
            extract_components<Ts...>(owner, std::back_inserter(components));
            for ( auto& t : components ) {
                std::apply(f, t);
            }
        } catch (...) {
            components.clear();
            throw;
        }
        components.clear();
    }

    template < typename... Ts, typename F >
    void for_extracted_components(const ecs::registry& owner, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<std::tuple<
            ecs::const_entity,
            std::reference_wrapper<const Ts>...>> components;
        try {
            extract_components<Ts...>(owner, std::back_inserter(components));
            for ( const auto& t : components ) {
                std::apply(f, t);
            }
        } catch (...) {
            components.clear();
            throw;
        }
        components.clear();
    }
}

namespace e2d::systems
{
    template < typename... Ts, typename Comp, typename F >
    void for_extracted_components(ecs::registry& owner, Comp&& comp, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<std::tuple<
            ecs::entity,
            std::reference_wrapper<Ts>...>> components;
        try {
            extract_components<Ts...>(owner, std::back_inserter(components));
            std::sort(components.begin(), components.end(), comp);
            for ( auto& t : components ) {
                std::apply(f, t);
            }
        } catch (...) {
            components.clear();
            throw;
        }
        components.clear();
    }

    template < typename... Ts, typename Comp, typename F >
    void for_extracted_components(const ecs::registry& owner, Comp&& comp, F&& f) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<std::tuple<
            ecs::const_entity,
            std::reference_wrapper<const Ts>...>> components;
        try {
            extract_components<Ts...>(owner, std::back_inserter(components));
            std::sort(components.begin(), components.end(), comp);
            for ( const auto& t : components ) {
                std::apply(f, t);
            }
        } catch (...) {
            components.clear();
            throw;
        }
        components.clear();
    }
}
