/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    template < typename C >
    class commands final {
    public:
        commands() = default;

        commands& add(C command);

        commands& set(vector<C>&& commands) noexcept;
        commands& set(const vector<C>& commands);

        [[nodiscard]] vector<C>& get() noexcept;
        [[nodiscard]] const vector<C>& get() const noexcept;

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
    commands<C>& commands<C>::add(C command) {
        commands_.push_back(std::move(command));
        return *this;
    }

    template < typename C >
    commands<C>& commands<C>::set(vector<C>&& commands) noexcept {
        commands_ = std::move(commands);
        return *this;
    }

    template < typename C >
    commands<C>& commands<C>::set(const vector<C>& commands) {
        commands_ = commands;
        return *this;
    }

    template < typename C >
    vector<C>& commands<C>::get() noexcept {
        return commands_;
    }

    template < typename C >
    const vector<C>& commands<C>::get() const noexcept {
        return commands_;
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
