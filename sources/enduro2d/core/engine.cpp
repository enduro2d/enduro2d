/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/core/engine.hpp>

#include <enduro2d/core/debug.hpp>
#include <enduro2d/core/input.hpp>
#include <enduro2d/core/render.hpp>
#include <enduro2d/core/vfs.hpp>
#include <enduro2d/core/window.hpp>

namespace
{
    using namespace e2d;

    template < typename Module, typename... Args >
    void safe_module_initialize(Args&&... args) {
        if ( !modules::is_initialized<Module>() ) {
            modules::initialize<Module>(std::forward<Args>(args)...);
        }
    }

    void safe_register_predef_path(
        vfs& the_vfs,
        str_view scheme,
        filesystem::predef_path predef_path)
    {
        str path;
        if ( filesystem::extract_predef_path(path, predef_path) ) {
            the_vfs.register_scheme_alias(scheme, url{"file", path});
        }
    }
}

namespace e2d
{
    //
    // application
    //

    bool application::initialize() {
        return true;
    }

    void application::shutdown() noexcept {
    }

    bool application::frame_tick() {
        return true;
    }

    //
    // engine::debug_parameters
    //

    engine::debug_parameters& engine::debug_parameters::log_filename(str_view value) {
        log_filename_ = value;
        return *this;
    }

    engine::debug_parameters& engine::debug_parameters::file_logging(bool value) noexcept {
        file_logging_ = value;
        return *this;
    }

    engine::debug_parameters& engine::debug_parameters::console_logging(bool value) noexcept {
        console_logging_ = value;
        return *this;
    }

    const str& engine::debug_parameters::log_filename() const noexcept {
        return log_filename_;
    }

    bool engine::debug_parameters::file_logging() const noexcept {
        return file_logging_;
    }

    bool engine::debug_parameters::console_logging() const noexcept {
        return console_logging_;
    }

    //
    // engine::timer_parameters
    //

    engine::timer_parameters& engine::timer_parameters::minimal_framerate(u32 value) noexcept {
        minimal_framerate_ = value;
        return *this;
    }

    engine::timer_parameters& engine::timer_parameters::maximal_framerate(u32 value) noexcept {
        maximal_framerate_ = value;
        return *this;
    }

    u32 engine::timer_parameters::minimal_framerate() const noexcept {
        return minimal_framerate_;
    }

    u32 engine::timer_parameters::maximal_framerate() const noexcept {
        return maximal_framerate_;
    }

    //
    // engine::window_parameters
    //

    engine::window_parameters& engine::window_parameters::caption(str_view value) {
        caption_ = value;
        return *this;
    }

    engine::window_parameters& engine::window_parameters::size(const v2u& value) noexcept {
        size_ = value;
        return *this;
    }

    engine::window_parameters& engine::window_parameters::fullscreen(bool value) noexcept {
        fullscreen_ = value;
        return *this;
    }

    const str& engine::window_parameters::caption() const noexcept {
        return caption_;
    }

    const v2u& engine::window_parameters::size() const noexcept {
        return size_;
    }

    bool engine::window_parameters::fullscreen() const noexcept {
        return fullscreen_;
    }

    //
    // engine::parameters
    //

    engine::parameters::parameters(str_view game_name, str_view company_name)
    : game_name_(game_name)
    , company_name_(company_name) {}

    engine::parameters& engine::parameters::game_name(str_view value) noexcept {
        game_name_ = value;
        return *this;
    }

    engine::parameters& engine::parameters::company_name(str_view value) noexcept {
        company_name_ = value;
        return *this;
    }

    engine::parameters& engine::parameters::debug_params(const debug_parameters& value) {
        debug_params_ = value;
        return *this;
    }

    engine::parameters& engine::parameters::window_params(const window_parameters& value) {
        window_params_ = value;
        return *this;
    }

    engine::parameters& engine::parameters::timer_params(const timer_parameters& value) {
        timer_params_ = value;
        return *this;
    }

    str& engine::parameters::game_name() noexcept {
        return game_name_;
    }

    str& engine::parameters::company_name() noexcept {
        return company_name_;
    }

    engine::debug_parameters& engine::parameters::debug_params() noexcept {
        return debug_params_;
    }

    engine::window_parameters& engine::parameters::window_params() noexcept {
        return window_params_;
    }

    engine::timer_parameters& engine::parameters::timer_params() noexcept {
        return timer_params_;
    }

    const str& engine::parameters::game_name() const noexcept {
        return game_name_;
    }

    const str& engine::parameters::company_name() const noexcept {
        return company_name_;
    }

    const engine::debug_parameters& engine::parameters::debug_params() const noexcept {
        return debug_params_;
    }

    const engine::window_parameters& engine::parameters::window_params() const noexcept {
        return window_params_;
    }

    const engine::timer_parameters& engine::parameters::timer_params() const noexcept {
        return timer_params_;
    }

    //
    // engine
    //

    class engine::internal_state final : private e2d::noncopyable {
    public:
        internal_state(const parameters& params)
        : timer_params_(params.timer_params())
        {
            const auto first_frame_time = math::clamp(
                math::max(timer_params_.minimal_framerate(), timer_params_.maximal_framerate()),
                1u,
                1000u);
            delta_time_us_.store(
                (time::second_us<u64>() / math::numeric_cast<u64>(first_frame_time)).value);
        }
        ~internal_state() noexcept = default;
    public:
        f32 time() const noexcept {
            return time::to_seconds(
                make_microseconds(time_us_.load()).cast_to<f32>()).value;
        }

        f32 delta_time() const noexcept {
            return time::to_seconds(
                make_microseconds(delta_time_us_.load()).cast_to<f32>()).value;
        }

        u32 frame_rate() const noexcept {
            return frame_rate_.load();
        }

        u32 frame_count() const noexcept {
            return frame_count_.load();
        }

        f32 realtime_time() const noexcept {
            const auto delta_us = time::now_us().cast_to<u64>() - init_time_;
            return time::to_seconds(delta_us.cast_to<f32>()).value;
        }
    public:
        void calculate_end_frame_timers() noexcept {
            const auto second_us = time::second_us<u64>();

            const auto minimal_delta_time_us =
                second_us / math::numeric_cast<u64>(math::clamp(
                    timer_params_.maximal_framerate(), 1u, 1000u));

            const auto maximal_delta_time_us =
                second_us / math::numeric_cast<u64>(math::clamp(
                    timer_params_.minimal_framerate(), 1u, 1000u));

            auto now_us = time::now_us().cast_to<u64>();
            while ( now_us - prev_frame_time_ < minimal_delta_time_us ) {
                std::this_thread::yield();
                now_us = time::now_us().cast_to<u64>();
            }

            delta_time_us_.store(math::minimized(
                now_us - prev_frame_time_,
                maximal_delta_time_us).value);

            time_us_.store((now_us - init_time_).value);
            prev_frame_time_ = now_us;

            frame_count_.fetch_add(1);
            frame_rate_counter_.fetch_add(1);
            while ( now_us - prev_frame_rate_time_ >= second_us ) {
                prev_frame_rate_time_ += second_us;
                frame_rate_.store(frame_rate_counter_.exchange(0));
            }
        }
    private:
        timer_parameters timer_params_;
        microseconds<u64> init_time_{time::now_us().cast_to<u64>()};
        microseconds<u64> prev_frame_time_{time::now_us().cast_to<u64>()};
        microseconds<u64> prev_frame_rate_time_{time::now_us().cast_to<u64>()};
        std::atomic_uint64_t time_us_{0};
        std::atomic_uint64_t delta_time_us_{0};
        std::atomic_uint32_t frame_rate_{0};
        std::atomic_uint32_t frame_count_{0};
        std::atomic_uint32_t frame_rate_counter_{0};
        u8 _pad[4] = {0};
    };

    //
    // engine
    //

    engine::engine(const parameters& params)
    : state_(new internal_state(params))
    {
        // setup debug

        safe_module_initialize<debug>();

        if ( params.debug_params().console_logging() ) {
            the<debug>().register_sink<debug_console_sink>();
        }

        // setup vfs

        safe_module_initialize<vfs>();

        the<vfs>().register_scheme<filesystem_file_source>("file");
        safe_register_predef_path(the<vfs>(), "home", filesystem::predef_path::home);
        safe_register_predef_path(the<vfs>(), "appdata", filesystem::predef_path::appdata);
        safe_register_predef_path(the<vfs>(), "desktop", filesystem::predef_path::desktop);
        safe_register_predef_path(the<vfs>(), "working", filesystem::predef_path::working);
        safe_register_predef_path(the<vfs>(), "documents", filesystem::predef_path::documents);
        safe_register_predef_path(the<vfs>(), "resources", filesystem::predef_path::resources);
        safe_register_predef_path(the<vfs>(), "executable", filesystem::predef_path::executable);

        if ( params.debug_params().file_logging() ) {
            url log_url = url("appdata://")
                / params.company_name()
                / params.game_name()
                / params.debug_params().log_filename();
            output_stream_uptr log_stream = the<vfs>().write(log_url, false);
            the<debug>().register_sink<debug_stream_sink>(std::move(log_stream));
        }

        // setup input

        safe_module_initialize<input>();

        // setup window

        safe_module_initialize<window>(
            params.window_params().size(),
            params.window_params().caption(),
            params.window_params().fullscreen());

        the<window>().register_event_listener<window_input_source>(the<input>());

        // setup render

        safe_module_initialize<render>(
            the<debug>(),
            the<window>());
    }

    engine::~engine() noexcept = default;

    bool engine::start(application_uptr app) {
        E2D_ASSERT(main_thread() == std::this_thread::get_id());

        if ( !app || !app->initialize() ) {
            the<debug>().error("ENGINE: Failed to initialize application");
            return false;
        }

        while ( true ) {
            try {
                if ( !app->frame_tick() ) {
                    break;
                }
                state_->calculate_end_frame_timers();
            } catch ( ... ) {
                app->shutdown();
                throw;
            }
            the<input>().frame_tick();
            window::poll_events();
        }

        app->shutdown();
        return true;
    }

    f32 engine::time() const noexcept {
        return state_->time();
    }

    f32 engine::delta_time() const noexcept {
        return state_->delta_time();
    }

    u32 engine::frame_rate() const noexcept {
        return state_->frame_rate();
    }

    u32 engine::frame_count() const noexcept {
        return state_->frame_count();
    }

    f32 engine::realtime_time() const noexcept {
        return state_->realtime_time();
    }
}
