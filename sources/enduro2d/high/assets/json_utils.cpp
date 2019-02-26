/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "json_utils.hpp"

namespace
{
    using namespace e2d;

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
}}
