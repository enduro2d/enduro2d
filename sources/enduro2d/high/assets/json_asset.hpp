/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <enduro2d/high/library.hpp>

#include "json_utils.hpp"

namespace e2d
{
    class json_asset final : public content_asset<json_asset, rapidjson::Document> {
    public:
        using content_asset<json_asset, rapidjson::Document>::content_asset;
        static load_async_result load_async(const library& library, str_view address);
    };
}
