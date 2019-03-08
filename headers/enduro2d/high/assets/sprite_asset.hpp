/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../sprite.hpp"
#include "../library.hpp"

namespace e2d
{
    class sprite_asset final : public content_asset<sprite_asset, sprite> {
    public:
        sprite_asset(content_type content)
        : content_asset<sprite_asset, sprite>(std::move(content)) {}
        static load_async_result load_async(library& library, str_view address);
    };
}
