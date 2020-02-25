/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "console_widget.hpp"

namespace
{
    using namespace e2d;

    str log_text_format(debug::level lvl, str_view text) {
        return modules::is_initialized<engine>()
            ? strings::rformat("[%0](%1) -> %2\n", lvl, the<engine>().realtime_time(), text)
            : strings::rformat("[%0] -> %1\n", lvl, text);
    }

    color debug_level_to_color(debug::level lvl) noexcept {
        #define DEFINE_CASE(x,y) case debug::level::x: return color::y();
        switch ( lvl ) {
        DEFINE_CASE(trace, white);
        DEFINE_CASE(warning, yellow);
        DEFINE_CASE(error, red);
        DEFINE_CASE(fatal, magenta);
        default:
            E2D_ASSERT_MSG(false, "unexpected debug level");
            return color::white();
        }
        #undef DEFINE_CASE
    }
}

namespace e2d::dbgui_widgets
{
    bool console_widget::debug_sink::on_message(debug::level lvl, str_view text) noexcept {
        std::lock_guard<std::recursive_mutex> guard(rmutex);
        items.push_back(item{log_text_format(lvl, text), lvl});
        return true;
    }

    console_widget::console_widget(debug& d)
    : debug_(d)
    , sink_(debug_.register_sink<debug_sink>()) {
        desc_.first_size = v2f(400.f, 300.f);
        levels_.set();
    }

    console_widget::~console_widget() noexcept {
        debug_.unregister_sink(sink_);
    }

    bool console_widget::show() {
        std::lock_guard<std::recursive_mutex> guard(sink_.rmutex);

        if ( ImGui::Button("clear") ) {
            sink_.items.clear();
        }

        imgex::with_popup("levels", [this](){
            for ( const auto level : enum_hpp::values<debug::level>() ) {
                const str name = str(enum_hpp::to_string_or_throw(level));
                const std::size_t index = enum_hpp::to_index_or_throw(level);

                bool level_enabled = levels_.test(index);
                if ( ImGui::Checkbox(name.c_str(), &level_enabled) ) {
                    levels_.set(index, level_enabled);
                }
            }
        });

        if ( ImGui::SameLine(); ImGui::Button("levels...") ) {
            ImGui::OpenPopup("levels");
        }

        ImGui::Separator();

        const ImGuiWindowFlags sr_flags = ImGuiWindowFlags_HorizontalScrollbar;
        imgex::with_child("e2d_scrolling_region", false, sr_flags, [this](){
            const bool scroll_to_bottom =
                ImGui::GetScrollY() >= ImGui::GetScrollMaxY();

            for ( const auto& item : sink_.items ) {
                if ( !levels_[enum_hpp::to_index_or_throw(item.level)] ) {
                    continue;
                }

                ImGui::TextColored(
                    make_vec4(debug_level_to_color(item.level)),
                    "%s", item.text.c_str());
            }

            if ( scroll_to_bottom ) {
                ImGui::SetScrollHereY(1.f);
            }
        });

        return true;
    }

    const console_widget::description& console_widget::desc() const noexcept {
        return desc_;
    }
}
