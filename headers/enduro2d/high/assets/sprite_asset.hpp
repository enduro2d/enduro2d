/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../library.hpp"
#include "../sprite.hpp"

namespace e2d
{
    class sprite_asset final : public content_asset<sprite_asset, sprite> {
    public:
        static load_async_result load_async(const library& library, str_view address);
    };
}
