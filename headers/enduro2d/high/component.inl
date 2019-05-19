/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#ifndef E2D_INCLUDE_GUARD_85A0D62C0D794C4C9DDFB1BAED3C0A18
#define E2D_INCLUDE_GUARD_85A0D62C0D794C4C9DDFB1BAED3C0A18
#pragma once

#include "_high.hpp"
#include "component.hpp"

namespace e2d
{
    //
    // component_creator
    //

    template < typename Component >
    component_creator<Component>::component_creator() {
        rapidjson::Document doc;
        if ( doc.Parse(loader_.schema_source).HasParseError() ) {
            the<debug>().error("COMPONENT: Failed to parse component loader schema");
            throw bad_component_factory_operation();
        }
        json_utils::add_common_schema_definitions(doc);
        schema_ = std::make_unique<rapidjson::SchemaDocument>(doc);
    }

    template < typename Component >
    bool component_creator<Component>::validate_json(
        const rapidjson::Value& root) const
    {
        rapidjson::SchemaValidator validator(*schema_);
        if ( root.Accept(validator) ) {
            return true;
        }
        rapidjson::StringBuffer sb;
        if ( !validator.GetInvalidDocumentPointer().StringifyUriFragment(sb) ) {
            the<debug>().error("COMPONENT: Failed to validate component json");
            return false;
        }
        the<debug>().error("COMPONENT: Failed to validate component json:\n"
            "--> Invalid schema keyword: %0\n"
            "--> Invalid document pointer: %1",
            validator.GetInvalidSchemaKeyword(),
            sb.GetString());
        return false;
    }

    template < typename Component >
    bool component_creator<Component>::fill_prototype(
        ecs::prototype& prototype,
        const component_loader<>::fill_context& ctx) const
    {
        Component component;
        prototype.apply_to_component(component);

        if ( !loader_(component, ctx) ) {
            return false;
        }

        prototype.component<Component>(component);
        return true;
    }

    template < typename Component >
    bool component_creator<Component>::collect_dependencies(
        asset_dependencies& dependencies,
        const component_loader<>::collect_context& ctx) const
    {
        return loader_(dependencies, ctx);
    }

    //
    // component_factory
    //

    template < typename Component >
    component_factory& component_factory::register_component(str_hash type) {
        std::lock_guard<std::mutex> guard(mutex_);
        if ( creators_.count(type) > 0 ) {
            throw bad_component_factory_operation();
        }
        component_creator_base_iptr creator(new component_creator<Component>());
        creators_.emplace(type, std::move(creator));
        return *this;
    }
}

#endif
