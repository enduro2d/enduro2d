/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d
{
    template < typename C >
    class commands final {
    public:
        commands() = default;

        [[nodiscard]]
        typename vector<C>::const_iterator
        begin() const noexcept;

        [[nodiscard]]
        typename vector<C>::const_iterator
        cbegin() const noexcept;

        [[nodiscard]]
        typename vector<C>::const_iterator
        end() const noexcept;

        [[nodiscard]]
        typename vector<C>::const_iterator
        cend() const noexcept;

        commands& add(C command);
        commands& clear() noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;
    private:
        vector<C> commands_;
    };
}

namespace e2d
{
    template < typename C >
    typename vector<C>::const_iterator commands<C>::begin() const noexcept {
        return commands_.begin();
    }

    template < typename C >
    typename vector<C>::const_iterator commands<C>::cbegin() const noexcept {
        return commands_.cbegin();
    }

    template < typename C >
    typename vector<C>::const_iterator commands<C>::end() const noexcept {
        return commands_.end();
    }

    template < typename C >
    typename vector<C>::const_iterator commands<C>::cend() const noexcept {
        return commands_.cend();
    }

    template < typename C >
    commands<C>& commands<C>::add(C command) {
        commands_.push_back(std::move(command));
        return *this;
    }

    template < typename C >
    commands<C>& commands<C>::clear() noexcept {
        commands_.clear();
        return *this;
    }

    template < typename C >
    bool commands<C>::empty() const noexcept {
        return commands_.empty();
    }

    template < typename C >
    std::size_t commands<C>::size() const noexcept {
        return commands_.size();
    }
}
