/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/core/input.hpp>

namespace
{
    using namespace e2d;

    enum class button_state : u8 {
        pressed,
        released,
        just_pressed,
        just_released
    };

    void update_button_state(enum button_state& state) noexcept {
        if ( state == button_state::just_pressed ) {
            state = button_state::pressed;
        } else if ( state == button_state::just_released ) {
            state = button_state::released;
        }
    }

    template < typename E >
    constexpr std::underlying_type_t<E> enum_to_number(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }
}

namespace e2d
{
    //
    // class mouse::state
    //

    class mouse::state final : private e2d::noncopyable {
    public:
        mutable std::mutex mutex;
        std::array<button_state, enum_to_number(mouse_button::unknown) + 1> button_states;
        char _pad[2];
    public:
        state() noexcept {
            std::fill(
                button_states.begin(),
                button_states.end(),
                button_state::released);
        }

        std::size_t button_index(mouse_button btn) const noexcept {
            const auto index = enum_to_number(btn);
            E2D_ASSERT(index < button_states.size());
            return math::numeric_cast<std::size_t>(index);
        }

        void frame_tick() noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            std::for_each(
                button_states.begin(),
                button_states.end(),
                &update_button_state);
        }

        void post_event(input::mouse_button_event evt) noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            const std::size_t index = button_index(evt.button);
            const enum button_state ms = button_states[index];
            switch ( evt.action ) {
                case mouse_button_action::press:
                    if ( ms == button_state::released || ms == button_state::just_released ) {
                        button_states[index] = button_state::just_pressed;
                    }
                    break;
                case mouse_button_action::release:
                    if ( ms == button_state::just_pressed || ms == button_state::pressed ) {
                        button_states[index] = button_state::just_released;
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

    class keyboard::state final : private e2d::noncopyable {
    public:
        mutable std::mutex mutex;
        std::array<button_state, enum_to_number(keyboard_key::unknown) + 1> key_states;
    public:
        state() noexcept {
            std::fill(
                key_states.begin(),
                key_states.end(),
                button_state::released);
        }

        std::size_t key_index(keyboard_key key) const noexcept {
            const auto index = enum_to_number(key);
            E2D_ASSERT(index < key_states.size());
            return math::numeric_cast<std::size_t>(index);
        }

        void frame_tick() noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            std::for_each(
                key_states.begin(),
                key_states.end(),
                &update_button_state);
        }

        void post_event(input::keyboard_key_event evt) noexcept {
            std::lock_guard<std::mutex> guard(mutex);
            const std::size_t index = key_index(evt.key);
            const enum button_state ks = key_states[index];
            switch ( evt.action ) {
                case keyboard_key_action::press:
                case keyboard_key_action::repeat:
                    if ( ks == button_state::released || ks == button_state::just_released ) {
                        key_states[index] = button_state::just_pressed;
                    }
                    break;
                case keyboard_key_action::release:
                    if ( ks == button_state::pressed || ks == button_state::just_pressed ) {
                        key_states[index] = button_state::just_released;
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
    : state_(new state()) {}
    mouse::~mouse() noexcept = default;

    bool mouse::is_any_button_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->button_states.cbegin(),
            state_->button_states.cend(),
            [](button_state s) noexcept {
                return s == button_state::just_pressed
                    || s == button_state::pressed;
            });
    }

    bool mouse::is_any_button_just_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->button_states.cbegin(),
            state_->button_states.cend(),
            [](button_state s) noexcept {
                return s == button_state::just_pressed;
            });
    }

    bool mouse::is_any_button_just_released() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->button_states.cbegin(),
            state_->button_states.cend(),
            [](button_state s) noexcept {
                return s == button_state::just_released;
            });
    }

    bool mouse::is_button_pressed(mouse_button btn) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->button_index(btn);
        const enum button_state ms = state_->button_states[index];
        return ms == button_state::just_pressed
            || ms == button_state::pressed;
    }

    bool mouse::is_button_just_pressed(mouse_button btn) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->button_index(btn);
        const enum button_state ms = state_->button_states[index];
        return ms == button_state::just_pressed;
    }

    bool mouse::is_button_just_released(mouse_button btn) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->button_index(btn);
        const enum button_state ms = state_->button_states[index];
        return ms == button_state::just_released;
    }

    void mouse::extract_pressed_buttons(std::vector<mouse_button>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->button_states.size(); ++i ) {
            button_state ks = state_->button_states[i];
            if ( ks == button_state::just_pressed || ks == button_state::pressed ) {
                dst.push_back(static_cast<mouse_button>(i));
            }
        }
    }

    void mouse::extract_just_pressed_buttons(std::vector<mouse_button>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->button_states.size(); ++i ) {
            button_state ks = state_->button_states[i];
            if ( ks == button_state::just_pressed ) {
                dst.push_back(static_cast<mouse_button>(i));
            }
        }
    }

    void mouse::extract_just_released_buttons(std::vector<mouse_button>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->button_states.size(); ++i ) {
            button_state ks = state_->button_states[i];
            if ( ks == button_state::just_released ) {
                dst.push_back(static_cast<mouse_button>(i));
            }
        }
    }

    //
    // class keyboard
    //

    keyboard::keyboard()
    : state_(new state()) {}
    keyboard::~keyboard() noexcept = default;

    bool keyboard::is_any_key_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->key_states.cbegin(),
            state_->key_states.cend(),
            [](button_state s) noexcept {
                return s == button_state::just_pressed
                    || s == button_state::pressed;
            });
    }

    bool keyboard::is_any_key_just_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->key_states.cbegin(),
            state_->key_states.cend(),
            [](button_state s) noexcept {
                return s == button_state::just_pressed;
            });
    }

    bool keyboard::is_any_key_just_released() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->key_states.cbegin(),
            state_->key_states.cend(),
            [](button_state s) noexcept {
                return s == button_state::just_released;
            });
    }

    bool keyboard::is_key_pressed(keyboard_key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_index(key);
        const enum button_state ks = state_->key_states[index];
        return ks == button_state::just_pressed
            || ks == button_state::pressed;
    }

    bool keyboard::is_key_just_pressed(keyboard_key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_index(key);
        const enum button_state ks = state_->key_states[index];
        return ks == button_state::just_pressed;
    }

    bool keyboard::is_key_just_released(keyboard_key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_index(key);
        const enum button_state ks = state_->key_states[index];
        return ks == button_state::just_released;
    }

    void keyboard::extract_pressed_keys(std::vector<keyboard_key>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->key_states.size(); ++i ) {
            button_state ks = state_->key_states[i];
            if ( ks == button_state::just_pressed || ks == button_state::pressed ) {
                dst.push_back(static_cast<keyboard_key>(i));
            }
        }
    }

    void keyboard::extract_just_pressed_keys(std::vector<keyboard_key>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->key_states.size(); ++i ) {
            button_state ks = state_->key_states[i];
            if ( ks == button_state::just_pressed ) {
                dst.push_back(static_cast<keyboard_key>(i));
            }
        }
    }

    void keyboard::extract_just_released_keys(std::vector<keyboard_key>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->key_states.size(); ++i ) {
            button_state ks = state_->key_states[i];
            if ( ks == button_state::just_released ) {
                dst.push_back(static_cast<keyboard_key>(i));
            }
        }
    }

    //
    // class input::state
    //

    class input::state final : private e2d::noncopyable {
    public:
        std::mutex mutex;
        str32 input_text;
        class mouse mouse;
        class keyboard keyboard;
    };

    //
    // class input
    //

    input::input()
    : state_(new state()) {}
    input::~input() noexcept = default;

    str32 input::input_text() const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->input_text;
    }

    void input::extract_input_text(str32& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        dst = state_->input_text;
    }

    const mouse& input::mouse() const noexcept {
        return state_->mouse;
    }

    const keyboard& input::keyboard() const noexcept {
        return state_->keyboard;
    }

    void input::post_event(input_char_event evt) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        state_->input_text += evt.uchar;
    }

    void input::post_event(mouse_button_event evt) noexcept {
        state_->mouse.state_->post_event(evt);
    }

    void input::post_event(keyboard_key_event evt) noexcept {
        state_->keyboard.state_->post_event(evt);
    }

    void input::frame_tick() noexcept {
        {
            std::lock_guard<std::mutex> guard(state_->mutex);
            state_->input_text.clear();
        }
        state_->mouse.state_->frame_tick();
        state_->keyboard.state_->frame_tick();
    }

    //
    // class window_input_source
    //

    window_input_source::window_input_source(input& input) noexcept
    : input_(input) {}

    void window_input_source::on_input_char(char32_t uchar) noexcept {
        input_.post_event({uchar});
    }

    void window_input_source::on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept {
        E2D_UNUSED(scancode);
        input_.post_event({key, act});
    }

    void window_input_source::on_mouse_button(mouse_button btn, mouse_button_action act) noexcept {
        input_.post_event({btn, act});
    }
}
