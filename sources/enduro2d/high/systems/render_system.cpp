/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/render_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/camera.hpp>
#include <enduro2d/high/components/scene.hpp>

#include "render_system_impl/render_system_base.hpp"
#include "render_system_impl/render_system_batcher.hpp"
#include "render_system_impl/render_system_drawer.hpp"

namespace
{
    using namespace e2d;
    using namespace e2d::render_system_impl;

    void for_all_scenes(drawer::context& ctx, const ecs::registry& owner) {
        const auto comp = [](const auto& l, const auto& r) noexcept {
            return std::get<1>(l).get().depth() < std::get<1>(r).get().depth();
        };
        const auto func = [&ctx](
            const ecs::const_entity&,
            const scene&,
            const actor& scn_a)
        {
            nodes::for_extracted_nodes(scn_a.node(), [&ctx](const const_node_iptr& node){
                ctx.draw(node);
            });
        };
        systems::for_extracted_components<scene, actor>(owner, comp, func);
    }

    void for_all_cameras(drawer& drawer, const ecs::registry& owner) {
        const auto comp = [](const auto& l, const auto& r) noexcept {
            return std::get<1>(l).get().depth() < std::get<1>(r).get().depth();
        };
        const auto func = [&drawer, &owner](
            const ecs::const_entity&,
            const camera& cam,
            const actor& cam_a)
        {
            drawer.with(cam, cam_a.node(), [&owner](drawer::context& ctx){
                for_all_scenes(ctx, owner);
            });
        };
        systems::for_extracted_components<camera, actor>(owner, comp, func);
    }
}

namespace e2d
{
    //
    // render_system::internal_state
    //

    class render_system::internal_state final : private noncopyable {
    public:
        internal_state()
        : drawer_(the<engine>(), the<debug>(), the<render>(), the<window>()) {}
        ~internal_state() noexcept = default;

        void process(ecs::registry& owner) {
            for_all_cameras(drawer_, owner);
        }
    private:
        drawer drawer_;
    };

    //
    // render_system
    //

    render_system::render_system()
    : state_(new internal_state()) {}
    render_system::~render_system() noexcept = default;

    void render_system::process(
        ecs::registry& owner,
        const ecs::after<systems::render_event>& trigger)
    {
        E2D_UNUSED(trigger);
        state_->process(owner);
    }
}
