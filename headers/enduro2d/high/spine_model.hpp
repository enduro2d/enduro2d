/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

struct spAnimationStateData;
struct spSkeletonData;
struct spAtlas;

namespace e2d
{

    class spine_model final {
    public:
        using animation_data_ptr = std::shared_ptr<spAnimationStateData>;
        using skeleton_data_ptr = std::shared_ptr<spSkeletonData>;
        using atlas_ptr = std::shared_ptr<spAtlas>;
    public:
        spine_model() noexcept = default;
        ~spine_model() noexcept = default;

        spine_model(spine_model&& other) noexcept;
        spine_model& operator=(spine_model&& other) noexcept;
        
        spine_model(const spine_model& other);
        spine_model& operator=(const spine_model& other);

        void clear() noexcept;
        void swap(spine_model& other) noexcept;

        spine_model& assign(spine_model&& other) noexcept;
        spine_model& assign(const spine_model& other);

        spine_model& set_skeleton(skeleton_data_ptr data);
        spine_model& set_atlas(atlas_ptr atlas, bool premultiplied_alpha);

        spine_model& mix_animations(const str& from, const str& to, secf duration);

        const atlas_ptr& atlas() const noexcept;
        const animation_data_ptr& animation() const noexcept;
        const skeleton_data_ptr& skeleton() const noexcept;
        bool premultiplied_alpha() const noexcept;
    private:
        animation_data_ptr animation_;
        skeleton_data_ptr skeleton_;
        atlas_ptr atlas_;
        bool premultiplied_alpha_ = false;
    };

    void swap(spine_model& l, spine_model& r) noexcept;
    bool operator==(const spine_model& l, const spine_model& r) noexcept;
    bool operator!=(const spine_model& l, const spine_model& r) noexcept;
}
