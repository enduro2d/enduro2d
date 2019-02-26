/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"
#include "../library.hpp"

#include "model.hpp"

namespace e2d
{
    class model_asset final : public content_asset<model_asset, model> {
    public:
        using content_asset<model_asset, model>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };
}
