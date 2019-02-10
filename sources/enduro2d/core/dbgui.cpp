/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/core/dbgui.hpp>

namespace e2d
{
    class dbgui::internal_state final : private e2d::noncopyable {
    public:
        internal_state(window& w, render& r)
        : window_(w)
        , render_(r) {}
        ~internal_state() noexcept = default;
    public:
        window& wnd() const noexcept {
            return window_;
        }

        render& rnd() const noexcept {
            return render_;
        }
    private:
        window& window_;
        render& render_;
    };
}

namespace e2d
{
    dbgui::dbgui(window& w, render& r)
    : state_(new internal_state(w, r)) {}
    dbgui::~dbgui() noexcept = default;

    void dbgui::frame_tick() noexcept {
    }

    void dbgui::frame_render() noexcept {
    }
}
