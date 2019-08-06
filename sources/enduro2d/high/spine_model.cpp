/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/spine_model.hpp>
#include <spine/AnimationStateData.h>
#include <spine/SkeletonData.h>
#include <spine/Atlas.h>

namespace e2d
{
    spine_model::spine_model(spine_model&& other) noexcept {
        assign(std::move(other));
    }

    spine_model& spine_model::operator=(spine_model&& other) noexcept {
        return assign(std::move(other));
    }
    
    spine_model::spine_model(const spine_model& other) {
        assign(other);
    }

    spine_model& spine_model::operator=(const spine_model& other) {
        return assign(other);
    }

    void spine_model::clear() noexcept {
        animation_.reset();
        skeleton_.reset();
        atlas_.reset();
    }

    void spine_model::swap(spine_model& other) noexcept {
        using std::swap;
        swap(animation_, other.animation_);
        swap(skeleton_, other.skeleton_);
        swap(atlas_, other.atlas_);
        swap(premultiplied_alpha_, other.premultiplied_alpha_);
    }

    spine_model& spine_model::assign(spine_model&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }
    
    spine_model& spine_model::assign(const spine_model& other) {
        if ( this != &other ) {
            spine_model m;
            m.animation_ = other.animation_;
            m.skeleton_ = other.skeleton_;
            m.atlas_ = other.atlas_;
            m.premultiplied_alpha_ = other.premultiplied_alpha_;
            swap(m);
        }
        return *this;
    }

    spine_model& spine_model::set_skeleton(skeleton_data_ptr data) {
        animation_.reset();
        skeleton_ = std::move(data);
        if ( skeleton_ ) {
            animation_ = animation_data_ptr(
                spAnimationStateData_create(skeleton_.get()),
                spAnimationStateData_dispose);
        }
        return *this;
    }

    spine_model& spine_model::set_atlas(atlas_ptr atlas, bool premultiplied_alpha) {
        atlas_ = std::move(atlas);
        premultiplied_alpha_ = premultiplied_alpha;
        return *this;
    }

    spine_model& spine_model::mix_animations(const str& from, const str& to, secf duration) {
        E2D_ASSERT(animation_);
        E2D_ASSERT(spSkeletonData_findAnimation(animation_->skeletonData, from.c_str()));
        E2D_ASSERT(spSkeletonData_findAnimation(animation_->skeletonData, to.c_str()));

        spAnimationStateData_setMixByName(animation_.get(), from.c_str(), to.c_str(), duration.value);
        return *this;
    }
    
    spine_model& spine_model::set_default_mix(secf duration) {
        E2D_ASSERT(animation_);
        animation_->defaultMix = duration.value;
        return *this;
    }

    const spine_model::atlas_ptr& spine_model::atlas() const noexcept {
        return atlas_;
    }

    const spine_model::animation_data_ptr& spine_model::animation() const noexcept {
        return animation_;
    }

    const spine_model::skeleton_data_ptr& spine_model::skeleton() const noexcept {
        return skeleton_;
    }
    
    bool spine_model::premultiplied_alpha() const noexcept {
        return premultiplied_alpha_;
    }
}

namespace e2d
{
    void swap(spine_model& l, spine_model& r) noexcept {
        l.swap(r);
    }
    
    bool operator==(const spine_model& l, const spine_model& r) noexcept {
        return l.atlas() == r.atlas()
            && l.animation() == r.animation()
            && l.skeleton() == r.skeleton()
            && l.premultiplied_alpha() == r.premultiplied_alpha();
    }

    bool operator!=(const spine_model& l, const spine_model& r) noexcept {
        return !(l == r);
    }
}
