#include "audio_engine.hpp"

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
    // audio_engine::internal_state
    //
    
    class audio_engine::internal_state final : private e2d::noncopyable {
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
    
    void sound_source::volume(float value) noexcept {
        E2D_UNUSED(value);
    }
    
    float sound_source::volume() const noexcept {
        return 1.0f;
    }
    
    void sound_source::position(seconds<float> pos) noexcept {
        E2D_UNUSED(pos);
    }
    
    seconds<float> sound_source::position() const noexcept {
        return {};
    }
    
    seconds<float> sound_source::duration() const noexcept {
        return {};
    }
    
    //
    // audio_engine
    //
    
    audio_engine::audio_engine(debug& d)
    : state_(std::make_unique<internal_state>()) {
        E2D_UNUSED(d);
    }
    
    audio_engine::~audio_engine() noexcept {
    }
    
    sound_stream_ptr audio_engine::create_stream(
        buffer_view sound_data) {
        E2D_UNUSED(sound_data);
        return nullptr;
    }
    
    sound_stream_ptr audio_engine::create_stream(
        const input_stream_uptr& file_stream) {
        E2D_UNUSED(file_stream);
        return nullptr;
    }
    
    sound_source_ptr audio_engine::create_source(
        const sound_stream_ptr &stream) {
        E2D_UNUSED(stream);
        return nullptr;
    }
    
    void audio_engine::volume(float value) noexcept {
        E2D_UNUSED(value);
    }
    
    float audio_engine::volume() const noexcept {
        return 1.0f;
    }
    
    void audio_engine::resume() noexcept {
    }
    
    void audio_engine::pause() noexcept {
    }
}

#endif
