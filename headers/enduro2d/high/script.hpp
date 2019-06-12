/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class script final {
    public:
        script() = default;
        ~script() noexcept = default;

        script(script&& other) noexcept;
        script& operator=(script&& other) noexcept;

        script(const script& other);
        script& operator=(const script& other);

        void clear() noexcept;
        void swap(script& other) noexcept;

        script& assign(script&& other) noexcept;
        script& assign(const script& other);
    };

    void swap(script& l, script& r) noexcept;
    bool operator==(const script& l, const script& r) noexcept;
    bool operator!=(const script& l, const script& r) noexcept;
}
