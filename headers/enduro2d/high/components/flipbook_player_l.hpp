/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "flipbook_player.hpp"

namespace e2d
{
    template <>
    class component_loader<flipbook_player> {
    public:
        constexpr static const char*
        schema_source = R"json({
            "type" : "object",
            "required" : [],
            "additionalProperties" : false,
            "properties" : {}
        })json";

        bool operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            asset_dependencies& dependencies) const
        {
            return true;
        }

        flipbook_player operator()(
            str_view parent_address,
            const rapidjson::Value& root,
            const asset_group& dependencies) const
        {
            flipbook_player component;
            return component;
        }
    };
}
