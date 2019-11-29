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
#include <enduro2d/high/components/colliders.hpp>
#include <enduro2d/high/components/commands.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/events.hpp>
#include <enduro2d/high/components/flipbook_player.hpp>
#include <enduro2d/high/components/label.hpp>
#include <enduro2d/high/components/model_renderer.hpp>
#include <enduro2d/high/components/named.hpp>
#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/rigid_body.hpp>
#include <enduro2d/high/components/scene.hpp>
#include <enduro2d/high/components/spine_player.hpp>
#include <enduro2d/high/components/sprite_renderer.hpp>
#include <enduro2d/high/components/touchable.hpp>

#include <enduro2d/high/systems/flipbook_system.hpp>
#include <enduro2d/high/systems/input_system.hpp>
#include <enduro2d/high/systems/label_system.hpp>
#include <enduro2d/high/systems/render_system.hpp>
#include <enduro2d/high/systems/script_system.hpp>
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
                .feature<struct input_feature>(ecs::feature()
                    .add_system<input_system>())
                .feature<struct label_feature>(ecs::feature()
                    .add_system<label_system>())
                .feature<struct render_feature>(ecs::feature()
                    .add_system<render_system>())
                .feature<struct sript_feature>(ecs::feature()
                    .add_system<script_system>())
                .feature<struct spine_feature>(ecs::feature()
                    .add_system<spine_system>());
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

        void frame_finalize() final {
            world& w = the<world>();
            ecs::registry& registry = w.registry();
            registry.process_event(systems::frame_finalize_event{});
            w.finalize_instances();
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
    // starter::library_parameters
    //

    starter::library_parameters& starter::library_parameters::root(url value) noexcept {
        root_ = std::move(value);
        return *this;
    }

    url& starter::library_parameters::root() noexcept {
        return root_;
    }

    const url& starter::library_parameters::root() const noexcept {
        return root_;
    }

    //
    // starter::parameters
    //

    starter::parameters::parameters(engine::parameters engine_params) noexcept
    : engine_params_(std::move(engine_params)) {}

    starter::parameters& starter::parameters::engine_params(engine::parameters value) noexcept {
        engine_params_ = std::move(value);
        return *this;
    }

    starter::parameters& starter::parameters::library_params(library_parameters value) noexcept {
        library_params_ = std::move(value);
        return *this;
    }

    engine::parameters& starter::parameters::engine_params() noexcept {
        return engine_params_;
    }

    starter::library_parameters& starter::parameters::library_params() noexcept {
        return library_params_;
    }

    const engine::parameters& starter::parameters::engine_params() const noexcept {
        return engine_params_;
    }

    const starter::library_parameters& starter::parameters::library_params() const noexcept {
        return library_params_;
    }

    //
    // starter
    //

    starter::starter(int argc, char *argv[], const parameters& params) {
        safe_module_initialize<engine>(
            argc, argv,
            params.engine_params());

        safe_module_initialize<factory>()
            .register_component<actor>("actor")
            .register_component<behaviour>("behaviour")
            .register_component<camera>("camera")
            .register_component<rect_collider>("rect_collider")
            .register_component<circle_collider>("circle_collider")
            .register_component<polygon_collider>("polygon_collider")
            .register_component<flipbook_player>("flipbook_player")
            .register_component<label>("label")
            .register_component<label::dirty>("label.dirty")
            .register_component<model_renderer>("model_renderer")
            .register_component<named>("named")
            .register_component<renderer>("renderer")
            .register_component<rigid_body>("rigid_body")
            .register_component<scene>("scene")
            .register_component<spine_player>("spine_player")
            .register_component<events<spine_player_events::event>>("spine_player_events")
            .register_component<commands<spine_player_commands::command>>("spine_player_commands")
            .register_component<sprite_renderer>("sprite_renderer")
            .register_component<touchable>("touchable");

        safe_module_initialize<luasol>();

        safe_module_initialize<library>(
            params.library_params().root());

        safe_module_initialize<world>();
    }

    starter::~starter() noexcept {
        the<luasol>().collect_garbage();

        modules::shutdown<world>();
        modules::shutdown<library>();
        modules::shutdown<luasol>();
        modules::shutdown<factory>();
        modules::shutdown<engine>();
    }

    bool starter::start(application_uptr app) {
        return the<engine>().start(
            std::make_unique<engine_application>(
                std::move(app)));
    }
}
