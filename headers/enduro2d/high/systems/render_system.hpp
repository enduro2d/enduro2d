/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_systems.hpp"

namespace e2d
{
    class render_system final
        : public ecs::system<ecs::after<systems::render_event>> {
    public:
        render_system();
        ~render_system() noexcept;

        void process(
            ecs::registry& owner,
            const ecs::after<systems::render_event>& trigger) override;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
