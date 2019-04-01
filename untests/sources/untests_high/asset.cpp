/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
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
                        .without_graphics(true)));
        }

        ~safe_starter_initializer() noexcept {
            modules::shutdown<starter>();
        }
    };

    class fake_asset final : public content_asset<fake_asset, int> {
    public:
        static load_async_result load_async(const library& library, str_view address) {
            E2D_UNUSED(library);
            return address == "42"
                ? stdex::make_resolved_promise(fake_asset::create(42, {
                    {"21", fake_asset::create(21)},
                    {"84", fake_asset::create(84)}}))
                : stdex::make_rejected_promise<load_result>(asset_loading_exception());
        }
    };
}

TEST_CASE("asset"){
    safe_starter_initializer initializer;
    library& l = the<library>();
    {
        REQUIRE_FALSE(l.load_asset<fake_asset>("none"));

        auto fa = l.load_asset<fake_asset>("42");
        REQUIRE(fa);
        REQUIRE(fa->content() == 42);

        REQUIRE_FALSE(fa->find_nested_asset("none"));
        REQUIRE(fa->find_nested_asset("21"));
        REQUIRE_FALSE(fa->find_nested_asset<binary_asset>("21"));
        REQUIRE(fa->find_nested_asset<fake_asset>("21"));
        REQUIRE(fa->find_nested_asset<fake_asset>("21")->content() == 21);
        REQUIRE(fa->find_nested_asset<fake_asset>("84")->content() == 84);
    }
}
