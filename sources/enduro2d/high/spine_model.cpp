/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/spine_model.hpp>

#include <spine/spine.h>

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
        atlas_.reset();
        skeleton_.reset();
        animation_.reset();
    }

    void spine_model::swap(spine_model& other) noexcept {
        using std::swap;
        swap(atlas_, other.atlas_);
        swap(premultiplied_alpha_, other.premultiplied_alpha_);
        swap(skeleton_, other.skeleton_);
        swap(animation_, other.animation_);
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
            m.atlas_ = other.atlas_;
            m.premultiplied_alpha_ = other.premultiplied_alpha_;
            m.skeleton_ = other.skeleton_;
            m.animation_ = other.animation_;
            swap(m);
        }
        return *this;
    }

    spine_model& spine_model::set_atlas(atlas_ptr atlas, bool premultiplied_alpha) {
        atlas_ = std::move(atlas);
        premultiplied_alpha_ = premultiplied_alpha;
        return *this;
    }

    spine_model& spine_model::set_skeleton(skeleton_data_ptr skeleton) {
        animation_data_ptr animation;
        if ( skeleton ) {
            animation.reset(
                spAnimationStateData_create(skeleton.get()),
                spAnimationStateData_dispose);
            if ( !animation ) {
                throw std::bad_alloc();
            }
        }
        skeleton_ = std::move(skeleton);
        animation_ = std::move(animation);
        return *this;
    }

    spine_model& spine_model::set_default_mix(secf duration) {
        if ( !animation_ ) {
            throw bad_spine_model_access();
        }
        animation_->defaultMix = duration.value;
        return *this;
    }

    spine_model& spine_model::set_animation_mix(
        const str& from,
        const str& to,
        secf duration)
    {
        spAnimation* from_anim = animation_
            ? spSkeletonData_findAnimation(animation_->skeletonData, from.c_str())
            : nullptr;

        spAnimation* to_anim = animation_
            ? spSkeletonData_findAnimation(animation_->skeletonData, to.c_str())
            : nullptr;

        if ( !from_anim || !to_anim ) {
            throw bad_spine_model_access();
        }

        spAnimationStateData_setMix(animation_.get(), from_anim, to_anim, duration.value);
        return *this;
    }

    const spine_model::atlas_ptr& spine_model::atlas() const noexcept {
        return atlas_;
    }

    const spine_model::skeleton_data_ptr& spine_model::skeleton() const noexcept {
        return skeleton_;
    }

    const spine_model::animation_data_ptr& spine_model::animation() const noexcept {
        return animation_;
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
            && l.skeleton() == r.skeleton()
            && l.animation() == r.animation();
    }

    bool operator!=(const spine_model& l, const spine_model& r) noexcept {
        return !(l == r);
    }
}
