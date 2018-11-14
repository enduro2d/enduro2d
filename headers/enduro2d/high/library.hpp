/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class bad_library_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad library operation";
        }
    };

    class library final : public module<library> {
    public:
        library();
        ~library() noexcept final;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
