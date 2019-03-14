/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    //
    // high_application
    //

    class high_application : private noncopyable {
    public:
        virtual ~high_application() noexcept = default;
        virtual bool initialize();
        virtual void shutdown() noexcept;
        virtual bool on_should_close();
    };
    using high_application_uptr = std::unique_ptr<high_application>;

    //
    // starter
    //

    class starter final : public module<starter> {
    public:
        class parameters {
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
    public:
        starter(int argc, char *argv[], const parameters& params);
        ~starter() noexcept final;

        template < typename HighApplication, typename... Args >
        bool start(Args&&... args);
        bool start(high_application_uptr app);
    };
}

namespace e2d
{
    template < typename HighApplication, typename... Args >
    bool starter::start(Args&&... args) {
        return start(std::make_unique<HighApplication>(std::forward<Args>(args)...));
    }
}
