/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/starter.hpp>

#include <enduro2d/high/world.hpp>
#include <enduro2d/high/luasol.hpp>
#include <enduro2d/high/factory.hpp>
#include <enduro2d/high/library.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/behaviour.hpp>
#include <enduro2d/high/components/camera.hpp>
#include <enduro2d/high/components/flipbook_player.hpp>
#include <enduro2d/high/components/label.hpp>
#include <enduro2d/high/components/model_renderer.hpp>
#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/scene.hpp>
#include <enduro2d/high/components/spine_player.hpp>
#include <enduro2d/high/components/spine_player_cmd.hpp>
#include <enduro2d/high/components/spine_player_evt.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>

#include <enduro2d/high/systems/flipbook_system.hpp>
#include <enduro2d/high/systems/label_system.hpp>
#include <enduro2d/high/systems/render_system.hpp>
#include <enduro2d/high/systems/spine_system.hpp>

namespace
{
    using namespace e2d;

    template < typename Module, typename... Args >
    Module& safe_module_initialize(Args&&... args) {
        return modules::is_initialized<Module>()
            ? modules::instance<Module>()
            : modules::initialize<Module>(std::forward<Args>(args)...);
    }

    class engine_application final : public engine::application {
    public:
        engine_application(starter::application_uptr application)
        : application_(std::move(application)) {}

        bool initialize() final {
            ecs::registry_filler(the<world>().registry())
                .feature<struct flipbook_feature>(ecs::feature()
                    .add_system<flipbook_system>())
                .feature<struct label_feature>(ecs::feature()
                    .add_system<label_system>())
                .feature<struct spine_feature>(ecs::feature()
                    .add_system<spine_system>())
                .feature<struct render_feature>(ecs::feature()
                    .add_system<render_system>());
            return !application_ || application_->initialize();
        }

        void shutdown() noexcept final {
            if ( application_ ) {
                application_->shutdown();
            }
        }

        bool frame_tick() final {
            engine& e = the<engine>();
            const f32 dt = e.delta_time();
            const f32 time = e.time();

            ecs::registry& registry = the<world>().registry();
            registry.process_event(systems::pre_update_event{dt,time});
            registry.process_event(systems::update_event{dt,time});
            registry.process_event(systems::post_update_event{dt,time});

            return !the<window>().should_close()
                || (application_ && !application_->on_should_close());
        }

        void frame_render() final {
            ecs::registry& registry = the<world>().registry();
            registry.process_event(systems::pre_render_event{});
            registry.process_event(systems::render_event{});
            registry.process_event(systems::post_render_event{});
        }
    private:
        starter::application_uptr application_;
    };
}

namespace e2d
{
    //
    // starter::application
    //

    bool starter::application::initialize() {
        return true;
    }

    void starter::application::shutdown() noexcept {
    }

    bool starter::application::on_should_close() {
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
        safe_module_initialize<luasol>();
        safe_module_initialize<factory>()
            .register_component<actor>("actor")
            .register_component<behaviour>("behaviour")
            .register_component<behaviour::started>("behaviour.started")
            .register_component<behaviour::disabled>("behaviour.disabled")
            .register_component<camera>("camera")
            .register_component<flipbook_player>("flipbook_player")
            .register_component<label>("label")
            .register_component<label::dirty>("label.dirty")
            .register_component<model_renderer>("model_renderer")
            .register_component<renderer>("renderer")
            .register_component<scene>("scene")
            .register_component<spine_player>("spine_player")
            .register_component<spine_player_cmd>("spine_player_cmd")
            .register_component<spine_player_evt>("spine_player_evt")
            .register_component<sprite_renderer>("sprite_renderer");
        safe_module_initialize<library>(params.library_root(), the<deferrer>());
        safe_module_initialize<world>();
    }

    starter::~starter() noexcept {
        the<luasol>().collect_garbage();

        modules::shutdown<world>();
        modules::shutdown<library>();
        modules::shutdown<factory>();
        modules::shutdown<luasol>();
        modules::shutdown<engine>();
    }

    bool starter::start(application_uptr app) {
        return the<engine>().start(
            std::make_unique<engine_application>(
                std::move(app)));
    }
}
