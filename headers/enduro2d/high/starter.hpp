/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    //
    // starter
    //

    class starter final : public module<starter> {
    public:
        class application;
        using application_uptr = std::unique_ptr<application>;
    public:
        class library_parameters;
        class parameters;
    public:
        starter(int argc, char *argv[], const parameters& params);
        ~starter() noexcept final;

        template < typename HighApplication, typename... Args >
        bool start(Args&&... args);
        bool start(application_uptr app);
    };

    //
    // starter::application
    //

    class starter::application : private e2d::noncopyable {
    public:
        virtual ~application() noexcept = default;
        virtual bool initialize();
        virtual void shutdown() noexcept;
        virtual bool on_should_close();
    };

    //
    // starter::library_parameters
    //

    class starter::library_parameters {
    public:
        library_parameters& root(url value) noexcept;
        const url& root() const noexcept;
    private:
        url root_{"resources://bin/library"};
    };

    //
    // starter::parameters
    //

    class starter::parameters {
    public:
        parameters() = delete;
        parameters(engine::parameters engine_params) noexcept;

        parameters& engine_params(engine::parameters value) noexcept;
        parameters& library_params(library_parameters value) noexcept;

        engine::parameters& engine_params() noexcept;
        library_parameters& library_params() noexcept;

        const engine::parameters& engine_params() const noexcept;
        const library_parameters& library_params() const noexcept;
    private:
        engine::parameters engine_params_;
        library_parameters library_params_;
    };
}

namespace e2d
{
    template < typename HighApplication, typename... Args >
    bool starter::start(Args&&... args) {
        return start(std::make_unique<HighApplication>(std::forward<Args>(args)...));
    }
}
