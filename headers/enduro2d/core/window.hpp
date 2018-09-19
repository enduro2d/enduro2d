/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class bad_window_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad window operation";
        }
    };

    class window final : public module<window> {
    public:
        window(const v2u& size, str_view title, bool fullscreen);
        ~window() noexcept;

        v2u real_size() const noexcept;
        v2u virtual_size() const noexcept;

        bool should_close() const noexcept;
        static bool poll_events() noexcept;
    private:
        class state;
        std::unique_ptr<state> state_;
    };
}
