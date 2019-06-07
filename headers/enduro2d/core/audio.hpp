#pragma once

#include "_core.hpp"

namespace e2d
{
    using sound_stream_ptr = std::shared_ptr<class sound_stream>;
    using sound_source_ptr = std::shared_ptr<class sound_source>;


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

        void looping(bool value) noexcept;
        [[nodiscard]] bool looping() noexcept;

        void volume(f32 value) noexcept;
        [[nodiscard]] f32 volume() const noexcept;

        void position(secf value) noexcept;
        [[nodiscard]] secf position() const noexcept;
        [[nodiscard]] secf duration() const noexcept;

        void position3d(const v3f& value) noexcept;
        [[nodiscard]] v3f position3d() const noexcept;

        void velocity(const v3f& value) noexcept;
        [[nodiscard]] v3f velocity() const noexcept;

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

        void volume(f32 value) noexcept;
        [[nodiscard]] f32 volume() const noexcept;

        sound_stream_ptr create_stream(
            buffer_view sound_data);

        sound_stream_ptr create_stream(
            const input_stream_uptr& file_stream);

        sound_source_ptr create_source(
            const sound_stream_ptr &stream);

        void resume() noexcept;
        void pause() noexcept;

        // Applies changes made to the 3D system.
        void apply3d() noexcept;

        void listener_position(const v3f& value) noexcept;
        [[nodiscard]] v3f listener_position() const noexcept;

        void listener_velocity(const v3f& value) noexcept;
        [[nodiscard]] v3f listener_velocity() const noexcept;

    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };

}
