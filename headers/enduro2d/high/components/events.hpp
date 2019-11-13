/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d
{
    template < typename E >
    class events final {
    public:
        events() = default;

        [[nodiscard]]
        typename vector<E>::const_iterator
        begin() const noexcept;

        [[nodiscard]]
        typename vector<E>::const_iterator
        cbegin() const noexcept;

        [[nodiscard]]
        typename vector<E>::const_iterator
        end() const noexcept;

        [[nodiscard]]
        typename vector<E>::const_iterator
        cend() const noexcept;

        events& add(E event);
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
    typename vector<E>::const_iterator events<E>::begin() const noexcept {
        return events_.begin();
    }

    template < typename E >
    typename vector<E>::const_iterator events<E>::cbegin() const noexcept {
        return events_.cbegin();
    }

    template < typename E >
    typename vector<E>::const_iterator events<E>::end() const noexcept {
        return events_.end();
    }

    template < typename E >
    typename vector<E>::const_iterator events<E>::cend() const noexcept {
        return events_.cend();
    }

    template < typename E >
    events<E>& events<E>::add(E event) {
        events_.push_back(std::move(event));
        return *this;
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
