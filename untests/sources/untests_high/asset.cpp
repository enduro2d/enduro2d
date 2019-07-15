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
                    engine::parameters("asset_untests", "enduro2d")
                        .without_audio(true)
                        .without_graphics(true)));
        }

        ~safe_starter_initializer() noexcept {
            modules::shutdown<starter>();
        }
    };

    class fake_nested_asset final : public content_asset<fake_nested_asset, int> {
    public:
        static const char* type_name() noexcept { return "fake_nested_asset"; }
    };

    class fake_asset final : public content_asset<fake_asset, int> {
    public:
        static const char* type_name() noexcept { return "fake_asset"; }

        static load_async_result load_async(const library& library, str_view address) {
            E2D_UNUSED(library);
            return address == "42"
                ? stdex::make_resolved_promise(fake_asset::create(42, {
                    {"21", fake_nested_asset::create(21, {
                        {"2", fake_nested_asset::create(2)}
                    })},
                    {"84", fake_nested_asset::create(84, {
                        {"8", fake_nested_asset::create(8)}
                    })}
                }))
                : stdex::make_rejected_promise<load_result>(asset_loading_exception());
        }
    };
}

TEST_CASE("asset") {
    safe_starter_initializer initializer;
    library& l = the<library>();
    SECTION("nested_assets") {
        {
            REQUIRE_FALSE(l.load_asset<fake_asset>("none"));

            auto fa = l.load_asset<fake_asset>("42");
            REQUIRE(fa);
            REQUIRE(fa->content() == 42);

            REQUIRE_FALSE(fa->find_nested_asset("none"));
            REQUIRE(fa->find_nested_asset("21"));
            REQUIRE(fa->find_nested_asset("21:/2"));
            REQUIRE_FALSE(fa->find_nested_asset<binary_asset>("21"));
            REQUIRE_FALSE(fa->find_nested_asset<binary_asset>("21:/2"));
            REQUIRE(fa->find_nested_asset<fake_nested_asset>("21"));
            REQUIRE(fa->find_nested_asset<fake_nested_asset>("21:/2"));
            REQUIRE(fa->find_nested_asset<fake_nested_asset>("21")->content() == 21);
            REQUIRE(fa->find_nested_asset<fake_nested_asset>("21:/2")->content() == 2);
            REQUIRE(fa->find_nested_asset<fake_nested_asset>("84")->content() == 84);
            REQUIRE(fa->find_nested_asset<fake_nested_asset>("84:/8")->content() == 8);
        }
        {
            REQUIRE(l.load_asset<fake_asset, fake_nested_asset>("42:/21"));
            REQUIRE(l.load_asset<fake_asset, fake_nested_asset>("42:/21")->content() == 21);

            REQUIRE_FALSE(l.load_asset<fake_asset, binary_asset>("42:/21"));
            REQUIRE_FALSE(l.load_asset<fake_asset, fake_nested_asset>("42:/none"));
            REQUIRE_FALSE(l.load_asset<fake_asset, fake_nested_asset>("42:/none:/21"));

            REQUIRE(l.load_asset<fake_asset, fake_nested_asset>("42:/21:/2"));
            REQUIRE(l.load_asset<fake_asset, fake_nested_asset>("42:/21:/2")->content() == 2);

            REQUIRE_FALSE(l.load_asset<fake_asset, binary_asset>("42:/21:/2"));
            REQUIRE_FALSE(l.load_asset<fake_asset, fake_nested_asset>("42:/21:/none"));
            REQUIRE_FALSE(l.load_asset<fake_asset, fake_nested_asset>("42:/21:/none:/2"));
        }
    }
}
