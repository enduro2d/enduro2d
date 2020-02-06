/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_core.hpp"
using namespace e2d;

namespace
{
    using namespace e2d;

    class test_sink final : public debug::sink {
    public:
        str on_message_acc;
        static str s_on_message_acc;

        bool on_message(debug::level lvl, str_view text) noexcept final {
            E2D_UNUSED(lvl);
            on_message_acc.append(text.cbegin(), text.cend());
            s_on_message_acc.append(text.cbegin(), text.cend());
            return true;
        }
    };
    str test_sink::s_on_message_acc;
}

TEST_CASE("debug"){
    {
        debug d;
        test_sink& s = d.register_sink<test_sink>();
        REQUIRE(test_sink::s_on_message_acc.empty());
        d.trace("h");
        d.warning("e");
        REQUIRE(test_sink::s_on_message_acc == "he");
        d.set_min_level(debug::level::error);
        d.trace("el");
        d.warning("lo");
        REQUIRE(test_sink::s_on_message_acc == "he");
        d.error("ll");
        REQUIRE(test_sink::s_on_message_acc == "hell");
        d.fatal("o");
        REQUIRE(test_sink::s_on_message_acc == "hello");
        d.unregister_sink(s);
        d.fatal("!!!");
        REQUIRE(test_sink::s_on_message_acc == "hello");
    }
    {
        modules::initialize<debug>();
        test_sink& s1 = the<debug>().register_sink_ex<test_sink>(debug::level::warning);
        test_sink& s2 = the<debug>().register_sink_ex<test_sink>(debug::level::error);
        REQUIRE(s1.on_message_acc.empty());
        REQUIRE(s2.on_message_acc.empty());
        the<debug>().trace("w");
        REQUIRE(s1.on_message_acc.empty());
        REQUIRE(s2.on_message_acc.empty());
        the<debug>().warning("w");
        REQUIRE(s1.on_message_acc == "w");
        REQUIRE(s2.on_message_acc.empty());
        the<debug>().error("q");
        REQUIRE(s1.on_message_acc == "wq");
        REQUIRE(s2.on_message_acc == "q");
        the<debug>().fatal("e");
        REQUIRE(s1.on_message_acc == "wqe");
        REQUIRE(s2.on_message_acc == "qe");
        the<debug>().set_min_level(debug::level::fatal);
        the<debug>().error("r");
        REQUIRE(s1.on_message_acc == "wqe");
        REQUIRE(s2.on_message_acc == "qe");
        the<debug>().fatal("r");
        REQUIRE(s1.on_message_acc == "wqer");
        REQUIRE(s2.on_message_acc == "qer");
        modules::shutdown<debug>();
    }
}
