/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "assets/sprite_asset.hpp"

namespace e2d
{
    class flipbook final {
    public:
        struct frame {
            sprite_asset::ptr sprite;
        };

        struct sequence {
            f32 fps{0.f};
            str_hash name;
            vector<std::size_t> frames;
        };
    public:
        flipbook() = default;
        ~flipbook() noexcept = default;

        flipbook(flipbook&& other) noexcept;
        flipbook& operator=(flipbook&& other) noexcept;

        flipbook(const flipbook& other);
        flipbook& operator=(const flipbook& other);

        void clear() noexcept;
        void swap(flipbook& other) noexcept;

        flipbook& assign(flipbook&& other) noexcept;
        flipbook& assign(const flipbook& other);

        flipbook& set_frames(vector<frame>&& frames) noexcept;
        flipbook& set_frames(const vector<frame>& frames);

        const vector<frame>& frames() const noexcept;
        const frame* find_frame(std::size_t index) const noexcept;

        flipbook& set_sequences(vector<sequence>&& sequences) noexcept;
        flipbook& set_sequences(const vector<sequence>& sequences);

        const vector<sequence>& sequences() const noexcept;
        const sequence* find_sequence(str_hash name) const noexcept;
    private:
        vector<frame> frames_;
        vector<sequence> sequences_;
    };

    void swap(flipbook& l, flipbook& r) noexcept;
    bool operator==(const flipbook& l, const flipbook& r) noexcept;
    bool operator!=(const flipbook& l, const flipbook& r) noexcept;

    void swap(flipbook::frame& l, flipbook::frame& r) noexcept;
    bool operator==(const flipbook::frame& l, const flipbook::frame& r) noexcept;
    bool operator!=(const flipbook::frame& l, const flipbook::frame& r) noexcept;

    void swap(flipbook::sequence& l, flipbook::sequence& r) noexcept;
    bool operator==(const flipbook::sequence& l, const flipbook::sequence& r) noexcept;
    bool operator!=(const flipbook::sequence& l, const flipbook::sequence& r) noexcept;
}
