/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_systems.hpp"

namespace e2d
{
    class frame_system final
        : public ecs::system<
            ecs::after<systems::frame_update_event>,
            ecs::after<systems::frame_render_event>> {
    public:
        frame_system();
        ~frame_system() noexcept;

        void process(
            ecs::registry& owner,
            const ecs::after<systems::frame_update_event>& trigger) override;

        void process(
            ecs::registry& owner,
            const ecs::after<systems::frame_render_event>& trigger) override;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
