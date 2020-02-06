/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

namespace e2d
{
    template < typename E >
    class events final {
    public:
        events() = default;

        events& add(E event);

        events& set(vector<E>&& events) noexcept;
        events& set(const vector<E>& events);

        [[nodiscard]] vector<E>& get() noexcept;
        [[nodiscard]] const vector<E>& get() const noexcept;

        events& clear() noexcept;
        [[nodiscard]] bool empty() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;
    private:
        vector<E> events_;
    };
}

namespace e2d
{
    template < typename E >
    events<E>& events<E>::add(E event) {
        events_.push_back(std::move(event));
        return *this;
    }

    template < typename E >
    events<E>& events<E>::set(vector<E>&& events) noexcept {
        events_ = std::move(events);
        return *this;
    }

    template < typename E >
    events<E>& events<E>::set(const vector<E>& events) {
        events_ = events;
        return *this;
    }

    template < typename E >
    vector<E>& events<E>::get() noexcept {
        return events_;
    }

    template < typename E >
    const vector<E>& events<E>::get() const noexcept {
        return events_;
    }

    template < typename E >
    events<E>& events<E>::clear() noexcept {
        events_.clear();
        return *this;
    }

    template < typename E >
    bool events<E>::empty() const noexcept {
        return events_.empty();
    }

    template < typename E >
    std::size_t events<E>::size() const noexcept {
        return events_.size();
    }
}
