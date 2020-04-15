/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../library.hpp"
#include "../resources/button_style.hpp"

namespace e2d
{
    class button_style_asset final : public content_asset<button_style_asset, button_style> {
    public:
        static const char* type_name() noexcept { return "button_style_asset"; }
        static load_async_result load_async(const library& library, str_view address);
    };
}
