/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../atlas.hpp"
#include "../library.hpp"

namespace e2d
{
    class atlas_asset final : public content_asset<atlas_asset, atlas> {
    public:
        atlas_asset(content_type content)
        : content_asset<atlas_asset, atlas>(std::move(content)) {}
        static load_async_result load_async(library& library, str_view address);
    };
}
