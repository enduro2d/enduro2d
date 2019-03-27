/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_high.hpp"

#include "assets/texture_asset.hpp"
#include "assets/material_asset.hpp"

namespace e2d
{
    class flipbook final {
    public:
        struct frame {
            v2f pivot;
            b2f texrect;
            texture_asset::ptr texture;
        };

        struct sequence {
            f32 fps{0.f};
            str_hash name;
            vector<u32> frames;
        };
    public:
        flipbook();
        ~flipbook() noexcept;

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
        const frame* find_frame(u32 index) const noexcept;

        flipbook& set_sequences(vector<sequence>&& sequences) noexcept;
        flipbook& set_sequences(const vector<sequence>& sequences);
        const vector<sequence>& sequences() const noexcept;
        const sequence* find_sequence(str_hash name) const noexcept;

        flipbook& set_material(const material_asset::ptr& material) noexcept;
        const material_asset::ptr& material() const noexcept;
    private:
        vector<frame> frames_;
        vector<sequence> sequences_;
        material_asset::ptr material_;
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