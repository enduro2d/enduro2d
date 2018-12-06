/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    //
    // application
    //

    class application : private noncopyable {
    public:
        virtual ~application() noexcept = default;
        virtual bool initialize();
        virtual void shutdown() noexcept;
        virtual bool frame_tick();
    };
    using application_uptr = std::unique_ptr<application>;

    //
    // bad_engine_operation
    //

    class bad_engine_operation final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad engine operation";
        }
    };

    //
    // engine
    //

    class engine final : public module<engine> {
    public:
        class debug_parameters {
        public:
            debug_parameters& log_filename(str_view value);
            debug_parameters& file_logging(bool value) noexcept;
            debug_parameters& console_logging(bool value) noexcept;

            const str& log_filename() const noexcept;
            bool file_logging() const noexcept;
            bool console_logging() const noexcept;
        private:
            str log_filename_{"log.txt"};
            bool file_logging_{true};
            bool console_logging_{true};
        };

        class window_parameters {
        public:
            window_parameters& caption(str_view value);
            window_parameters& size(const v2u& value) noexcept;
            window_parameters& fullscreen(bool value) noexcept;

            const str& caption() const noexcept;
            const v2u& size() const noexcept;
            bool fullscreen() const noexcept;
        private:
            str caption_{"Enduro2D"};
            v2u size_{640, 480};
            bool fullscreen_{false};
        };

        class timer_parameters {
        public:
            timer_parameters& minimal_framerate(u32 value) noexcept;
            timer_parameters& maximal_framerate(u32 value) noexcept;

            u32 minimal_framerate() const noexcept;
            u32 maximal_framerate() const noexcept;
        private:
            u32 minimal_framerate_{30u};
            u32 maximal_framerate_{1000u};
        };

        class parameters {
        public:
            parameters() = delete;
            parameters(str_view game_name, str_view company_name);

            parameters& game_name(str_view value);
            parameters& company_name(str_view value);
            parameters& debug_params(const debug_parameters& value);
            parameters& window_params(const window_parameters& value);
            parameters& timer_params(const timer_parameters& value);

            str& game_name() noexcept;
            str& company_name() noexcept;
            debug_parameters& debug_params() noexcept;
            window_parameters& window_params() noexcept;
            timer_parameters& timer_params() noexcept;

            const str& game_name() const noexcept;
            const str& company_name() const noexcept;
            const debug_parameters& debug_params() const noexcept;
            const window_parameters& window_params() const noexcept;
            const timer_parameters& timer_params() const noexcept;
        private:
            str game_name_{"noname"};
            str company_name_{"noname"};
            debug_parameters debug_params_;
            window_parameters window_params_;
            timer_parameters timer_params_;
        };
    public:
        engine(int argc, char *argv[], const parameters& params);
        ~engine() noexcept final;

        template < typename Application, typename... Args >
        bool start(Args&&... args);
        bool start(application_uptr app);

        f32 time() const noexcept;
        f32 delta_time() const noexcept;

        u32 frame_rate() const noexcept;
        u32 frame_count() const noexcept;
        f32 realtime_time() const noexcept;

        scheduler& main_thread_scheduler() noexcept;
        const scheduler& main_thread_scheduler() const noexcept;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}

namespace e2d
{
    template < typename Application, typename... Args >
    bool engine::start(Args&&... args) {
        return start(std::make_unique<Application>(std::forward<Args>(args)...));
    }
}
