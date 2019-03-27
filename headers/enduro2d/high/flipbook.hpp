/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class flipbook final {
    public:
        flipbook();
        ~flipbook() noexcept;

        flipbook(flipbook&& other) noexcept;
        flipbook& operator=(flipbook&& other) noexcept;

        flipbook(const flipbook& other);
        flipbook& operator=(const flipbook& other);

        void clear() noexcept;
        void swap(flipbook& other) noexcept;

        flipbook& assign(flipbook&& other) noexcept;
        flipbook& assign(const flipbook& other);
    };

    void swap(flipbook& l, flipbook& r) noexcept;
    bool operator==(const flipbook& l, const flipbook& r) noexcept;
    bool operator!=(const flipbook& l, const flipbook& r) noexcept;
}
