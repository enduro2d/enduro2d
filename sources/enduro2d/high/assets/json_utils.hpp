/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
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

    bool try_parse_value(const rapidjson::Value& root, b2i& b) noexcept;
    bool try_parse_value(const rapidjson::Value& root, b3i& b) noexcept;

    bool try_parse_value(const rapidjson::Value& root, b2f& b) noexcept;
    bool try_parse_value(const rapidjson::Value& root, b3f& b) noexcept;

    bool try_parse_value(const rapidjson::Value& root, color& c) noexcept;

    void add_common_schema_definitions(rapidjson::Document& schema);
}}
