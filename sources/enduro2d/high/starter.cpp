/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/starter.hpp>

#include <enduro2d/high/world.hpp>
#include <enduro2d/high/library.hpp>
#include <enduro2d/high/component.hpp>

#include <enduro2d/high/components/actor_l.hpp>
#include <enduro2d/high/components/camera_l.hpp>
#include <enduro2d/high/components/flipbook_player_l.hpp>
#include <enduro2d/high/components/flipbook_source_l.hpp>
#include <enduro2d/high/components/model_renderer_l.hpp>
#include <enduro2d/high/components/renderer_l.hpp>
#include <enduro2d/high/components/scene_l.hpp>
#include <enduro2d/high/components/sprite_renderer_l.hpp>

#include <enduro2d/high/systems/flipbook_system.hpp>
#include <enduro2d/high/systems/render_system.hpp>

namespace
{
    using namespace e2d;

    template < typename Module, typename... Args >
    Module& safe_module_initialize(Args&&... args) {
        return modules::is_initialized<Module>()
            ? modules::instance<Module>()
            : modules::initialize<Module>(std::forward<Args>(args)...);
    }

    class starter_application final : public application {
    public:
        starter_application(high_application_uptr application)
        : high_application_(std::move(application)) {}

        bool initialize() final {
            ecs::registry_filler(the<world>().registry())
                .system<flipbook_system>(world::priority_update)
                .system<render_system>(world::priority_render);
            return !high_application_ || high_application_->initialize();
        }

        void shutdown() noexcept final {
            if ( high_application_ ) {
                high_application_->shutdown();
            }
        }

        bool frame_tick() final {
            the<world>().registry().process_systems_in_range(
                world::priority_update_section_begin,
                world::priority_update_section_end);
            return !the<window>().should_close()
                || (high_application_ && !high_application_->on_should_close());
        }

        void frame_render() final {
            the<world>().registry().process_systems_in_range(
                world::priority_render_section_begin,
                world::priority_render_section_end);
        }
    private:
        high_application_uptr high_application_;
    };
}

namespace e2d
{
    //
    // high_application
    //

    bool high_application::initialize() {
        return true;
    }

    void high_application::shutdown() noexcept {
    }

    bool high_application::on_should_close() {
        return true;
    }

    //
    // starter::parameters
    //

    starter::parameters::parameters(const engine::parameters& engine_params)
    : engine_params_(engine_params) {}

    starter::parameters& starter::parameters::library_root(const url& value) {
        library_root_ = value;
        return *this;
    }

    starter::parameters& starter::parameters::engine_params(const engine::parameters& value) {
        engine_params_ = value;
        return *this;
    }

    url& starter::parameters::library_root() noexcept {
        return library_root_;
    }

    engine::parameters& starter::parameters::engine_params() noexcept {
        return engine_params_;
    }

    const url& starter::parameters::library_root() const noexcept {
        return library_root_;
    }

    const engine::parameters& starter::parameters::engine_params() const noexcept {
        return engine_params_;
    }

    //
    // starter
    //

    starter::starter(int argc, char *argv[], const parameters& params) {
        safe_module_initialize<engine>(argc, argv, params.engine_params());
        safe_module_initialize<component_factory>()
            .register_component<actor>("actor")
            .register_component<camera>("camera")
            .register_component<flipbook_player>("flipbook_player")
            .register_component<flipbook_source>("flipbook_source")
            .register_component<model_renderer>("model_renderer")
            .register_component<renderer>("renderer")
            .register_component<scene>("scene")
            .register_component<sprite_renderer>("sprite_renderer");
        safe_module_initialize<library>(params.library_root(), the<deferrer>());
        safe_module_initialize<world>();
    }

    starter::~starter() noexcept {
        modules::shutdown<world>();
        modules::shutdown<library>();
        modules::shutdown<component_factory>();
        modules::shutdown<engine>();
    }

    bool starter::start(high_application_uptr app) {
        return the<engine>().start(
            std::make_unique<starter_application>(
                std::move(app)));
    }
}
