/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
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
    // starter::parameters
    //

    class starter::parameters {
    public:
        parameters() = delete;
        parameters(const engine::parameters& engine_params);

        parameters& library_root(const url& value);
        parameters& engine_params(const engine::parameters& value);

        url& library_root() noexcept;
        engine::parameters& engine_params() noexcept;

        const url& library_root() const noexcept;
        const engine::parameters& engine_params() const noexcept;
    private:
        url library_root_{"resources://bin/library"};
        engine::parameters engine_params_;
    };
}

namespace e2d
{
    template < typename HighApplication, typename... Args >
    bool starter::start(Args&&... args) {
        return start(std::make_unique<HighApplication>(std::forward<Args>(args)...));
    }
}
