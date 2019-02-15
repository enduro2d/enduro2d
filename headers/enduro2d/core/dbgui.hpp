/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    //
    // bad_dbgui_operation
    //

    class bad_dbgui_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad dbgui operation";
        }
    };

    //
    // dbgui
    //

    class dbgui final : public module<dbgui> {
    public:
        dbgui(debug& d, input& i, render& r, window& w);
        ~dbgui() noexcept final;

        void frame_tick();
        void frame_render();
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
