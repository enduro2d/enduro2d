/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../assets/flipbook_asset.hpp"

namespace e2d
{
    class flipbook_player final {
    public:
        flipbook_player() = default;

        flipbook_player& time(f32 value) noexcept;
        f32 time() const noexcept;

        flipbook_player& speed(f32 value) noexcept;
        f32 speed() const noexcept;

        flipbook_player& paused(bool value) noexcept;
        bool paused() const noexcept;

        flipbook_player& looped(bool value) noexcept;
        bool looped() const noexcept;

        flipbook_player& sequence(str_hash value) noexcept;
        str_hash sequence() const noexcept;
    private:
        f32 time_{0.f};
        f32 speed_{1.f};
        bool paused_{false};
        bool looped_{false};
        str_hash sequence_;
    };
}

namespace e2d
{
    inline flipbook_player& flipbook_player::time(f32 value) noexcept {
        time_ = value;
        return *this;
    }

    inline f32 flipbook_player::time() const noexcept {
        return time_;
    }

    inline flipbook_player& flipbook_player::speed(f32 value) noexcept {
        speed_ = value;
        return *this;
    }

    inline f32 flipbook_player::speed() const noexcept {
        return speed_;
    }

    inline flipbook_player& flipbook_player::paused(bool value) noexcept {
        paused_ = value;
        return *this;
    }

    inline bool flipbook_player::paused() const noexcept {
        return paused_;
    }

    inline flipbook_player& flipbook_player::looped(bool value) noexcept {
        looped_ = value;
        return *this;
    }

    inline bool flipbook_player::looped() const noexcept {
        return looped_;
    }

    inline flipbook_player& flipbook_player::sequence(str_hash value) noexcept {
        sequence_ = value;
        return *this;
    }

    inline str_hash flipbook_player::sequence() const noexcept {
        return sequence_;
    }
}
