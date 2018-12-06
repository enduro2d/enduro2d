/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("scheduler") {
    {
        scheduler s;
        i32 counter = 0;
        s.schedule([&counter](){ ++counter; });
        REQUIRE(counter == 0);
        s.process_all_tasks();
        REQUIRE(counter == 1);
        s.schedule([&counter](){ ++counter; });
        REQUIRE(counter == 1);
        s.process_all_tasks();
        REQUIRE(counter == 2);
    }
    {
        scheduler s;
        i32 counter = 0;
        for ( std::size_t i = 0; i < 10; ++i ) {
            s.schedule([&counter](){
                ++counter;
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            });
        }
        s.process_all_tasks_for(make_milliseconds(-1));
        s.process_all_tasks_for(make_milliseconds(0));
        REQUIRE(counter == 0);
        s.process_all_tasks_for(make_milliseconds(60));
        REQUIRE(counter > 2);
        REQUIRE(counter < 10);
        s.process_all_tasks_for(make_seconds(1));
        REQUIRE(counter == 10);
    }
    {
        scheduler s;
        i32 counter = 0;
        for ( std::size_t i = 0; i < 10; ++i ) {
            s.schedule([&counter](){
                ++counter;
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            });
        }
        s.process_all_tasks_until(time::now_ms() - make_milliseconds<i64>(1));
        s.process_all_tasks_until(time::now_ms());
        REQUIRE(counter == 0);
        s.process_all_tasks_until(time::now_ms() + make_milliseconds<i64>(60));
        REQUIRE(counter > 2);
        REQUIRE(counter < 10);
        s.process_all_tasks_until(time::now_s() + make_seconds<i64>(1));
        REQUIRE(counter == 10);
    }
    {
        scheduler s;
        str accumulator;
        s.schedule(scheduler::priority::lowest, [](str& acc){
            acc.append("o");
        }, std::ref(accumulator));
        s.schedule(scheduler::priority::below_normal, [](str& acc){
            acc.append("l");
        }, std::ref(accumulator));
        s.schedule(scheduler::priority::highest, [](str& acc){
            acc.append("h");
        }, std::ref(accumulator));
        s.schedule(scheduler::priority::above_normal, [](str& acc){
            acc.append("e");
        }, std::ref(accumulator));
        s.schedule(scheduler::priority::normal, [](str& acc){
            acc.append("l");
        }, std::ref(accumulator));
        s.process_all_tasks();
        REQUIRE(accumulator == "hello");
    }
}
