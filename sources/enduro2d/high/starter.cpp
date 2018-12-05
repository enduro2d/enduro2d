/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/starter.hpp>

#include <enduro2d/high/library.hpp>

namespace
{
    using namespace e2d;

    template < typename Module, typename... Args >
    void safe_module_initialize(Args&&... args) {
        if ( !modules::is_initialized<Module>() ) {
            modules::initialize<Module>(std::forward<Args>(args)...);
        }
    }
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
    }

    starter::~starter() noexcept = default;

    bool starter::start(application_uptr app) {
        return the<engine>().start(std::move(app));
    }
}
