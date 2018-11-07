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

    //
    // engine::internal_state
    //

    class engine::internal_state final : private e2d::noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;
    };

    //
    // engine
    //

    engine::engine(const parameters& params)
    : state_(new internal_state())
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
        if ( !app || !app->initialize() ) {
            the<debug>().error("ENGINE: Failed to initialize application");
            return false;
        }
        try {
            while ( app->frame_tick() ) {
                the<input>().frame_tick();
                window::poll_events();
            }
        } catch ( ... ) {
            app->shutdown();
            throw;
        }
        app->shutdown();
        return true;
    }
}
