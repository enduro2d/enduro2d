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
            HSAMPLE sound);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        HSAMPLE sound() const noexcept;
    private:
        debug& debug_;
        HSAMPLE sound_;
    };

    //
    // sound_source::internal_state
    //

    class sound_source::internal_state final : private e2d::noncopyable {
    public:
        internal_state(
            debug& debug,
            HCHANNEL channel);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
        HCHANNEL channel() const noexcept;
    private:
        debug& debug_;
        HCHANNEL channel_;
    };

    //
    // audio::internal_state
    //

    class audio::internal_state final : private e2d::noncopyable {
    public:
        internal_state(debug& debug);
        ~internal_state() noexcept = default;
    public:
        debug& dbg() const noexcept;
    private:
        debug& debug_;
    };

}

#endif
