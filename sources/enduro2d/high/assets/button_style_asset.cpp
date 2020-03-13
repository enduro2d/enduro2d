/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/button_style_asset.hpp>

#include <enduro2d/high/assets/json_asset.hpp>
#include <enduro2d/high/assets/atlas_asset.hpp>
#include <enduro2d/high/assets/sprite_asset.hpp>

namespace
{
    using namespace e2d;

    class button_style_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "button style asset loading exception";
        }
    };

    const char* button_style_asset_schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "normal_tint" : { "$ref": "#/common_definitions/color" },
            "pushing_tint" : { "$ref": "#/common_definitions/color" },
            "hovering_tint" : { "$ref": "#/common_definitions/color" },
            "disabled_tint" : { "$ref": "#/common_definitions/color" },

            "normal_atlas" : { "$ref": "#/common_definitions/address" },
            "pushing_atlas" : { "$ref": "#/common_definitions/address" },
            "hovering_atlas" : { "$ref": "#/common_definitions/address" },
            "disabled_atlas" : { "$ref": "#/common_definitions/address" },

            "normal_sprite" : { "$ref": "#/common_definitions/address" },
            "pushing_sprite" : { "$ref": "#/common_definitions/address" },
            "hovering_sprite" : { "$ref": "#/common_definitions/address" },
            "disabled_sprite" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    const rapidjson::SchemaDocument& button_style_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(button_style_asset_schema_source).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse button style asset schema");
                throw button_style_asset_loading_exception();
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
        // "normal_atlas" : { "$ref": "#/common_definitions/address" },
        // "pushing_atlas" : { "$ref": "#/common_definitions/address" },
        // "hovering_atlas" : { "$ref": "#/common_definitions/address" },
        // "disabled_atlas" : { "$ref": "#/common_definitions/address" },

        if ( root.HasMember("normal_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_atlas"].GetString()));
        }

        if ( root.HasMember("pushing_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pushing_atlas"].GetString()));
        }

        if ( root.HasMember("hovering_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["hovering_atlas"].GetString()));
        }

        if ( root.HasMember("disabled_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["disabled_atlas"].GetString()));
        }

        // "normal_sprite" : { "$ref": "#/common_definitions/address" },
        // "pushing_sprite" : { "$ref": "#/common_definitions/address" },
        // "hovering_sprite" : { "$ref": "#/common_definitions/address" },
        // "disabled_sprite" : { "$ref": "#/common_definitions/address" }

        if ( root.HasMember("normal_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["normal_sprite"].GetString()));
        }

        if ( root.HasMember("pushing_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["pushing_sprite"].GetString()));
        }

        if ( root.HasMember("hovering_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["hovering_sprite"].GetString()));
        }

        if ( root.HasMember("disabled_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["disabled_sprite"].GetString()));
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

    button_style parse_button_style(
        str_view parent_address,
        const rapidjson::Value& root,
        const asset_group& dependencies)
    {
        button_style style;

        // "normal_tint" : { "$ref": "#/common_definitions/color" },
        // "pushing_tint" : { "$ref": "#/common_definitions/color" },
        // "hovering_tint" : { "$ref": "#/common_definitions/color" },
        // "disabled_tint" : { "$ref": "#/common_definitions/color" },

        if ( root.HasMember("normal_tint") ) {
            color32 normal_tint = style.normal_tint();
            if ( !json_utils::try_parse_value(root["normal_tint"], normal_tint) ) {
                the<debug>().error("BUTTON_STYLE: Incorrect formatting of 'normal_tint' property");
                throw button_style_asset_loading_exception();
            }
            style.set_normal_tint(normal_tint);
        }

        if ( root.HasMember("pushing_tint") ) {
            color32 pushing_tint = style.pushing_tint();
            if ( !json_utils::try_parse_value(root["pushing_tint"], pushing_tint) ) {
                the<debug>().error("BUTTON_STYLE: Incorrect formatting of 'pushing_tint' property");
                throw button_style_asset_loading_exception();
            }
            style.set_pushing_tint(pushing_tint);
        }

        if ( root.HasMember("hovering_tint") ) {
            color32 hovering_tint = style.hovering_tint();
            if ( !json_utils::try_parse_value(root["hovering_tint"], hovering_tint) ) {
                the<debug>().error("BUTTON_STYLE: Incorrect formatting of 'hovering_tint' property");
                throw button_style_asset_loading_exception();
            }
            style.set_hovering_tint(hovering_tint);
        }

        if ( root.HasMember("disabled_tint") ) {
            color32 disabled_tint = style.disabled_tint();
            if ( !json_utils::try_parse_value(root["disabled_tint"], disabled_tint) ) {
                the<debug>().error("BUTTON_STYLE: Incorrect formatting of 'disabled_tint' property");
                throw button_style_asset_loading_exception();
            }
            style.set_disabled_tint(disabled_tint);
        }

        // "normal_atlas" : { "$ref": "#/common_definitions/address" },
        // "pushing_atlas" : { "$ref": "#/common_definitions/address" },
        // "hovering_atlas" : { "$ref": "#/common_definitions/address" },
        // "disabled_atlas" : { "$ref": "#/common_definitions/address" },

        if ( root.HasMember("normal_atlas") ) {
            auto normal_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_atlas"].GetString()));
            if ( !normal_sprite ) {
                the<debug>().error("BUTTON_STYLE: Dependency 'normal_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_atlas"].GetString());
                throw button_style_asset_loading_exception();
            }
            style.set_normal_sprite(normal_sprite);
        }

        if ( root.HasMember("pushing_atlas") ) {
            auto pushing_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pushing_atlas"].GetString()));
            if ( !pushing_sprite ) {
                the<debug>().error("BUTTON_STYLE: Dependency 'pushing_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pushing_atlas"].GetString());
                throw button_style_asset_loading_exception();
            }
            style.set_pushing_sprite(pushing_sprite);
        }

        if ( root.HasMember("hovering_atlas") ) {
            auto hovering_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["hovering_atlas"].GetString()));
            if ( !hovering_sprite ) {
                the<debug>().error("BUTTON_STYLE: Dependency 'hovering_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["hovering_atlas"].GetString());
                throw button_style_asset_loading_exception();
            }
            style.set_hovering_sprite(hovering_sprite);
        }

        if ( root.HasMember("disabled_atlas") ) {
            auto disabled_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["disabled_atlas"].GetString()));
            if ( !disabled_sprite ) {
                the<debug>().error("BUTTON_STYLE: Dependency 'disabled_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["disabled_atlas"].GetString());
                throw button_style_asset_loading_exception();
            }
            style.set_disabled_sprite(disabled_sprite);
        }

        // "normal_sprite" : { "$ref": "#/common_definitions/address" },
        // "pushing_sprite" : { "$ref": "#/common_definitions/address" },
        // "hovering_sprite" : { "$ref": "#/common_definitions/address" },
        // "disabled_sprite" : { "$ref": "#/common_definitions/address" }

        if ( root.HasMember("normal_sprite") ) {
            auto normal_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["normal_sprite"].GetString()));
            if ( !normal_sprite ) {
                the<debug>().error("BUTTON_STYLE: Dependency 'normal_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_sprite"].GetString());
                throw button_style_asset_loading_exception();
            }
            style.set_normal_sprite(normal_sprite);
        }

        if ( root.HasMember("pushing_sprite") ) {
            auto pushing_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["pushing_sprite"].GetString()));
            if ( !pushing_sprite ) {
                the<debug>().error("BUTTON_STYLE: Dependency 'pushing_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pushing_sprite"].GetString());
                throw button_style_asset_loading_exception();
            }
            style.set_pushing_sprite(pushing_sprite);
        }

        if ( root.HasMember("hovering_sprite") ) {
            auto hovering_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["hovering_sprite"].GetString()));
            if ( !hovering_sprite ) {
                the<debug>().error("BUTTON_STYLE: Dependency 'hovering_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["hovering_sprite"].GetString());
                throw button_style_asset_loading_exception();
            }
            style.set_hovering_sprite(hovering_sprite);
        }

        if ( root.HasMember("disabled_sprite") ) {
            auto disabled_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["disabled_sprite"].GetString()));
            if ( !disabled_sprite ) {
                the<debug>().error("BUTTON_STYLE: Dependency 'disabled_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["disabled_sprite"].GetString());
                throw button_style_asset_loading_exception();
            }
            style.set_disabled_sprite(disabled_sprite);
        }

        return style;
    }
}

namespace e2d
{
    button_style_asset::load_async_result button_style_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            address = str(address),
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& button_style_data){
            return the<deferrer>().do_in_worker_thread([address, button_style_data](){
                const rapidjson::Document& doc = *button_style_data->content();
                rapidjson::SchemaValidator validator(button_style_asset_schema());

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

                throw button_style_asset_loading_exception();
            })
            .then([&library, parent_address, button_style_data](){
                return collect_dependencies(
                    library, parent_address, *button_style_data->content());
            })
            .then([parent_address, button_style_data](const asset_group& dependencies){
                return button_style_asset::create(parse_button_style(
                    parent_address, *button_style_data->content(), dependencies));
            });
        });
    }
}
