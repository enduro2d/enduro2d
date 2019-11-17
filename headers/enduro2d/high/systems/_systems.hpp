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
    template < typename... Ts, typename Iter, typename... Opts >
    std::size_t extract_components(ecs::registry& owner, Iter iter, Opts&&... opts) {
        std::size_t count{0u};
        owner.for_joined_components<Ts...>(
        [&iter, &count](const ecs::entity& e, Ts&... cs){
            iter++ = std::make_tuple(e, cs...);
            ++count;
        }, std::forward<Opts>(opts)...);
        return count;
    }

    template < typename... Ts, typename Iter, typename... Opts >
    std::size_t extract_components(const ecs::registry& owner, Iter iter, Opts&&... opts) {
        std::size_t count{0u};
        owner.for_joined_components<Ts...>(
        [&iter, &count](const ecs::const_entity& e, const Ts&... cs){
            iter++ = std::make_tuple(e, cs...);
            ++count;
        }, std::forward<Opts>(opts)...);
        return count;
    }
}

namespace e2d::systems
{
    template < typename... Ts, typename F, typename... Opts >
    void for_extracted_components(ecs::registry& owner, F&& f, Opts&&... opts) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<
            std::tuple<ecs::entity, Ts...>> components;
        const std::size_t begin_index = components.size();

        try {
            extract_components<Ts...>(
                owner,
                std::back_inserter(components),
                std::forward<Opts>(opts)...);

            const std::size_t end_index = components.size();
            for ( std::size_t i = begin_index; i < end_index; ++i ) {
                std::apply(f, components[i]);
            }
        } catch (...) {
            components.erase(
                components.begin() + begin_index,
                components.end());
            throw;
        }

        components.erase(
            components.begin() + begin_index,
            components.end());
    }

    template < typename... Ts, typename F, typename... Opts >
    void for_extracted_components(const ecs::registry& owner, F&& f, Opts&&... opts) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<
            std::tuple<ecs::const_entity, Ts...>> components;
        const std::size_t begin_index = components.size();

        try {
            extract_components<Ts...>(
                owner,
                std::back_inserter(components),
                std::forward<Opts>(opts)...);

            const std::size_t end_index = components.size();
            for ( std::size_t i = begin_index; i < end_index; ++i ) {
                std::apply(f, components[i]);
            }
        } catch (...) {
            components.erase(
                components.begin() + begin_index,
                components.end());
            throw;
        }

        components.erase(
            components.begin() + begin_index,
            components.end());
    }
}

namespace e2d::systems
{
    template < typename... Ts, typename Comp, typename F, typename... Opts >
    void for_extracted_sorted_components(ecs::registry& owner, Comp&& comp, F&& f, Opts&&... opts) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<
            std::tuple<ecs::entity, Ts...>> components;
        const std::size_t begin_index = components.size();

        try {
            extract_components<Ts...>(
                owner,
                std::back_inserter(components),
                std::forward<Opts>(opts)...);

            std::sort(
                components.begin() + begin_index,
                components.end(),
                comp);

            const std::size_t end_index = components.size();
            for ( std::size_t i = begin_index; i < end_index; ++i ) {
                std::apply(f, components[i]);
            }
        } catch (...) {
            components.erase(
                components.begin() + begin_index,
                components.end());
            throw;
        }

        components.erase(
            components.begin() + begin_index,
            components.end());
    }

    template < typename... Ts, typename Comp, typename F, typename... Opts >
    void for_extracted_sorted_components(const ecs::registry& owner, Comp&& comp, F&& f, Opts&&... opts) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<
            std::tuple<ecs::const_entity, Ts...>> components;
        const std::size_t begin_index = components.size();

        try {
            extract_components<Ts...>(
                owner,
                std::back_inserter(components),
                std::forward<Opts>(opts)...);

            std::sort(
                components.begin() + begin_index,
                components.end(),
                comp);

            const std::size_t end_index = components.size();
            for ( std::size_t i = begin_index; i < end_index; ++i ) {
                std::apply(f, components[i]);
            }
        } catch (...) {
            components.erase(
                components.begin() + begin_index,
                components.end());
            throw;
        }

        components.erase(
            components.begin() + begin_index,
            components.end());
    }
}
