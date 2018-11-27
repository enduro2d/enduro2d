/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_high.hpp"
using namespace e2d;

TEST_CASE("library"){
    modules::initialize<vfs>();
    modules::initialize<debug>();
    modules::initialize<library>(url{"resources://bin/library"});
    modules::initialize<asset_cache<text_asset>>(the<library>());
    modules::initialize<asset_cache<image_asset>>(the<library>());
    modules::initialize<asset_cache<binary_asset>>(the<library>());
    {
        the<debug>().register_sink<debug_console_sink>();
        the<vfs>().register_scheme<filesystem_file_source>("file");
    }
    {
        str resources;
        REQUIRE(filesystem::extract_predef_path(
            resources,
            filesystem::predef_path::resources));
        REQUIRE(the<vfs>().register_scheme_alias(
            "resources",
            {"file", resources}));
    }
    {
        library& l = the<library>();

        auto text_res = l.load_asset<text_asset>("text_asset.txt");
        REQUIRE(text_res);
        REQUIRE(text_res->content() == "hello");

        auto binary_res = l.load_asset<binary_asset>("binary_asset.bin");
        REQUIRE(binary_res);
        REQUIRE(binary_res->content() == buffer("world", 5));

        auto empty_res = l.load_asset<binary_asset>("empty_asset");
        REQUIRE_FALSE(empty_res);

        auto binary_res_from_cache = l.load_asset<binary_asset>("binary_asset.bin");
        REQUIRE(binary_res == binary_res_from_cache);

        REQUIRE(the<asset_cache<text_asset>>().asset_count() == 1);
        REQUIRE(the<asset_cache<binary_asset>>().asset_count() == 1);

        the<asset_cache<text_asset>>().unload_unused_assets();
        the<asset_cache<binary_asset>>().unload_unused_assets();

        REQUIRE(the<asset_cache<text_asset>>().asset_count() == 1);
        REQUIRE(the<asset_cache<binary_asset>>().asset_count() == 1);

        text_res.reset();
        the<asset_cache<text_asset>>().unload_unused_assets();
        REQUIRE(the<asset_cache<text_asset>>().asset_count() == 0);
        REQUIRE(the<asset_cache<binary_asset>>().asset_count() == 1);

        binary_res.reset();
        the<asset_cache<binary_asset>>().unload_unused_assets();
        REQUIRE(the<asset_cache<binary_asset>>().asset_count() == 1);
        binary_res_from_cache.reset();
        the<asset_cache<binary_asset>>().unload_unused_assets();
        REQUIRE(the<asset_cache<binary_asset>>().asset_count() == 0);
    }
    {
        library& l = the<library>();
        auto image_res = l.load_asset<image_asset>("image.png");
        REQUIRE(image_res);
        REQUIRE(!image_res->content().empty());

        REQUIRE(the<asset_cache<image_asset>>().find("image.png"));
        REQUIRE(the<asset_cache<binary_asset>>().find("image.png"));

        the<asset_cache<binary_asset>>().unload_unused_assets();
        REQUIRE(the<asset_cache<image_asset>>().find("image.png"));
        REQUIRE_FALSE(the<asset_cache<binary_asset>>().find("image.png"));

        image_res.reset();
        the<asset_cache<image_asset>>().unload_unused_assets();
        REQUIRE_FALSE(the<asset_cache<image_asset>>().find("image.png"));
        REQUIRE_FALSE(the<asset_cache<binary_asset>>().find("image.png"));
    }
    modules::shutdown<asset_cache<binary_asset>>();
    modules::shutdown<asset_cache<image_asset>>();
    modules::shutdown<asset_cache<text_asset>>();
    modules::shutdown<library>();
    modules::shutdown<debug>();
    modules::shutdown<vfs>();
}
