/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/core/input.hpp>

namespace
{
    template < typename E >
    constexpr std::underlying_type_t<E> enum_to_number(E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }
}

namespace e2d
{
    //
    // class input::state
    //

    class input::state final : private e2d::noncopyable {
    public:
        enum class button_state : u8 {
            pressed,
            released,
            just_pressed,
            just_released
        };
        mutable std::mutex mutex;
        std::array<button_state, enum_to_number(keyboard_key::unknown) + 1> key_button_states;
        std::array<button_state, enum_to_number(mouse_button::unknown) + 1> mouse_button_states;
        char _pad[2];
    public:
        state() noexcept {
            std::fill(
                key_button_states.begin(),
                key_button_states.end(),
                button_state::released);
            std::fill(
                mouse_button_states.begin(),
                mouse_button_states.end(),
                button_state::released);
        }

        std::size_t mouse_button_index(mouse_button btn) const noexcept {
            const auto index = enum_to_number(btn);
            E2D_ASSERT(index < mouse_button_states.size());
            return math::numeric_cast<std::size_t>(index);
        }

        std::size_t keyboard_key_index(keyboard_key key) const noexcept {
            const auto index = enum_to_number(key);
            E2D_ASSERT(index < key_button_states.size());
            return math::numeric_cast<std::size_t>(index);
        }

        static void update_button_state(enum button_state& state) noexcept {
            if ( state == button_state::just_pressed ) {
                state = button_state::pressed;
            } else if ( state == button_state::just_released ) {
                state = button_state::released;
            }
        }
    };

    //
    // class input
    //

    input::input()
    : state_(new state()) {}
    input::~input() noexcept = default;

    bool input::is_any_mouse_button_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->mouse_button_states.cbegin(),
            state_->mouse_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::just_pressed
                    || s == state::button_state::pressed;
            });
    }

    bool input::is_any_mouse_button_just_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->mouse_button_states.cbegin(),
            state_->mouse_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::just_pressed;
            });
    }

    bool input::is_any_mouse_button_just_released() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->mouse_button_states.cbegin(),
            state_->mouse_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::just_released;
            });
    }

    bool input::is_any_keyboard_key_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->key_button_states.cbegin(),
            state_->key_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::just_pressed
                    || s == state::button_state::pressed;
            });
    }

    bool input::is_any_keyboard_key_just_pressed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->key_button_states.cbegin(),
            state_->key_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::just_pressed;
            });
    }

    bool input::is_any_keyboard_key_just_released() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->key_button_states.cbegin(),
            state_->key_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::just_released;
            });
    }

    bool input::is_mouse_button_pressed(mouse_button btn) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->mouse_button_index(btn);
        const enum state::button_state ms = state_->mouse_button_states[index];
        return ms == state::button_state::just_pressed
            || ms == state::button_state::pressed;
    }

    bool input::is_mouse_button_just_pressed(mouse_button btn) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->mouse_button_index(btn);
        const enum state::button_state ms = state_->mouse_button_states[index];
        return ms == state::button_state::just_pressed;
    }

    bool input::is_mouse_button_just_released(mouse_button btn) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->mouse_button_index(btn);
        const enum state::button_state ms = state_->mouse_button_states[index];
        return ms == state::button_state::just_released;
    }

    bool input::is_keyboard_key_pressed(keyboard_key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->keyboard_key_index(key);
        const enum state::button_state ks = state_->key_button_states[index];
        return ks == state::button_state::just_pressed
            || ks == state::button_state::pressed;
    }

    bool input::is_keyboard_key_just_pressed(keyboard_key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->keyboard_key_index(key);
        const enum state::button_state ks = state_->key_button_states[index];
        return ks == state::button_state::just_pressed;
    }

    bool input::is_keyboard_key_just_released(keyboard_key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->keyboard_key_index(key);
        const enum state::button_state ks = state_->key_button_states[index];
        return ks == state::button_state::just_released;
    }

    void input::extract_pressed_mouse_buttons(std::vector<mouse_button>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->mouse_button_states.size(); ++i ) {
            state::button_state ks = state_->mouse_button_states[i];
            if ( ks == state::button_state::just_pressed || ks == state::button_state::pressed ) {
                dst.push_back(static_cast<mouse_button>(i));
            }
        }
    }

    void input::extract_just_pressed_mouse_buttons(std::vector<mouse_button>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->mouse_button_states.size(); ++i ) {
            state::button_state ks = state_->mouse_button_states[i];
            if ( ks == state::button_state::just_pressed ) {
                dst.push_back(static_cast<mouse_button>(i));
            }
        }
    }

    void input::extract_just_released_mouse_buttons(std::vector<mouse_button>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->mouse_button_states.size(); ++i ) {
            state::button_state ks = state_->mouse_button_states[i];
            if ( ks == state::button_state::just_released ) {
                dst.push_back(static_cast<mouse_button>(i));
            }
        }
    }

    void input::extract_pressed_keyboard_keys(std::vector<keyboard_key>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->key_button_states.size(); ++i ) {
            state::button_state ks = state_->key_button_states[i];
            if ( ks == state::button_state::just_pressed || ks == state::button_state::pressed ) {
                dst.push_back(static_cast<keyboard_key>(i));
            }
        }
    }

    void input::extract_just_pressed_keyboard_keys(std::vector<keyboard_key>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->key_button_states.size(); ++i ) {
            state::button_state ks = state_->key_button_states[i];
            if ( ks == state::button_state::just_pressed ) {
                dst.push_back(static_cast<keyboard_key>(i));
            }
        }
    }

    void input::extract_just_released_keyboard_keys(std::vector<keyboard_key>& dst) const {
        std::lock_guard<std::mutex> guard(state_->mutex);
        for ( std::size_t i = 0; i < state_->key_button_states.size(); ++i ) {
            state::button_state ks = state_->key_button_states[i];
            if ( ks == state::button_state::just_released ) {
                dst.push_back(static_cast<keyboard_key>(i));
            }
        }
    }

    void input::frame_tick() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        std::for_each(
            state_->key_button_states.begin(),
            state_->key_button_states.end(),
            &state::update_button_state);
        std::for_each(
            state_->mouse_button_states.begin(),
            state_->mouse_button_states.end(),
            &state::update_button_state);
    }

    void input::post_event(keyboard_key key, keyboard_key_action action) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->keyboard_key_index(key);
        const enum state::button_state ks = state_->key_button_states[index];
        switch ( action ) {
            case keyboard_key_action::press:
            case keyboard_key_action::repeat:
                if ( ks == state::button_state::released || ks == state::button_state::just_released ) {
                    state_->key_button_states[index] = state::button_state::just_pressed;
                }
                break;
            case keyboard_key_action::release:
                if ( ks == state::button_state::pressed || ks == state::button_state::just_pressed ) {
                    state_->key_button_states[index] = state::button_state::just_released;
                }
                break;
            case keyboard_key_action::unknown:
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected key action");
                break;
        }
    }

    void input::post_event(mouse_button mouse, mouse_button_action action) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->mouse_button_index(mouse);
        const enum state::button_state ms = state_->mouse_button_states[index];
        switch ( action ) {
            case mouse_button_action::press:
                if ( ms == state::button_state::released || ms == state::button_state::just_released ) {
                    state_->mouse_button_states[index] = state::button_state::just_pressed;
                }
                break;
            case mouse_button_action::release:
                if ( ms == state::button_state::just_pressed || ms == state::button_state::pressed ) {
                    state_->mouse_button_states[index] = state::button_state::just_released;
                }
                break;
            case mouse_button_action::unknown:
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected mouse action");
                break;
        }
    }

    //
    // class window_input_source
    //

    window_input_source::window_input_source(input& input) noexcept
    : input_(input) {}

    void window_input_source::on_keyboard_key(keyboard_key key, u32 scancode, keyboard_key_action act) noexcept {
        E2D_UNUSED(scancode);
        input_.post_event(key, act);
    }

    void window_input_source::on_mouse_button(mouse_button btn, mouse_button_action act) noexcept {
        input_.post_event(btn, act);
    }
}
