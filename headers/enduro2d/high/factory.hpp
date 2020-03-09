/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    //
    // bad_factory_operation
    //

    class bad_factory_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad factory operation";
        }
    };

    //
    // factory_loader
    //

    template < typename Component = void >
    class factory_loader;

    template <>
    class factory_loader<> {
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
    };

    template < typename Component >
    class empty_factory_loader : factory_loader<> {
    public:
        static constexpr const char* const schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {}
        })json";

        bool operator()(
            Component& component,
            const fill_context& ctx) const
        {
            E2D_UNUSED(component, ctx);
            return true;
        }

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const
        {
            E2D_UNUSED(dependencies, ctx);
            return true;
        }
    };

    namespace impl
    {
        template < typename Component >
        using has_component_loader =
            decltype(std::declval<factory_loader<Component>>()(
                std::declval<Component&>(),
                std::declval<const factory_loader<>::fill_context&>()));

        template < typename Component >
        using has_component_loader_collect =
            decltype(std::declval<factory_loader<Component>>()(
                std::declval<asset_dependencies&>(),
                std::declval<const factory_loader<>::collect_context&>()));
    }

    //
    // factory_creator
    //

    namespace impl
    {
        class factory_creator;
        using factory_creator_iptr = intrusive_ptr<factory_creator>;

        class factory_creator
            : private noncopyable
            , public ref_counter<factory_creator> {
        public:
            factory_creator() = default;
            virtual ~factory_creator() noexcept = default;

            virtual bool validate_json(
                const rapidjson::Value& root) const = 0;

            virtual bool fill_prototype(
                ecs::prototype& prototype,
                const factory_loader<>::fill_context& ctx) const = 0;

            virtual bool collect_dependencies(
                asset_dependencies& dependencies,
                const factory_loader<>::collect_context& ctx) const = 0;
        };

        template < typename Component >
        class typed_factory_creator final : public factory_creator {
        public:
            typed_factory_creator();
            ~typed_factory_creator() noexcept final = default;

            bool validate_json(
                const rapidjson::Value& root) const final;

            bool fill_prototype(
                ecs::prototype& prototype,
                const factory_loader<>::fill_context& ctx) const final;

            bool collect_dependencies(
                asset_dependencies& dependencies,
                const factory_loader<>::collect_context& ctx) const final;
        private:
            factory_loader<Component> loader_;
            std::unique_ptr<rapidjson::SchemaDocument> schema_;
        };
    }

    //
    // factory
    //

    class factory : public module<factory> {
    public:
        factory() = default;
        ~factory() noexcept final = default;

        template < typename Component >
        factory& register_component(str_hash type);

        bool validate_json(
            str_hash type,
            const rapidjson::Value& root) const;

        bool fill_prototype(
            str_hash type,
            ecs::prototype& prototype,
            const factory_loader<>::fill_context& ctx) const;

        bool collect_dependencies(
            str_hash type,
            asset_dependencies& dependencies,
            const factory_loader<>::collect_context& ctx) const;
    private:
        impl::factory_creator_iptr find_creator(str_hash type) const;
    private:
        mutable std::mutex mutex_;
        hash_map<str_hash, impl::factory_creator_iptr> creators_;
    };
}

#include "factory.inl"
