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
            return v2f.new((v + v + 2).y)
        )lua");
        v3f r1 = l.lua().script(R"lua(
            local v = v3f.new(1,2,3)
            return v3f.new((v + v + 2).y)
        )lua");
        v4f r2 = l.lua().script(R"lua(
            local v = v4f.new(1,2,3,4)
            return v4f.new((v + v + 2).y)
        )lua");
        REQUIRE(r0 == v2f(6));
        REQUIRE(r1 == v3f(6));
        REQUIRE(r2 == v4f(6));
    }

    SECTION("quat") {
        v3f r0 = l.lua().script(R"lua(
            return v3f.new(1,2,3) * q4f.make_quat_from_axis_angle(radf.new(10), v3f.new(1,2,3))
        )lua");
        REQUIRE(r0 == v3f(1,2,3) * math::make_quat_from_axis_angle(radf(10.f), v3f(1,2,3)));
    }
}
