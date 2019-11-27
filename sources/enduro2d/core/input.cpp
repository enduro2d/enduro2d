/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/input.hpp>

namespace e2d
{
    //
    // class mouse::state
    //

    class mouse::internal_state final : private e2d::noncopyable {
    public:
        mutable std::mutex mutex;
        v2f cursor_pos;
        v2f scroll_delta;
        std::bitset<enum_hpp::size<mouse_button>()> pressed;
        std::bitset<enum_hpp::size<mouse_button>()> just_pressed;
        std::bitset<enum_hpp::size<mouse_button>()> just_released;
    public:
        std::size_t button_index(mouse_button btn) const noexcept {
            const auto index = utils::enum_to_underlying(btn);
            E2D_ASSERT(index < pressed.size());
            return math::numeric_cast<std::size_t>(index);
        }

        void frame_tick() noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            scroll_delta = v2f::zero();
            just_pressed.reset();
            just_released.reset();
        }

        void post_event(input::move_cursor_event evt) noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            cursor_pos = evt.pos;
        }

        void post_event(input::mouse_scroll_event evt) noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            scroll_delta += evt.delta;
        }

        void post_event(input::mouse_button_event evt) noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            const std::size_t index = button_index(evt.button);
            switch ( evt.action ) {
                case mouse_button_action::press:
                    if ( !pressed.test(index) ) {
                        pressed.set(index);
                        just_pressed.set(index);
                    }
                    break;
                case mouse_button_action::release:
                    if ( pressed.test(index) ) {
                        pressed.reset(index);
                        just_released.set(index);
                    }
                    break;
                case mouse_button_action::unknown:
                    break;
                default:
                    E2D_ASSERT_MSG(false, "unexpected mouse action");
                    break;
            }
        }
    };

    //
    // class keyboard::state
    //

    class keyboard::internal_state final : private e2d::noncopyable {
    public:
        mutable std::mutex mutex;
        str32 input_text;
        std::bitset<enum_hpp::size<keyboard_key>()> pressed;
        std::bitset<enum_hpp::size<keyboard_key>()> just_pressed;
        std::bitset<enum_hpp::size<keyboard_key>()> just_released;
    public:
        std::size_t key_index(keyboard_key key) const noexcept {
            const auto index = utils::enum_to_underlying(key);
            E2D_ASSERT(index < pressed.size());
            return math::numeric_cast<std::size_t>(index);
        }

        void frame_tick() noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            input_text.clear();
            just_pressed.reset();
            just_released.reset();
        }

        void post_event(input::input_char_event evt) noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            input_text += evt.uchar;
        }

        void post_event(input::keyboard_key_event evt) noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            const std::size_t index = key_index(evt.key);
            switch ( evt.action ) {
                case keyboard_key_action::press:
                case keyboard_key_action::repeat:
                    if ( !pressed.test(index) ) {
                        pressed.set(index);
                        just_pressed.set(index);
                    }
                    break;
                case keyboard_key_action::release:
                    if ( pressed.test(index) ) {
                        pressed.reset(index);
                        just_released.set(index);
                    }
                    break;
                case keyboard_key_action::unknown:
                    break;
                default:
                    E2D_ASSERT_MSG(false, "unexpected key action");
                    break;
            }
        }
    };

    //
    // class mouse
    //

    mouse::mouse()
    : state_(new internal_state()) {}
    mouse::~mouse() noexcept = default;

    v2f mouse::cursor_pos() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->cursor_pos;
    }

    v2f mouse::scroll_delta() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->scroll_delta;
    }

    bool mouse::is_any_button_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->pressed.any();
    }

    bool mouse::is_any_button_just_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->just_pressed.any();
    }

    bool mouse::is_any_button_just_released() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->just_released.any();
    }

    bool mouse::is_button_pressed(mouse_button btn) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->button_index(btn);
        return state_->pressed.test(index);
    }

    bool mouse::is_button_just_pressed(mouse_button btn) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->button_index(btn);
        return state_->just_pressed.test(index);
    }

    bool mouse::is_button_just_released(mouse_button btn) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->button_index(btn);
        return state_->just_released.test(index);
    }

    vector<mouse_button> mouse::pressed_buttons() const {
        vector<mouse_button> buttons;
        extract_pressed_buttons(buttons);
        return buttons;
    }

    vector<mouse_button> mouse::just_pressed_buttons() const {
        vector<mouse_button> buttons;
        extract_just_pressed_buttons(buttons);
        return buttons;
    }

    vector<mouse_button> mouse::just_released_buttons() const {
        vector<mouse_button> buttons;
        extract_just_released_buttons(buttons);
        return buttons;
    }

    void mouse::extract_pressed_buttons(vector<mouse_button>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->pressed.size(); ++i ) {
            if ( state_->pressed.test(i) ) {
                dst.push_back(static_cast<mouse_button>(i));
            }
        }
    }

    void mouse::extract_just_pressed_buttons(vector<mouse_button>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->just_pressed.size(); ++i ) {
            if ( state_->just_pressed.test(i) ) {
                dst.push_back(static_cast<mouse_button>(i));
            }
        }
    }

    void mouse::extract_just_released_buttons(vector<mouse_button>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->just_released.size(); ++i ) {
            if ( state_->just_released.test(i) ) {
                dst.push_back(static_cast<mouse_button>(i));
            }
        }
    }

    //
    // class keyboard
    //

    keyboard::keyboard()
    : state_(new internal_state()) {}
    keyboard::~keyboard() noexcept = default;

    str32 keyboard::input_text() const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->input_text;
    }

    void keyboard::extract_input_text(str32& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        dst = state_->input_text;
    }

    bool keyboard::is_any_key_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->pressed.any();
    }

    bool keyboard::is_any_key_just_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->just_pressed.any();
    }

    bool keyboard::is_any_key_just_released() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->just_released.any();
    }

    bool keyboard::is_key_pressed(keyboard_key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_index(key);
        return state_->pressed.test(index);
    }

    bool keyboard::is_key_just_pressed(keyboard_key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_index(key);
        return state_->just_pressed.test(index);
    }

    bool keyboard::is_key_just_released(keyboard_key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_index(key);
        return state_->just_released.test(index);
    }

    vector<keyboard_key> keyboard::pressed_keys() const {
        vector<keyboard_key> keys;
        extract_pressed_keys(keys);
        return keys;
    }

    vector<keyboard_key> keyboard::just_pressed_keys() const {
        vector<keyboard_key> keys;
        extract_just_pressed_keys(keys);
        return keys;
    }

    vector<keyboard_key> keyboard::just_released_keys() const {
        vector<keyboard_key> keys;
        extract_just_released_keys(keys);
        return keys;
    }

    void keyboard::extract_pressed_keys(vector<keyboard_key>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->pressed.size(); ++i ) {
            if ( state_->pressed.test(i) ) {
                dst.push_back(static_cast<keyboard_key>(i));
            }
        }
    }

    void keyboard::extract_just_pressed_keys(vector<keyboard_key>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->just_pressed.size(); ++i ) {
            if ( state_->just_pressed.test(i) ) {
                dst.push_back(static_cast<keyboard_key>(i));
            }
        }
    }

    void keyboard::extract_just_released_keys(vector<keyboard_key>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->just_released.size(); ++i ) {
            if ( state_->just_released.test(i) ) {
                dst.push_back(static_cast<keyboard_key>(i));
            }
        }
    }

    //
    // class input::state
    //

    class input::internal_state final : private e2d::noncopyable {
    public:
        class mouse mouse;
        class keyboard keyboard;
    };

    //
    // class input
    //

    input::input()
    : state_(new internal_state()) {}
    input::~input() noexcept = default;

    const class mouse& input::mouse() const noexcept {
        return state_->mouse;
    }

    const class keyboard& input::keyboard() const noexcept {
        return state_->keyboard;
    }

    void input::post_event(input_char_event evt) noexcept {
        state_->keyboard.state_->post_event(evt);
    }

    void input::post_event(move_cursor_event evt) noexcept {
        state_->mouse.state_->post_event(evt);
    }

    void input::post_event(mouse_scroll_event evt) noexcept {
        state_->mouse.state_->post_event(evt);
    }

    void input::post_event(mouse_button_event evt) noexcept {
        state_->mouse.state_->post_event(evt);
    }

    void input::post_event(keyboard_key_event evt) noexcept {
        state_->keyboard.state_->post_event(evt);
    }

    void input::frame_tick() noexcept {
        state_->mouse.state_->frame_tick();
        state_->keyboard.state_->frame_tick();
    }

    //
    // class window_input_source
    //

    window_input_source::window_input_source(input& input) noexcept
    : input_(input) {}

    void window_input_source::on_input_char(char32_t uchar) noexcept {
        input_.post_event(input::input_char_event{uchar});
    }

    void window_input_source::on_move_cursor(const v2f& pos) noexcept {
        input_.post_event(input::move_cursor_event{pos});
    }

    void window_input_source::on_mouse_scroll(const v2f& delta) noexcept {
        input_.post_event(input::mouse_scroll_event{delta});
    }

    void window_input_source::on_mouse_button(mouse_button btn, mouse_button_action act) noexcept {
        input_.post_event(input::mouse_button_event{btn, act});
    }

    void window_input_source::on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept {
        E2D_UNUSED(scancode);
        input_.post_event(input::keyboard_key_event{key, act});
    }
}
