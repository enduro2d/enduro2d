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
        window(const v2u& size, str_view title, bool vsync, bool fullscreen);
        ~window() noexcept;

        void hide() noexcept;
        void show() noexcept;
        void restore() noexcept;
        void minimize() noexcept;

        bool visible() const noexcept;
        bool focused() const noexcept;
        bool minimized() const noexcept;

        bool vsync() const noexcept;
        bool fullscreen() const noexcept;

        bool toggle_vsync(bool yesno) noexcept;
        bool toggle_fullscreen(bool yesno) noexcept;

        v2u real_size() const noexcept;
        v2u virtual_size() const noexcept;
        v2u framebuffer_size() const noexcept;

        const str& title() const noexcept;
        void set_title(str_view title);

        bool should_close() const noexcept;
        void set_should_close(bool yesno) noexcept;

        void swap_buffers() noexcept;
        static bool poll_events() noexcept;
    private:
        class state;
        std::unique_ptr<state> state_;
    };
}
