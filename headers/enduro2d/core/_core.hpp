/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../base/_all.hpp"
#include "../math/_all.hpp"
#include "../utils/_all.hpp"

namespace e2d
{
    class debug;
}

namespace e2d
{
    template < typename ModuleT >
    ModuleT& the() {
        return modules::instance<ModuleT>();
    }
}
