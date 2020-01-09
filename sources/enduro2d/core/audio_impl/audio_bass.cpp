/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "audio_bass_impl.hpp"

#if defined(E2D_AUDIO_MODE) && E2D_AUDIO_MODE == E2D_AUDIO_MODE_BASS

namespace
{
    using namespace e2d;

    u32 max_sound_channels = 4;

    void CALLBACK sound_stream_close_proc(void* user) noexcept {
        E2D_UNUSED(user);
    }

    QWORD CALLBACK sound_stream_length_proc(void* user) noexcept {
        auto* self = static_cast<input_stream*>(user);
        return math::numeric_cast<QWORD>(self->length());
    }

    DWORD CALLBACK sound_stream_read_proc(void* buffer, DWORD length, void* user) noexcept {
        auto* self = static_cast<input_stream*>(user);
        try {
            return math::numeric_cast<DWORD>(self->read(buffer, length));
        } catch (...) {
            return -1;
        }
    }

    BOOL CALLBACK sound_stream_seek_proc(QWORD offset, void* user) noexcept {
        auto* self = static_cast<input_stream*>(user);
        try {
            return offset == self->seek(
                math::numeric_cast<std::ptrdiff_t>(offset),
                false);
        } catch(...) {
            return false;
        }
    }
}

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
        if ( !BASS_ChannelPlay(state().channel(), TRUE) ) {
            state_->dbg().error("AUDIO: Failed to play sound");
        }
    }

    void sound_source::stop() noexcept {
        BASS_ChannelStop(state().channel());
    }

    void sound_source::pause() noexcept {
        BASS_ChannelPause(state().channel());
    }

    void sound_source::resume() noexcept {
        if ( !BASS_ChannelPlay(state().channel(), FALSE) ) {
            state_->dbg().error("AUDIO: Failed to resume sound");
        }
    }

    bool sound_source::playing() const noexcept {
        return BASS_ChannelIsActive(state().channel()) == BASS_ACTIVE_PLAYING;
    }

    void sound_source::looping(bool value) noexcept {
        BASS_ChannelFlags(state().channel(), value ? BASS_SAMPLE_LOOP : 0, BASS_SAMPLE_LOOP);
    }

    bool sound_source::looping() noexcept {
        return math::check_all_flags(
            BASS_ChannelFlags(state().channel(), 0, 0),
            static_cast<DWORD>(BASS_SAMPLE_LOOP));
    }

    void sound_source::volume(f32 value) noexcept {
        BASS_ChannelSetAttribute(state().channel(), BASS_ATTRIB_VOL, value);
    }

    f32 sound_source::volume() const noexcept {
        f32 volume = 1.0f;
        BASS_ChannelGetAttribute(state().channel(), BASS_ATTRIB_VOL, &volume);
        return volume;
    }

    void sound_source::position(f32 pos) noexcept {
        QWORD byte_pos = BASS_ChannelSeconds2Bytes(state().channel(), pos);
        BASS_ChannelSetPosition(state().channel(), byte_pos, BASS_POS_BYTE);
    }

    f32 sound_source::position() const noexcept {
        QWORD byte_pos = BASS_ChannelGetPosition(state().channel(), BASS_POS_BYTE);
        return math::numeric_cast<f32>(
            BASS_ChannelBytes2Seconds(state().channel(), byte_pos));
    }

    f32 sound_source::duration() const noexcept {
        QWORD byte_len = BASS_ChannelGetLength(state().channel(), BASS_POS_BYTE);
        return math::numeric_cast<f32>(
            BASS_ChannelBytes2Seconds(state().channel(), byte_len));
    }

    //
    // audio
    //

    audio::audio(debug& d)
    : state_(std::make_unique<internal_state>(d)) {}

    audio::~audio() noexcept {
    }

    sound_stream_ptr audio::create_stream(
        buffer_view sound_data)
    {
        if ( !state_->initialized() ) {
            state_->dbg().error("AUDIO: Not initialized");
            return nullptr;
        }

        if ( sound_data.empty() ) {
            state_->dbg().error("AUDIO: Sound data is empty");
            return nullptr;
        }

        HSAMPLE sample = BASS_SampleLoad(
            TRUE,
            sound_data.data(),
            0,
            math::numeric_cast<DWORD>(sound_data.size()),
            max_sound_channels,
            BASS_SAMPLE_OVER_POS);

        if ( sample == 0 ) {
            state_->dbg().error(
                "AUDIO: Failed to load sound sample, code (%0)",
                BASS_ErrorGetCode());
            return nullptr;
        }

        return std::make_shared<sound_stream>(
            std::make_unique<sound_stream::internal_state>(
                state_->dbg(), sample, nullptr));
    }

    sound_stream_ptr audio::create_stream(
        input_stream_uptr file_stream)
    {
        if ( !state_->initialized() ) {
            state_->dbg().error("AUDIO: Not initialized");
            return nullptr;
        }

        if ( !file_stream ) {
            state_->dbg().error("AUDIO: file stream is null");
            return nullptr;
        }

        BASS_FILEPROCS file_procs = {
            sound_stream_close_proc,
            sound_stream_length_proc,
            sound_stream_read_proc,
            sound_stream_seek_proc};

        HSTREAM stream = BASS_StreamCreateFileUser(
            STREAMFILE_NOBUFFER,
            BASS_ASYNCFILE,
            &file_procs,
            file_stream.get());

        if ( stream == 0 ) {
            state_->dbg().error(
                "AUDIO: Failed to create sound stream, code (%0)",
                BASS_ErrorGetCode());
            return nullptr;
        }

        return std::make_shared<sound_stream>(
            std::make_unique<sound_stream::internal_state>(
                state_->dbg(), stream, std::move(file_stream)));
    }

    sound_source_ptr audio::create_source(
        const sound_stream_ptr& stream)
    {
        if ( !state_->initialized() ) {
            state_->dbg().error("AUDIO: Not initialized");
            return nullptr;
        }

        if ( !stream ) {
            state_->dbg().error("AUDIO: stream is null");
            return nullptr;
        }

        HCHANNEL channel = stream->state().stream()
            ? stream->state().sound()
            : BASS_SampleGetChannel(stream->state().sound(), FALSE);
            
        if ( !channel ) {
            state_->dbg().error("AUDIO: can net retrive sound channel");
            return nullptr;
        }

        return std::make_shared<sound_source>(
            std::make_unique<sound_source::internal_state>(
                state_->dbg(), stream, channel));
    }

    bool audio::initialized() const noexcept {
        return state_->initialized();
    }

    void audio::volume(f32 value) noexcept {
        BASS_SetVolume(value);
    }

    f32 audio::volume() const noexcept {
        return BASS_GetVolume();
    }

    void audio::resume() noexcept {
        if ( !state_->initialized() ) {
            state_->dbg().error("AUDIO: Not initialized");
            return;
        }

        if ( !BASS_Start() ) {
            state_->dbg().error("AUDIO: Failed to resume audio output");
        }
    }

    void audio::pause() noexcept {
        if ( !state_->initialized() ) {
            state_->dbg().error("AUDIO: Not initialized");
            return;
        }

        if ( !BASS_Pause() ) {
            state_->dbg().error("AUDIO: Failed to resume audio output");
        }
    }
}

#endif
