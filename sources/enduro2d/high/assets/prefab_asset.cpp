/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/prefab_asset.hpp>

#include <enduro2d/high/component.hpp>
#include <enduro2d/high/assets/json_asset.hpp>

namespace
{
    using namespace e2d;

    class prefab_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "prefab asset loading exception";
        }
    };

    const char* prefab_asset_schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "prototype" : { "$ref": "#/common_definitions/address" },
            "components" : { "type" : "object" },
            "children" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/child" }
            }
        },
        "definitions" : {
            "child" : {
                "type" : "object",
                "required" : [],
                "additionalProperties" : false,
                "properties" : {
                    "prototype" : { "$ref": "#/common_definitions/address" },
                    "components" : { "type" : "object" },
                    "children" : {
                        "type" : "array",
                        "items" : { "$ref": "#/definitions/child" }
                    }
                }
            }
        }
    })json";

    const rapidjson::SchemaDocument& prefab_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(prefab_asset_schema_source).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse prefab asset schema");
                throw prefab_asset_loading_exception();
            }
            json_utils::add_common_schema_definitions(doc);
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }

        return *schema;
    }

    void collect_dependencies(
        str_view parent_address,
        const rapidjson::Value& root,
        asset_dependencies& dependencies)
    {
        if ( root.HasMember("prototype") ) {
            dependencies.add_dependency<prefab_asset>(
                path::combine(parent_address, root["prototype"].GetString()));
        }

        if ( root.HasMember("components") ) {
            const rapidjson::Value& components_root = root["components"];
            for ( rapidjson::Value::ConstMemberIterator component_root = components_root.MemberBegin();
                component_root != components_root.MemberEnd();
                ++component_root )
            {
                component_loader<>::collect_context ctx(
                    parent_address,
                    component_root->value);
                the<component_factory>().collect_dependencies(
                    component_root->name.GetString(),
                    dependencies,
                    ctx);
            }
        }

        if ( root.HasMember("children") ) {
            const rapidjson::Value& children_root = root["children"];
            for ( rapidjson::SizeType i = 0; i < children_root.Size(); ++i ) {
                collect_dependencies(parent_address, children_root[i], dependencies);
            }
        }
    }

    stdex::promise<asset_group> collect_dependencies(
        const library& library,
        str_view parent_address,
        const rapidjson::Value& root)
    {
        asset_dependencies dependencies;
        collect_dependencies(parent_address, root, dependencies);
        return dependencies.load_async(library);
    }

    prefab parse_prefab(
        str_view parent_address,
        const rapidjson::Value& root,
        const asset_group& dependencies)
    {
        prefab content;

        if ( root.HasMember("prototype") ) {
            auto proto_res = dependencies.find_asset<prefab_asset>(
                path::combine(parent_address, root["prototype"].GetString()));
            if ( !proto_res ) {
                throw prefab_asset_loading_exception();
            }
            content = proto_res->content();
        }

        if ( root.HasMember("components") ) {
            const rapidjson::Value& components_root = root["components"];
            for ( rapidjson::Value::ConstMemberIterator component_root = components_root.MemberBegin();
                component_root != components_root.MemberEnd();
                ++component_root )
            {
                component_loader<>::fill_context ctx(
                    parent_address,
                    component_root->value,
                    dependencies);
                the<component_factory>().fill_prototype(
                    component_root->name.GetString(),
                    content.prototype(),
                    ctx);
            }
        }

        if ( root.HasMember("children") ) {
            const rapidjson::Value& children_root = root["children"];

            vector<prefab> children;
            children.reserve(children_root.Size());

            for ( rapidjson::SizeType i = 0; i < children_root.Size(); ++i ) {
                children.emplace_back(parse_prefab(
                    parent_address, children_root[i], dependencies));
            }

            content.set_children(std::move(children));
        }

        return content;
    }
}

namespace e2d
{
    prefab_asset::load_async_result prefab_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& prefab_data){
            return the<deferrer>().do_in_worker_thread([prefab_data](){
                const rapidjson::Document& doc = prefab_data->content();
                rapidjson::SchemaValidator validator(prefab_asset_schema());
                if ( !doc.Accept(validator) ) {
                    throw prefab_asset_loading_exception();
                }
            })
            .then([&library, parent_address, prefab_data](){
                return collect_dependencies(
                    library, parent_address, prefab_data->content());
            })
            .then([parent_address, prefab_data](const asset_group& dependencies){
                return prefab_asset::create(parse_prefab(
                    parent_address, prefab_data->content(), dependencies));
            });
        });
    }
}
