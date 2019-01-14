/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d
{
    namespace systems
    {
        class sprite_system : public ecs::system {
        public:
            sprite_system();
            ~sprite_system() noexcept final;
            void process(ecs::registry& owner) override;
        private:
            class internal_state;
            std::unique_ptr<internal_state> state_;
        };
    }
}
