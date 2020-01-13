/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
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
        class widget : private e2d::noncopyable {
        public:
            struct description {
                std::optional<str> title;
                std::optional<v2f> first_size;
            };
        public:
            virtual ~widget() noexcept = default;
            virtual bool show() = 0;
            virtual const description& desc() const noexcept = 0;
        };
        using widget_uptr = std::unique_ptr<widget>;
    public:
        dbgui(debug& d, input& i, render& r, window& w);
        ~dbgui() noexcept final;

        template < typename T, typename... Args >
        void register_menu_widget(str menu, str item, Args&&... args);
        void register_menu_widget(str menu, str item, widget_uptr widget);

        bool visible() const noexcept;
        void toggle_visible(bool yesno) noexcept;

        void frame_tick();
        void frame_render();
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}

namespace e2d
{
    template < typename T, typename... Args >
    void dbgui::register_menu_widget(str menu, str item, Args&&... args) {
        register_menu_widget(
            std::move(menu),
            std::move(item),
            std::make_unique<T>(std::forward<Args>(args)...));
    }
}
