/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"
#include "../library.hpp"

#include "model.hpp"
#include "sprite.hpp"

namespace e2d
{
    class text_asset final : public content_asset<text_asset, str> {
    public:
        using content_asset<text_asset, str>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };

    class mesh_asset final : public content_asset<mesh_asset, mesh> {
    public:
        using content_asset<mesh_asset, mesh>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };

    class model_asset final : public content_asset<model_asset, model> {
    public:
        using content_asset<model_asset, model>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };

    class image_asset final : public content_asset<image_asset, image> {
    public:
        using content_asset<image_asset, image>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };

    class sprite_asset final : public content_asset<sprite_asset, sprite> {
    public:
        using content_asset<sprite_asset, sprite>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };

    class binary_asset final : public content_asset<binary_asset, buffer> {
    public:
        using content_asset<binary_asset, buffer>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };

    class shader_asset final : public content_asset<shader_asset, shader_ptr> {
    public:
        using content_asset<shader_asset, shader_ptr>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };

    class texture_asset final : public content_asset<texture_asset, texture_ptr> {
    public:
        using content_asset<texture_asset, texture_ptr>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };

    class material_asset final : public content_asset<material_asset, render::material> {
    public:
        using content_asset<material_asset, render::material>::content_asset;
        static load_async_result load_async(library& library, str_view address);
    };
}
