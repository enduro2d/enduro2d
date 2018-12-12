/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/assets.hpp>

#include <3rdparty/rapidjson/schema.h>
#include <3rdparty/rapidjson/document.h>

namespace
{
    using namespace e2d;

    template < typename T >
    const char* asset_schema_source() noexcept;

    template <>
    const char* asset_schema_source<shader_asset>() noexcept {
        return R"json({
            "type" : "object",
            "required" : [ "vertex", "fragment" ],
            "additionalProperties" : false,
            "properties" : {
                "vertex" : { "$ref": "#/definitions/generic_address" },
                "fragment" : { "$ref": "#/definitions/generic_address" }
            },
            "definitions" : {
                "generic_address" : {
                    "type" : "string",
                    "minLength" : 1
                }
            }
        })json";
    }

    template <>
    const char* asset_schema_source<material_asset>() noexcept {
        return R"json({
            "type" : "object",
            "additionalProperties" : false,
            "properties" : {
                "passes" : {
                    "type" : "array",
                    "items" : { "$ref": "#/definitions/pass_state" }
                },
                "property_block" : { "$ref": "#/definitions/property_block" }
            },
            "definitions" : {
                "pass_state" : {
                    "type" : "object",
                    "required" : [ "shader" ],
                    "additionalProperties" : false,
                    "properties" : {
                        "shader" : { "$ref": "#/definitions/generic_address" },
                        "state_block" : { "$ref": "#/definitions/state_block" },
                        "property_block" : { "$ref": "#/definitions/property_block" }
                    }
                },
                "state_block" : {
                    "type" : "object",
                    "additionalProperties" : false,
                    "properties" : {
                        "depth_state" : { "$ref": "#/definitions/depth_state" },
                        "stencil_state" : { "$ref": "#/definitions/stencil_state" },
                        "culling_state" : { "$ref": "#/definitions/culling_state" },
                        "blending_state" : { "$ref": "#/definitions/blending_state" },
                        "capabilities_state" : { "$ref": "#/definitions/capabilities_state" }
                    }
                },
                "depth_state" : {
                    "type" : "object",
                    "additionalProperties" : false,
                    "properties" : {
                        "range" : {
                            "type" : "object",
                            "additionalProperties" : false,
                            "properties" : {
                                "near" : { "type" : "number" },
                                "far" : { "type" : "number" }
                            }
                        },
                        "write" : { "type" : "boolean" },
                        "func" : { "$ref" : "#/definitions/compare_func" }
                    }
                },
                "stencil_state" : {
                    "type" : "object",
                    "additionalProperties" : false,
                    "properties" : {
                        "write" : { "type" : "integer", "minimum" : 0, "maximum": 255 },
                        "func" : { "$ref" : "#/definitions/compare_func" },
                        "ref" : { "type" : "integer", "minimum" : 0, "maximum": 255 },
                        "mask" : { "type" : "integer", "minimum" : 0, "maximum": 255 },
                        "pass" : { "$ref" : "#/definitions/stencil_op" },
                        "sfail" : { "$ref" : "#/definitions/stencil_op" },
                        "zfail" : { "$ref" : "#/definitions/stencil_op" }
                    }
                },
                "culling_state" : {
                    "type" : "object",
                    "additionalProperties" : false,
                    "properties" : {
                        "mode" : { "$ref" : "#/definitions/culling_mode" },
                        "face" : { "$ref" : "#/definitions/culling_face" }
                    }
                },
                "blending_state" : {
                    "type" : "object",
                    "additionalProperties" : false,
                    "properties" : {
                        "constant_color" : {
                            "$ref" : "#/definitions/color"
                        },
                        "color_mask" : {
                            "$ref" : "#/definitions/color_mask"
                        },
                        "src_factor" : {
                            "anyOf" : [{
                                "type" : "object",
                                "additionalProperties" : false,
                                "properties" : {
                                    "rgb" : { "$ref" : "#/definitions/blending_factor" },
                                    "alpha" : { "$ref" : "#/definitions/blending_factor" }
                                }
                            }, {
                                "$ref" : "#/definitions/blending_factor"
                            }]
                        },
                        "dst_factor" : {
                            "anyOf" : [{
                                "type" : "object",
                                "additionalProperties" : false,
                                "properties" : {
                                    "rgb" : { "$ref" : "#/definitions/blending_factor" },
                                    "alpha" : { "$ref" : "#/definitions/blending_factor" }
                                }
                            }, {
                                "$ref" : "#/definitions/blending_factor"
                            }]
                        },
                        "equation" : {
                            "anyOf" : [{
                                "type" : "object",
                                "additionalProperties" : false,
                                "properties" : {
                                    "rgb" : { "$ref" : "#/definitions/blending_equation" },
                                    "alpha" : { "$ref" : "#/definitions/blending_equation" }
                                }
                            }, {
                                "$ref" : "#/definitions/blending_equation"
                            }]
                        }
                    }
                },
                "capabilities_state" : {
                    "type" : "object",
                    "additionalProperties" : false,
                    "properties" : {
                        "culling" : { "type" : "boolean" },
                        "blending" : { "type" : "boolean" },
                        "depth_test" : { "type" : "boolean" },
                        "stencil_test" : { "type" : "boolean" }
                    }
                },
                "property_block" : {
                    "type" : "object",
                    "additionalProperties" : false,
                    "properties" : {
                        "samplers" : {
                            "type" : "array",
                            "items" : { "$ref" : "#/definitions/sampler" }
                        },
                        "properties" : {
                            "type" : "array",
                            "items" : { "$ref" : "#/definitions/property" }
                        }
                    }
                },
                "sampler" : {
                    "type" : "object",
                    "required" : [ "name", "texture" ],
                    "additionalProperties" : false,
                    "properties" : {
                        "name" : {
                            "$ref" : "#/definitions/generic_name"
                        },
                        "texture" : {
                            "$ref" : "#/definitions/generic_address"
                        },
                        "wrap" : {
                            "anyOf" : [{
                                "type" : "object",
                                "additionalProperties" : false,
                                "properties" : {
                                    "s" : { "$ref" : "#/definitions/sampler_wrap" },
                                    "t" : { "$ref" : "#/definitions/sampler_wrap" },
                                    "r" : { "$ref" : "#/definitions/sampler_wrap" }
                                }
                            }, {
                                "$ref" : "#/definitions/sampler_wrap"
                            }]
                        },
                        "filter" : {
                            "anyOf" : [{
                                "type" : "object",
                                "additionalProperties" : false,
                                "properties" : {
                                    "min" : { "$ref" : "#/definitions/sampler_filter" },
                                    "mag" : { "$ref" : "#/definitions/sampler_filter" }
                                }
                            }, {
                                "$ref" : "#/definitions/sampler_filter"
                            }]
                        }
                    }
                },
                "property" : {
                    "type" : "object",
                    "required" : [ "name", "type" ],
                    "additionalProperties" : false,
                    "properties" : {
                        "name" : { "$ref" : "#/definitions/generic_name" },
                        "type" : { "$ref" : "#/definitions/property_type" },
                        "value" : { "$ref" : "#/definitions/property_value" }
                    }
                },
                "property_type" : {
                    "type" : "string",
                    "enum" : [
                        "i32", "f32",
                        "v2i", "v3i", "v4i",
                        "v2f", "v3f", "v4f"
                    ]
                },
                "property_value" : {
                    "anyOf": [
                        { "type" : "number" },
                        { "$ref" : "#/definitions/vector" }
                    ]
                },
                "stencil_op" : {
                    "type" : "string",
                    "enum" : [
                        "keep",
                        "zero",
                        "replace",
                        "incr",
                        "incr_wrap",
                        "decr",
                        "decr_wrap",
                        "invert"
                    ]
                },
                "compare_func" : {
                    "type" : "string",
                    "enum" : [
                        "never",
                        "less",
                        "lequal",
                        "greater",
                        "gequal",
                        "equal",
                        "notequal",
                        "always"
                    ]
                },
                "culling_mode" : {
                    "type" : "string",
                    "enum" : [
                        "cw",
                        "ccw"
                    ]
                },
                "culling_face" : {
                    "type" : "string",
                    "enum" : [
                        "back",
                        "front",
                        "back_and_front"
                    ]
                },
                "blending_factor" : {
                    "type" : "string",
                    "enum" : [
                        "zero",
                        "one",
                        "src_color",
                        "one_minus_src_color",
                        "dst_color",
                        "one_minus_dst_color",
                        "src_alpha",
                        "one_minus_src_alpha",
                        "dst_alpha",
                        "one_minus_dst_alpha",
                        "constant_color",
                        "one_minus_constant_color",
                        "constant_alpha",
                        "one_minus_constant_alpha",
                        "src_alpha_saturate"
                    ]
                },
                "blending_equation" : {
                    "type" : "string",
                    "enum" : [
                        "add",
                        "subtract",
                        "reverse_subtract"
                    ]
                },
                "blending_color_mask" : {
                    "type" : "string",
                    "enum" : [
                        "none",
                        "r",
                        "g",
                        "b",
                        "a",
                        "rg",
                        "rb",
                        "ra",
                        "gb",
                        "ga",
                        "ba",
                        "rgb",
                        "rga",
                        "rba",
                        "gba",
                        "rgba"
                    ]
                },
                "sampler_wrap" : {
                    "type" : "string",
                    "enum" : [
                        "clamp",
                        "repeat",
                        "mirror"
                    ]
                },
                "sampler_filter" : {
                    "type" : "string",
                    "enum" : [
                        "nearest",
                        "linear",
                        "nearest_mipmap_nearest",
                        "linear_mipmap_nearest",
                        "nearest_mipmap_linear",
                        "linear_mipmap_linear"
                    ]
                },
                "color" : {
                    "type" : "object",
                    "additionalProperties" : false,
                    "properties" : {
                        "r" : { "type" : "number" },
                        "g" : { "type" : "number" },
                        "b" : { "type" : "number" },
                        "a" : { "type" : "number" }
                    }
                },
                "vector" : {
                    "anyOf" : [{
                        "type" : "number"
                    }, {
                        "type" : "array",
                        "maxItems" : 4,
                        "items" : { "type" : "number" }
                    }, {
                        "type" : "object",
                        "additionalProperties" : false,
                        "properties" : {
                            "x" : { "type" : "number" },
                            "y" : { "type" : "number" },
                            "z" : { "type" : "number" },
                            "w" : { "type" : "number" }
                        }
                    }]
                },
                "generic_name" : {
                    "type" : "string",
                    "minLength" : 1
                },
                "generic_address" : {
                    "type" : "string",
                    "minLength" : 1
                }
            }
        })json";
    }

    template < typename T >
    const rapidjson::SchemaDocument& asset_file_schema() {
        static std::unique_ptr<rapidjson::SchemaDocument> schema;
        if ( !schema ) {
            rapidjson::Document doc;
            if ( doc.Parse(asset_schema_source<T>()).HasParseError() ) {
                the<debug>().error("ASSETS: Failed to parse asset file schema");
                throw bad_library_operation();
            }
            schema = std::make_unique<rapidjson::SchemaDocument>(doc);
        }
        return *schema;
    }

    template < typename T >
    bool load_json_asset_file(library& library, str_view address, rapidjson::Document& doc) {
        const auto json_data = library.load_asset<text_asset>(address);
        if ( !json_data ) {
            return false;
        }

        if ( doc.Parse(json_data->content().c_str()).HasParseError() ) {
            the<debug>().error("ASSETS: Failed to parse json asset file:\n"
                "--> Address: %0",
                address);
            return false;
        }

        rapidjson::SchemaValidator validator(asset_file_schema<T>());
        if ( !doc.Accept(validator) ) {
            the<debug>().error("ASSETS: Failed to validate json asset file:\n"
                "--> Address: %0",
                address);
            return false;
        }

        return true;
    }

    bool parse_stencil_op(str_view str, render::stencil_op& op) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { op = render::stencil_op::x; return true; }
        DEFINE_IF(keep);
        DEFINE_IF(zero);
        DEFINE_IF(replace);
        DEFINE_IF(incr);
        DEFINE_IF(incr_wrap);
        DEFINE_IF(decr);
        DEFINE_IF(decr_wrap);
        DEFINE_IF(invert);
    #undef DEFINE_IF
        return false;
    }

    bool parse_compare_func(str_view str, render::compare_func& func) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { func = render::compare_func::x; return true; }
        DEFINE_IF(never);
        DEFINE_IF(less);
        DEFINE_IF(lequal);
        DEFINE_IF(greater);
        DEFINE_IF(gequal);
        DEFINE_IF(equal);
        DEFINE_IF(notequal);
        DEFINE_IF(always);
    #undef DEFINE_IF
        return false;
    }

    bool parse_culling_mode(str_view str, render::culling_mode& mode) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { mode = render::culling_mode::x; return true; }
        DEFINE_IF(cw);
        DEFINE_IF(ccw);
    #undef DEFINE_IF
        return false;
    }

    bool parse_culling_face(str_view str, render::culling_face& face) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { face = render::culling_face::x; return true; }
        DEFINE_IF(back);
        DEFINE_IF(front);
        DEFINE_IF(back_and_front);
    #undef DEFINE_IF
        return false;
    }

    bool parse_blending_factor(str_view str, render::blending_factor& factor) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { factor = render::blending_factor::x; return true; }
        DEFINE_IF(zero);
        DEFINE_IF(one);
        DEFINE_IF(src_color);
        DEFINE_IF(one_minus_src_color);
        DEFINE_IF(dst_color);
        DEFINE_IF(one_minus_dst_color);
        DEFINE_IF(src_alpha);
        DEFINE_IF(one_minus_src_alpha);
        DEFINE_IF(dst_alpha);
        DEFINE_IF(one_minus_dst_alpha);
        DEFINE_IF(constant_color);
        DEFINE_IF(one_minus_constant_color);
        DEFINE_IF(constant_alpha);
        DEFINE_IF(one_minus_constant_alpha);
        DEFINE_IF(src_alpha_saturate);
    #undef DEFINE_IF
        return false;
    }

    bool parse_blending_equation(str_view str, render::blending_equation& equation) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { equation = render::blending_equation::x; return true; }
        DEFINE_IF(add);
        DEFINE_IF(subtract);
        DEFINE_IF(reverse_subtract);
    #undef DEFINE_IF
        return false;
    }

    bool parse_blending_color_mask(str_view str, render::blending_color_mask& mask) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { mask = render::blending_color_mask::x; return true; }
        DEFINE_IF(none);
        DEFINE_IF(r);
        DEFINE_IF(g);
        DEFINE_IF(b);
        DEFINE_IF(a);
        DEFINE_IF(rg);
        DEFINE_IF(rb);
        DEFINE_IF(ra);
        DEFINE_IF(gb);
        DEFINE_IF(ga);
        DEFINE_IF(ba);
        DEFINE_IF(rgb);
        DEFINE_IF(rga);
        DEFINE_IF(rba);
        DEFINE_IF(gba);
        DEFINE_IF(rgba);
    #undef DEFINE_IF
        return false;
    }

    bool parse_sampler_wrap(str_view str, render::sampler_wrap& wrap) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { wrap = render::sampler_wrap::x; return true; }
        DEFINE_IF(clamp);
        DEFINE_IF(repeat);
        DEFINE_IF(mirror);
    #undef DEFINE_IF
        return false;
    }

    bool parse_sampler_min_filter(str_view str, render::sampler_min_filter& filter) noexcept {
    #define DEFINE_IF(x) if ( str == #x ) { filter = render::sampler_min_filter::x; return true; }
        DEFINE_IF(nearest);
        DEFINE_IF(linear);
        DEFINE_IF(nearest_mipmap_nearest);
        DEFINE_IF(linear_mipmap_nearest);
        DEFINE_IF(nearest_mipmap_linear);
        DEFINE_IF(linear_mipmap_linear);
    #undef DEFINE_IF
        return false;
    }

    bool parse_sampler_mag_filter(str_view str, render::sampler_mag_filter& filter) noexcept {
    #define DEFINE_IF(x,y) if ( str == #x ) { filter = render::sampler_mag_filter::y; return true; }
        DEFINE_IF(nearest, nearest);
        DEFINE_IF(linear, linear);
        DEFINE_IF(nearest_mipmap_nearest, nearest);
        DEFINE_IF(linear_mipmap_nearest, linear);
        DEFINE_IF(nearest_mipmap_linear, nearest);
        DEFINE_IF(linear_mipmap_linear, linear);
    #undef DEFINE_IF
        return false;
    }

    bool parse_property_value(const rapidjson::Value& root, v4i& v) {
        if ( root.IsNumber() ) {
            v = v4i(root.GetInt());
            return true;
        }

        if ( root.IsObject() ) {
            if ( root.HasMember("x") ) {
                E2D_ASSERT(root["x"].IsNumber());
                v.x = root["x"].GetInt();
            }
            if ( root.HasMember("y") ) {
                E2D_ASSERT(root["y"].IsNumber());
                v.y = root["y"].GetInt();
            }
            if ( root.HasMember("z") ) {
                E2D_ASSERT(root["z"].IsNumber());
                v.z = root["z"].GetInt();
            }
            if ( root.HasMember("w") ) {
                E2D_ASSERT(root["w"].IsNumber());
                v.w = root["w"].GetInt();
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() > 0 ) {
                E2D_ASSERT(root[0].IsNumber());
                v.x = root[0].GetInt();
            }
            if ( root.Size() > 1 ) {
                E2D_ASSERT(root[1].IsNumber());
                v.y = root[1].GetInt();
            }
            if ( root.Size() > 2 ) {
                E2D_ASSERT(root[2].IsNumber());
                v.z = root[2].GetInt();
            }
            if ( root.Size() > 3 ) {
                E2D_ASSERT(root[3].IsNumber());
                v.w = root[3].GetInt();
            }
            return true;
        }

        return false;
    }

    bool parse_property_value(const rapidjson::Value& root, v3i& v) {
        v4i vv;
        if ( !parse_property_value(root, vv) ) {
            return false;
        }
        v = v3i(vv);
        return true;
    }

    bool parse_property_value(const rapidjson::Value& root, v2i& v) {
        v4i vv;
        if ( !parse_property_value(root, vv) ) {
            return false;
        }
        v = v2i(vv);
        return true;
    }

    bool parse_property_value(const rapidjson::Value& root, v4f& v) {
        if ( root.IsNumber() ) {
            v = v4f(root.GetFloat());
            return true;
        }

        if ( root.IsObject() ) {
            if ( root.HasMember("x") ) {
                E2D_ASSERT(root["x"].IsNumber());
                v.x = root["x"].GetFloat();
            }
            if ( root.HasMember("y") ) {
                E2D_ASSERT(root["y"].IsNumber());
                v.y = root["y"].GetFloat();
            }
            if ( root.HasMember("z") ) {
                E2D_ASSERT(root["z"].IsNumber());
                v.z = root["z"].GetFloat();
            }
            if ( root.HasMember("w") ) {
                E2D_ASSERT(root["w"].IsNumber());
                v.w = root["w"].GetFloat();
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() > 0 ) {
                E2D_ASSERT(root[0].IsNumber());
                v.x = root[0].GetFloat();
            }
            if ( root.Size() > 1 ) {
                E2D_ASSERT(root[1].IsNumber());
                v.y = root[1].GetFloat();
            }
            if ( root.Size() > 2 ) {
                E2D_ASSERT(root[2].IsNumber());
                v.z = root[2].GetFloat();
            }
            if ( root.Size() > 3 ) {
                E2D_ASSERT(root[3].IsNumber());
                v.w = root[3].GetFloat();
            }
            return true;
        }

        return false;
    }

    bool parse_property_value(const rapidjson::Value& root, v3f& v) {
        v4f vv;
        if ( !parse_property_value(root, vv) ) {
            return false;
        }
        v = v3f(vv);
        return true;
    }

    bool parse_property_value(const rapidjson::Value& root, v2f& v) {
        v4f vv;
        if ( !parse_property_value(root, vv) ) {
            return false;
        }
        v = v2f(vv);
        return true;
    }

    bool parse_property_value(const rapidjson::Value& root, color& v) {
        if ( root.IsNumber() ) {
            v = color(root.GetFloat(), root.GetFloat(), root.GetFloat());
            return true;
        }

        if ( root.IsObject() ) {
            if ( root.HasMember("r") ) {
                E2D_ASSERT(root["r"].IsNumber());
                v.r = root["r"].GetFloat();
            }
            if ( root.HasMember("g") ) {
                E2D_ASSERT(root["g"].IsNumber());
                v.g = root["g"].GetFloat();
            }
            if ( root.HasMember("b") ) {
                E2D_ASSERT(root["b"].IsNumber());
                v.b = root["b"].GetFloat();
            }
            if ( root.HasMember("a") ) {
                E2D_ASSERT(root["a"].IsNumber());
                v.a = root["a"].GetFloat();
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() > 0 ) {
                E2D_ASSERT(root[0].IsNumber());
                v.r = root[0].GetFloat();
            }
            if ( root.Size() > 1 ) {
                E2D_ASSERT(root[1].IsNumber());
                v.g = root[1].GetFloat();
            }
            if ( root.Size() > 2 ) {
                E2D_ASSERT(root[2].IsNumber());
                v.b = root[2].GetFloat();
            }
            if ( root.Size() > 3 ) {
                E2D_ASSERT(root[3].IsNumber());
                v.a = root[3].GetFloat();
            }
            return true;
        }

        return false;
    }

    bool parse_property_block_samplers(
        library& library,
        str_view address,
        const rapidjson::Value& root,
        render::property_block& props)
    {
        const auto parent_address = path::parent_path(address);

        if ( root.HasMember("samplers") ) {
            E2D_ASSERT(root["samplers"].IsArray());
            const auto& samplers_json = root["samplers"];

            for ( rapidjson::SizeType i = 0; i < samplers_json.Size(); ++i ) {
                E2D_ASSERT(samplers_json[i].IsObject());
                const auto& sampler_json = samplers_json[i];

                E2D_ASSERT(sampler_json.HasMember("name") && sampler_json["name"].IsString());
                E2D_ASSERT(sampler_json.HasMember("texture") && sampler_json["texture"].IsString());

                const auto texture = library.load_asset<texture_asset>(
                    path::combine(parent_address, sampler_json["texture"].GetString()));

                auto sampler = render::sampler_state()
                    .texture(texture ? texture->content() : texture_ptr());

                if ( sampler_json.HasMember("wrap") ) {
                    if ( sampler_json["wrap"].IsObject() ) {
                        const auto& sampler_wrap_json = sampler_json["wrap"];

                        if ( sampler_wrap_json.HasMember("s") ) {
                            E2D_ASSERT(sampler_wrap_json["s"].IsString());
                            auto wrap = sampler.s_wrap();
                            if ( parse_sampler_wrap(sampler_wrap_json["s"].GetString(), wrap) ) {
                                sampler.s_wrap(wrap);
                            } else {
                                E2D_ASSERT_MSG(false, "unexpected sampler wrap");
                            }
                        }

                        if ( sampler_wrap_json.HasMember("t") ) {
                            E2D_ASSERT(sampler_wrap_json["t"].IsString());
                            auto wrap = sampler.t_wrap();
                            if ( parse_sampler_wrap(sampler_wrap_json["t"].GetString(), wrap) ) {
                                sampler.t_wrap(wrap);
                            } else {
                                E2D_ASSERT_MSG(false, "unexpected sampler wrap");
                            }
                        }

                        if ( sampler_wrap_json.HasMember("r") ) {
                            E2D_ASSERT(sampler_wrap_json["r"].IsString());
                            auto wrap = sampler.r_wrap();
                            if ( parse_sampler_wrap(sampler_wrap_json["r"].GetString(), wrap) ) {
                                sampler.r_wrap(wrap);
                            } else {
                                E2D_ASSERT_MSG(false, "unexpected sampler wrap");
                            }
                        }
                    } else if ( sampler_json["wrap"].IsString() ) {
                        auto wrap = sampler.s_wrap();
                        if ( parse_sampler_wrap(sampler_json["wrap"].GetString(), wrap) ) {
                            sampler.wrap(wrap);
                        } else {
                            E2D_ASSERT_MSG(false, "unexpected sampler wrap");
                        }
                    }
                }

                if ( sampler_json.HasMember("filter") ) {
                    if ( sampler_json["filter"].IsObject() ) {
                        const auto& sampler_filter_json = sampler_json["filter"];

                        if ( sampler_filter_json.HasMember("min") ) {
                            E2D_ASSERT(sampler_filter_json["min"].IsString());
                            auto filter = sampler.min_filter();
                            if ( parse_sampler_min_filter(sampler_filter_json["min"].GetString(), filter) ) {
                                sampler.min_filter(filter);
                            } else {
                                E2D_ASSERT_MSG(false, "unexpected sampler min filter");
                            }
                        }

                        if ( sampler_filter_json.HasMember("mag") ) {
                            E2D_ASSERT(sampler_filter_json["mag"].IsString());
                            auto filter = sampler.mag_filter();
                            if ( parse_sampler_mag_filter(sampler_filter_json["mag"].GetString(), filter) ) {
                                sampler.mag_filter(filter);
                            } else {
                                E2D_ASSERT_MSG(false, "unexpected sampler mag filter");
                            }
                        }
                    } else if ( sampler_json["filter"].IsString() ) {
                        auto min_filter = sampler.min_filter();
                        if ( parse_sampler_min_filter(sampler_json["filter"].GetString(), min_filter) ) {
                            sampler.min_filter(min_filter);
                        } else {
                            E2D_ASSERT_MSG(false, "unexpected sampler filter");
                        }

                        auto mag_filter = sampler.mag_filter();
                        if ( parse_sampler_mag_filter(sampler_json["filter"].GetString(), mag_filter) ) {
                            sampler.mag_filter(mag_filter);
                        } else {
                            E2D_ASSERT_MSG(false, "unexpected sampler filter");
                        }
                    }
                }

                props.sampler(
                    sampler_json["name"].GetString(),
                    sampler);
            }
        }
        return true;
    }

    bool parse_property_block_properties(
        const rapidjson::Value& root,
        render::property_block& props)
    {
        if ( root.HasMember("properties") ) {
            E2D_ASSERT(root["properties"].IsArray());
            const auto& root_properties = root["properties"];

            for ( rapidjson::SizeType i = 0; i < root_properties.Size(); ++i ) {
                E2D_ASSERT(root_properties[i].IsObject());
                const auto& property_json = root_properties[i];

                E2D_ASSERT(property_json.HasMember("name") && property_json["name"].IsString());
                E2D_ASSERT(property_json.HasMember("type") && property_json["type"].IsString());

                if ( 0 == std::strcmp(property_json["type"].GetString(), "i32") ) {
                    i32 value = 0;
                    if ( property_json.HasMember("value") ) {
                        E2D_ASSERT(property_json["value"].IsNumber());
                        value = property_json["value"].GetInt();
                    }
                    props.property(property_json["name"].GetString(), value);
                } else if ( 0 == std::strcmp(property_json["type"].GetString(), "f32") ) {
                    f32 value = 0;
                    if ( property_json.HasMember("value") ) {
                        E2D_ASSERT(property_json["value"].IsNumber());
                        value = property_json["value"].GetFloat();
                    }
                    props.property(property_json["name"].GetString(), value);
                } else if ( 0 == std::strcmp(property_json["type"].GetString(), "v2i") ) {
                    v2i value;
                    if ( property_json.HasMember("value") ) {
                        if ( !parse_property_value(property_json["value"], value) ) {
                            E2D_ASSERT_MSG(false, "unexpected property value");
                            return false;
                        }
                    }
                    props.property(property_json["name"].GetString(), value);
                } else if ( 0 == std::strcmp(property_json["type"].GetString(), "v3i") ) {
                    v3i value;
                    if ( property_json.HasMember("value") ) {
                        if ( !parse_property_value(property_json["value"], value) ) {
                            E2D_ASSERT_MSG(false, "unexpected property value");
                            return false;
                        }
                    }
                    props.property(property_json["name"].GetString(), value);
                } else if ( 0 == std::strcmp(property_json["type"].GetString(), "v4i") ) {
                    v4i value;
                    if ( property_json.HasMember("value") ) {
                        if ( !parse_property_value(property_json["value"], value) ) {
                            E2D_ASSERT_MSG(false, "unexpected property value");
                            return false;
                        }
                    }
                    props.property(property_json["name"].GetString(), value);
                } else if ( 0 == std::strcmp(property_json["type"].GetString(), "v2f") ) {
                    v2f value;
                    if ( property_json.HasMember("value") ) {
                        if ( !parse_property_value(property_json["value"], value) ) {
                            E2D_ASSERT_MSG(false, "unexpected property value");
                            return false;
                        }
                    }
                    props.property(property_json["name"].GetString(), value);
                } else if ( 0 == std::strcmp(property_json["type"].GetString(), "v3f") ) {
                    v3f value;
                    if ( property_json.HasMember("value") ) {
                        if ( !parse_property_value(property_json["value"], value) ) {
                            E2D_ASSERT_MSG(false, "unexpected property value");
                            return false;
                        }
                    }
                    props.property(property_json["name"].GetString(), value);
                } else if ( 0 == std::strcmp(property_json["type"].GetString(), "v4f") ) {
                    v4f value;
                    if ( property_json.HasMember("value") ) {
                        if ( !parse_property_value(property_json["value"], value) ) {
                            E2D_ASSERT_MSG(false, "unexpected property value");
                            return false;
                        }
                    }
                    props.property(property_json["name"].GetString(), value);
                } else {
                    E2D_ASSERT_MSG(false, "unexpected property type");
                    return false;
                }
            }
        }
        return true;
    }

    bool parse_property_block(
        library& library,
        str_view address,
        const rapidjson::Value& root,
        render::property_block& props)
    {
        return parse_property_block_samplers(library, address, root, props)
            && parse_property_block_properties(root, props);
    }

    bool parse_depth_state(
        const rapidjson::Value& root,
        render::depth_state& depth)
    {
        if ( root.HasMember("range") ) {
            E2D_ASSERT(root["range"].IsObject());
            const auto& root_range = root["range"];

            if ( root_range.HasMember("near") ) {
                E2D_ASSERT(root_range["near"].IsNumber());
                depth.range(
                    root_range["near"].GetFloat(),
                    depth.range_far());
            }

            if ( root_range.HasMember("far") ) {
                E2D_ASSERT(root_range["far"].IsNumber());
                depth.range(
                    depth.range_near(),
                    root_range["far"].GetFloat());
            }
        }

        if ( root.HasMember("write") ) {
            E2D_ASSERT(root["write"].IsBool());
            depth.write(
                root["write"].GetBool());
        }

        if ( root.HasMember("func") ) {
            E2D_ASSERT(root["func"].IsString());

            render::compare_func func = depth.func();
            if ( !parse_compare_func(root["func"].GetString(), func) ) {
                E2D_ASSERT_MSG(false, "unexpected depth state func");
                return false;
            }

            depth.func(func);
        }

        return true;
    }

    bool parse_stencil_state(
        const rapidjson::Value& root,
        render::stencil_state& stencil)
    {
        if ( root.HasMember("write") ) {
            E2D_ASSERT(root["write"].IsUint() && root["write"].GetUint() <= 255);
            stencil.write(math::numeric_cast<u8>(
                root["write"].GetUint()));
        }

        if ( root.HasMember("func") ) {
            E2D_ASSERT(root["func"].IsString());

            render::compare_func func = stencil.func();
            if ( !parse_compare_func(root["func"].GetString(), func) ) {
                E2D_ASSERT_MSG(false, "unexpected stencil state func");
                return false;
            }

            stencil.func(
                func,
                stencil.ref(),
                stencil.mask());
        }

        if ( root.HasMember("ref") ) {
            E2D_ASSERT(root["ref"].IsUint() && root["ref"].GetUint() <= 255);

            stencil.func(
                stencil.func(),
                math::numeric_cast<u8>(root["ref"].GetUint()),
                stencil.mask());
        }

        if ( root.HasMember("mask") ) {
            E2D_ASSERT(root["mask"].IsUint() && root["mask"].GetUint() <= 255);

            stencil.func(
                stencil.func(),
                stencil.ref(),
                math::numeric_cast<u8>(root["mask"].GetUint()));
        }

        if ( root.HasMember("pass") ) {
            E2D_ASSERT(root["pass"].IsString());

            render::stencil_op op = stencil.pass();
            if ( !parse_stencil_op(root["pass"].GetString(), op) ) {
                E2D_ASSERT_MSG(false, "unexpected stencil state pass");
                return false;
            }

            stencil.op(
                op,
                stencil.sfail(),
                stencil.zfail());
        }

        if ( root.HasMember("sfail") ) {
            E2D_ASSERT(root["sfail"].IsString());

            render::stencil_op op = stencil.sfail();
            if ( !parse_stencil_op(root["sfail"].GetString(), op) ) {
                E2D_ASSERT_MSG(false, "unexpected stencil state sfail");
                return false;
            }

            stencil.op(
                stencil.pass(),
                op,
                stencil.zfail());
        }

        if ( root.HasMember("zfail") ) {
            E2D_ASSERT(root["zfail"].IsString());

            render::stencil_op op = stencil.zfail();
            if ( !parse_stencil_op(root["zfail"].GetString(), op) ) {
                E2D_ASSERT_MSG(false, "unexpected stencil state zfail");
                return false;
            }

            stencil.op(
                stencil.pass(),
                stencil.sfail(),
                op);
        }

        return true;
    }

    bool parse_culling_state(
        const rapidjson::Value& root,
        render::culling_state& culling)
    {
        if ( root.HasMember("mode") ) {
            E2D_ASSERT(root["mode"].IsString());

            render::culling_mode mode = culling.mode();
            if ( !parse_culling_mode(root["mode"].GetString(), mode) ) {
                E2D_ASSERT_MSG(false, "unexpected culling state mode");
                return false;
            }

            culling.mode(mode);
        }

        if ( root.HasMember("face") ) {
            E2D_ASSERT(root["face"].IsString());

            render::culling_face face = culling.face();
            if ( !parse_culling_face(root["face"].GetString(), face) ) {
                E2D_ASSERT_MSG(false, "unexpected culling state face");
                return false;
            }

            culling.face(face);
        }

        return true;
    }

    bool parse_blending_state(
        const rapidjson::Value& root,
        render::blending_state& blending)
    {
        if ( root.HasMember("constant_color") ) {
            E2D_ASSERT(root["constant_color"].IsObject());

            color color = blending.constant_color();
            if ( !parse_property_value(root["constant_color"], color) ) {
                E2D_ASSERT_MSG(false, "unexpected blending state constant_color");
                return false;
            }

            blending.constant_color(color);
        }

        if ( root.HasMember("color_mask") ) {
            E2D_ASSERT(root["color_mask"].IsString());

            render::blending_color_mask mask = blending.color_mask();
            if ( !parse_blending_color_mask(root["color_mask"].GetString(), mask) ) {
                E2D_ASSERT_MSG(false, "unexpected blending state color mask");
                return false;
            }

            blending.color_mask(mask);
        }

        if ( root.HasMember("src_factor") ) {
            if ( root["src_factor"].IsString() ) {
                render::blending_factor factor = blending.src_rgb_factor();
                if ( !parse_blending_factor(root["src_factor"].GetString(), factor) ) {
                    E2D_ASSERT_MSG(false, "unexpected blending state src factor");
                    return false;
                }
                blending.src_factor(factor);
            } else if ( root["src_factor"].IsObject() ) {
                const auto& root_src_factor = root["src_factor"];

                if ( root_src_factor.HasMember("rgb") ) {
                    E2D_ASSERT(root_src_factor["rgb"].IsString());
                    render::blending_factor factor = blending.src_rgb_factor();
                    if ( !parse_blending_factor(root_src_factor["rgb"].GetString(), factor) ) {
                        E2D_ASSERT_MSG(false, "unexpected blending state src factor");
                        return false;
                    }
                    blending.src_rgb_factor(factor);
                }

                if ( root_src_factor.HasMember("alpha") ) {
                    E2D_ASSERT(root_src_factor["alpha"].IsString());
                    render::blending_factor factor = blending.src_alpha_factor();
                    if ( !parse_blending_factor(root_src_factor["alpha"].GetString(), factor) ) {
                        E2D_ASSERT_MSG(false, "unexpected blending state src factor");
                        return false;
                    }
                    blending.src_alpha_factor(factor);
                }
            } else {
                E2D_ASSERT_MSG(false, "unexpected blending state src factor");
            }
        }

        if ( root.HasMember("dst_factor") ) {
            if ( root["dst_factor"].IsString() ) {
                render::blending_factor factor = blending.dst_rgb_factor();
                if ( !parse_blending_factor(root["dst_factor"].GetString(), factor) ) {
                    E2D_ASSERT_MSG(false, "unexpected blending state dst factor");
                    return false;
                }
                blending.dst_factor(factor);
            } else if ( root["dst_factor"].IsObject() ) {
                const auto& root_dst_factor = root["dst_factor"];

                if ( root_dst_factor.HasMember("rgb") ) {
                    E2D_ASSERT(root_dst_factor["rgb"].IsString());
                    render::blending_factor factor = blending.dst_rgb_factor();
                    if ( !parse_blending_factor(root_dst_factor["rgb"].GetString(), factor) ) {
                        E2D_ASSERT_MSG(false, "unexpected blending state dst factor");
                        return false;
                    }
                    blending.dst_rgb_factor(factor);
                }

                if ( root_dst_factor.HasMember("alpha") ) {
                    E2D_ASSERT(root_dst_factor["alpha"].IsString());
                    render::blending_factor factor = blending.dst_alpha_factor();
                    if ( !parse_blending_factor(root_dst_factor["alpha"].GetString(), factor) ) {
                        E2D_ASSERT_MSG(false, "unexpected blending state dst factor");
                        return false;
                    }
                    blending.dst_alpha_factor(factor);
                }
            } else {
                E2D_ASSERT_MSG(false, "unexpected blending state dst factor");
            }
        }

        if ( root.HasMember("equation") ) {
            if ( root["equation"].IsString() ) {
                render::blending_equation equation = blending.rgb_equation();
                if ( !parse_blending_equation(root["equation"].GetString(), equation) ) {
                    E2D_ASSERT_MSG(false, "unexpected blending state equation");
                    return false;
                }
                blending.equation(equation);
            } else if ( root["equation"].IsObject() ) {
                const auto& root_equation = root["equation"];

                if ( root_equation.HasMember("rgb") ) {
                    E2D_ASSERT(root_equation["rgb"].IsString());
                    render::blending_equation equation = blending.rgb_equation();
                    if ( !parse_blending_equation(root_equation["rgb"].GetString(), equation) ) {
                        E2D_ASSERT_MSG(false, "unexpected blending state equation");
                        return false;
                    }
                    blending.rgb_equation(equation);
                }

                if ( root_equation.HasMember("alpha") ) {
                    E2D_ASSERT(root_equation["alpha"].IsString());
                    render::blending_equation equation = blending.alpha_equation();
                    if ( !parse_blending_equation(root_equation["alpha"].GetString(), equation) ) {
                        E2D_ASSERT_MSG(false, "unexpected blending state equation");
                        return false;
                    }
                    blending.alpha_equation(equation);
                }
            } else {
                E2D_ASSERT_MSG(false, "unexpected blending state equation");
            }
        }

        return true;
    }

    bool parse_capabilities_state(
        const rapidjson::Value& root,
        render::capabilities_state& capabilities)
    {
        if ( root.HasMember("culling") ) {
            E2D_ASSERT(root["culling"].IsBool());
            capabilities.culling(
                root["culling"].GetBool());
        }

        if ( root.HasMember("blending") ) {
            E2D_ASSERT(root["blending"].IsBool());
            capabilities.blending(
                root["blending"].GetBool());
        }

        if ( root.HasMember("depth_test") ) {
            E2D_ASSERT(root["depth_test"].IsBool());
            capabilities.depth_test(
                root["depth_test"].GetBool());
        }

        if ( root.HasMember("stencil_test") ) {
            E2D_ASSERT(root["stencil_test"].IsBool());
            capabilities.stencil_test(
                root["stencil_test"].GetBool());
        }

        return true;
    }

    bool parse_state_block(
        const rapidjson::Value& root,
        render::state_block& states)
    {
        if ( root.HasMember("depth_state") ) {
            E2D_ASSERT(root["depth_state"].IsObject());
            if ( !parse_depth_state(root["depth_state"], states.depth()) ) {
                return false;
            }
        }

        if ( root.HasMember("stencil_state") ) {
            E2D_ASSERT(root["stencil_state"].IsObject());
            if ( !parse_stencil_state(root["stencil_state"], states.stencil()) ) {
                return false;
            }
        }

        if ( root.HasMember("culling_state") ) {
            E2D_ASSERT(root["culling_state"].IsObject());
            if ( !parse_culling_state(root["culling_state"], states.culling()) ) {
                return false;
            }
        }

        if ( root.HasMember("blending_state") ) {
            E2D_ASSERT(root["blending_state"].IsObject());
            if ( !parse_blending_state(root["blending_state"], states.blending()) ) {
                return false;
            }
        }

        if ( root.HasMember("capabilities_state") ) {
            E2D_ASSERT(root["capabilities_state"].IsObject());
            if ( !parse_capabilities_state(root["capabilities_state"], states.capabilities()) ) {
                return false;
            }
        }

        return true;
    }

    bool parse_pass_state(
        library& library,
        str_view address,
        const rapidjson::Value& root,
        render::pass_state& pass)
    {
        const auto parent_address = path::parent_path(address);

        if ( root.HasMember("shader") ) {
            E2D_ASSERT(root["shader"].IsString());

            const auto shader = library.load_asset<shader_asset>(
                path::combine(parent_address, root["shader"].GetString()));

            pass.shader(shader ? shader->content() : shader_ptr());
        }

        if ( root.HasMember("state_block") ) {
            E2D_ASSERT(root["state_block"].IsObject());
            const auto& state_block_json = root["state_block"];

            render::state_block states;
            if ( !parse_state_block(state_block_json, states) ) {
                return false;
            }

            pass.states(states);
        }

        if ( root.HasMember("property_block") ) {
            E2D_ASSERT(root["property_block"].IsObject());
            const auto& property_block_json = root["property_block"];

            render::property_block props;
            if ( !parse_property_block(library, address, property_block_json, props) ) {
                return false;
            }

            pass.properties(props);
        }

        return true;
    }
}

namespace e2d
{
    //
    // text_asset
    //

    std::shared_ptr<text_asset> text_asset::load(library& library, str_view address) {
        E2D_UNUSED(library);

        const auto asset_url = library.root() / address;
        input_stream_uptr stream = modules::is_initialized<vfs>()
            ? the<vfs>().read(asset_url)
            : input_stream_uptr();

        if ( !stream ) {
            the<debug>().error("ASSETS: Failed to open text asset file:\n"
                "--> Url: %0",
                asset_url);
            return nullptr;
        }

        str content;
        if ( !streams::try_read_tail(content, stream) ) {
            the<debug>().error("ASSETS: Failed to read text asset file:\n"
                "--> Url: %0",
                asset_url);
            return nullptr;
        }

        return std::make_shared<text_asset>(std::move(content));
    }

    //
    // mesh_asset
    //

    std::shared_ptr<mesh_asset> mesh_asset::load(library& library, str_view address) {
        const auto mesh_data = library.load_asset<binary_asset>(address);
        if ( !mesh_data ) {
            return nullptr;
        }

        mesh content;
        if ( !meshes::try_load_mesh(content, mesh_data->content()) ) {
            the<debug>().error("ASSETS: Failed to create mesh asset:\n"
                "--> Address: %0",
                address);
            return nullptr;
        }

        return std::make_shared<mesh_asset>(std::move(content));
    }

    //
    // image_asset
    //

    std::shared_ptr<image_asset> image_asset::load(library& library, str_view address) {
        const auto image_data = library.load_asset<binary_asset>(address);
        if ( !image_data ) {
            return nullptr;
        }

        image content;
        if ( !images::try_load_image(content, image_data->content()) ) {
            the<debug>().error("ASSETS: Failed to create image asset:\n"
                "--> Address: %0",
                address);
            return nullptr;
        }

        return std::make_shared<image_asset>(std::move(content));
    }

    //
    // binary_asset
    //

    std::shared_ptr<binary_asset> binary_asset::load(library& library, str_view address) {
        E2D_UNUSED(library);

        const auto asset_url = library.root() / address;
        input_stream_uptr stream = modules::is_initialized<vfs>()
            ? the<vfs>().read(asset_url)
            : input_stream_uptr();

        if ( !stream ) {
            the<debug>().error("ASSETS: Failed to open binary asset file:\n"
                "--> Url: %0",
                asset_url);
            return nullptr;
        }

        buffer content;
        if ( !streams::try_read_tail(content, stream) ) {
            the<debug>().error("ASSETS: Failed to read binary asset file:\n"
                "--> Url: %0",
                asset_url);
            return nullptr;
        }

        return std::make_shared<binary_asset>(std::move(content));
    }

    //
    // shader_asset
    //

    std::shared_ptr<shader_asset> shader_asset::load(library& library, str_view address) {
        rapidjson::Document doc;
        if ( !load_json_asset_file<shader_asset>(library, address, doc) ) {
            return nullptr;
        }

        const auto parent_address = path::parent_path(address);

        E2D_ASSERT(doc.HasMember("vertex") && doc["vertex"].IsString());
        const auto vertex_source_data = library.load_asset<text_asset>(
            path::combine(parent_address, doc["vertex"].GetString()));

        if ( !vertex_source_data ) {
            return nullptr;
        }

        E2D_ASSERT(doc.HasMember("fragment") && doc["fragment"].IsString());
        const auto fragment_source_data = library.load_asset<text_asset>(
            path::combine(parent_address, doc["fragment"].GetString()));

        if ( !fragment_source_data ) {
            return nullptr;
        }

        const auto content = modules::is_initialized<render>()
            ? the<render>().create_shader(
                vertex_source_data->content(),
                fragment_source_data->content())
            : shader_ptr();

        if ( !content ) {
            the<debug>().error("ASSETS: Failed to create shader asset:\n"
                "--> Address: %0",
                address);
            return nullptr;
        }

        return std::make_shared<shader_asset>(content);
    }

    //
    // texture_asset
    //

    std::shared_ptr<texture_asset> texture_asset::load(library& library, str_view address) {
        const auto texture_data = library.load_asset<image_asset>(address);
        if ( !texture_data ) {
            return nullptr;
        }

        const auto content = modules::is_initialized<render>()
            ? the<render>().create_texture(texture_data->content())
            : texture_ptr();

        if ( !content ) {
            the<debug>().error("ASSETS: Failed to create texture asset:\n"
                "--> Address: %0",
                address);
            return nullptr;
        }

        return std::make_shared<texture_asset>(content);
    }

    //
    // material_asset
    //

    std::shared_ptr<material_asset> material_asset::load(library& library, str_view address) {
        rapidjson::Document doc;
        if ( !load_json_asset_file<material_asset>(library, address, doc) ) {
            return nullptr;
        }

        render::material content;

        if ( doc.HasMember("passes") ) {
            E2D_ASSERT(doc["passes"].IsArray());
            const auto& passes_json = doc["passes"];

            for ( rapidjson::SizeType i = 0; i < passes_json.Size(); ++i ) {
                E2D_ASSERT(passes_json[i].IsObject());
                const auto& pass_json = passes_json[i];

                render::pass_state pass;
                if ( !parse_pass_state(library, address, pass_json, pass) ) {
                    return nullptr;
                }

                content.add_pass(pass);
            }
        }

        if ( doc.HasMember("property_block") ) {
            E2D_ASSERT(doc["property_block"].IsObject());
            const auto& property_block_json = doc["property_block"];

            render::property_block props;
            if ( !parse_property_block(library, address, property_block_json, props) ) {
                return nullptr;
            }

            content.properties(props);
        }

        return std::make_shared<material_asset>(content);
    }
}
