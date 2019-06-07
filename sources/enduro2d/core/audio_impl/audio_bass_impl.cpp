
#include "audio_bass_impl.hpp"

#if defined(E2D_AUDIO_MODE) && E2D_AUDIO_MODE == E2D_AUDIO_MODE_BASS

namespace e2d
{
    
    //
    // sound_stream::internal_state
    //
    
    sound_stream::internal_state::internal_state(
        debug& debug,
        HSAMPLE sound)
    : debug_(debug)
    , sound_(sound){
        E2D_ASSERT(sound);
    }
    
    debug& sound_stream::internal_state::dbg() const noexcept {
        return debug_;
    }
    
    HSAMPLE sound_stream::internal_state::sound() const noexcept {
        return sound_;
    }
    
    //
    // sound_source::internal_state
    //
 
    sound_source::internal_state::internal_state(
        debug& debug,
        HCHANNEL channel)
    : debug_(debug)
    , channel_(channel) {
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
    }
    
    debug& audio::internal_state::dbg() const noexcept {
        return debug_;
    }
}

#endif
