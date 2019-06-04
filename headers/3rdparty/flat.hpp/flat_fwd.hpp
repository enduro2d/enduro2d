/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/flat.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <vector>
#include <cassert>
#include <utility>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <initializer_list>

#include "detail/is_sorted.hpp"
#include "detail/eq_compare.hpp"
#include "detail/pair_compare.hpp"
#include "detail/is_transparent.hpp"

namespace flat_hpp
{
    struct sorted_range_t {};
    inline constexpr sorted_range_t sorted_range = sorted_range_t();

    struct sorted_unique_range_t : public sorted_range_t {};
    inline constexpr sorted_unique_range_t sorted_unique_range = sorted_unique_range_t();

    template < typename Key
             , typename Compare = std::less<Key>
             , typename Container = std::vector<Key> >
    class flat_set;

    template < typename Key
             , typename Compare = std::less<Key>
             , typename Container = std::vector<Key> >
    class flat_multiset;

    template < typename Key
             , typename Value
             , typename Compare = std::less<Key>
             , typename Container = std::vector<std::pair<Key, Value>> >
    class flat_map;

    template < typename Key
             , typename Value
             , typename Compare = std::less<Key>
             , typename Container = std::vector<std::pair<Key, Value>> >
    class flat_multimap;
}
