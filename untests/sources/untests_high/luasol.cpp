/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

namespace
{
    class safe_starter_initializer final : private noncopyable {
    public:
        safe_starter_initializer() {
            modules::initialize<starter>(0, nullptr,
                starter::parameters(
                    engine::parameters("luasol_untests", "enduro2d")
                        .without_graphics(true)));
        }

        ~safe_starter_initializer() noexcept {
            modules::shutdown<starter>();
        }
    };
}

TEST_CASE("luasol") {
    safe_starter_initializer initializer;
    luasol& l = the<luasol>();
    SECTION("vec2/vec3/vec4") {
        v2f r0 = l.lua().script(R"lua(
            local v = v2f.new(1,2)
            return v2f.new((v + v).y)
        )lua");
        v3f r1 = l.lua().script(R"lua(
            local v = v3f.new(1,2,3)
            return v3f.new((v + v).y)
        )lua");
        v4f r2 = l.lua().script(R"lua(
            local v = v4f.new(1,2,3,4)
            return v4f.new((v + v).y)
        )lua");
        REQUIRE(r0 == v2f(4));
        REQUIRE(r1 == v3f(4));
        REQUIRE(r2 == v4f(4));
    }
}
