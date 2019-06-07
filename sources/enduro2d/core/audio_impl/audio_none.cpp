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

    void sound_source::position(secf pos) noexcept {
        E2D_UNUSED(pos);
    }

    secf sound_source::position() const noexcept {
        return {};
    }

    secf sound_source::duration() const noexcept {
        return {};
    }

    void sound_source::position3d(const v3f &value) noexcept {
        E2D_UNUSED(value);
    }

    v3f sound_source::position3d() const noexcept {
        return {};
    }

    void sound_source::velocity(const v3f &value) noexcept {
        E2D_UNUSED(value);
    }

    v3f sound_source::velocity() const noexcept {
        return {};
    }

    //
    // audio
    //

    audio::audio(debug& d)
    : state_(std::make_unique<internal_state>()) {
        E2D_UNUSED(d);
    }

    audio::audio() noexcept {
    }

    sound_stream_ptr audio::create_stream(
        buffer_view sound_data) {
        E2D_UNUSED(sound_data);
        return nullptr;
    }

    sound_stream_ptr audio::create_stream(
        const input_stream_uptr& file_stream) {
        E2D_UNUSED(file_stream);
        return nullptr;
    }

    sound_source_ptr audio::create_source(
        const sound_stream_ptr &stream) {
        E2D_UNUSED(stream);
        return nullptr;
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

    void audio::listener_position(const v3f &value) noexcept {
        E2D_UNUSED(value);
    }

    v3f audio::listener_position() const noexcept {
        return {};
    }

    void audio::listener_velocity(const v3f &value) noexcept {
        E2D_UNUSED(value);
    }

    v3f audio::listener_velocity() const noexcept {
        return {};
    }

    void audio::apply3d() noexcept {
    }
}

#endif
