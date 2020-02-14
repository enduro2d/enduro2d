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
        class defer_impl : noncopyable {
        public:
            explicit defer_impl(F f)
            : f_(std::move(f)) {}

            virtual ~defer_impl() noexcept {
                if ( !dismissed_ ) {
                    f_();
                }
            }

            void dismiss() noexcept {
                dismissed_ = true;
            }
        private:
            F f_;
            bool dismissed_{};
        };

        template < typename F >
        class error_defer_impl final : public defer_impl<F> {
        public:
            explicit error_defer_impl(F f)
            : defer_impl<F>(std::move(f))
            , exceptions_(std::uncaught_exceptions()) {}

            ~error_defer_impl() noexcept final {
                if ( exceptions_ == std::uncaught_exceptions() ) {
                    this->dismiss();
                }
            }
        private:
            int exceptions_{};
        };

        template < typename F >
        class return_defer_impl final : public defer_impl<F> {
        public:
            explicit return_defer_impl(F f)
            : defer_impl<F>(std::move(f))
            , exceptions_(std::uncaught_exceptions()) {}

            ~return_defer_impl() noexcept final {
                if ( exceptions_ != std::uncaught_exceptions() ) {
                    this->dismiss();
                }
            }
        private:
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

#define E2D_DEFER(...)\
    auto E2D_PP_CAT(e2d_generated_defer_, __LINE__) =\
        ::e2d::make_defer(__VA_ARGS__)

#define E2D_ERROR_DEFER(...)\
    auto E2D_PP_CAT(e2d_generated_error_defer_, __LINE__) =\
        ::e2d::make_error_defer(__VA_ARGS__)

#define E2D_RETURN_DEFER(...)\
    auto E2D_PP_CAT(e2d_generated_return_defer_, __LINE__) =\
        ::e2d::make_return_defer(__VA_ARGS__)
