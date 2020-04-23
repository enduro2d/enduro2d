/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/prefab_asset.hpp>

#include <enduro2d/high/factory.hpp>
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
            "uuid" : { "$ref": "#/common_definitions/uuid" },
            "prefab" : { "$ref": "#/common_definitions/address" },
            "children" : { "$ref": "#/definitions/children" },
            "mod_children" : { "$ref": "#/definitions/mod_children" },
            "components" : { "type" : "object" }
        },
        "definitions" : {
            "children" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/child" }
            },
            "child" : {
                "type" : "object",
                "required" : [],
                "additionalProperties" : false,
                "properties" : {
                    "uuid" : { "$ref": "#/common_definitions/uuid" },
                    "prefab" : { "$ref": "#/common_definitions/address" },
                    "children" : { "$ref": "#/definitions/children" },
                    "mod_children" : { "$ref": "#/definitions/mod_children" },
                    "components" : { "type" : "object" }
                }
            },
            "mod_children" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/mod_child" }
            },
            "mod_child" : {
                "type" : "object",
                "required" : [ "uuid" ],
                "additionalProperties" : false,
                "properties" : {
                    "uuid" : { "$ref": "#/common_definitions/uuid" },
                    "children" : { "$ref": "#/definitions/children" },
                    "mod_children" : { "$ref": "#/definitions/mod_children" },
                    "components" : { "type" : "object" }
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
        if ( root.HasMember("prefab") ) {
            dependencies.add_dependency<prefab_asset>(
                path::combine(parent_address, root["prefab"].GetString()));
        }

        if ( root.HasMember("children") ) {
            const rapidjson::Value& children_root = root["children"];
            for ( rapidjson::SizeType i = 0; i < children_root.Size(); ++i ) {
                collect_dependencies(parent_address, children_root[i], dependencies);
            }
        }

        if ( root.HasMember("mod_children") ) {
            const rapidjson::Value& mod_children_root = root["mod_children"];
            for ( rapidjson::SizeType i = 0; i < mod_children_root.Size(); ++i ) {
                collect_dependencies(parent_address, mod_children_root[i], dependencies);
            }
        }

        if ( root.HasMember("components") ) {
            const rapidjson::Value& components_root = root["components"];
            for ( rapidjson::Value::ConstMemberIterator component_root = components_root.MemberBegin();
                component_root != components_root.MemberEnd();
                ++component_root )
            {
                {
                    const bool success = the<factory>().validate_json(
                        component_root->name.GetString(),
                        component_root->value);

                    if ( !success ) {
                        throw prefab_asset_loading_exception();
                    }
                }
                {
                    factory_loader<>::collect_context ctx(
                        str(parent_address),
                        component_root->value);

                    const bool success = the<factory>().collect_dependencies(
                        component_root->name.GetString(),
                        dependencies,
                        ctx);

                    if ( !success ) {
                        throw prefab_asset_loading_exception();
                    }
                }
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

    prefab* find_prefab_child(prefab& root, str_view child_uuid) noexcept {
        for ( prefab& child : root.children() ) {
            if ( child.uuid() == child_uuid ) {
                return &child;
            }
        }

        for ( prefab& child : root.children() ) {
            if ( prefab* sub_child = find_prefab_child(child, child_uuid) ) {
                return sub_child;
            }
        }

        return nullptr;
    }

    void parse_prefab_inplace(
        prefab& content,
        str_view parent_address,
        const rapidjson::Value& root,
        const asset_group& dependencies)
    {
        if ( root.HasMember("uuid") ) {
            str uuid = root["uuid"].GetString();
            content.set_uuid(std::move(uuid));
        }

        if ( root.HasMember("prefab") ) {
            auto proto_res = dependencies.find_asset<prefab_asset>(
                path::combine(parent_address, root["prefab"].GetString()));
            if ( !proto_res ) {
                the<debug>().error("PREFAB: Dependency 'prefab' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["prefab"].GetString());
                throw prefab_asset_loading_exception();
            }
            content.set_children(proto_res->content().children());
            content.set_prototype(proto_res->content().prototype());
        }

        if ( root.HasMember("children") ) {
            const rapidjson::Value& children_root = root["children"];

            for ( rapidjson::SizeType i = 0; i < children_root.Size(); ++i ) {
                const rapidjson::Value& child_root = children_root[i];

                prefab child;
                parse_prefab_inplace(
                    child,
                    parent_address,
                    child_root,
                    dependencies);

                content.children().push_back(std::move(child));
            }
        }

        if ( root.HasMember("mod_children") ) {
            const rapidjson::Value& mod_children_root = root["mod_children"];

            for ( rapidjson::SizeType i = 0; i < mod_children_root.Size(); ++i ) {
                const rapidjson::Value& mod_child_root = mod_children_root[i];

                E2D_ASSERT(
                    mod_child_root.HasMember("uuid") &&
                    mod_child_root["uuid"].IsString());

                prefab* const child_prefab = find_prefab_child(
                    content,
                    mod_child_root["uuid"].GetString());

                if ( !child_prefab ) {
                    the<debug>().error("PREFAB: Modifiable child is not found:\n"
                        "--> Child UUID: %0",
                        mod_child_root["uuid"].GetString());
                    throw prefab_asset_loading_exception();
                }

                parse_prefab_inplace(
                    *child_prefab,
                    parent_address,
                    mod_child_root,
                    dependencies);
            }
        }

        if ( root.HasMember("components") ) {
            const rapidjson::Value& components_root = root["components"];
            for ( rapidjson::Value::ConstMemberIterator component_root = components_root.MemberBegin();
                component_root != components_root.MemberEnd();
                ++component_root )
            {
                factory_loader<>::fill_context ctx(
                    str(parent_address),
                    component_root->value,
                    dependencies);

                const bool success = the<factory>().fill_prototype(
                    component_root->name.GetString(),
                    content.prototype(),
                    ctx);

                if ( !success ) {
                    throw prefab_asset_loading_exception();
                }
            }
        }
    }

    prefab parse_prefab(
        str_view parent_address,
        const rapidjson::Value& root,
        const asset_group& dependencies)
    {
        prefab content;
        parse_prefab_inplace(
            content,
            parent_address,
            root, dependencies);
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
            address = str(address),
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& prefab_data){
            return the<deferrer>().do_in_worker_thread([address, prefab_data](){
                const rapidjson::Document& doc = *prefab_data->content();
                rapidjson::SchemaValidator validator(prefab_asset_schema());

                if ( doc.Accept(validator) ) {
                    return;
                }

                rapidjson::StringBuffer sb;
                if ( validator.GetInvalidDocumentPointer().StringifyUriFragment(sb) ) {
                    the<debug>().error("ASSET: Failed to validate asset json:\n"
                        "--> Address: %0\n"
                        "--> Invalid schema keyword: %1\n"
                        "--> Invalid document pointer: %2",
                        address,
                        validator.GetInvalidSchemaKeyword(),
                        sb.GetString());
                } else {
                    the<debug>().error("ASSET: Failed to validate asset json");
                }

                throw prefab_asset_loading_exception();
            })
            .then([&library, parent_address, prefab_data](){
                return collect_dependencies(
                    library, parent_address, *prefab_data->content());
            })
            .then([parent_address, prefab_data](const asset_group& dependencies){
                return prefab_asset::create(parse_prefab(
                    parent_address, *prefab_data->content(), dependencies));
            });
        });
    }
}
