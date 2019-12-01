/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    namespace impl
    {
        template < typename F >
        class defer_impl final : noncopyable {
        public:
            explicit defer_impl(F f)
            : f_(std::move(f)) {}

            ~defer_impl() noexcept(std::is_nothrow_invocable_v<F>) {
                if ( !cancelled_ ) {
                    f_();
                }
            }

            void cancel() noexcept {
                cancelled_ = true;
            }
        private:
            F f_;
            bool cancelled_{false};
        };
    }

    template < typename F >
    impl::defer_impl<std::decay_t<F>> make_defer(F&& f) {
        return impl::defer_impl<std::decay_t<F>>(std::forward<F>(f));
    }
}

#define E2D_DEFER(lambda)\
    auto E2D_PP_CAT(e2d_generated_defer_, __LINE__) = ::e2d::make_defer(lambda)
