/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_math.hpp"
using namespace e2d;

#include <random>

TEST_CASE("quat") {
    {
        REQUIRE(q4f() == q4f{0,0,0,1});
        REQUIRE(q4f::zero() == q4f{0,0,0,0});
        REQUIRE(q4f::identity() == q4f{0,0,0,1});
        REQUIRE(q4i(1,2,3,4).x == 1);
        REQUIRE(q4i(1,2,3,4).y == 2);
        REQUIRE(q4i(1,2,3,4).z == 3);
        REQUIRE(q4i(1,2,3,4).w == 4);
        REQUIRE(q4i(v4i{1,2,3,4}) == q4i(1,2,3,4));
    }
    {
        auto q = q4i(1,2,3,4);
        REQUIRE(q[0] == 1);
        REQUIRE(q[1] == 2);
        REQUIRE(q[2] == 3);
        REQUIRE(q[3] == 4);
        REQUIRE(q.data()[0] == 1);
        REQUIRE(q.data()[1] == 2);
        REQUIRE(q.data()[2] == 3);
        REQUIRE(q.data()[3] == 4);

        const auto qq = q;
        REQUIRE(qq[0] == 1);
        REQUIRE(qq[1] == 2);
        REQUIRE(qq[2] == 3);
        REQUIRE(qq[3] == 4);
        REQUIRE(qq.data()[0] == 1);
        REQUIRE(qq.data()[1] == 2);
        REQUIRE(qq.data()[2] == 3);
        REQUIRE(qq.data()[3] == 4);
    }
    {
        auto q = q4i(1,2,3,4);
        q += 2;
        REQUIRE(q == q4i{3,4,5,6});
        q *= 2;
        REQUIRE(q == q4i{6,8,10,12});
        q += q4i{1,2,3,4};
        REQUIRE(q == q4i{7,10,13,16});
    }
    {
        auto q = q4i(1,2,3,4);
        auto qu = q.cast_to<u32>();
        REQUIRE(qu == q4u{1,2,3,4});
    }
    {
        REQUIRE(make_quat(1,2,3,4) == q4i(1,2,3,4));
        REQUIRE(make_quat(v4i(1,2,3,4)) == q4i(1,2,3,4));
    }
    {
        REQUIRE(q4i(1,2,3,4) == q4i(1,2,3,4));
        REQUIRE_FALSE(q4i(1,2,3,4) != q4i(1,2,3,4));

        REQUIRE_FALSE(q4i(1,2,3,4) == q4i(0,2,3,4));
        REQUIRE_FALSE(q4i(1,2,3,4) == q4i(1,3,3,4));
        REQUIRE_FALSE(q4i(1,2,3,4) == q4i(1,2,2,4));
        REQUIRE_FALSE(q4i(1,2,3,4) == q4i(1,2,3,5));

        REQUIRE(q4i(1,2,3,4) != q4i(0,2,3,4));
        REQUIRE(q4i(1,2,3,4) != q4i(1,3,3,4));
        REQUIRE(q4i(1,2,3,4) != q4i(1,2,2,4));
        REQUIRE(q4i(1,2,3,4) != q4i(1,2,3,5));
    }
    {
        auto q1 = math::make_quat_from_axis_angle(make_rad(11.f), v3f::unit_z());
        auto q2 = math::make_quat_from_axis_angle(make_rad(12.f), v3f::unit_z());
        auto q3 = math::make_quat_from_axis_angle(make_rad(23.f), v3f::unit_z());
        REQUIRE(q1 * q2 == q3);
    }
    {
        {
            auto q1 = math::make_quat_from_euler_angles(make_rad(2.f), make_rad(0.f), make_rad(0.f));
            auto q2 = math::make_quat_from_euler_angles(make_rad(4.f), make_rad(0.f), make_rad(0.f));
            auto q3 = math::make_quat_from_euler_angles(make_rad(6.f), make_rad(0.f), make_rad(0.f));
            REQUIRE(q1 * q2 == q3);
        }
        {
            auto q1 = math::make_quat_from_euler_angles(make_rad(0.f), make_rad(3.f), make_rad(0.f));
            auto q2 = math::make_quat_from_euler_angles(make_rad(0.f), make_rad(4.f), make_rad(0.f));
            auto q3 = math::make_quat_from_euler_angles(make_rad(0.f), make_rad(7.f), make_rad(0.f));
            REQUIRE(q1 * q2 == q3);
        }
        {
            auto q1 = math::make_quat_from_euler_angles(make_rad(0.f), make_rad(0.f), make_rad(1.f));
            auto q2 = math::make_quat_from_euler_angles(make_rad(0.f), make_rad(0.f), make_rad(5.f));
            auto q3 = math::make_quat_from_euler_angles(make_rad(0.f), make_rad(0.f), make_rad(6.f));
            REQUIRE(q1 * q2 == q3);
        }
    }
    {
        {
            auto q = math::make_quat_from_euler_angles(make_rad(0.f), make_rad(0.f), make_deg(90.f));
            auto qq = math::make_quat_from_euler_angles(make_rad(0.f), make_rad(0.f), make_deg(-90.f));
            REQUIRE(v3f(1,1,0) * q == v3f(-1,1,0));
            REQUIRE(v3f(1,1,0) * qq == v3f(1,-1,0));
        }
        {
            auto q = math::make_quat_from_euler_angles(make_rad(0.f), make_deg(90.f), make_deg(0.f));
            auto qq = math::make_quat_from_euler_angles(make_rad(0.f), make_deg(-90.f), make_deg(0.f));
            REQUIRE(v3f(1,1,0) * q == v3f(0,1,-1));
            REQUIRE(v3f(1,1,0) * qq == v3f(0,1,1));
        }
        {
            auto q = math::make_quat_from_euler_angles(make_deg(90.f), make_deg(0.f), make_deg(0.f));
            auto qq = math::make_quat_from_euler_angles(make_deg(-90.f), make_deg(0.f), make_deg(0.f));
            REQUIRE(v3f(1,1,0) * q == v3f(1,0,1));
            REQUIRE(v3f(1,1,0) * qq == v3f(1,0,-1));
        }
    }
    {
        const auto q1 = math::make_quat_from_euler_angles(make_rad(2.f), make_rad(0.f), make_rad(0.f));
        const auto q2 = math::make_quat_from_euler_angles(make_rad(4.f), make_rad(0.f), make_rad(0.f));
        REQUIRE(math::lerp(q1, q2, 0.0f) == q1);
        REQUIRE(math::lerp(q1, q2, 1.0f) == q2);
    }
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<f32> dis(-1000.f, 1000.f);
        for ( std::size_t i = 0; i < 1000; ++i ) {
            const auto q = math::make_quat_from_euler_angles(
                make_rad(dis(gen)),
                make_rad(dis(gen)),
                make_rad(dis(gen)));
            const auto v = v3f(dis(gen), dis(gen), dis(gen));
            const auto v1 = v * q;
            const auto v2 = v * math::make_rotation_matrix3(q);
            const auto v3 = v4f(v, 1.f) * math::make_rotation_matrix4(q);
            REQUIRE(math::approximately(v1, v2, 0.001f));
            REQUIRE(math::approximately(v2, v3f(v3), 0.001f));
        }
    }
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<f32> dis(-1000.f, 1000.f);
        for ( std::size_t i = 0; i < 1000; ++i ) {
            const auto angle = make_rad(dis(gen));
            const auto axis = math::normalized(make_vec3(dis(gen), dis(gen), dis(gen)));
            const auto q = math::make_quat_from_axis_angle(angle, axis);

            const auto v = v3f(dis(gen), dis(gen), dis(gen));
            const auto v1 = v * q;
            const auto v2 = v * math::make_rotation_matrix3(q);
            const auto v3 = v4f(v, 1.f) * math::make_rotation_matrix4(q);

            REQUIRE(math::approximately(v1, v2, 0.001f));
            REQUIRE(math::approximately(v2, v3f(v3), 0.001f));
        }
    }
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<f32> dis(-1000.f, 1000.f);
        for ( std::size_t i = 0; i < 1000; ++i ) {
            const auto angle = make_rad(dis(gen));
            const auto axis = math::normalized(make_vec3(dis(gen), dis(gen), dis(gen)));

            const auto q = math::make_quat_from_axis_angle(angle, axis);
            const auto m1 = math::make_rotation_matrix3(angle, axis);
            const auto m2 = math::make_rotation_matrix4(q);

            const auto v = make_vec3(dis(gen), dis(gen), dis(gen));

            const auto v1 = v * q;
            const auto v2 = v * m1;
            const auto v3 = v4f(v, 1.f) * m2;

            REQUIRE(math::approximately(v1, v2, 0.001f));
            REQUIRE(math::approximately(v2, v3f(v3), 0.001f));
        }
    }
}
