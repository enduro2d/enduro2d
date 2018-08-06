/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../catch/catch.hpp"
#include <enduro2d/enduro2d.hpp>

namespace e2d_untests
{
    using namespace e2d;

    template < typename TimeTag >
    class verbose_profiler : noncopyable {
    public:
        verbose_profiler(const str& desc)
        : desc_(desc) {
            begin_ = time::now<TimeTag>();
        }

        template < typename T >
        void done(const T& result) const {
            const auto end = time::now<TimeTag>();
            std::printf(
                "result: %s, time: %s, desc: %s\n",
                std::to_string(result).c_str(),
                std::to_string((end - begin_).value).c_str(),
                desc_.c_str());
        }
    private:
        str desc_;
        unit<i64, TimeTag> begin_;
    };
    using verbose_profiler_us = verbose_profiler<microseconds_tag>;
    using verbose_profiler_ms = verbose_profiler<milliseconds_tag>;
}
