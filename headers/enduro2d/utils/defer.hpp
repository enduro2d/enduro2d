/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
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

            ~defer_impl() noexcept {
                f_();
            }
        private:
            F f_;
        };

        template < typename F >
        class error_defer_impl final : noncopyable {
        public:
            explicit error_defer_impl(F f)
            : f_(std::move(f))
            , exceptions_(std::uncaught_exceptions()) {}

            ~error_defer_impl() noexcept {
                if ( exceptions_ != std::uncaught_exceptions() ) {
                    f_();
                }
            }
        private:
            F f_{};
            int exceptions_{};
        };

        template < typename F >
        class return_defer_impl final : noncopyable {
        public:
            explicit return_defer_impl(F f)
            : f_(std::move(f))
            , exceptions_(std::uncaught_exceptions()) {}

            ~return_defer_impl() noexcept {
                if ( exceptions_ == std::uncaught_exceptions() ) {
                    f_();
                }
            }
        private:
            F f_{};
            int exceptions_{};
        };
    }

    template < typename F >
    impl::defer_impl<std::decay_t<F>> make_defer(F&& f) {
        return impl::defer_impl<std::decay_t<F>>(std::forward<F>(f));
    }

    template < typename F >
    impl::error_defer_impl<std::decay_t<F>> make_error_defer(F&& f) {
        return impl::error_defer_impl<std::decay_t<F>>(std::forward<F>(f));
    }

    template < typename F >
    impl::return_defer_impl<std::decay_t<F>> make_return_defer(F&& f) {
        return impl::return_defer_impl<std::decay_t<F>>(std::forward<F>(f));
    }
}

#define E2D_DEFER(lambda)\
    auto E2D_PP_CAT(e2d_generated_defer_, __LINE__) =\
        ::e2d::make_defer(lambda)

#define E2D_ERROR_DEFER(lambda)\
    auto E2D_PP_CAT(e2d_generated_error_defer_, __LINE__) =\
        ::e2d::make_error_defer(lambda)

#define E2D_RETURN_DEFER(lambda)\
    auto E2D_PP_CAT(e2d_generated_return_defer_, __LINE__) =\
        ::e2d::make_return_defer(lambda)
