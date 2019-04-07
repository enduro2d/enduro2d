/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/high/_high.hpp>

#include <3rdparty/rapidjson/schema.h>
#include <3rdparty/rapidjson/document.h>

namespace e2d { namespace json_utils
{
    bool try_parse_value(const rapidjson::Value& root, v2i& v) noexcept;
    bool try_parse_value(const rapidjson::Value& root, v3i& v) noexcept;
    bool try_parse_value(const rapidjson::Value& root, v4i& v) noexcept;

    bool try_parse_value(const rapidjson::Value& root, v2f& v) noexcept;
    bool try_parse_value(const rapidjson::Value& root, v3f& v) noexcept;
    bool try_parse_value(const rapidjson::Value& root, v4f& v) noexcept;

    bool try_parse_value(const rapidjson::Value& root, m2f& v) noexcept;
    bool try_parse_value(const rapidjson::Value& root, m3f& v) noexcept;
    bool try_parse_value(const rapidjson::Value& root, m4f& v) noexcept;

    bool try_parse_value(const rapidjson::Value& root, b2i& b) noexcept;
    bool try_parse_value(const rapidjson::Value& root, b3i& b) noexcept;

    bool try_parse_value(const rapidjson::Value& root, b2f& b) noexcept;
    bool try_parse_value(const rapidjson::Value& root, b3f& b) noexcept;

    bool try_parse_value(const rapidjson::Value& root, color& c) noexcept;

    bool try_parse_value(const rapidjson::Value& root, str& s) noexcept;
    bool try_parse_value(const rapidjson::Value& root, wstr& s) noexcept;
    bool try_parse_value(const rapidjson::Value& root, str16& s) noexcept;
    bool try_parse_value(const rapidjson::Value& root, str32& s) noexcept;
    bool try_parse_value(const rapidjson::Value& root, str_hash& s) noexcept;

    void add_common_schema_definitions(rapidjson::Document& schema);
}}

namespace e2d { namespace json_utils
{
    template < typename T >
    std::enable_if_t<
        std::is_integral<T>::value &&
        std::is_signed<T>::value, bool>
    try_parse_value(const rapidjson::Value& root, T& v) noexcept {
        if ( !root.IsNumber() || !root.IsInt() ) {
            return false;
        }
        const auto iv = root.GetInt();
        if ( iv < std::numeric_limits<T>::min() ) {
            return false;
        }
        if ( iv > std::numeric_limits<T>::max() ) {
            return false;
        }
        v = math::numeric_cast<T>(iv);
        return true;
    }

    template < typename T >
    std::enable_if_t<
        std::is_integral<T>::value &&
        std::is_unsigned<T>::value, bool>
    try_parse_value(const rapidjson::Value& root, T& v) noexcept {
        i32 iv{0};
        if ( !try_parse_value(root, iv) || iv < 0 ) {
            return false;
        }
        v = math::numeric_cast<T>(iv);
        return true;
    }

    template < typename T >
    std::enable_if_t<
        std::is_floating_point<T>::value, bool>
    try_parse_value(const rapidjson::Value& root, T& v) noexcept {
        if ( !root.IsNumber() ) {
            return false;
        }
        v = math::numeric_cast<T>(root.GetDouble());
        return true;
    }

    template < typename T >
    bool try_parse_values(
        const rapidjson::Value& root,
        vector<T>& v,
        const T& dv = T())
    {
        E2D_ASSERT(root.IsArray());
        vector<T> tv(root.Size(), dv);
        for ( rapidjson::SizeType i = 0; i < root.Size(); ++i ) {
            if ( !try_parse_value(root[i], tv[i]) ) {
                return false;
            }
        }
        v = std::move(tv);
        return true;
    }
}}
