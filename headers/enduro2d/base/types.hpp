/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "stdex.hpp"

namespace e2d
{
    using f32 = float;
    using f64 = double;

    using i8  = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using u8  = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;
}

namespace e2d
{
    class exception
    : public std::exception {};

    template < typename Value
             , std::size_t Size >
    using array = std::array<Value, Size>;

    template < typename Value
             , typename Allocator = std::allocator<Value> >
    using vector = std::vector<Value, Allocator>;

    template < typename Key
             , typename Comp = std::less<Key>
             , typename Allocator = std::allocator<Key> >
    using set = std::set<Key, Comp, Allocator>;

    template < typename Key
             , typename Value
             , typename Comp = std::less<Key>
             , typename Allocator = std::allocator<std::pair<const Key, Value>> >
    using map = std::map<Key, Value, Comp, Allocator>;

    template < typename Value
             , typename Hash = std::hash<Value>
             , typename Pred = std::equal_to<Value>
             , typename Allocator = std::allocator<Value> >
    using hash_set = std::unordered_set<Value, Hash, Pred, Allocator>;

    template < typename Key
             , typename Value
             , typename Hash = std::hash<Key>
             , typename Pred = std::equal_to<Key>
             , typename Allocator = std::allocator<std::pair<const Key, Value>> >
    using hash_map = std::unordered_map<Key, Value, Hash, Pred, Allocator>;

    template < typename Char
             , typename Traits = std::char_traits<Char>
             , typename Allocator = std::allocator<Char> >
    using basic_string = std::basic_string<Char, Traits, Allocator>;

    template < typename Char
             , typename Traits = std::char_traits<Char> >
    using basic_string_view = stdex::basic_string_view<Char, Traits>;
}
