/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

TEST_CASE("starter"){
    modules::initialize<starter>(0, nullptr,
        starter::parameters(
            engine::parameters("starter_untests", "enduro2d")));
    modules::shutdown<starter>();
}
