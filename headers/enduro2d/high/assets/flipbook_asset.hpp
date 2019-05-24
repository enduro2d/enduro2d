/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../library.hpp"
#include "../flipbook.hpp"

namespace e2d
{
    class flipbook_asset final : public content_asset<flipbook_asset, flipbook> {
    public:
        static const char* type_name() noexcept { return "flipbook_asset"; }
        static load_async_result load_async(const library& library, str_view address);
    };
}
