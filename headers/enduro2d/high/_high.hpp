/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../core/_all.hpp"

#include <ecs.hpp/ecs.hpp>

#define SOL_ALL_SAFETIES_ON 1
#include <3rdparty/sol/sol.hpp>

namespace e2d
{
    namespace ecs
    {
        using namespace ecs_hpp;
    }
}

namespace e2d
{
    class atlas_asset;
    class binary_asset;
    class button_style_asset;
    class flipbook_asset;
    class font_asset;
    class image_asset;
    class json_asset;
    class material_asset;
    class mesh_asset;
    class model_asset;
    class prefab_asset;
    class script_asset;
    class shader_asset;
    class shape_asset;
    class sound_asset;
    class spine_asset;
    class sprite_asset;
    class text_asset;
    class texture_asset;
    class toggle_style_asset;
    class xml_asset;

    class actor;
    class behaviour;
    class button;
    class camera;
    class rect_collider;
    class circle_collider;
    class polygon_collider;
    template < typename C >
    class commands;
    template < typename T >
    class disabled;
    template < typename E >
    class events;
    class flipbook_player;
    class handle;
    class label;
    class layout;
    class model_renderer;
    class named;
    class renderer;
    class scene;
    class slider;
    class spine_player;
    class sprite_renderer;
    class toggle_group;
    class toggle;
    class touchable;
    class widget;

    class atlas;
    class button_style;
    class flipbook;
    class model;
    class prefab;
    class script;
    class spine;
    class sprite;
    class toggle_style;

    class button_system;
    class camera_system;
    class flipbook_system;
    class frame_system;
    class gizmos_system;
    class label_system;
    class layout_system;
    class render_system;
    class script_system;
    class slider_system;
    class spine_system;
    class toggle_system;
    class touch_system;
    class widget_system;
    class world_system;

    template < typename Asset, typename Content >
    class content_asset;
    class asset;

    class factory;
    class library;
    class asset_store;
    class asset_group;
    class asset_dependencies;

    class editor;
    class inspector;
    class luasol;
    class starter;
    class world;

    class node;
    class gobject;
    template < typename T >
    class gcomponent;
    template < typename T >
    class const_gcomponent;
}

namespace sol
{
    template < typename T >
    struct unique_usertype_traits<e2d::intrusive_ptr<T>> {
        using type = T;
        using actual_type = e2d::intrusive_ptr<T>;
        static const bool value = true;

        static bool is_null(const actual_type& ptr) {
            return !ptr;
        }

        static type* get(actual_type& ptr) {
            return ptr.get();
        }
    };
}

namespace e2d::ecsex
{
    template < typename T, typename Disposer, typename... Opts >
    void remove_all_components_with_disposer(
        ecs::registry& owner,
        Disposer&& disposer,
        Opts&&... opts)
    {
        static thread_local vector<ecs::entity> to_remove_components;
        E2D_DEFER([](){ to_remove_components.clear(); });

        owner.for_each_component<T>([](const ecs::entity& e, const T&){
            to_remove_components.push_back(e);
        }, std::forward<Opts>(opts)...);

        for ( ecs::entity& e : to_remove_components ) {
            std::invoke(disposer, e, e.get_component<T>());
            e.remove_component<T>();
        }
    }

    template < typename T, typename... Opts >
    void remove_all_components(ecs::registry& owner, Opts&&... opts) {
        remove_all_components_with_disposer<T>(
            owner,
            null_disposer(),
            std::forward<Opts>(opts)...);
    }
}

namespace e2d::ecsex
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

namespace e2d::ecsex
{
    template < typename... Ts, typename F, typename... Opts >
    void for_extracted_components(ecs::registry& owner, F&& f, Opts&&... opts) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<
            std::tuple<ecs::entity, Ts...>> components;

        const std::size_t begin_index = components.size();
        E2D_DEFER([begin_index](){
            components.erase(
                components.begin() + begin_index,
                components.end());
        });

        extract_components<Ts...>(
            owner,
            std::back_inserter(components),
            std::forward<Opts>(opts)...);

        const std::size_t end_index = components.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            std::apply(f, components[i]);
        }
    }

    template < typename... Ts, typename F, typename... Opts >
    void for_extracted_components(const ecs::registry& owner, F&& f, Opts&&... opts) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<
            std::tuple<ecs::const_entity, Ts...>> components;

        const std::size_t begin_index = components.size();
        E2D_DEFER([begin_index](){
            components.erase(
                components.begin() + begin_index,
                components.end());
        });

        extract_components<Ts...>(
            owner,
            std::back_inserter(components),
            std::forward<Opts>(opts)...);

        const std::size_t end_index = components.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            std::apply(f, components[i]);
        }
    }
}

namespace e2d::ecsex
{
    template < typename... Ts, typename Comp, typename F, typename... Opts >
    void for_extracted_sorted_components(ecs::registry& owner, Comp&& comp, F&& f, Opts&&... opts) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<
            std::tuple<ecs::entity, Ts...>> components;

        const std::size_t begin_index = components.size();
        E2D_DEFER([begin_index](){
            components.erase(
                components.begin() + begin_index,
                components.end());
        });

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
    }

    template < typename... Ts, typename Comp, typename F, typename... Opts >
    void for_extracted_sorted_components(const ecs::registry& owner, Comp&& comp, F&& f, Opts&&... opts) {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<
            std::tuple<ecs::const_entity, Ts...>> components;

        const std::size_t begin_index = components.size();
        E2D_DEFER([begin_index](){
            components.erase(
                components.begin() + begin_index,
                components.end());
        });

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
    }
}

namespace e2d::strings
{
    template <>
    class format_arg<ecs::entity> {
        ecs::entity value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1)",
                    make_format_arg(ecs::detail::entity_id_index(value_.id()), width_),
                    make_format_arg(ecs::detail::entity_id_version(value_.id()), width_)));
        }
    };

    template <>
    class format_arg<ecs::const_entity> {
        ecs::const_entity value_;
        u8 width_;
    public:
        template < typename U >
        explicit format_arg(U&& value, u8 width = 0) noexcept
        : value_(std::forward<U>(value)), width_(width) {}

        std::ptrdiff_t write(char* dst, size_t size) const {
            return math::numeric_cast<std::ptrdiff_t>(
                format(dst, size, "(%0,%1)",
                    make_format_arg(ecs::detail::entity_id_index(value_.id()), width_),
                    make_format_arg(ecs::detail::entity_id_version(value_.id()), width_)));
        }
    };
}
