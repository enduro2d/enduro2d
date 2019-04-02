/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/library.hpp>

namespace e2d
{
    library::library(const url& root)
    : root_(root) {}

    library::~library() noexcept = default;

    const url& library::root() const noexcept {
        return root_;
    }

    std::size_t library::unload_unused_assets() noexcept {
        return asset_cache_base::unload_all_unused_assets();
    }
}
