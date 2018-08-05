/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

TEST_CASE("mat2") {
    {
        {
            auto m = m2i::identity();
            REQUIRE(m.rows[0][0] == 1);
            REQUIRE(m[1][1] == 1);
            REQUIRE((m[0][0] == m[1][1]));
            REQUIRE((m.data()[0] == m.data()[3]));
            REQUIRE((m.data()[3] == 1));
            REQUIRE((m[0][1] == 0 && m[1][0] == 0));
            REQUIRE((m.data()[1] == 0 && m.data()[2] == 0));
            m[0] = v2i::unit(); m[1] = v2i::unit() * 2;
            REQUIRE((m[0] == v2i::unit() && m[1] == v2i::unit() * 2));
            m[0][1] = 3; m[1][0] = 4;
            REQUIRE((m.rows[0][1] == 3 && m.rows[1][0] == 4));
            REQUIRE((m.data()[0*2+1] == 3 && m.data()[1*2+0] == 4));
        }
        REQUIRE(math::make_scale_matrix2(1,1) == math::make_scale_matrix2(v2i::unit()));
        REQUIRE(math::make_scale_matrix2(v2i(1,1)) == math::make_scale_matrix2(v2i::unit()));
    }
    {
        REQUIRE(m2f(m2f::identity()) == m2f::identity());
        m2f m0(m2f::identity());
        m0 = m2f::zero();
        REQUIRE(m0 == m2f::zero());

        REQUIRE(-m2f::identity() == math::make_scale_matrix2(1.f, 1.f) * -1.f);

        REQUIRE(math::make_scale_matrix2(1.1f, 2.2f).cast_to<i32>() ==
                math::make_scale_matrix2(1, 2));
    }
    {
        REQUIRE(m2f() == m2f(1, 0,
                             0, 1));

        REQUIRE(math::make_scale_matrix2(2.f,3.f) + 2.f == m2f(4, 2,
                                                               2, 5));

        REQUIRE(math::make_scale_matrix2(2, 3) * 2 == m2i(4, 0,
                                                          0, 6));

        m2f m0 = math::make_scale_matrix2(2.f, 3.f);
        m0 += 2.f;
        REQUIRE(m0 == math::make_scale_matrix2(2.f, 3.f) + 2.f);

        m2f m2 = math::make_scale_matrix2(2.f, 3.f);
        m2 *= 2.f;
        REQUIRE(m2 == math::make_scale_matrix2(2.f, 3.f) * 2.f);
    }
    {
        REQUIRE(math::make_scale_matrix2(2, 3) +
                math::make_scale_matrix2(2, 3) ==
                math::make_scale_matrix2(2, 3) * 2);

        REQUIRE(math::make_scale_matrix2(2, 3) *
                math::make_scale_matrix2(2, 3) ==
                math::make_scale_matrix2(4, 9));

        m2f m0 = math::make_scale_matrix2(2.f, 3.f);
        m0 += math::make_scale_matrix2(2.f, 3.f);
        REQUIRE(m0 == math::make_scale_matrix2(2, 3).cast_to<f32>() * 2.f);

        m2f m2 = math::make_scale_matrix2(2.f, 3.f);
        m2 *= math::make_scale_matrix2(2.f, 3.f);
        REQUIRE(m2 == math::make_scale_matrix2(4, 9).cast_to<f32>());
    }
    {
        REQUIRE(v2f(2,3) * math::make_scale_matrix2(3.f,4.f) == v2f(6,12));
    }
    {
        REQUIRE(v2f(2,3) * math::make_scale_matrix2(3.f,4.f) == v2f(6,12));
        REQUIRE(math::approximately(
            v2f(20,30) * math::make_rotation_matrix2(math::half_pi<f32>()),
            v2f(30.f,-20.f)));

        REQUIRE(v2f(2,3) * math::make_scale_matrix2(3.f,4.f) == v2f(6.f,12.f));
        REQUIRE(math::approximately(
            v2f(20,30) * math::make_rotation_matrix2(math::half_pi<f32>()),
            v2f(30.f,-20.f)));

        REQUIRE(v2f(2,3) * math::make_scale_matrix2(3.f,4.f) == v2f(6.f,12.f));
        REQUIRE(math::approximately(
            v2f(20,30) * math::make_rotation_matrix2(math::half_pi<f32>()),
            v2f(30.f,-20.f)));
    }
    {
        auto v2 = v2f(2,3);
        auto sm = math::make_scale_matrix2(2.f, 3.f);
        REQUIRE(v2 * sm == v2f(2*2, 3*3));
        REQUIRE(v2 * sm == v2 * sm);
    }
    {
        REQUIRE(v2f::unit_x() * math::make_rotation_matrix2(math::half_pi<f32>()) == v2f(0,-1));
        REQUIRE(-v2f::unit_x() * math::make_rotation_matrix2(math::half_pi<f32>()) == v2f(0,1));
    }
    {
        REQUIRE(math::transposed(m2f::identity()) == m2f::identity());
        REQUIRE(math::transposed(m2i( 1, 2,
                                      5, 6)) == m2i(1,5,
                                                    2,6));
    }
    {
        REQUIRE(
            math::inversed(math::make_scale_matrix2(2.f,4.f)).first ==
            math::make_scale_matrix2(0.5f, 0.25f));

        auto m0 = math::make_scale_matrix2(2.f,4.f) *
                  math::make_rotation_matrix2(make_rad(1.f));
        REQUIRE(math::inversed(m0).first * m0 == m2f::identity());
        REQUIRE(m0 * math::inversed(m0).first == m2f::identity());

        REQUIRE(math::inversed(m2f::zero()).second == false);
        REQUIRE(math::inversed(m2f::identity()).second == true);
    }
    {
        -m2i::identity();
        m2i::identity() + 2;
        m2i::identity() * 2;
        m2i::identity() + m2i::identity();
        m2i::identity() * m2i::identity();
        v2f::unit() * m2f::identity();
        v2f::unit() * m2f::identity();
        v2i::unit() * m2i::identity();
        REQUIRE(m2i::identity() == m2i::identity());
        REQUIRE_FALSE(m2i::identity() != m2i::identity());
    }
}
