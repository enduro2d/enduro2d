/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

namespace
{
    using namespace e2d;

    class mul_module : public module<mul_module> {
    public:
        virtual u32 act(u32 v) const noexcept = 0;
        virtual u32 multiplier() const noexcept = 0;
    };

    class mul_module_impl : public mul_module {
    public:
        mul_module_impl(u32 m) : m_(m) {}
        u32 act(u32 v) const noexcept { return v * m_; }
        u32 multiplier() const noexcept { return m_; }
        void multiplier(u32 v) noexcept { m_ = v; }
    private:
        u32 m_;
    };

    class empty_module : public module<empty_module> {
    };

    class empty_module_impl : public empty_module {
    };
}

TEST_CASE("module") {
    {
        REQUIRE_FALSE(module<mul_module>::is_initialized());
        REQUIRE_FALSE(module<mul_module_impl>::is_initialized());

        REQUIRE_THROWS_AS(module<mul_module>::instance(), module_not_initialized);
        REQUIRE_THROWS_AS(module<mul_module_impl>::instance(), module_not_initialized);

        REQUIRE_NOTHROW(module<mul_module>::initialize<mul_module_impl>(2));
        REQUIRE_THROWS_AS(module<mul_module>::initialize<mul_module_impl>(1), module_already_initialized);

        REQUIRE(module<mul_module>::is_initialized());
        REQUIRE_FALSE(module<mul_module_impl>::is_initialized());

        REQUIRE_NOTHROW(module<mul_module>::instance());
        REQUIRE_THROWS_AS(module<mul_module_impl>::instance(), module_not_initialized);

        REQUIRE(module<mul_module>::instance().act(10) == 20);
        REQUIRE(module<mul_module>::instance().multiplier() == 2);

        REQUIRE_NOTHROW(module<mul_module>::shutdown());

        REQUIRE_FALSE(module<mul_module>::is_initialized());
        REQUIRE_FALSE(module<mul_module_impl>::is_initialized());

        REQUIRE_THROWS_AS(module<mul_module>::instance(), module_not_initialized);
        REQUIRE_THROWS_AS(module<mul_module_impl>::instance(), module_not_initialized);
    }
    {
        REQUIRE_FALSE(modules::is_initialized<mul_module>());
        REQUIRE_FALSE(modules::is_initialized<mul_module_impl>());

        REQUIRE_THROWS_AS(modules::instance<mul_module>(), module_not_initialized);
        REQUIRE_THROWS_AS(modules::instance<mul_module_impl>(), module_not_initialized);

        REQUIRE_NOTHROW(modules::initialize<mul_module_impl>(3));

        REQUIRE(modules::is_initialized<mul_module>());
        REQUIRE(modules::is_initialized<mul_module_impl>());
        REQUIRE_NOTHROW(modules::instance<mul_module>());
        REQUIRE_NOTHROW(modules::instance<mul_module_impl>());

        REQUIRE(modules::instance<mul_module>().act(10) == 30);
        REQUIRE(modules::instance<mul_module_impl>().act(20) == 60);

        REQUIRE(modules::instance<mul_module>().multiplier() == 3);
        REQUIRE(modules::instance<mul_module_impl>().multiplier() == 3);

        REQUIRE_NOTHROW(modules::instance<mul_module_impl>().multiplier(5));

        REQUIRE(modules::instance<mul_module>().act(10) == 50);
        REQUIRE(modules::instance<mul_module_impl>().act(20) == 100);

        REQUIRE(modules::instance<mul_module>().multiplier() == 5);
        REQUIRE(modules::instance<mul_module_impl>().multiplier() == 5);

        REQUIRE_NOTHROW(modules::shutdown<mul_module_impl>());

        REQUIRE_FALSE(modules::is_initialized<mul_module>());
        REQUIRE_FALSE(modules::is_initialized<mul_module_impl>());

        REQUIRE_THROWS_AS(modules::instance<mul_module>(), module_not_initialized);
        REQUIRE_THROWS_AS(modules::instance<mul_module_impl>(), module_not_initialized);
    }
    {
        REQUIRE_FALSE(modules::is_initialized<mul_module>());
        REQUIRE_FALSE(modules::is_initialized<empty_module>());
        REQUIRE_FALSE(modules::is_initialized<mul_module, empty_module>());

        REQUIRE_NOTHROW(modules::initialize<mul_module_impl>(3));

        REQUIRE(modules::is_initialized<mul_module>());
        REQUIRE_FALSE(modules::is_initialized<empty_module>());
        REQUIRE_FALSE(modules::is_initialized<mul_module, empty_module>());

        REQUIRE_NOTHROW(modules::initialize<empty_module_impl>());

        REQUIRE(modules::is_initialized<mul_module>());
        REQUIRE(modules::is_initialized<empty_module>());
        REQUIRE(modules::is_initialized<mul_module, empty_module>());

        REQUIRE_NOTHROW(modules::shutdown<empty_module, mul_module_impl>());

        REQUIRE_FALSE(modules::is_initialized<mul_module>());
        REQUIRE_FALSE(modules::is_initialized<empty_module>());
        REQUIRE_FALSE(modules::is_initialized<mul_module, empty_module>());
    }
}
