/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "audio.hpp"

#if defined(E2D_AUDIO_MODE) && E2D_AUDIO_MODE == E2D_AUDIO_MODE_NONE

namespace e2d
{
    //
    // sound_stream::internal_state
    //

    class sound_stream::internal_state final : private e2d::noncopyable {
    public:
        internal_state() noexcept = default;
        ~internal_state() noexcept = default;
    };

    //
    // sound_source::internal_state
    //

    class sound_source::internal_state final : private e2d::noncopyable {
    public:
        internal_state() noexcept = default;
        ~internal_state() noexcept = default;
    };

    //
    // audio::internal_state
    //

    class audio::internal_state final : private e2d::noncopyable {
    public:
        internal_state() noexcept = default;
        ~internal_state() noexcept = default;
    };

    //
    // sound_stream
    //

    sound_stream::sound_stream(internal_state_uptr state)
    : state_(std::move(state)) {
        E2D_ASSERT(state_);
    }

    const sound_stream::internal_state& sound_stream::state() const noexcept {
        return *state_;
    }

    //
    // sound_source
    //

    sound_source::sound_source(internal_state_uptr state)
    : state_(std::move(state)) {
        E2D_ASSERT(state_);
    }

    const sound_source::internal_state& sound_source::state() const noexcept {
        return *state_;
    }

    void sound_source::play() noexcept {
    }

    void sound_source::stop() noexcept {
    }

    void sound_source::pause() noexcept {
    }

    bool sound_source::playing() const noexcept {
        return false;
    }

    void sound_source::looping(bool value) noexcept {
        E2D_UNUSED(value);
    }

    bool sound_source::looping() noexcept {
        return false;
    }

    void sound_source::volume(f32 value) noexcept {
        E2D_UNUSED(value);
    }

    f32 sound_source::volume() const noexcept {
        return 1.0f;
    }

    void sound_source::position(secf value) noexcept {
        E2D_UNUSED(value);
    }

    secf sound_source::position() const noexcept {
        return {};
    }

    secf sound_source::duration() const noexcept {
        return {};
    }

    //
    // audio
    //

    audio::audio(debug& d)
    : state_(std::make_unique<internal_state>()) {
        E2D_UNUSED(d);
    }

    audio::~audio() noexcept {
    }

    sound_stream_ptr audio::preload_stream(
        buffer_view sound_data)
    {
        E2D_UNUSED(sound_data);
        return nullptr;
    }

    sound_stream_ptr audio::preload_stream(
        const input_stream_uptr& file_stream)
    {
        E2D_UNUSED(file_stream);
        return nullptr;
    }

    sound_stream_ptr audio::create_stream(
        input_stream_uptr file_stream)
    {
        E2D_UNUSED(file_stream);
        return nullptr;
    }

    sound_source_ptr audio::create_source(
        const sound_stream_ptr& stream)
    {
        E2D_UNUSED(stream);
        return nullptr;
    }

    bool audio::initialized() const noexcept {
        return false;
    }

    void audio::volume(f32 value) noexcept {
        E2D_UNUSED(value);
    }

    f32 audio::volume() const noexcept {
        return 1.0f;
    }

    void audio::resume() noexcept {
    }

    void audio::pause() noexcept {
    }
}

#endif
