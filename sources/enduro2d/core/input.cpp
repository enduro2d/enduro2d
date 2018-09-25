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
            none,
            press,
            pressed,
            release
        };
        mutable std::mutex mutex;
        std::array<button_state, enum_to_number(key::unknown) + 1> key_button_states;
        std::array<button_state, enum_to_number(mouse::unknown) + 1> mouse_button_states;
        char _pad[2];
    public:
        state() noexcept {
            std::fill(
                key_button_states.begin(),
                key_button_states.end(),
                button_state::none);
            std::fill(
                mouse_button_states.begin(),
                mouse_button_states.end(),
                button_state::none);
        }

        std::size_t key_button_index(key key) const noexcept {
            const auto index = enum_to_number(key);
            E2D_ASSERT(index < key_button_states.size());
            return math::numeric_cast<std::size_t>(index);
        }

        std::size_t mouse_button_index(mouse mouse) const noexcept {
            const auto index = enum_to_number(mouse);
            E2D_ASSERT(index < mouse_button_states.size());
            return math::numeric_cast<std::size_t>(index);
        }

        static void update_button_state(enum button_state& state) noexcept {
            if ( state == button_state::press ) {
                state = button_state::pressed;
            } else if ( state == button_state::release ) {
                state = button_state::none;
            }
        }
    };

    //
    // class input
    //

    input::input()
    : state_(new state()) {}
    input::~input() noexcept = default;

    bool input::any_key_up() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->key_button_states.cbegin(),
            state_->key_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::release;
            });
    }

    bool input::any_key_down() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->key_button_states.cbegin(),
            state_->key_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::press;
            });
    }

    bool input::any_key_downed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->key_button_states.cbegin(),
            state_->key_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::press
                    || s == state::button_state::pressed;
            });
    }

    bool input::any_mouse_up() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->mouse_button_states.cbegin(),
            state_->mouse_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::release;
            });
    }

    bool input::any_mouse_down() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->mouse_button_states.cbegin(),
            state_->mouse_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::press;
            });
    }

    bool input::any_mouse_downed() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return std::any_of(
            state_->mouse_button_states.cbegin(),
            state_->mouse_button_states.cend(),
            [](state::button_state s) noexcept {
                return s == state::button_state::press
                    || s == state::button_state::pressed;
            });
    }

    bool input::key_up(key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_button_index(key);
        const enum state::button_state ks = state_->key_button_states[index];
        return ks == state::button_state::release;
    }

    bool input::key_down(key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_button_index(key);
        const enum state::button_state ks = state_->key_button_states[index];
        return ks == state::button_state::press;
    }

    bool input::key_downed(key key) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_button_index(key);
        const enum state::button_state ks = state_->key_button_states[index];
        return ks == state::button_state::press
            || ks == state::button_state::pressed;
    }

    bool input::mouse_up(mouse mouse) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->mouse_button_index(mouse);
        const enum state::button_state ms = state_->mouse_button_states[index];
        return ms == state::button_state::release;
    }

    bool input::mouse_down(mouse mouse) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->mouse_button_index(mouse);
        const enum state::button_state ms = state_->mouse_button_states[index];
        return ms == state::button_state::press;
    }

    bool input::mouse_downed(mouse mouse) const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->mouse_button_index(mouse);
        const enum state::button_state ms = state_->mouse_button_states[index];
        return ms == state::button_state::press
            || ms == state::button_state::pressed;
    }

    void input::extract_up_keys(std::vector<key>& result) const {
        for ( std::size_t i = 0; i < state_->key_button_states.size(); ++i ) {
            state::button_state ks = state_->key_button_states[i];
            if ( ks == state::button_state::release ) {
                result.push_back(static_cast<key>(i));
            }
        }
    }

    void input::extract_down_keys(std::vector<key>& result) const {
        for ( std::size_t i = 0; i < state_->key_button_states.size(); ++i ) {
            state::button_state ks = state_->key_button_states[i];
            if ( ks == state::button_state::press ) {
                result.push_back(static_cast<key>(i));
            }
        }
    }

    void input::extract_downed_keys(std::vector<key>& result) const {
        for ( std::size_t i = 0; i < state_->key_button_states.size(); ++i ) {
            state::button_state ks = state_->key_button_states[i];
            if ( ks == state::button_state::press || ks == state::button_state::pressed ) {
                result.push_back(static_cast<key>(i));
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

    void input::post_event(key key, key_action action) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->key_button_index(key);
        const enum state::button_state ks = state_->key_button_states[index];
        switch ( action ) {
            case key_action::press:
            case key_action::repeat:
                if ( ks == state::button_state::none || ks == state::button_state::release ) {
                    state_->key_button_states[index] = state::button_state::press;
                }
                break;
            case key_action::release:
                if ( ks == state::button_state::press || ks == state::button_state::pressed ) {
                    state_->key_button_states[index] = state::button_state::release;
                }
                break;
            case key_action::unknown:
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected key action");
                break;
        }
    }

    void input::post_event(mouse mouse, mouse_action action) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        const std::size_t index = state_->mouse_button_index(mouse);
        const enum state::button_state ms = state_->mouse_button_states[index];
        switch ( action ) {
            case mouse_action::press:
                if ( ms == state::button_state::none || ms == state::button_state::release ) {
                    state_->mouse_button_states[index] = state::button_state::press;
                }
                break;
            case mouse_action::release:
                if ( ms == state::button_state::press || ms == state::button_state::pressed ) {
                    state_->mouse_button_states[index] = state::button_state::release;
                }
                break;
            case mouse_action::unknown:
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

    void window_input_source::on_key(key key, u32 scancode, key_action action) noexcept {
        E2D_UNUSED(scancode);
        input_.post_event(key, action);
    }

    void window_input_source::on_mouse(mouse mouse, mouse_action action) noexcept {
        input_.post_event(mouse, action);
    }
}
