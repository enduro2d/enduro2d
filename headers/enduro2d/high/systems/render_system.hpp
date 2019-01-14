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
        class render_system final : public ecs::system {
        public:
            render_system();
            ~render_system() noexcept final;
            void process(ecs::registry& owner) override;
        private:
            class internal_state;
            std::unique_ptr<internal_state> state_;
        };
    }
}
