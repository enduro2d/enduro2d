
#include "audio_bass_impl.hpp"
#include "enduro2d/core/debug.hpp"
#include <3rdparty/bass/bass.h>

#if defined(E2D_AUDIO_MODE) && E2D_AUDIO_MODE == E2D_AUDIO_MODE_BASS

namespace e2d
{

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

    static void CALLBACK sound_stream_close_proc(void *) {
    }

    static QWORD CALLBACK sound_stream_length_proc(void *user) {
        auto* self = static_cast<input_stream*>(user);
        return self->length();
    }

    static DWORD CALLBACK sound_stream_read_proc(void *buffer, DWORD length, void *user) {
        auto* self = static_cast<input_stream*>(user);
        return self->read(buffer, length);
    }

    static BOOL CALLBACK sound_stream_seek_proc(QWORD offset, void *user) {
        auto* self = static_cast<input_stream*>(user);
        return self->seek(offset, false);
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
        if ( !BASS_ChannelPlay(state().channel(), false) )
            state_->dbg().error("AUDIO: Failed to play sound");
    }

    void sound_source::stop() noexcept {
        BASS_ChannelStop(state().channel());
    }

    void sound_source::pause() noexcept {
        BASS_ChannelPause(state().channel());
    }

    bool sound_source::playing() const noexcept {
        return BASS_ChannelIsActive(state().channel()) == BASS_ACTIVE_PLAYING;
    }

    void sound_source::looping(bool value) noexcept {
        BASS_ChannelFlags(state().channel(), value ? BASS_SAMPLE_LOOP : 0, BASS_SAMPLE_LOOP);
    }

    bool sound_source::looping() noexcept {
        return math::check_all_flags(BASS_ChannelFlags(state().channel(), 0, 0), BASS_SAMPLE_LOOP);
    }

    void sound_source::volume(f32 value) noexcept {
        BASS_ChannelSetAttribute(state().channel(), BASS_ATTRIB_VOL, value);
    }

    f32 sound_source::volume() const noexcept {
        f32 volume = 1.0f;
        BASS_ChannelGetAttribute(state().channel(), BASS_ATTRIB_VOL, &volume);
        return volume;
    }

    void sound_source::position(secf pos) noexcept {
        auto byte_pos = BASS_ChannelSeconds2Bytes(state().channel(), pos.value);
        BASS_ChannelSetPosition(state().channel(), byte_pos, BASS_POS_BYTE);
    }

    secf sound_source::position() const noexcept {
        auto byte_pos = BASS_ChannelGetPosition(state().channel(), BASS_POS_BYTE);
        return secf{f32(BASS_ChannelBytes2Seconds(state().channel(), byte_pos))};
    }

    secf sound_source::duration() const noexcept {
        auto byte_len = BASS_ChannelGetLength(state().channel(), BASS_POS_BYTE);
        return secf{f32(BASS_ChannelBytes2Seconds(state().channel(), byte_len))};
    }

    void sound_source::position3d(const v3f& value) noexcept {
        BASS_3DVECTOR pos = {value.x, value.y, value.z};
        BASS_ChannelGet3DPosition(state().channel(), &pos, nullptr, nullptr);
    }

    v3f sound_source::position3d() const noexcept {
        BASS_3DVECTOR pos = {};
        BASS_ChannelSet3DPosition(state().channel(), &pos, nullptr, nullptr);
        return v3f(pos.x, pos.y, pos.z);
    }

    void sound_source::velocity(const v3f& value) noexcept {
        BASS_3DVECTOR vel = {value.x, value.y, value.z};
        BASS_ChannelGet3DPosition(state().channel(), nullptr, nullptr, &vel);
    }

    v3f sound_source::velocity() const noexcept {
        BASS_3DVECTOR vel = {};
        BASS_ChannelSet3DPosition(state().channel(), nullptr, nullptr, &vel);
        return v3f(vel.x, vel.y, vel.z);
    }

    //
    // audio
    //

    audio::audio(debug& d)
    : state_(std::make_unique<internal_state>(d)) {
        if ( !BASS_Init(-1, 44100, BASS_DEVICE_3D, nullptr, nullptr) ) {
            state_->dbg().error("AUDIO: Initialization failed");
            return;
        }
        initialized_ = true;
        // uaw one additional thread for streaming
        BASS_SetConfig(BASS_CONFIG_UPDATETHREADS, 1);

    }

    audio::~audio() noexcept {
        BASS_Free();
    }

    sound_stream_ptr audio::preload_stream(
        buffer_view sound_data) {
        if ( !initialized_ ) {
            state_->dbg().error("AUDIO: Not initialized");
            return nullptr;
        }
        if ( sound_data.empty() ) {
            state_->dbg().error("AUDIO: Sound data is empty");
            return nullptr;
        }
        HSAMPLE sample = BASS_SampleLoad(true, sound_data.data(), 0, sound_data.size(), max_channels_, BASS_SAMPLE_OVER_POS );
        if ( sample == 0 ) {
            state_->dbg().error("AUDIO: Failed to load sound sample, code (%0)", std::to_string(BASS_ErrorGetCode()));
            return nullptr;
        }
        return std::make_shared<sound_stream>(
                std::make_unique<sound_stream::internal_state>(state_->dbg(), sample, nullptr));
    }

    sound_stream_ptr audio::preload_stream(
        const input_stream_uptr& file_stream) {
        if ( !initialized_ ) {
            state_->dbg().error("AUDIO: Not initialized");
            return nullptr;
        }
        buffer file_data;
        if ( !streams::try_read_tail(file_data, file_stream) ) {
            state_->dbg().error("AUDIO: Failed to read file");
            return nullptr;
        }
        return preload_stream(buffer_view(file_data));
    }

    sound_stream_ptr audio::create_stream(
        input_stream_uptr file_stream) {
        if ( !initialized_ ) {
            state_->dbg().error("AUDIO: Not initialized");
            return nullptr;
        }
        if ( !file_stream ) {
            state_->dbg().error("AUDIO: file stream is null");
            return nullptr;
        }
        BASS_FILEPROCS procs;
        procs.close = &sound_stream_close_proc;
        procs.length = &sound_stream_length_proc;
        procs.read = &sound_stream_read_proc;
        procs.seek = &sound_stream_seek_proc;
        HSTREAM stream = BASS_StreamCreateFileUser(STREAMFILE_NOBUFFER, 0, &procs, file_stream.get());
        if ( stream == 0 ) {
            state_->dbg().error("AUDIO: Failed to create sound stream, code (%0)", std::to_string(BASS_ErrorGetCode()));
            return nullptr;
        }
        return std::make_shared<sound_stream>(
                std::make_unique<sound_stream::internal_state>(state_->dbg(), stream, std::move(file_stream)));
    }

    sound_source_ptr audio::create_source(
        const sound_stream_ptr &stream) {
        if ( !initialized_ ) {
            state_->dbg().error("AUDIO: Not initialized");
            return nullptr;
        }
        if ( !stream ) {
            state_->dbg().error("AUDIO: stream is null");
            return nullptr;
        }
        HCHANNEL channel = 0;
        if ( stream->state().stream() )
            channel = stream->state().sound();
        else
            channel = BASS_SampleGetChannel(stream->state().sound(), false);
            
        if ( !channel ) {
            state_->dbg().error("AUDIO: can net retrive sound channel");
            return nullptr;
        }
        return std::make_shared<sound_source>(
                std::make_unique<sound_source::internal_state>(state_->dbg(), stream, channel));
    }

    bool audio::initialized() const noexcept {
        return initialized_;
    }

    void audio::volume(f32 value) noexcept {
        BASS_SetVolume(value);
    }

    f32 audio::volume() const noexcept {
        return BASS_GetVolume();
    }

    void audio::resume() noexcept {
        if ( !initialized_ ) {
            state_->dbg().error("AUDIO: Not initialized");
            return;
        }
        if ( !BASS_Start() )
            state_->dbg().error("AUDIO: Failed to resume audio output");
    }

    void audio::pause() noexcept {
        if ( !initialized_ ) {
            state_->dbg().error("AUDIO: Not initialized");
            return;
        }
        if ( !BASS_Pause() )
            state_->dbg().error("AUDIO: Failed to resume audio output");
    }

    void audio::listener_position(const v3f &value) noexcept {
        BASS_3DVECTOR pos = {value.x, value.y, value.z};
        BASS_Set3DPosition(&pos, nullptr, nullptr, nullptr);
    }

    v3f audio::listener_position() const noexcept {
        BASS_3DVECTOR pos = {};
        BASS_Get3DPosition(&pos, nullptr, nullptr, nullptr);
        return v3f{pos.x, pos.y, pos.z};
    }

    void audio::listener_velocity(const v3f &value) noexcept {
        BASS_3DVECTOR vel = {value.x, value.y, value.z};
        BASS_Set3DPosition(nullptr, &vel, nullptr, nullptr);
    }

    v3f audio::listener_velocity() const noexcept {
        BASS_3DVECTOR vel = {};
        BASS_Get3DPosition(nullptr, &vel, nullptr, nullptr);
        return v3f{vel.x, vel.y, vel.z};
    }

    void audio::apply3d() noexcept {
        BASS_Apply3D();
    }
}

#endif
