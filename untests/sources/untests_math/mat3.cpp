/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("mat3") {
    {
        {
            auto m = m3i::identity();
            REQUIRE(m.rows[0][0] == 1);
            REQUIRE(m[2][2] == 1);
            REQUIRE((m[0][0] == m[1][1] && m[1][1] == m[2][2]));
            REQUIRE((m.data()[0] == m.data()[4] && m.data()[4] == m.data()[8]));
            REQUIRE((m.data()[8] == 1));
            REQUIRE((m[0][1] == 0 && m[0][2] == 0));
            REQUIRE((m[1][0] == 0 && m[1][2] == 0));
            REQUIRE((m.data()[1] == 0 && m.data()[2] == 0 && m.data()[3] == 0));
            REQUIRE((m.data()[5] == 0 && m.data()[6] == 0 && m.data()[7] == 0));
            m[0] = v3i::unit(); m[1] = v3i::unit() * 2;
            REQUIRE((m[0] == v3i::unit() && m[1] == v3i::unit() * 2));
            m[1][2] = 3; m[2][1] = 4;
            REQUIRE((m.rows[1][2] == 3 && m.rows[2][1] == 4));
            REQUIRE((m.data()[1*3+2] == 3 && m.data()[2*3+1] == 4));
        }
        REQUIRE(math::make_scale_matrix3(1,1,1) == math::make_scale_matrix3(v3i::unit()));
        REQUIRE(math::make_scale_matrix3(v2i(1,1),1) == math::make_scale_matrix3(v3i::unit()));

        REQUIRE(math::make_rotation_matrix3(make_rad(1.f),1.f,1.f,1.f) == math::make_rotation_matrix3(make_rad(1.f), v3f::unit()));
        REQUIRE(math::make_rotation_matrix3(make_rad(1.f),v2f(1.f,1.f),1.f) == math::make_rotation_matrix3(make_rad(1.f), v3f::unit()));
    }
    {
        REQUIRE(m3f(m3f::identity()) == m3f::identity());
        m3f m0(m3f::identity());
        m0 = m3f::zero();
        REQUIRE(m0 == m3f::zero());

        REQUIRE(-m3f::identity() == math::make_scale_matrix3(1.f, 1.f, 1.f) * -1.f);

        REQUIRE(math::make_scale_matrix3(1.1f, 2.2f, 3.3f).cast_to<i32>() ==
                math::make_scale_matrix3(1, 2, 3));
    }
    {
        REQUIRE(m3f() == m3f(1, 0, 0,
                             0, 1, 0,
                             0, 0, 1));

        REQUIRE(math::make_scale_matrix3(2.f,3.f,4.f) + 2.f == m3f(4, 2, 2,
                                                                   2, 5, 2,
                                                                   2, 2, 6));

        REQUIRE(math::make_scale_matrix3(2, 3, 4) * 2 == m3i(4, 0, 0,
                                                             0, 6, 0,
                                                             0, 0, 8));

        m3f m0 = math::make_scale_matrix3(2.f, 3.f, 4.f);
        m0 += 2.f;
        REQUIRE(m0 == math::make_scale_matrix3(2.f, 3.f, 4.f) + 2.f);

        m3f m2 = math::make_scale_matrix3(2.f, 3.f, 4.f);
        m2 *= 2.f;
        REQUIRE(m2 == math::make_scale_matrix3(2.f, 3.f, 4.f) * 2.f);
    }
    {
        REQUIRE(math::make_scale_matrix3(2, 3, 4) +
                math::make_scale_matrix3(2, 3, 4) ==
                math::make_scale_matrix3(2, 3, 4) * 2);

        REQUIRE(math::make_scale_matrix3(2, 3, 4) *
                math::make_scale_matrix3(2, 3, 4) ==
                math::make_scale_matrix3(4, 9, 16));

        m3f m0 = math::make_scale_matrix3(2.f, 3.f, 4.f);
        m0 += math::make_scale_matrix3(2.f, 3.f, 4.f);
        REQUIRE(m0 == math::make_scale_matrix3(2, 3, 4).cast_to<f32>() * 2.f);

        m3f m2 = math::make_scale_matrix3(2.f, 3.f, 4.f);
        m2 *= math::make_scale_matrix3(2.f, 3.f, 4.f);
        REQUIRE(m2 == math::make_scale_matrix3(4, 9, 16).cast_to<f32>());
    }
    {
        REQUIRE(v3f(2,3,4) * math::make_scale_matrix3(3.f,4.f,5.f) == v3f(6.f,12.f,20.f));
        REQUIRE(math::approximately(
            v3f(20,30,10) * math::make_rotation_matrix3(math::half_pi<f32>(), v3f::unit_z()),
            v3f(30.f,-20.f,10)));

        REQUIRE(v3f(2,3,4) * math::make_scale_matrix3(3.f,4.f,5.f) == v3f(6.f,12.f,20.f));
        REQUIRE(math::approximately(
            v3f(20,30,10) * math::make_rotation_matrix3(math::half_pi<f32>(), v3f::unit_z()),
            v3f(30.f,-20.f,10)));
    }
    {
        auto v3 = v3f(2,3,4);
        auto sm = math::make_scale_matrix3(2.f, 3.f, 4.f);
        REQUIRE(v3 * sm == v3f(2*2, 3*3, 4*4));
        REQUIRE(v3 * sm == v3 * sm);
    }
    {
        REQUIRE(v3f::unit_x() * math::make_rotation_matrix3(math::half_pi<f32>(), v3f::unit_z()) == v3f(0,-1,0));
        REQUIRE(-v3f::unit_x() * math::make_rotation_matrix3(math::half_pi<f32>(), v3f::unit_z()) == v3f(0,1,0));
    }
    {
        REQUIRE(math::transposed(m3f::identity()) == m3f::identity());
        REQUIRE(math::transposed(m3i( 1, 2, 3,
                                      5, 6, 7,
                                      9,10,11)) == m3i(1,5, 9,
                                                       2,6,10,
                                                       3,7,11));
    }
    {
        REQUIRE(
            math::inversed(math::make_scale_matrix3(2.f,4.f)).first ==
            math::make_scale_matrix3(0.5f, 0.25f));

        auto m0 = math::make_scale_matrix3(2.f,4.f,5.f) *
                  math::make_rotation_matrix3(make_rad(1.f), v3f::unit_z());
        REQUIRE(math::inversed(m0).first * m0 == m3f::identity());
        REQUIRE(m0 * math::inversed(m0).first == m3f::identity());

        REQUIRE(math::inversed(m3f::zero()).second == false);
        REQUIRE(math::inversed(m3f::identity()).second == true);
    }
    {
        -m3i::identity();
        m3i::identity() + 2;
        m3i::identity() * 2;
        m3i::identity() + m3i::identity();
        m3i::identity() * m3i::identity();
        v3f::unit() * m3f::identity();
        v3i::unit() * m3i::identity();
        REQUIRE(m3i::identity() == m3i::identity());
        REQUIRE_FALSE(m3i::identity() != m3i::identity());
    }
}
