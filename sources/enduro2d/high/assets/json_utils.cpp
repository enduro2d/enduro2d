/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "json_utils.hpp"

namespace
{
    using namespace e2d;

    class json_utils_exception final : public exception {
        const char* what() const noexcept final {
            return "json utils exception";
        }
    };

    const char* common_schema_definitions_source = R"json({
        "v2" : {
            "anyOf" : [{
                "type" : "number"
            }, {
                "type" : "array",
                "minItems" : 2,
                "maxItems" : 2,
                "items" : { "type" : "number" }
            }, {
                "type" : "object",
                "additionalProperties" : false,
                "properties" : {
                    "x" : { "type" : "number" },
                    "y" : { "type" : "number" }
                }
            }]
        },
        "v3" : {
            "anyOf" : [{
                "type" : "number"
            }, {
                "type" : "array",
                "minItems" : 3,
                "maxItems" : 3,
                "items" : { "type" : "number" }
            }, {
                "type" : "object",
                "additionalProperties" : false,
                "properties" : {
                    "x" : { "type" : "number" },
                    "y" : { "type" : "number" },
                    "z" : { "type" : "number" }
                }
            }]
        },
        "v4" : {
            "anyOf" : [{
                "type" : "number"
            }, {
                "type" : "array",
                "minItems" : 4,
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
        "m2" : {
            "anyOf" : [{
                "type" : "array",
                "minItems" : 2,
                "maxItems" : 2,
                "items" : { "$ref": "#/common_definitions/v2" }
            }, {
                "type" : "array",
                "minItems" : 4,
                "maxItems" : 4,
                "items" : { "type" : "number" }
            }]
        },
        "m3" : {
            "anyOf" : [{
                "type" : "array",
                "minItems" : 3,
                "maxItems" : 3,
                "items" : { "$ref": "#/common_definitions/v3" }
            }, {
                "type" : "array",
                "minItems" : 9,
                "maxItems" : 9,
                "items" : { "type" : "number" }
            }]
        },
        "m4" : {
            "anyOf" : [{
                "type" : "array",
                "minItems" : 4,
                "maxItems" : 4,
                "items" : { "$ref": "#/common_definitions/v4" }
            }, {
                "type" : "array",
                "minItems" : 16,
                "maxItems" : 16,
                "items" : { "type" : "number" }
            }]
        },
        "b2" : {
            "anyOf" : [{
                "type" : "array",
                "minItems" : 2,
                "maxItems" : 2,
                "items" : { "type" : "number" }
            }, {
                "type" : "array",
                "minItems" : 4,
                "maxItems" : 4,
                "items" : { "type" : "number" }
            }, {
                "type" : "object",
                "additionalProperties" : false,
                "properties" : {
                    "x" : { "type" : "number" },
                    "y" : { "type" : "number" },
                    "w" : { "type" : "number" },
                    "h" : { "type" : "number" }
                }
            }]
        },
        "b3" : {
            "anyOf" : [{
                "type" : "array",
                "minItems" : 3,
                "maxItems" : 3,
                "items" : { "type" : "number" }
            }, {
                "type" : "array",
                "minItems" : 6,
                "maxItems" : 6,
                "items" : { "type" : "number" }
            }, {
                "type" : "object",
                "additionalProperties" : false,
                "properties" : {
                    "x" : { "type" : "number" },
                    "y" : { "type" : "number" },
                    "z" : { "type" : "number" },
                    "w" : { "type" : "number" },
                    "h" : { "type" : "number" },
                    "d" : { "type" : "number" }
                }
            }]
        },
        "color" : {
            "anyOf" : [{
                "type" : "number"
            }, {
                "type" : "array",
                "minItems" : 3,
                "maxItems" : 4,
                "items" : { "type" : "number" }
            }, {
                "type" : "object",
                "additionalProperties" : false,
                "properties" : {
                    "r" : { "type" : "number" },
                    "g" : { "type" : "number" },
                    "b" : { "type" : "number" },
                    "a" : { "type" : "number" }
                }
            }]
        },
        "name" : {
            "type" : "string",
            "minLength" : 1
        },
        "address" : {
            "type" : "string",
            "minLength" : 1
        }
    })json";

    const rapidjson::Value& common_schema_definitions() {
        static std::mutex mutex;
        static std::unique_ptr<rapidjson::Document> defs_doc;

        std::lock_guard<std::mutex> guard(mutex);
        if ( !defs_doc ) {
            rapidjson::Document doc;
            if ( doc.Parse(common_schema_definitions_source).HasParseError() ) {
                the<debug>().error("JSON_UTILS: Failed to parse common schema definitions");
                throw json_utils_exception();
            }
            defs_doc = std::make_unique<rapidjson::Document>(std::move(doc));
        }

        return *defs_doc;
    }

    template < std::size_t N, typename V, typename FV >
    bool parse_vN(const rapidjson::Value& root, V& v, FV&& f) noexcept {
        if ( root.IsNumber() ) {
            v = V(f(root));
            return true;
        }

        if ( root.IsObject() ) {
            const char* const props[] = { "x", "y", "z", "w" };
            for ( std::size_t i = 0; i < math::min(N, E2D_COUNTOF(props)); ++i ) {
                if ( root.HasMember(props[i]) ) {
                    const auto& jv = root[props[i]];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    v[i] = f(jv);
                }
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() == N ) {
                for ( u32 i = 0; i < N; ++i ) {
                    const auto& jv = root[i];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    v[i] = f(jv);
                }
                return true;
            }
        }

        return false;
    }

    template < std::size_t N, typename V, typename FV >
    bool parse_mN(const rapidjson::Value& root, V& v, FV&& f) noexcept {
        if ( root.IsArray() ) {
            if ( root.Size() == N ) {
                for ( u32 i = 0; i < N; ++i ) {
                    const auto& jv = root[i];
                    if ( !parse_vN<N>(jv, v[i], f) ) {
                        return false;
                    }
                }
                return true;
            }

            if ( root.Size() == N * N ) {
                for ( u32 i = 0; i < N; ++i ) {
                    for ( u32 j = 0; j < N; ++j ) {
                        const auto& jv = root[i * N + j];
                        if ( !jv.IsNumber() ) {
                            return false;
                        }
                        v[i][j] = f(jv);
                    }
                }
                return true;
            }
        }
        return false;
    }

    template < typename V, typename FV >
    bool parse_b2(const rapidjson::Value& root, rect<V>& b, FV&& f) {
        if ( root.IsObject() ) {
            const char* const props[] = { "x", "y", "w", "h" };
            for ( std::size_t i = 0; i < E2D_COUNTOF(props); ++i ) {
                if ( root.HasMember(props[i]) ) {
                    const auto& jv = root[props[i]];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    b[i] = f(jv);
                }
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() == 2 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    b.size[i] = f(jv);
                }
                return true;
            }

            if ( root.Size() == 4 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    b[i] = f(jv);
                }
                return true;
            }
        }

        return false;
    }

    template < typename V, typename FV >
    bool parse_b3(const rapidjson::Value& root, aabb<V>& b, FV&& f) {
        if ( root.IsObject() ) {
            const char* const props[] = { "x", "y", "z", "w", "h", "d" };
            for ( std::size_t i = 0; i < E2D_COUNTOF(props); ++i ) {
                if ( root.HasMember(props[i]) ) {
                    const auto& jv = root[props[i]];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    b[i] = f(jv);
                }
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() == 3 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    b.size[i] = f(jv);
                }
                return true;
            }

            if ( root.Size() == 6 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    b[i] = f(jv);
                }
                return true;
            }
        }

        return false;
    }

    bool parse_clr(const rapidjson::Value& root, color& c) {
        if ( root.IsNumber() ) {
            f32 cv = root.GetFloat();
            c = color(cv, cv, cv, c.a);
            return true;
        }

        if ( root.IsObject() ) {
            const char* const props[] = { "r", "g", "b", "a" };
            for ( std::size_t i = 0; i < E2D_COUNTOF(props); ++i ) {
                if ( root.HasMember(props[i]) ) {
                    const auto& jv = root[props[i]];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    c[i] = jv.GetFloat();
                }
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() == 3 || root.Size() == 4 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    c[i] = jv.GetFloat();
                }
                return true;
            }
        }

        return false;
    }

    template < u32 N, typename V >
    bool parse_vNi(const rapidjson::Value& root, V& v) noexcept {
        return parse_vN<N>(root, v, [](const rapidjson::Value& jv){
            E2D_ASSERT(jv.IsNumber());
            return jv.GetInt();
        });
    }

    template < u32 N, typename V >
    bool parse_vNf(const rapidjson::Value& root, V& v) noexcept {
        return parse_vN<N>(root, v, [](const rapidjson::Value& jv){
            E2D_ASSERT(jv.IsNumber());
            return jv.GetFloat();
        });
    }

    template < u32 N, typename V >
    bool parse_mNf(const rapidjson::Value& root, V& v) noexcept {
        return parse_mN<N>(root, v, [](const rapidjson::Value& jv){
            E2D_ASSERT(jv.IsNumber());
            return jv.GetFloat();
        });
    }

    template < typename B >
    bool parse_b2i(const rapidjson::Value& root, B& b) noexcept {
        return parse_b2(root, b, [](const rapidjson::Value& jv){
            E2D_ASSERT(jv.IsNumber());
            return jv.GetInt();
        });
    }

    template < typename B >
    bool parse_b2f(const rapidjson::Value& root, B& b) noexcept {
        return parse_b2(root, b, [](const rapidjson::Value& jv){
            E2D_ASSERT(jv.IsNumber());
            return jv.GetFloat();
        });
    }

    template < typename B >
    bool parse_b3i(const rapidjson::Value& root, B& b) noexcept {
        return parse_b3(root, b, [](const rapidjson::Value& jv){
            E2D_ASSERT(jv.IsNumber());
            return jv.GetInt();
        });
    }

    template < typename B >
    bool parse_b3f(const rapidjson::Value& root, B& b) noexcept {
        return parse_b3(root, b, [](const rapidjson::Value& jv){
            E2D_ASSERT(jv.IsNumber());
            return jv.GetFloat();
        });
    }
}

namespace e2d { namespace json_utils
{
    bool try_parse_value(const rapidjson::Value& root, v2i& v) noexcept {
        return parse_vNi<2>(root, v);
    }

    bool try_parse_value(const rapidjson::Value& root, v3i& v) noexcept {
        return parse_vNi<3>(root, v);
    }

    bool try_parse_value(const rapidjson::Value& root, v4i& v) noexcept {
        return parse_vNi<4>(root, v);
    }

    bool try_parse_value(const rapidjson::Value& root, v2f& v) noexcept {
        return parse_vNf<2>(root, v);
    }

    bool try_parse_value(const rapidjson::Value& root, v3f& v) noexcept {
        return parse_vNf<3>(root, v);
    }

    bool try_parse_value(const rapidjson::Value& root, v4f& v) noexcept {
        return parse_vNf<4>(root, v);
    }

    bool try_parse_value(const rapidjson::Value& root, m2f& v) noexcept {
        return parse_mNf<2>(root, v);
    }

    bool try_parse_value(const rapidjson::Value& root, m3f& v) noexcept {
        return parse_mNf<3>(root, v);
    }

    bool try_parse_value(const rapidjson::Value& root, m4f& v) noexcept {
        return parse_mNf<4>(root, v);
    }

    bool try_parse_value(const rapidjson::Value& root, b2i& b) noexcept {
        return parse_b2i(root, b);
    }

    bool try_parse_value(const rapidjson::Value& root, b3i& b) noexcept {
        return parse_b3i(root, b);
    }

    bool try_parse_value(const rapidjson::Value& root, b2f& b) noexcept {
        return parse_b2f(root, b);
    }

    bool try_parse_value(const rapidjson::Value& root, b3f& b) noexcept {
        return parse_b3f(root, b);
    }

    bool try_parse_value(const rapidjson::Value& root, color& c) noexcept {
        return parse_clr(root, c);
    }

    bool try_parse_value(const rapidjson::Value& root, str& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_utf8(root.GetString());
        } catch (...) {
            return false;
        }
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, wstr& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_wide(root.GetString());
        } catch (...) {
            return false;
        }
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, str16& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_utf16(root.GetString());
        } catch (...) {
            return false;
        }
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, str32& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_utf32(root.GetString());
        } catch (...) {
            return false;
        }
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, str_hash& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_hash(root.GetString());
        } catch (...) {
            return false;
        }
        return true;
    }

    void add_common_schema_definitions(rapidjson::Document& schema) {
        schema.AddMember(
            "common_definitions",
            rapidjson::Value(
                common_schema_definitions(),
                schema.GetAllocator()).Move(),
            schema.GetAllocator());
    }
}}
