/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "script.hpp"

namespace e2d
{
    class luasol final : public module<luasol> {
    public:
        luasol();
        ~luasol() noexcept final = default;

        template < typename F  >
        decltype(auto) with_state(F&& f);
        template < typename F >
        decltype(auto) with_state(F&& f) const;

        std::optional<script> load_script(buffer_view src);
        std::optional<script> load_script(const input_stream_uptr& src);
    private:
        sol::state state_;
    };
}

namespace e2d
{
    template < typename F  >
    decltype(auto) luasol::with_state(F&& f) {
        E2D_ASSERT(is_in_main_thread());
        return std::invoke(std::forward<F>(f), state_);
    }

    template < typename F >
    decltype(auto) luasol::with_state(F&& f) const {
        E2D_ASSERT(is_in_main_thread());
        return std::invoke(std::forward<F>(f), state_);
    }
}
