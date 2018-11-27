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
        static std::shared_ptr<text_asset> load(library& library, const url& url);
    };

    class binary_asset final : public content_asset<buffer> {
    public:
        using content_asset<buffer>::content_asset;
        static std::shared_ptr<binary_asset> load(library& library, const url& url);
    };
}
