/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_base.hpp"

namespace e2d { namespace stdex
{
    namespace impl
    {
        template < typename T >
        struct unique_if {
            using unique_single = std::unique_ptr<T>;
        };

        template < typename T >
        struct unique_if<T[]> {
            using unique_array_unknown_bound = std::unique_ptr<T[]>;
        };

        template < typename T, std::size_t N >
        struct unique_if<T[N]> {
            using unique_array_known_bound = void;
        };
    }

    template < typename T, typename... Args >
    typename impl::unique_if<T>::unique_single
        make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template < typename T >
    typename impl::unique_if<T>::unique_array_unknown_bound
        make_unique(std::size_t n)
    {
        using U = std::remove_extent_t<T>;
        return std::unique_ptr<T>(new U[n]());
    }

    template < typename T, typename... Args >
    typename impl::unique_if<T>::unique_array_known_bound
        make_unique(Args&&...) = delete;
}}
