/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_high_binds.hpp"

#include <enduro2d/high/assets/atlas_asset.hpp>
#include <enduro2d/high/assets/binary_asset.hpp>
#include <enduro2d/high/assets/flipbook_asset.hpp>
#include <enduro2d/high/assets/font_asset.hpp>
#include <enduro2d/high/assets/image_asset.hpp>
#include <enduro2d/high/assets/json_asset.hpp>
#include <enduro2d/high/assets/material_asset.hpp>
#include <enduro2d/high/assets/mesh_asset.hpp>
#include <enduro2d/high/assets/model_asset.hpp>
#include <enduro2d/high/assets/prefab_asset.hpp>
#include <enduro2d/high/assets/script_asset.hpp>
#include <enduro2d/high/assets/shader_asset.hpp>
#include <enduro2d/high/assets/shape_asset.hpp>
#include <enduro2d/high/assets/sound_asset.hpp>
#include <enduro2d/high/assets/spine_asset.hpp>
#include <enduro2d/high/assets/sprite_asset.hpp>
#include <enduro2d/high/assets/text_asset.hpp>
#include <enduro2d/high/assets/texture_asset.hpp>
#include <enduro2d/high/assets/xml_asset.hpp>

namespace
{
    using namespace e2d;

    template < typename Asset >
    void bind_asset(const str& base_name, sol::state& l) {
        l.new_usertype<Asset>(base_name + "_asset",
            sol::no_constructor,

            "content", sol::property([](const Asset& a){
                return a.content();
            })
        );

        l["library"][str("load_") + base_name] =
        [](const library& l, str_view address){
            return l.load_main_asset<Asset>(address);
        };
    }
}

namespace e2d::bindings::high
{
    void bind_library(sol::state& l) {
        l.new_usertype<library>("library",
            sol::no_constructor
        );

        bind_asset<atlas_asset>("atlas", l);
        bind_asset<binary_asset>("binary", l);
        bind_asset<flipbook_asset>("flipbook", l);
        bind_asset<font_asset>("font", l);
        bind_asset<image_asset>("image", l);
        bind_asset<json_asset>("json", l);
        bind_asset<material_asset>("material", l);
        bind_asset<mesh_asset>("mesh", l);
        bind_asset<model_asset>("model", l);
        bind_asset<prefab_asset>("prefab", l);
        bind_asset<script_asset>("script", l);
        bind_asset<shader_asset>("shader", l);
        bind_asset<shape_asset>("shape", l);
        bind_asset<sound_asset>("sound", l);
        bind_asset<spine_asset>("spine", l);
        bind_asset<sprite_asset>("sprite", l);
        bind_asset<text_asset>("text", l);
        bind_asset<texture_asset>("texture", l);
        bind_asset<xml_asset>("xml", l);
    }
}
