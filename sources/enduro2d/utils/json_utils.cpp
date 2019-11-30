/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/utils/json_utils.hpp>

#include <enduro2d/utils/color.hpp>
#include <enduro2d/utils/color32.hpp>
#include <enduro2d/utils/strings.hpp>

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
        "q4" : {
            "anyOf" : [{
                "type" : "number"
            }, {
                "type" : "object",
                "required" : [ "angle", "axis" ],
                "additionalProperties" : false,
                "properties" : {
                    "angle" : { "type" : "number" },
                    "axis" : { "$ref": "#/common_definitions/v3" }
                }
            }, {
                "type" : "object",
                "required" : [ "roll", "pitch", "yaw" ],
                "additionalProperties" : false,
                "properties" : {
                    "roll" : { "type" : "number" },
                    "pitch" : { "type" : "number" },
                    "yaw" : { "type" : "number" }
                }
            }, {
                "type" : "array",
                "minItems" : 3,
                "maxItems" : 3,
                "items" : { "type" : "number" }
            }, {
                "type" : "array",
                "minItems" : 4,
                "maxItems" : 4,
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
        "t2" : {
            "type" : "object",
            "additionalProperties" : false,
            "properties" : {
                "translation" : { "$ref": "#/common_definitions/v2" },
                "rotation" : { "type" : "number" },
                "scale" : { "$ref": "#/common_definitions/v2" }
            }
        },
        "t3" : {
            "type" : "object",
            "additionalProperties" : false,
            "properties" : {
                "translation" : { "$ref": "#/common_definitions/v3" },
                "rotation" : { "$ref": "#/common_definitions/q4" },
                "scale" : { "$ref": "#/common_definitions/v3" }
            }
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
                throw json_utils_exception();
            }
            defs_doc = std::make_unique<rapidjson::Document>(std::move(doc));
        }

        return *defs_doc;
    }
}

namespace
{
    using namespace e2d;

    //
    // vec
    //

    template < std::size_t N, typename V, typename FV >
    bool parse_vN(const rapidjson::Value& root, V& v, FV&& f) noexcept {
        if ( root.IsNumber() ) {
            typename V::value_type tv;
            if ( f(root, tv) ) {
                v = V(tv);
                return true;
            }
        }

        if ( root.IsObject() ) {
            const char* const props[] = { "x", "y", "z", "w" };
            for ( std::size_t i = 0; i < math::min(N, std::size(props)); ++i ) {
                if ( root.HasMember(props[i]) ) {
                    const auto& jv = root[props[i]];
                    if ( !f(jv, v[i]) ) {
                        return false;
                    }
                }
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() == N ) {
                for ( u32 i = 0; i < N; ++i ) {
                    const auto& jv = root[i];
                    if ( !f(jv, v[i]) ) {
                        return false;
                    }
                }
                return true;
            }
        }

        return false;
    }

    template < u32 N, typename V >
    bool parse_vNi(const rapidjson::Value& root, V& v) noexcept {
        return parse_vN<N>(root, v, [](const rapidjson::Value& jv, i32& o){
            if ( !jv.IsInt() ) {
                return false;
            }
            o = jv.GetInt();
            return true;
        });
    }

    template < u32 N, typename V >
    bool parse_vNf(const rapidjson::Value& root, V& v) noexcept {
        return parse_vN<N>(root, v, [](const rapidjson::Value& jv, f32& o){
            if ( !jv.IsNumber() ) {
                return false;
            }
            o = jv.GetFloat();
            return true;
        });
    }

    //
    // mat
    //

    template < u32 N, typename V, typename FV >
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
                        if ( !f(jv, v[i][j]) ) {
                            return false;
                        }
                    }
                }
                return true;
            }
        }
        return false;
    }

    template < u32 N, typename V >
    bool parse_mNf(const rapidjson::Value& root, V& v) noexcept {
        return parse_mN<N>(root, v, [](const rapidjson::Value& jv, f32& o){
            if ( !jv.IsNumber() ) {
                return false;
            }
            o = jv.GetFloat();
            return true;
        });
    }

    //
    // quat
    //

    template < typename V, typename FV >
    bool parse_q4(const rapidjson::Value& root, V& v, FV&& f) noexcept {
        if ( root.IsNumber() ) {
            typename V::value_type tv;
            if ( f(root, tv) ) {
                v = math::make_quat_from_axis_angle(
                    make_deg(tv),
                    vec3<typename V::value_type>::unit_z());
                return true;
            }
        }

        if ( root.IsObject() ) {
            if ( root.HasMember("angle") && root.HasMember("axis") ) {
                typename V::value_type tangle;
                vec3<typename V::value_type> vaxis;
                if ( f(root["angle"], tangle) && parse_vN<3>(root["axis"], vaxis, f) ) {
                    v = math::make_quat_from_axis_angle(
                        make_deg(tangle),
                        vaxis);
                    return true;
                }
            }

            if ( root.HasMember("roll") && root.HasMember("pitch") && root.HasMember("yaw") ) {
                typename V::value_type troll, tpitch, tyaw;
                if ( f(root["roll"], troll) && f(root["pitch"], tpitch) && f(root["yaw"], tyaw) ) {
                    v = math::make_quat_from_euler_angles(
                        make_deg(troll),
                        make_deg(tpitch),
                        make_deg(tyaw));
                    return true;
                }
            }
        }

        if ( root.IsArray() ) {
            if ( root.Size() == 3 ) {
                vec3<typename V::value_type> veuler;
                if ( parse_vN<3>(root, veuler, f) ) {
                    v = math::make_quat_from_euler_angles(
                        make_deg(veuler.x),
                        make_deg(veuler.y),
                        make_deg(veuler.z));
                    return true;
                }
            }

            if ( root.Size() == 4 ) {
                vec4<typename V::value_type>  vq;
                if ( parse_vN<4>(root, vq, f) ) {
                    v = V(vq);
                    return true;
                }
            }
        }

        return false;
    }

    bool parse_q4f(const rapidjson::Value& root, q4f& v) noexcept {
        return parse_q4(root, v, [](const rapidjson::Value& jv, f32& o){
            if ( !jv.IsNumber() ) {
                return false;
            }
            o = jv.GetFloat();
            return true;
        });
    }

    //
    // rect
    //

    template < typename V, typename FV >
    bool parse_b2(const rapidjson::Value& root, rect<V>& b, FV&& f) {
        if ( root.IsObject() ) {
            const char* const props[] = { "x", "y", "w", "h" };
            for ( std::size_t i = 0; i < std::size(props); ++i ) {
                if ( root.HasMember(props[i]) ) {
                    const auto& jv = root[props[i]];
                    if ( !f(jv, b[i]) ) {
                        return false;
                    }
                }
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() == 2 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !f(jv, b.size[i]) ) {
                        return false;
                    }
                }
                return true;
            }

            if ( root.Size() == 4 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !f(jv, b[i]) ) {
                        return false;
                    }
                }
                return true;
            }
        }

        return false;
    }

    template < typename B >
    bool parse_b2i(const rapidjson::Value& root, B& b) noexcept {
        return parse_b2(root, b, [](const rapidjson::Value& jv, i32& o){
            if ( !jv.IsInt() ) {
                return false;
            }
            o = jv.GetInt();
            return true;
        });
    }

    template < typename B >
    bool parse_b2f(const rapidjson::Value& root, B& b) noexcept {
        return parse_b2(root, b, [](const rapidjson::Value& jv, f32& o){
            if ( !jv.IsNumber() ) {
                return false;
            }
            o = jv.GetFloat();
            return true;
        });
    }

    //
    // aabb
    //

    template < typename V, typename FV >
    bool parse_b3(const rapidjson::Value& root, aabb<V>& b, FV&& f) {
        if ( root.IsObject() ) {
            const char* const props[] = { "x", "y", "z", "w", "h", "d" };
            for ( std::size_t i = 0; i < std::size(props); ++i ) {
                if ( root.HasMember(props[i]) ) {
                    const auto& jv = root[props[i]];
                    if ( !f(jv, b[i]) ) {
                        return false;
                    }
                }
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() == 3 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !f(jv, b.size[i]) ) {
                        return false;
                    }
                }
                return true;
            }

            if ( root.Size() == 6 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !f(jv, b[i]) ) {
                        return false;
                    }
                }
                return true;
            }
        }

        return false;
    }

    template < typename B >
    bool parse_b3i(const rapidjson::Value& root, B& b) noexcept {
        return parse_b3(root, b, [](const rapidjson::Value& jv, i32& o){
            if ( !jv.IsInt() ) {
                return false;
            }
            o = jv.GetInt();
            return true;
        });
    }

    template < typename B >
    bool parse_b3f(const rapidjson::Value& root, B& b) noexcept {
        return parse_b3(root, b, [](const rapidjson::Value& jv, f32& o){
            if ( !jv.IsNumber() ) {
                return false;
            }
            o = jv.GetFloat();
            return true;
        });
    }

    //
    // color
    //

    bool parse_clr(const rapidjson::Value& root, color& c) {
        if ( root.IsNumber() ) {
            f32 cv = math::clamp(root.GetFloat(), 0.f, 1.f);
            c = color(cv, cv, cv, c.a);
            return true;
        }

        if ( root.IsObject() ) {
            const char* const props[] = { "r", "g", "b", "a" };
            for ( std::size_t i = 0; i < std::size(props); ++i ) {
                if ( root.HasMember(props[i]) ) {
                    const auto& jv = root[props[i]];
                    if ( !jv.IsNumber() ) {
                        return false;
                    }
                    c[i] = math::clamp(jv.GetFloat(), 0.f, 1.f);
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
                    c[i] = math::clamp(jv.GetFloat(), 0.f, 1.f);
                }
                return true;
            }
        }

        return false;
    }

    //
    // color32
    //

    bool parse_clr(const rapidjson::Value& root, color32& c) {
        if ( root.IsUint() ) {
            u8 cv = math::numeric_cast<u8>(math::clamp(root.GetUint(), 0u, 255u));
            c = color32(cv, cv, cv, c.a);
            return true;
        }

        if ( root.IsObject() ) {
            const char* const props[] = { "r", "g", "b", "a" };
            for ( std::size_t i = 0; i < std::size(props); ++i ) {
                if ( root.HasMember(props[i]) ) {
                    const auto& jv = root[props[i]];
                    if ( !jv.IsUint() ) {
                        return false;
                    }
                    c[i] = math::numeric_cast<u8>(math::clamp(jv.GetUint(), 0u, 255u));
                }
            }
            return true;
        }

        if ( root.IsArray() ) {
            if ( root.Size() == 3 || root.Size() == 4 ) {
                for ( u32 i = 0; i < root.Size(); ++i ) {
                    const auto& jv = root[i];
                    if ( !jv.IsUint() ) {
                        return false;
                    }
                    c[i] = math::numeric_cast<u8>(math::clamp(jv.GetUint(), 0u, 255u));
                }
                return true;
            }
        }

        return false;
    }
}

namespace e2d::json_utils
{
    void add_common_schema_definitions(rapidjson::Document& schema) {
        schema.AddMember(
            "common_definitions",
            rapidjson::Value(
                common_schema_definitions(),
                schema.GetAllocator()).Move(),
            schema.GetAllocator());
    }
}

namespace e2d::json_utils
{
    bool try_parse_value(const rapidjson::Value& root, v2i& v) noexcept {
        v2i tv;
        if ( !parse_vNi<2>(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, v3i& v) noexcept {
        v3i tv;
        if ( !parse_vNi<3>(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, v4i& v) noexcept {
        v4i tv;
        if ( !parse_vNi<4>(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, v2u& v) noexcept {
        v2i vi;
        if ( !try_parse_value(root, vi) ) {
            return false;
        }
        if ( vi.x < 0 || vi.y < 0 ) {
            return false;
        }
        v = vi.cast_to<u32>();
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, v3u& v) noexcept {
        v3i vi;
        if ( !try_parse_value(root, vi) ) {
            return false;
        }
        if ( vi.x < 0 || vi.y < 0 || vi.z < 0 ) {
            return false;
        }
        v = vi.cast_to<u32>();
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, v4u& v) noexcept {
        v4i vi;
        if ( !try_parse_value(root, vi) ) {
            return false;
        }
        if ( vi.x < 0 || vi.y < 0 || vi.z < 0 || vi.w < 0 ) {
            return false;
        }
        v = vi.cast_to<u32>();
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, v2f& v) noexcept {
        v2f tv;
        if ( !parse_vNf<2>(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, v3f& v) noexcept {
        v3f tv;
        if ( !parse_vNf<3>(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, v4f& v) noexcept {
        v4f tv;
        if ( !parse_vNf<4>(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, m2f& v) noexcept {
        m2f tv;
        if ( !parse_mNf<2>(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, m3f& v) noexcept {
        m3f tv;
        if ( !parse_mNf<3>(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, m4f& v) noexcept {
        m4f tv;
        if ( !parse_mNf<4>(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, q4f& v) noexcept {
        q4f tv;
        if ( !parse_q4f(root, tv) ) {
            return false;
        }
        v = tv;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, b2i& b) noexcept {
        b2i tb;
        if ( !parse_b2i(root, tb) ) {
            return false;
        }
        b = tb;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, b3i& b) noexcept {
        b3i tb;
        if ( !parse_b3i(root, tb) ) {
            return false;
        }
        b = tb;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, b2u& b) noexcept {
        b2i bi;
        if ( !try_parse_value(root, bi) ) {
            return false;
        }
        if ( bi.position.x < 0 || bi.position.y < 0 ) {
            return false;
        }
        if ( bi.size.x < 0 || bi.size.y < 0 ) {
            return false;
        }
        b = bi.cast_to<u32>();
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, b3u& b) noexcept {
        b3i bi;
        if ( !try_parse_value(root, bi) ) {
            return false;
        }
        if ( bi.position.x < 0 || bi.position.y < 0 || bi.position.z < 0 ) {
            return false;
        }
        if ( bi.size.x < 0 || bi.size.y < 0 || bi.size.z < 0 ) {
            return false;
        }
        b = bi.cast_to<u32>();
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, b2f& b) noexcept {
        b2f tb;
        if ( !parse_b2f(root, tb) ) {
            return false;
        }
        b = tb;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, b3f& b) noexcept {
        b3f tb;
        if ( !parse_b3f(root, tb) ) {
            return false;
        }
        b = tb;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, color& c) noexcept {
        color tc;
        if ( !parse_clr(root, tc) ) {
            return false;
        }
        c = tc;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, color32& c) noexcept {
        color32 tc;
        if ( !parse_clr(root, tc) ) {
            return false;
        }
        c = tc;
        return true;
    }

    bool try_parse_value(const rapidjson::Value& root, str& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_utf8(root.GetString());
            return true;
        } catch (...) {
            return false;
        }
    }

    bool try_parse_value(const rapidjson::Value& root, wstr& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_wide(root.GetString());
            return true;
        } catch (...) {
            return false;
        }
    }

    bool try_parse_value(const rapidjson::Value& root, str16& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_utf16(root.GetString());
            return true;
        } catch (...) {
            return false;
        }
    }

    bool try_parse_value(const rapidjson::Value& root, str32& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_utf32(root.GetString());
            return true;
        } catch (...) {
            return false;
        }
    }

    bool try_parse_value(const rapidjson::Value& root, str_hash& s) noexcept {
        if ( !root.IsString() ) {
            return false;
        }
        try {
            s = make_hash(root.GetString());
            return true;
        } catch (...) {
            return false;
        }
    }
}
