/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/assets/toggle_style_asset.hpp>

#include <enduro2d/high/assets/json_asset.hpp>
#include <enduro2d/high/assets/atlas_asset.hpp>
#include <enduro2d/high/assets/sprite_asset.hpp>

namespace
{
    using namespace e2d;

    class toggle_style_asset_loading_exception final : public asset_loading_exception {
        const char* what() const noexcept final {
            return "toggle style asset loading exception";
        }
    };

    const char* toggle_style_asset_schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "normal_tint" : { "$ref": "#/common_definitions/color" },
            "normal_pushing_tint" : { "$ref": "#/common_definitions/color" },
            "normal_hovering_tint" : { "$ref": "#/common_definitions/color" },
            "normal_disabled_tint" : { "$ref": "#/common_definitions/color" },

            "pressed_tint" : { "$ref": "#/common_definitions/color" },
            "pressed_pushing_tint" : { "$ref": "#/common_definitions/color" },
            "pressed_hovering_tint" : { "$ref": "#/common_definitions/color" },
            "pressed_disabled_tint" : { "$ref": "#/common_definitions/color" },

            "normal_atlas" : { "$ref": "#/common_definitions/address" },
            "normal_pushing_atlas" : { "$ref": "#/common_definitions/address" },
            "normal_hovering_atlas" : { "$ref": "#/common_definitions/address" },
            "normal_disabled_atlas" : { "$ref": "#/common_definitions/address" },

            "pressed_atlas" : { "$ref": "#/common_definitions/address" },
            "pressed_pushing_atlas" : { "$ref": "#/common_definitions/address" },
            "pressed_hovering_atlas" : { "$ref": "#/common_definitions/address" },
            "pressed_disabled_atlas" : { "$ref": "#/common_definitions/address" },

            "normal_sprite" : { "$ref": "#/common_definitions/address" },
            "normal_pushing_sprite" : { "$ref": "#/common_definitions/address" },
            "normal_hovering_sprite" : { "$ref": "#/common_definitions/address" },
            "normal_disabled_sprite" : { "$ref": "#/common_definitions/address" },

            "pressed_sprite" : { "$ref": "#/common_definitions/address" },
            "pressed_pushing_sprite" : { "$ref": "#/common_definitions/address" },
            "pressed_hovering_sprite" : { "$ref": "#/common_definitions/address" },
            "pressed_disabled_sprite" : { "$ref": "#/common_definitions/address" }
        }
    })json";

    const rapidjson::SchemaDocument& toggle_style_asset_schema() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::SchemaDocument> schema;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(toggle_style_asset_schema_source).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse toggle style asset schema");
                throw toggle_style_asset_loading_exception();
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
        // "normal_pushing_atlas" : { "$ref": "#/common_definitions/address" },
        // "normal_hovering_atlas" : { "$ref": "#/common_definitions/address" },
        // "normal_disabled_atlas" : { "$ref": "#/common_definitions/address" },

        if ( root.HasMember("normal_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_atlas"].GetString()));
        }

        if ( root.HasMember("normal_pushing_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_pushing_atlas"].GetString()));
        }

        if ( root.HasMember("normal_hovering_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_hovering_atlas"].GetString()));
        }

        if ( root.HasMember("normal_disabled_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_disabled_atlas"].GetString()));
        }

        // "pressed_atlas" : { "$ref": "#/common_definitions/address" },
        // "pressed_pushing_atlas" : { "$ref": "#/common_definitions/address" },
        // "pressed_hovering_atlas" : { "$ref": "#/common_definitions/address" },
        // "pressed_disabled_atlas" : { "$ref": "#/common_definitions/address" },

        if ( root.HasMember("pressed_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pressed_atlas"].GetString()));
        }

        if ( root.HasMember("pressed_pushing_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pressed_pushing_atlas"].GetString()));
        }

        if ( root.HasMember("pressed_hovering_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pressed_hovering_atlas"].GetString()));
        }

        if ( root.HasMember("pressed_disabled_atlas") ) {
            dependencies.add_dependency<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pressed_disabled_atlas"].GetString()));
        }

        // "normal_sprite" : { "$ref": "#/common_definitions/address" },
        // "normal_pushing_sprite" : { "$ref": "#/common_definitions/address" },
        // "normal_hovering_sprite" : { "$ref": "#/common_definitions/address" },
        // "normal_disabled_sprite" : { "$ref": "#/common_definitions/address" },

        if ( root.HasMember("normal_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["normal_sprite"].GetString()));
        }

        if ( root.HasMember("normal_pushing_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["normal_pushing_sprite"].GetString()));
        }

        if ( root.HasMember("normal_hovering_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["normal_hovering_sprite"].GetString()));
        }

        if ( root.HasMember("normal_disabled_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["normal_disabled_sprite"].GetString()));
        }

        // "pressed_sprite" : { "$ref": "#/common_definitions/address" },
        // "pressed_pushing_sprite" : { "$ref": "#/common_definitions/address" },
        // "pressed_hovering_sprite" : { "$ref": "#/common_definitions/address" },
        // "pressed_disabled_sprite" : { "$ref": "#/common_definitions/address" }

        if ( root.HasMember("pressed_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["pressed_sprite"].GetString()));
        }

        if ( root.HasMember("pressed_pushing_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["pressed_pushing_sprite"].GetString()));
        }

        if ( root.HasMember("pressed_hovering_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["pressed_hovering_sprite"].GetString()));
        }

        if ( root.HasMember("pressed_disabled_sprite") ) {
            dependencies.add_dependency<sprite_asset>(
                path::combine(parent_address, root["pressed_disabled_sprite"].GetString()));
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

    toggle_style parse_toggle_style(
        str_view parent_address,
        const rapidjson::Value& root,
        const asset_group& dependencies)
    {
        toggle_style style;

        // "normal_tint" : { "$ref": "#/common_definitions/color" },
        // "normal_pushing_tint" : { "$ref": "#/common_definitions/color" },
        // "normal_hovering_tint" : { "$ref": "#/common_definitions/color" },
        // "normal_disabled_tint" : { "$ref": "#/common_definitions/color" },

        if ( root.HasMember("normal_tint") ) {
            color32 normal_tint = style.normal_tint();
            if ( !json_utils::try_parse_value(root["normal_tint"], normal_tint) ) {
                the<debug>().error("TOGGLE_STYLE: Incorrect formatting of 'normal_tint' property");
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_tint(normal_tint);
        }

        if ( root.HasMember("normal_pushing_tint") ) {
            color32 normal_pushing_tint = style.normal_pushing_tint();
            if ( !json_utils::try_parse_value(root["normal_pushing_tint"], normal_pushing_tint) ) {
                the<debug>().error("TOGGLE_STYLE: Incorrect formatting of 'normal_pushing_tint' property");
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_pushing_tint(normal_pushing_tint);
        }

        if ( root.HasMember("normal_hovering_tint") ) {
            color32 normal_hovering_tint = style.normal_hovering_tint();
            if ( !json_utils::try_parse_value(root["normal_hovering_tint"], normal_hovering_tint) ) {
                the<debug>().error("TOGGLE_STYLE: Incorrect formatting of 'normal_hovering_tint' property");
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_hovering_tint(normal_hovering_tint);
        }

        if ( root.HasMember("normal_disabled_tint") ) {
            color32 normal_disabled_tint = style.normal_disabled_tint();
            if ( !json_utils::try_parse_value(root["normal_disabled_tint"], normal_disabled_tint) ) {
                the<debug>().error("TOGGLE_STYLE: Incorrect formatting of 'normal_disabled_tint' property");
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_disabled_tint(normal_disabled_tint);
        }

        // "pressed_tint" : { "$ref": "#/common_definitions/color" },
        // "pressed_pushing_tint" : { "$ref": "#/common_definitions/color" },
        // "pressed_hovering_tint" : { "$ref": "#/common_definitions/color" },
        // "pressed_disabled_tint" : { "$ref": "#/common_definitions/color" },

        if ( root.HasMember("pressed_tint") ) {
            color32 pressed_tint = style.pressed_tint();
            if ( !json_utils::try_parse_value(root["pressed_tint"], pressed_tint) ) {
                the<debug>().error("TOGGLE_STYLE: Incorrect formatting of 'pressed_tint' property");
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_tint(pressed_tint);
        }

        if ( root.HasMember("pressed_pushing_tint") ) {
            color32 pressed_pushing_tint = style.pressed_pushing_tint();
            if ( !json_utils::try_parse_value(root["pressed_pushing_tint"], pressed_pushing_tint) ) {
                the<debug>().error("TOGGLE_STYLE: Incorrect formatting of 'pressed_pushing_tint' property");
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_pushing_tint(pressed_pushing_tint);
        }

        if ( root.HasMember("pressed_hovering_tint") ) {
            color32 pressed_hovering_tint = style.pressed_hovering_tint();
            if ( !json_utils::try_parse_value(root["pressed_hovering_tint"], pressed_hovering_tint) ) {
                the<debug>().error("TOGGLE_STYLE: Incorrect formatting of 'pressed_hovering_tint' property");
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_hovering_tint(pressed_hovering_tint);
        }

        if ( root.HasMember("pressed_disabled_tint") ) {
            color32 pressed_disabled_tint = style.pressed_disabled_tint();
            if ( !json_utils::try_parse_value(root["pressed_disabled_tint"], pressed_disabled_tint) ) {
                the<debug>().error("TOGGLE_STYLE: Incorrect formatting of 'pressed_disabled_tint' property");
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_disabled_tint(pressed_disabled_tint);
        }

        // "normal_atlas" : { "$ref": "#/common_definitions/address" },
        // "normal_pushing_atlas" : { "$ref": "#/common_definitions/address" },
        // "normal_hovering_atlas" : { "$ref": "#/common_definitions/address" },
        // "normal_disabled_atlas" : { "$ref": "#/common_definitions/address" },

        if ( root.HasMember("normal_atlas") ) {
            auto normal_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_atlas"].GetString()));
            if ( !normal_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'normal_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_atlas"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_sprite(normal_sprite);
        }

        if ( root.HasMember("normal_pushing_atlas") ) {
            auto normal_pushing_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_pushing_atlas"].GetString()));
            if ( !normal_pushing_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'normal_pushing_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_pushing_atlas"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_pushing_sprite(normal_pushing_sprite);
        }

        if ( root.HasMember("normal_hovering_atlas") ) {
            auto normal_hovering_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_hovering_atlas"].GetString()));
            if ( !normal_hovering_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'normal_hovering_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_hovering_atlas"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_hovering_sprite(normal_hovering_sprite);
        }

        if ( root.HasMember("normal_disabled_atlas") ) {
            auto normal_disabled_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["normal_disabled_atlas"].GetString()));
            if ( !normal_disabled_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'normal_disabled_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_disabled_atlas"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_disabled_sprite(normal_disabled_sprite);
        }

        // "pressed_atlas" : { "$ref": "#/common_definitions/address" },
        // "pressed_pushing_atlas" : { "$ref": "#/common_definitions/address" },
        // "pressed_hovering_atlas" : { "$ref": "#/common_definitions/address" },
        // "pressed_disabled_atlas" : { "$ref": "#/common_definitions/address" },

        if ( root.HasMember("pressed_atlas") ) {
            auto pressed_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pressed_atlas"].GetString()));
            if ( !pressed_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'pressed_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pressed_atlas"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_sprite(pressed_sprite);
        }

        if ( root.HasMember("pressed_pushing_atlas") ) {
            auto pressed_pushing_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pressed_pushing_atlas"].GetString()));
            if ( !pressed_pushing_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'pressed_pushing_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pressed_pushing_atlas"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_pushing_sprite(pressed_pushing_sprite);
        }

        if ( root.HasMember("pressed_hovering_atlas") ) {
            auto pressed_hovering_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pressed_hovering_atlas"].GetString()));
            if ( !pressed_hovering_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'pressed_hovering_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pressed_hovering_atlas"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_hovering_sprite(pressed_hovering_sprite);
        }

        if ( root.HasMember("pressed_disabled_atlas") ) {
            auto pressed_disabled_sprite = dependencies.find_asset<atlas_asset, sprite_asset>(
                path::combine(parent_address, root["pressed_disabled_atlas"].GetString()));
            if ( !pressed_disabled_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'pressed_disabled_atlas' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pressed_disabled_atlas"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_disabled_sprite(pressed_disabled_sprite);
        }

        // "normal_sprite" : { "$ref": "#/common_definitions/address" },
        // "normal_pushing_sprite" : { "$ref": "#/common_definitions/address" },
        // "normal_hovering_sprite" : { "$ref": "#/common_definitions/address" },
        // "normal_disabled_sprite" : { "$ref": "#/common_definitions/address" },

        if ( root.HasMember("normal_sprite") ) {
            auto normal_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["normal_sprite"].GetString()));
            if ( !normal_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'normal_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_sprite"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_sprite(normal_sprite);
        }

        if ( root.HasMember("normal_pushing_sprite") ) {
            auto normal_pushing_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["normal_pushing_sprite"].GetString()));
            if ( !normal_pushing_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'normal_pushing_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_pushing_sprite"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_pushing_sprite(normal_pushing_sprite);
        }

        if ( root.HasMember("normal_hovering_sprite") ) {
            auto normal_hovering_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["normal_hovering_sprite"].GetString()));
            if ( !normal_hovering_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'normal_hovering_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_hovering_sprite"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_hovering_sprite(normal_hovering_sprite);
        }

        if ( root.HasMember("normal_disabled_sprite") ) {
            auto normal_disabled_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["normal_disabled_sprite"].GetString()));
            if ( !normal_disabled_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'normal_disabled_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["normal_disabled_sprite"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_normal_disabled_sprite(normal_disabled_sprite);
        }

        // "pressed_sprite" : { "$ref": "#/common_definitions/address" },
        // "pressed_pushing_sprite" : { "$ref": "#/common_definitions/address" },
        // "pressed_hovering_sprite" : { "$ref": "#/common_definitions/address" },
        // "pressed_disabled_sprite" : { "$ref": "#/common_definitions/address" }

        if ( root.HasMember("pressed_sprite") ) {
            auto pressed_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["pressed_sprite"].GetString()));
            if ( !pressed_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'pressed_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pressed_sprite"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_sprite(pressed_sprite);
        }

        if ( root.HasMember("pressed_pushing_sprite") ) {
            auto pressed_pushing_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["pressed_pushing_sprite"].GetString()));
            if ( !pressed_pushing_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'pressed_pushing_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pressed_pushing_sprite"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_pushing_sprite(pressed_pushing_sprite);
        }

        if ( root.HasMember("pressed_hovering_sprite") ) {
            auto pressed_hovering_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["pressed_hovering_sprite"].GetString()));
            if ( !pressed_hovering_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'pressed_hovering_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pressed_hovering_sprite"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_hovering_sprite(pressed_hovering_sprite);
        }

        if ( root.HasMember("pressed_disabled_sprite") ) {
            auto pressed_disabled_sprite = dependencies.find_asset<sprite_asset>(
                path::combine(parent_address, root["pressed_disabled_sprite"].GetString()));
            if ( !pressed_disabled_sprite ) {
                the<debug>().error("TOGGLE_STYLE: Dependency 'pressed_disabled_sprite' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    parent_address,
                    root["pressed_disabled_sprite"].GetString());
                throw toggle_style_asset_loading_exception();
            }
            style.set_pressed_disabled_sprite(pressed_disabled_sprite);
        }

        return style;
    }
}

namespace e2d
{
    toggle_style_asset::load_async_result toggle_style_asset::load_async(
        const library& library, str_view address)
    {
        return library.load_asset_async<json_asset>(address)
        .then([
            &library,
            address = str(address),
            parent_address = path::parent_path(address)
        ](const json_asset::load_result& toggle_style_data){
            return the<deferrer>().do_in_worker_thread([address, toggle_style_data](){
                const rapidjson::Document& doc = *toggle_style_data->content();
                rapidjson::SchemaValidator validator(toggle_style_asset_schema());

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

                throw toggle_style_asset_loading_exception();
            })
            .then([&library, parent_address, toggle_style_data](){
                return collect_dependencies(
                    library, parent_address, *toggle_style_data->content());
            })
            .then([parent_address, toggle_style_data](const asset_group& dependencies){
                return toggle_style_asset::create(parse_toggle_style(
                    parent_address, *toggle_style_data->content(), dependencies));
            });
        });
    }
}
