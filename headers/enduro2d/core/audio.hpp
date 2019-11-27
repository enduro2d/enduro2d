/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class audio;
    class sound_stream;
    class sound_source;

    using sound_stream_ptr = std::shared_ptr<sound_stream>;
    using sound_source_ptr = std::shared_ptr<sound_source>;

    //
    // bad_audio_operation
    //

    class bad_audio_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad audio operation";
        }
    };

    //
    // sound_stream
    //

    class sound_stream final : noncopyable {
    public:
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
        const internal_state& state() const noexcept;
    public:
        explicit sound_stream(internal_state_uptr);
    private:
        internal_state_uptr state_;
    };

    //
    // sound_source
    //

    class sound_source final : noncopyable {
    public:
        class internal_state;
        using internal_state_uptr = std::unique_ptr<internal_state>;
        const internal_state& state() const noexcept;
    public:
        explicit sound_source(internal_state_uptr);

        void play() noexcept;
        void stop() noexcept;
        void pause() noexcept;
        void resume() noexcept;
        [[nodiscard]] bool playing() const noexcept;

        void looping(bool value) noexcept;
        [[nodiscard]] bool looping() noexcept;

        void volume(f32 value) noexcept;
        [[nodiscard]] f32 volume() const noexcept;

        void position(f32 value) noexcept;
        [[nodiscard]] f32 position() const noexcept;
        [[nodiscard]] f32 duration() const noexcept;
    private:
        internal_state_uptr state_;
    };

    //
    // audio
    //

    class audio final : public module<audio> {
    public:
        audio(debug& d);
        ~audio() noexcept;

        [[nodiscard]] sound_stream_ptr preload_stream(
            buffer_view sound_data);

        [[nodiscard]] sound_stream_ptr preload_stream(
            const input_stream_uptr& file_stream);

        [[nodiscard]] sound_stream_ptr create_stream(
            input_stream_uptr file_stream);

        [[nodiscard]] sound_source_ptr create_source(
            const sound_stream_ptr& stream);

        [[nodiscard]] bool initialized() const noexcept;

        void volume(f32 value) noexcept;
        [[nodiscard]] f32 volume() const noexcept;
        
        void resume() noexcept;
        void pause() noexcept;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
