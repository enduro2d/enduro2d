/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"
#include "library.hpp"

namespace e2d
{
    template < typename T >
    class content_asset : public asset {
    public:
        content_asset(T content)
        : content_(std::move(content)) {}

        const T& content() const noexcept {
            return content_;
        }
    private:
        T content_;
    };

    class text_asset final : public content_asset<str> {
    public:
        using content_asset<str>::content_asset;
        static std::shared_ptr<text_asset> load(library& library, str_view address);
    };

    class mesh_asset final : public content_asset<mesh> {
    public:
        using content_asset<mesh>::content_asset;
        static std::shared_ptr<mesh_asset> load(library& library, str_view address);
    };

    class image_asset final : public content_asset<image> {
    public:
        using content_asset<image>::content_asset;
        static std::shared_ptr<image_asset> load(library& library, str_view address);
    };

    class binary_asset final : public content_asset<buffer> {
    public:
        using content_asset<buffer>::content_asset;
        static std::shared_ptr<binary_asset> load(library& library, str_view address);
    };

    class shader_asset final : public content_asset<shader_ptr> {
    public:
        using content_asset<shader_ptr>::content_asset;
        static std::shared_ptr<shader_asset> load(library& library, str_view address);
    };

    class texture_asset final : public content_asset<texture_ptr> {
    public:
        using content_asset<texture_ptr>::content_asset;
        static std::shared_ptr<texture_asset> load(library& library, str_view address);
    };

    class material_asset final : public content_asset<render::material> {
    public:
        using content_asset<render::material>::content_asset;
        static std::shared_ptr<material_asset> load(library& library, str_view address);
    };
}
