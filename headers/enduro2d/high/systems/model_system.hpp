/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d
{
    class model_system : public ecs::system {
    public:
        model_system();
        ~model_system() noexcept final;
        void process(ecs::registry& owner) override;
    private:
        class internal_state;
        std::unique_ptr<internal_state> state_;
    };
}
