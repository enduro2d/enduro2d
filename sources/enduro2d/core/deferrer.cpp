/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/core/deferrer.hpp>

#include <enduro2d/core/profiler.hpp>

namespace e2d
{
    deferrer::deferrer()
    : worker_(math::max(2u, std::thread::hardware_concurrency()) - 1u) {}

    stdex::jobber& deferrer::worker() noexcept {
        return worker_;
    }

    const stdex::jobber& deferrer::worker() const noexcept {
        return worker_;
    }

    stdex::scheduler& deferrer::scheduler() noexcept {
        return scheduler_;
    }

    const stdex::scheduler& deferrer::scheduler() const noexcept {
        return scheduler_;
    }

    void deferrer::frame_tick() noexcept {
        E2D_PROFILER_SCOPE("deferrer.frame_tick");
        scheduler_.process_all_tasks();
    }
}
