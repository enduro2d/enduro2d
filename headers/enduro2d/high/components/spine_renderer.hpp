/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"
#include "../assets/spine_model_asset.hpp"

struct spSkeleton;
struct spSkeletonClipping;
struct spVertexEffect;

namespace e2d
{

    class spine_renderer final {
    public:
        using skeleton_ptr = std::shared_ptr<spSkeleton>;
        using clipping_ptr = std::shared_ptr<spSkeletonClipping>;
        using effect_ptr = std::shared_ptr<spVertexEffect>;
    public:
        spine_renderer() = default;
        spine_renderer(const spine_model_asset::ptr& model);

        spine_renderer& skin(const str& value);
        spine_renderer& attachment(const str& slot, const str& name);

        const skeleton_ptr& skeleton() const noexcept;
        const clipping_ptr& clipper() const noexcept;
        const effect_ptr& effect() const noexcept;
        const spine_model_asset::ptr& model() const noexcept;
    private:
        skeleton_ptr skeleton_;
        clipping_ptr clipping_;
        effect_ptr effect_;
        spine_model_asset::ptr model_;
    };

    template <>
    class factory_loader<spine_renderer> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            spine_renderer& component,
            const fill_context& ctx) const;
            
        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}
