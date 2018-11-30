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
    //modules::initialize<window>(v2u{640,480}, "", false);
    //modules::initialize<render>(the<debug>(), the<window>());
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
    {
        //library& l = the<library>();
        //auto shader_res = l.load_asset<shader_asset>("shader.json");
        //REQUIRE(shader_res);
        //REQUIRE(shader_res->content());
    }
    {
        library& l = the<library>();
        auto material_res = l.load_asset<material_asset>("material.json");
        REQUIRE(material_res);
        {
            const auto* sampler = material_res->content().properties().sampler("s");
            REQUIRE(sampler);
            REQUIRE(sampler->s_wrap() == render::sampler_wrap::clamp);
            REQUIRE(sampler->t_wrap() == render::sampler_wrap::repeat);
            REQUIRE(sampler->r_wrap() == render::sampler_wrap::mirror);
            REQUIRE(sampler->min_filter() == render::sampler_min_filter::linear_mipmap_linear);
            REQUIRE(sampler->mag_filter() == render::sampler_mag_filter::linear);
        }
        {
            const auto* property = material_res->content().properties().property("i");
            REQUIRE(property);
            REQUIRE(property->index() == 0);
            REQUIRE(stdex::get<i32>(*property) == 42);
        }

        REQUIRE(material_res->content().pass_count() == 1);
        const auto& pass = material_res->content().pass(0);
        //REQUIRE(pass.shader());

        {
            const auto* property = pass.properties().property("f");
            REQUIRE(property);
            REQUIRE(property->index() == 1);
            REQUIRE(math::approximately(stdex::get<f32>(*property), 4.2f));
        }
        {
            const auto* property = pass.properties().property("v1");
            REQUIRE(property);
            REQUIRE(property->index() == 2);
            REQUIRE(stdex::get<v2i>(*property) == v2i(1,2));
        }
        {
            const auto* property = pass.properties().property("v2");
            REQUIRE(property);
            REQUIRE(property->index() == 6);
            REQUIRE(stdex::get<v3f>(*property) == v3f(3.f));
        }
        {
            const auto* property = pass.properties().property("v3");
            REQUIRE(property);
            REQUIRE(property->index() == 4);
            REQUIRE(stdex::get<v4i>(*property) == v4i(1,2,3,4));
        }

        REQUIRE(pass.states().depth().range_near() == 1.f);
        REQUIRE(pass.states().depth().range_far() == 2.f);
        REQUIRE(pass.states().depth().write() == false);
        REQUIRE(pass.states().depth().func() == render::compare_func::greater);

        REQUIRE(pass.states().stencil().write() == 2u);
        REQUIRE(pass.states().stencil().func() == render::compare_func::never);
        REQUIRE(pass.states().stencil().ref() == 4u);
        REQUIRE(pass.states().stencil().mask() == 5u);
        REQUIRE(pass.states().stencil().pass() == render::stencil_op::incr);
        REQUIRE(pass.states().stencil().sfail() == render::stencil_op::decr);
        REQUIRE(pass.states().stencil().zfail() == render::stencil_op::invert);

        REQUIRE(pass.states().culling().mode() == render::culling_mode::cw);
        REQUIRE(pass.states().culling().face() == render::culling_face::front);

        REQUIRE(pass.states().capabilities().culling());
        REQUIRE(pass.states().capabilities().blending());
        REQUIRE(pass.states().capabilities().depth_test());
        REQUIRE(pass.states().capabilities().stencil_test());

        REQUIRE(pass.states().blending().constant_color() == color::white());
        REQUIRE(pass.states().blending().color_mask() == render::blending_color_mask::gba);

        REQUIRE(pass.states().blending().src_rgb_factor() == render::blending_factor::dst_alpha);
        REQUIRE(pass.states().blending().src_alpha_factor() == render::blending_factor::dst_alpha);

        REQUIRE(pass.states().blending().dst_rgb_factor() == render::blending_factor::dst_color);
        REQUIRE(pass.states().blending().dst_alpha_factor() == render::blending_factor::src_color);

        REQUIRE(pass.states().blending().rgb_equation() == render::blending_equation::subtract);
        REQUIRE(pass.states().blending().alpha_equation() == render::blending_equation::reverse_subtract);
    }
    modules::shutdown<asset_cache<binary_asset>>();
    modules::shutdown<asset_cache<image_asset>>();
    modules::shutdown<asset_cache<text_asset>>();
    modules::shutdown<library>();
    modules::shutdown<debug>();
    modules::shutdown<vfs>();
}
