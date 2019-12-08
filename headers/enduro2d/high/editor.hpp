/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "gobject.hpp"

namespace e2d
{
    class editor final : public module<editor> {
    public:
        editor();
        ~editor() noexcept final;

        void select(const gobject& go);
        bool is_selected(const gobject& go) const noexcept;
        void unselect(const gobject& go) noexcept;
        void clear_selection() noexcept;
    private:
        mutable std::mutex mutex_;
        flat_set<gobject> selection_;
    };
}
