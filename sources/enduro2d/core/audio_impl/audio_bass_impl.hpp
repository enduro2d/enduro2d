/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "audio.hpp"
#include <3rdparty/bass/bass.h>

#if defined(E2D_AUDIO_MODE) && E2D_AUDIO_MODE == E2D_AUDIO_MODE_BASS

namespace e2d
{

    //
    // sound_stream::internal_state
    //

    class sound_stream::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            DWORD sound,
            input_stream_uptr stream);
        ~internal_state() noexcept;
    public:
        [[nodiscard]] debug& dbg() const noexcept;
        [[nodiscard]] DWORD sound() const noexcept;
        [[nodiscard]] const input_stream_uptr& stream() const noexcept;
    private:
        debug& debug_;
        DWORD sound_;
        input_stream_uptr stream_;
    };

    //
    // sound_source::internal_state
    //

    class sound_source::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            const sound_stream_ptr& stream,
            HCHANNEL channel);
        ~internal_state() noexcept = default;
    public:
        [[nodiscard]] debug& dbg() const noexcept;
        [[nodiscard]] HCHANNEL channel() const noexcept;
    private:
        debug& debug_;
        HCHANNEL channel_;
        sound_stream_ptr stream_;
    };

    //
    // audio::internal_state
    //

    class audio::internal_state final : private e2d::noncopyable {
    public:
        internal_state(debug& debug);
        ~internal_state() noexcept = default;
    public:
        [[nodiscard]] debug& dbg() const noexcept;
    private:
        debug& debug_;
    };

}

#endif
