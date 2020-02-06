/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_systems.hpp"

namespace e2d
{
    class script_system final
        : public ecs::system<
            systems::update_event,
            ecs::before<systems::update_event>> {
    public:
        script_system();
        ~script_system() noexcept;

        void process(
            ecs::registry& owner,
            const systems::update_event& event) override;

        void process(
            ecs::registry& owner,
            const ecs::before<systems::update_event>& trigger) override;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
