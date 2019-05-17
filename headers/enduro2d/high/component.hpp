/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_DEE612552A524342A523868F34FF88BA
#define E2D_INCLUDE_GUARD_DEE612552A524342A523868F34FF88BA
#pragma once

#include "_high.hpp"

namespace e2d
{
    //
    // bad_component_factory_operation
    //

    class bad_component_factory_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad component factory operation";
        }
    };

    //
    // component_loader
    //

    template < typename Component = void >
    class component_loader;

    template <>
    class component_loader<> {
    public:
        struct fill_context {
            const str parent_address;
            const rapidjson::Value& root;
            const asset_group& dependencies;

            fill_context(
                str nparent_address,
                const rapidjson::Value& nroot,
                const asset_group& ndependencies)
            : parent_address(std::move(nparent_address))
            , root(nroot)
            , dependencies(ndependencies) {}
        };

        struct collect_context {
            const str parent_address;
            const rapidjson::Value& root;

            collect_context(
                str nparent_address,
                const rapidjson::Value& nroot)
            : parent_address(std::move(nparent_address))
            , root(nroot) {}
        };
    public:
    };
    
    //
    // component_creator_base
    //

    class component_creator_base;
    using component_creator_base_iptr = intrusive_ptr<component_creator_base>;

    class component_creator_base
        : private noncopyable
        , public ref_counter<component_creator_base> {
    public:
        component_creator_base() = default;
        virtual ~component_creator_base() noexcept = default;

        virtual bool fill_prototype(
            ecs::prototype& prototype,
            const component_loader<>::fill_context& ctx) const = 0;

        virtual bool collect_dependencies(
            asset_dependencies& dependencies,
            const component_loader<>::collect_context& ctx) const = 0;
    };

    //
    // component_creator
    //

    template < typename Component >
    class component_creator : public component_creator_base {
    public:
        component_creator() = default;
        ~component_creator() noexcept override = default;

        bool fill_prototype(
            ecs::prototype& prototype,
            const component_loader<>::fill_context& ctx) const override;

        bool collect_dependencies(
            asset_dependencies& dependencies,
            const component_loader<>::collect_context& ctx) const override;
    private:
        component_loader<Component> loader_;
    };

    //
    // component_factory
    //

    class component_factory : public module<component_factory> {
    public:
        component_factory() = default;
        ~component_factory() noexcept final = default;

        template < typename Component >
        component_factory& register_component(str_hash type);

        bool fill_prototype(
            str_hash type,
            ecs::prototype& prototype,
            const component_loader<>::fill_context& ctx) const;

        bool collect_dependencies(
            str_hash type,
            asset_dependencies& dependencies,
            const component_loader<>::collect_context& ctx) const;
    private:
        component_creator_base_iptr find_creator(str_hash type) const;
    private:
        mutable std::mutex mutex_;
        hash_map<str_hash, component_creator_base_iptr> creators_;
    };
}

#include "component.inl"
#endif
