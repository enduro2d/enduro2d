/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../dbgui.hpp"

namespace e2d::dbgui_widgets
{
    class console_widget final : public dbgui::widget {
    public:
        class debug_sink final : public debug::sink {
        public:
            struct item {
                str text;
                debug::level level = debug::level::trace;
            };
        public:
            debug_sink() = default;
            bool on_message(debug::level lvl, str_view text) noexcept final;
        public:
            std::recursive_mutex rmutex;
            vector<item> items;
        };
    public:
        console_widget(debug& d);
        ~console_widget() noexcept;

        bool show() override;
        const description& desc() const noexcept override;
    private:
        debug& debug_;
        debug_sink& sink_;
        description desc_;
        std::bitset<enum_hpp::size<debug::level>()> levels_;
    };
}
