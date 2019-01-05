/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/starter.hpp>

#include <enduro2d/high/assets.hpp>
#include <enduro2d/high/library.hpp>
#include <enduro2d/high/world.hpp>

namespace
{
    using namespace e2d;

    template < typename Module, typename... Args >
    void safe_module_initialize(Args&&... args) {
        if ( !modules::is_initialized<Module>() ) {
            modules::initialize<Module>(std::forward<Args>(args)...);
        }
    }

    class starter_application final : public application {
    public:
        starter_application(application_uptr application)
        : application_(std::move(application)) {}

        bool initialize() final {
            return application_ && application_->initialize();
        }

        void shutdown() noexcept final {
            if ( application_ ) {
                application_->shutdown();
            }
        }

        bool frame_tick() final {
            return application_ && application_->frame_tick();
        }
    private:
        application_uptr application_;
    };
}

namespace e2d
{
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
        safe_module_initialize<library>(params.library_root());
        safe_module_initialize<asset_cache<text_asset>>(the<library>());
        safe_module_initialize<asset_cache<mesh_asset>>(the<library>());
        safe_module_initialize<asset_cache<image_asset>>(the<library>());
        safe_module_initialize<asset_cache<binary_asset>>(the<library>());
        safe_module_initialize<asset_cache<shader_asset>>(the<library>());
        safe_module_initialize<asset_cache<texture_asset>>(the<library>());
        safe_module_initialize<asset_cache<material_asset>>(the<library>());
        safe_module_initialize<world>();
    }

    starter::~starter() noexcept {
        modules::shutdown<world>();
        modules::shutdown<asset_cache<material_asset>>();
        modules::shutdown<asset_cache<texture_asset>>();
        modules::shutdown<asset_cache<shader_asset>>();
        modules::shutdown<asset_cache<binary_asset>>();
        modules::shutdown<asset_cache<image_asset>>();
        modules::shutdown<asset_cache<mesh_asset>>();
        modules::shutdown<asset_cache<text_asset>>();
        modules::shutdown<library>();
        modules::shutdown<engine>();
    }

    bool starter::start(application_uptr app) {
        return the<engine>().start(
            std::make_unique<starter_application>(
                std::move(app)));
    }
}
