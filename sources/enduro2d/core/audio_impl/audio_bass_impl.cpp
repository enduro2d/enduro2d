/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "audio_bass_impl.hpp"

#if defined(E2D_AUDIO_MODE) && E2D_AUDIO_MODE == E2D_AUDIO_MODE_BASS

namespace e2d
{
    //
    // sound_stream::internal_state
    //

    sound_stream::internal_state::internal_state(
        debug& debug,
        DWORD sound,
        input_stream_uptr stream)
    : debug_(debug)
    , sound_(sound)
    , stream_(std::move(stream)){
        E2D_ASSERT(sound);
    }

    sound_stream::internal_state::~internal_state() noexcept {
        if ( stream_ ) {
            BASS_StreamFree(sound_);
        } else {
            BASS_SampleFree(sound_);
        }
    }

    debug& sound_stream::internal_state::dbg() const noexcept {
        return debug_;
    }

    HSAMPLE sound_stream::internal_state::sound() const noexcept {
        return sound_;
    }

    const input_stream_uptr& sound_stream::internal_state::stream() const noexcept {
        return stream_;
    }

    //
    // sound_source::internal_state
    //

    sound_source::internal_state::internal_state(
        debug& debug,
        const sound_stream_ptr& stream,
        HCHANNEL channel)
    : debug_(debug)
    , channel_(channel)
    , stream_(stream) {
        E2D_ASSERT(channel);
    }

    debug& sound_source::internal_state::dbg() const noexcept {
        return debug_;
    }

    HCHANNEL sound_source::internal_state::channel() const noexcept {
        return channel_;
    }

    //
    // audio::internal_state
    //

    audio::internal_state::internal_state(debug& debug)
    : debug_(debug) {
        if ( !BASS_Init(-1, 44100, 0, nullptr, nullptr) ) {
            debug_.error("AUDIO: Initialization failed");
            return;
        }
        initialized_ = true;
        BASS_SetConfig(BASS_CONFIG_UPDATETHREADS, 1);
    }

    audio::internal_state::~internal_state() noexcept {
        if ( initialized_ ) {
            BASS_Free();
        }
    }

    debug& audio::internal_state::dbg() const noexcept {
        return debug_;
    }

    bool audio::internal_state::initialized() const noexcept {
        return initialized_;
    }
}

#endif
