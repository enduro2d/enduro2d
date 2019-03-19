/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class atlas final {
    public:
        atlas();
        ~atlas() noexcept;

        atlas(atlas&& other) noexcept;
        atlas& operator=(atlas&& other) noexcept;

        atlas(const atlas& other);
        atlas& operator=(const atlas& other);

        void clear() noexcept;
        void swap(atlas& other) noexcept;

        atlas& assign(atlas&& other) noexcept;
        atlas& assign(const atlas& other);
    };

    void swap(atlas& l, atlas& r) noexcept;
    bool operator==(const atlas& l, const atlas& r) noexcept;
    bool operator!=(const atlas& l, const atlas& r) noexcept;
}
