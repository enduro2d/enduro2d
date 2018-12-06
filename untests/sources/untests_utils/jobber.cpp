/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("jobber") {
    {
        jobber j(1);
        auto pv0 = j.async([](){
            throw exception();
        });
        REQUIRE_THROWS_AS(pv0.get(), exception);
    }
    {
        i32 v5 = 5;

        jobber j(1);
        auto pv0 = j.async([](i32 v){
            REQUIRE(v == 5);
            throw exception();
        }, v5);
        REQUIRE_THROWS_AS(pv0.get(), exception);

        auto pv1 = j.async([](int& v){
            REQUIRE(v == 5);
            return v != 5
                ? 0
                : throw exception();
        }, std::ref(v5));
        REQUIRE_THROWS_AS(pv1.get(), exception);

        auto pv3 = j.async([](int& v){
            v = 4;
            return v;
        }, std::ref(v5));
        REQUIRE(pv3.get() == v5);
        REQUIRE(v5 == 4);
    }
    {
        jobber j(1);
        auto p0 = j.async([](rad<f32> angle){
            return math::sin(angle);
        }, math::pi<f32>());
        auto p1 = j.async([](rad<f32> angle){
            return math::cos(angle);
        }, math::two_pi<f32>());
        REQUIRE(math::approximately(p0.get(), 0.f));
        REQUIRE(math::approximately(p1.get(), 1.f));
    }
    {
        jobber j(1);
        j.pause();
        jobber::priority max_priority = jobber::priority::highest;
        j.async([](){
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        });
        for ( std::size_t i = 0; i < 10; ++i ) {
            jobber::priority p = static_cast<jobber::priority>(
                i % static_cast<std::size_t>(jobber::priority::highest));
            j.async(p, [&max_priority](jobber::priority priority) {
                REQUIRE(priority <= max_priority);
                max_priority = priority;
            }, p);
        }
        j.resume();
        j.wait_all();
    }
    {
        jobber j(1);
        i32 counter = 0;
        j.pause();
        for ( std::size_t i = 0; i < 10; ++i ) {
            j.async([&counter](){
                ++counter;
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            });
        }

        const auto b = time::now_ms();

        j.wait_for(make_milliseconds(15));
        REQUIRE(time::now_ms() - b > make_milliseconds<i64>(10));
        REQUIRE(counter == 0);

        j.wait_until(time::now_ms() + make_milliseconds<i64>(15));
        REQUIRE(time::now_ms() - b > make_milliseconds<i64>(20));
        REQUIRE(counter == 0);

        j.active_wait_for(make_milliseconds(60));
        REQUIRE(time::now_ms() - b > make_milliseconds<i64>(70));
        REQUIRE(counter > 2);
        REQUIRE(counter < 10);

        j.active_wait_until(time::now_s() + make_seconds<i64>(1));
        REQUIRE(counter == 10);
    }
    {
        jobber j(2);
        jobber g(2);

        std::vector<std::future<f32>> jp(50);
        for ( std::size_t i = 0; i < jp.size(); ++i ) {
            jp[i] = j.async([&g](){
                std::vector<std::future<f32>> gp(50);
                for ( std::size_t ii = 0; ii < gp.size(); ++ii ) {
                    gp[ii] = g.async([](rad<f32> angle){
                        return math::sin(angle);
                    }, make_rad(ii).cast_to<f32>());
                }
                return std::accumulate(gp.begin(), gp.end(), 0.f,
                    [](f32 r, std::future<f32>& f){
                        return r + f.get();
                    });
            });
        }
        f32 r0 = std::accumulate(jp.begin(), jp.end(), 0.f,
            [](f32 r, std::future<f32>& f){
                return r + f.get();
            });
        f32 r1 = 0.f;
        for ( std::size_t i = 0; i < 50; ++i ) {
            r1 += math::sin(make_rad(i).cast_to<f32>());
        }
        REQUIRE(math::approximately(r0, r1 * 50.f));
    }
    SECTION("performance") {
        std::printf("-= jobber::performance tests =-\n");
    #if defined(E2D_BUILD_MODE) && E2D_BUILD_MODE == E2D_BUILD_MODE_DEBUG
        const std::size_t task_n = 100'000;
    #else
        const std::size_t task_n = 1'000'000;
    #endif
        const auto big_task = [](std::size_t b, std::size_t n){
            f32 result = 0.f;
            for ( std::size_t i = 0; i < n; ++i ) {
                result += math::sin(make_rad(b + i).cast_to<f32>());
                result += math::cos(make_rad(b + i).cast_to<f32>());
            }
            return result;
        };
        const auto multi_task = [&](u32 threads, std::size_t tasks){
            e2d_untests::verbose_profiler_ms p(
                str("cores: ") + std::to_string(threads) +
                str(", tasks: ") + std::to_string(tasks));
            jobber j(threads);
            std::vector<std::future<f32>> rs(tasks);
            for ( std::size_t i = 0; i < tasks; ++i ) {
                const std::size_t n = task_n / tasks;
                rs[i] = j.async(big_task, i * n, n);
            }
            f32 result = std::accumulate(rs.begin(), rs.end(), 0.f,
                [](f32 r, std::future<f32>& f){
                    return r + f.get();
                });
            p.done(result);
        };
        for ( u32 i = 1; i <= 4; ++i ) {
            multi_task(i, 10);
            multi_task(i, 100);
            multi_task(i, 1000);
        }
    }
}
