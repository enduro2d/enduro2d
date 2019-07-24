/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"
#include "../assets/spine_model_asset.hpp"

struct spAnimationState;

namespace e2d
{
    class spine_player final {
    public:
        using animation_ptr = std::shared_ptr<spAnimationState>;
    public:
        spine_player() = default;
        spine_player(const spine_model_asset::ptr& model);
        
        spine_player& set_animation(int track, const str& name, bool loop);
        spine_player& add_animation(int track, const str& name, bool loop, secf delay);
        
        const animation_ptr& animation() const noexcept;
        const spine_model_asset::ptr& model() const noexcept;
    private:
        animation_ptr animation_;
        spine_model_asset::ptr model_;
    };

    template <>
    class factory_loader<spine_player> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            spine_player& component,
            const fill_context& ctx) const;
            
        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
}
