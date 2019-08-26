/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

struct spAtlas;
struct spSkeletonData;
struct spAnimationStateData;

namespace e2d
{
    class bad_spine_model_access final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad spine model access";
        }
    };

    class spine_model final {
    public:
        using atlas_ptr = std::shared_ptr<spAtlas>;
        using skeleton_data_ptr = std::shared_ptr<spSkeletonData>;
        using animation_data_ptr = std::shared_ptr<spAnimationStateData>;
    public:
        spine_model() = default;
        ~spine_model() noexcept = default;

        spine_model(spine_model&& other) noexcept;
        spine_model& operator=(spine_model&& other) noexcept;
        
        spine_model(const spine_model& other);
        spine_model& operator=(const spine_model& other);

        void clear() noexcept;
        void swap(spine_model& other) noexcept;

        spine_model& assign(spine_model&& other) noexcept;
        spine_model& assign(const spine_model& other);

        spine_model& set_atlas(atlas_ptr atlas);
        spine_model& set_skeleton(skeleton_data_ptr skeleton);

        spine_model& set_default_mix(secf duration);
        spine_model& set_animation_mix(
            const str& from,
            const str& to,
            secf duration);

        const atlas_ptr& atlas() const noexcept;
        const skeleton_data_ptr& skeleton() const noexcept;
        const animation_data_ptr& animation() const noexcept;
    private:
        atlas_ptr atlas_;
        skeleton_data_ptr skeleton_;
        animation_data_ptr animation_;
    };

    void swap(spine_model& l, spine_model& r) noexcept;
    bool operator==(const spine_model& l, const spine_model& r) noexcept;
    bool operator!=(const spine_model& l, const spine_model& r) noexcept;
}
