/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "touch_system_dispatcher.hpp"

#include "touch_system_collector.hpp"

namespace
{
    using namespace e2d;
    using namespace e2d::touch_system_impl;

    std::pair<v2f, bool> screen_to_world_point(
        const gobject& input_camera,
        const collector::mouse_event& event)
    {
        if ( !input_camera || !input_camera.component<camera>() ) {
            return {v2f::zero(), false};
        }

        const camera& cam = *input_camera.component<camera>();

        const v2f& mouse_p = math::normalized_to_point(
            b2f(event.event_window.framebuffer_size().cast_to<f32>()),
            math::point_to_normalized(
                b2f(event.event_window.real_size().cast_to<f32>()),
                event.screen_point));

        const m4f& cam_vp =
            cam.view() * cam.projection();

        const b2f& cam_viewport = b2f(
            cam.viewport().position * event.event_window.framebuffer_size().cast_to<f32>(),
            cam.viewport().size * event.event_window.framebuffer_size().cast_to<f32>());

        const auto [inv_cam_vp, inv_cam_vp_success] = math::inversed(cam_vp, 0.f);
        if ( !inv_cam_vp_success ) {
            return {v2f::zero(), false};
        }

        const auto& [world_point, world_point_success] = math::unproject(
            v3f(mouse_p, 0.f),
            inv_cam_vp,
            cam_viewport);

        return std::make_pair(v2f(world_point), world_point_success);
    }
}

namespace
{
    using namespace e2d;
    using namespace e2d::touch_system_impl;

    void apply_event(
        gobject target,
        const touchable_events::mouse_drag_evt& event)
    {
        target.component<events<touchable_events::event>>().ensure()
            .add(event);
    }

    void apply_event(
        gobject target,
        const touchable_events::mouse_move_evt& event)
    {
        target.component<events<touchable_events::event>>().ensure()
            .add(event);
    }

    void apply_event(
        gobject target,
        const touchable_events::mouse_hover_evt& event)
    {
        target.component<events<touchable_events::event>>().ensure()
            .add(event);

        switch ( event.type() ) {
        case touchable_events::mouse_hover_evt::types::over:
            target.component<touchable::hover_over>().ensure();
            break;
        case touchable_events::mouse_hover_evt::types::out:
            target.component<touchable::hover_out>().ensure();
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected mouse hover event type");
            break;
        }
    }

    void apply_event(
        gobject target,
        const touchable_events::mouse_scroll_evt& event)
    {
        target.component<events<touchable_events::event>>().ensure()
            .add(event);
    }

    void apply_event(
        gobject target,
        const touchable_events::mouse_button_evt& event)
    {
        target.component<events<touchable_events::event>>().ensure()
            .add(event);

        if ( event.button() != mouse_button::left ) {
            return;
        }

        switch ( event.type() ) {
        case touchable_events::mouse_button_evt::types::clicked:
            target.component<touchable::clicked>().ensure();
            break;
        case touchable_events::mouse_button_evt::types::pressed:
            target.component<touchable::pressed>().ensure();
            break;
        case touchable_events::mouse_button_evt::types::released:
            target.component<touchable::released>().ensure();
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected mouse button event type");
            break;
        }
    }
}

namespace
{
    using namespace e2d;
    using namespace e2d::touch_system_impl;

    gobject find_current_target(const ecs::registry& owner) {
        static thread_local vector<std::tuple<
            ecs::const_entity,
            scene,
            actor>> scenes;

        DEFER([](){ scenes.clear(); });
        ecsex::extract_components<scene, actor>(
            owner,
            std::back_inserter(scenes),
            !ecs::exists_any<
                disabled<scene>,
                disabled<actor>>());

        std::sort(scenes.begin(), scenes.end(), [](const auto& l, const auto& r){
            return std::get<scene>(l).depth() >= std::get<scene>(r).depth();
        });

        for ( const auto& t : scenes ) {
            gobject target = nodes::find_component_from_children<touchable_under_mouse>(
                std::get<actor>(t).node(),
                nodes::options()
                    .reversed(true)
                    .recursive(true)
                    .include_root(true)).owner();
            if ( target ) {
                return target;
            }
        }

        return gobject();
    }

    bool capture_target(gobject target) {
        if ( !target ) {
            return false;
        }

        const_gcomponent<actor> target_actor{target};
        const_gcomponent<touchable> target_touchable{target};

        if ( !target_actor || !target_touchable ) {
            return false;
        }

        static thread_local std::vector<
            const_gcomponent<touchable>> parents;

        const std::size_t begin_index = parents.size();
        DEFER([begin_index](){
            parents.erase(
                parents.begin() + begin_index,
                parents.end());
        });

        nodes::extract_components_from_parents<touchable>(
            target_actor->node(),
            std::back_inserter(parents),
            nodes::options()
                .reversed(true)
                .recursive(true));

        const std::size_t end_index = parents.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            const const_gcomponent<touchable>& parent = parents[i];

            const bool parent_disabled = ecs::exists_any<
                disabled<actor>,
                disabled<touchable>
            >()(parent.owner().raw_entity());

            if ( !parent_disabled && !parent->capturing() ) {
                return false;
            }
        }

        return true;
    }

    template < typename E >
    void bubble_event_from_target(gobject target, const E& event) {
        if ( !target ) {
            return;
        }

        const_gcomponent<actor> target_actor{target};
        const_gcomponent<touchable> target_touchable{target};

        if ( !target_actor || !target_touchable ) {
            return;
        }

        if ( !target_touchable->bubbling() ) {
            return;
        }

        static thread_local std::vector<
            const_gcomponent<touchable>> parents;

        const std::size_t begin_index = parents.size();
        DEFER([begin_index](){
            parents.erase(
                parents.begin() + begin_index,
                parents.end());
        });

        nodes::extract_components_from_parents<touchable>(
            target_actor->node(),
            std::back_inserter(parents),
            nodes::options()
                .recursive(true));

        const std::size_t end_index = parents.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            const const_gcomponent<touchable>& parent = parents[i];

            const bool parent_disabled = ecs::exists_any<
                disabled<actor>,
                disabled<touchable>
            >()(parent.owner().raw_entity());

            if ( !parent_disabled ) {
                apply_event(parent.owner(), event);
                if ( !parent->bubbling() ) {
                    return;
                }
            }
        }
    }

    template < typename E >
    bool dispatch_event(gobject target, const E& event) {
        if ( !capture_target(target) ) {
            return false;
        }

        apply_event(target, event);

        if ( event.bubbling() ) {
            bubble_event_from_target(target, event);
        }

        return true;
    }
}

namespace
{
    using namespace e2d;
    using namespace e2d::touch_system_impl;

    hovering_info_opt process_hover_target(
        ecs::registry& owner,
        gobject current_target,
        hovering_info_opt hovering,
        dragging_info_opt dragging)
    {
        E2D_UNUSED(owner, dragging);
        using namespace touchable_events;

        if ( gobject last_target = hovering ? hovering->target : gobject();
            last_target != current_target )
        {
            if ( last_target ) {
                dispatch_event(last_target, mouse_hover_evt(
                    last_target,
                    mouse_hover_evt::types::out));

                last_target.component<touchable::hovering>().remove();
            }

            if ( current_target ) {
                const bool captured = dispatch_event(current_target, mouse_hover_evt(
                    current_target,
                    mouse_hover_evt::types::over));

                if ( captured ) {
                    current_target.component<touchable::hovering>().ensure();
                }
            }
        }

        const_gcomponent<touchable_under_mouse> current_target_under_mouse{current_target};
        return current_target_under_mouse && current_target_under_mouse->input_camera
            ? std::make_optional<hovering_info>(
                current_target,
                current_target_under_mouse->input_camera)
            : std::nullopt;
    }
}

namespace
{
    using namespace e2d;
    using namespace e2d::touch_system_impl;

    void process_hovering_target_event(
        ecs::registry& owner,
        const hovering_info_opt& hovering,
        const dragging_info_opt& dragging,
        const collector::mouse_move_event& event)
    {
        E2D_UNUSED(owner, dragging, event);
        using namespace touchable_events;

        const_gcomponent<touchable_under_mouse> under_mouse = hovering
            ? hovering->target
            : const_gcomponent<touchable_under_mouse>();

        if ( !hovering || !under_mouse ) {
            return;
        }

        dispatch_event(
            hovering->target,
            mouse_move_evt(
                hovering->target,
                under_mouse->local_point,
                under_mouse->world_point));
    }

    void process_hovering_target_event(
        ecs::registry& owner,
        const hovering_info_opt& hovering,
        const dragging_info_opt& dragging,
        const collector::mouse_scroll_event& event)
    {
        E2D_UNUSED(owner, dragging);
        using namespace touchable_events;

        const_gcomponent<touchable_under_mouse> under_mouse = hovering
            ? hovering->target
            : const_gcomponent<touchable_under_mouse>();

        if ( !hovering || !under_mouse ) {
            return;
        }

        dispatch_event(
            hovering->target,
            mouse_scroll_evt(
                hovering->target,
                event.delta,
                under_mouse->local_point,
                under_mouse->world_point));
    }

    void process_hovering_target_event(
        ecs::registry& owner,
        const hovering_info_opt& hovering,
        const dragging_info_opt& dragging,
        const collector::mouse_button_event& event)
    {
        E2D_UNUSED(owner, dragging);
        using namespace touchable_events;

        gobject current_target = hovering ? hovering->target : gobject();
        const_gcomponent<touchable_under_mouse> under_mouse{current_target};

        const auto make_mouse_button_evt = [
            &event,
            &under_mouse,
            &current_target
        ](mouse_button_evt::types type){
            return mouse_button_evt(
                current_target,
                type,
                event.button,
                under_mouse->local_point,
                under_mouse->world_point);
        };

        if ( event.action == mouse_button_action::press ) {
            if ( under_mouse ) {
                const bool captured = dispatch_event(
                    current_target,
                    make_mouse_button_evt(mouse_button_evt::types::pressed));

                if ( captured && event.button == mouse_button::left ) {
                    current_target.component<touchable::pushing>().ensure();
                }
            }
        }

        if ( event.action == mouse_button_action::release ) {
            DEFER([&owner](){
                owner.remove_all_components<touchable::pushing>();
            });

            if ( under_mouse ) {
                const bool captured = dispatch_event(
                    current_target,
                    make_mouse_button_evt(mouse_button_evt::types::released));

                if ( captured && event.button == mouse_button::left ) {
                    if ( current_target.component<touchable::pushing>() ) {
                        dispatch_event(
                            current_target,
                            make_mouse_button_evt(mouse_button_evt::types::clicked));
                    }
                }
            }
        }
    }
}

namespace
{
    using namespace e2d;
    using namespace e2d::touch_system_impl;

    dragging_info_opt process_dragging_target_event(
        ecs::registry& owner,
        hovering_info_opt hovering,
        dragging_info_opt dragging,
        const collector::mouse_move_event& event)
    {
        E2D_UNUSED(owner, hovering);
        using namespace touchable_events;

        if ( !dragging || !dragging->target || !dragging->camera ) {
            return std::nullopt;
        }

        const auto& [world_point, world_point_success] = screen_to_world_point(
            dragging->camera,
            event);

        if ( !world_point_success ) {
            return std::nullopt;
        }

        const_gcomponent<actor> dragging_target_a{dragging->target};
        if ( !dragging_target_a || !dragging_target_a->node() ) {
            return std::nullopt;
        }

        v2f local_point = v2f(dragging_target_a->node()->world_to_local(
            v4f(world_point, 0.f, 1.f)));

        dispatch_event(dragging->target, mouse_drag_evt(
            dragging->target,
            mouse_drag_evt::types::move,
            local_point,
            world_point));

        return dragging;
    }

    dragging_info_opt process_dragging_target_event(
        ecs::registry& owner,
        hovering_info_opt hovering,
        dragging_info_opt dragging,
        const collector::mouse_scroll_event& event)
    {
        E2D_UNUSED(owner, hovering, dragging, event);
        return dragging;
    }

    dragging_info_opt process_dragging_target_event(
        ecs::registry& owner,
        hovering_info_opt hovering,
        dragging_info_opt dragging,
        const collector::mouse_button_event& event)
    {
        E2D_UNUSED(hovering);
        using namespace touchable_events;

        if ( event.action == mouse_button_action::press ) {
            gobject current_target = hovering ? hovering->target : gobject();
            const_gcomponent<touchable_under_mouse> under_mouse{current_target};

            if ( under_mouse ) {
                const bool captured = dispatch_event(current_target, mouse_drag_evt(
                    current_target,
                    mouse_drag_evt::types::start,
                    under_mouse->local_point,
                    under_mouse->world_point));

                if ( captured && event.button == mouse_button::left ) {
                    current_target.component<touchable::dragging>().ensure();
                    dragging = std::make_optional<dragging_info>(
                        current_target,
                        under_mouse->input_camera);
                }
            }
        }

        if ( event.action == mouse_button_action::release ) {
            DEFER([&owner, &dragging](){
                dragging.reset();
                owner.remove_all_components<touchable::dragging>();
            });

            if ( dragging && dragging->target && dragging->camera ) {
                const auto& [world_point, world_point_success] = screen_to_world_point(
                    dragging->camera,
                    event);

                if ( world_point_success ) {
                    const_gcomponent<actor> dragging_target_a{dragging->target};
                    if ( dragging_target_a && dragging_target_a->node() ) {
                        v2f local_point = v2f(dragging_target_a->node()->world_to_local(
                            v4f(world_point, 0.f, 1.f)));
                        dispatch_event(dragging->target, mouse_drag_evt(
                            dragging->target,
                            mouse_drag_evt::types::end,
                            local_point,
                            world_point));
                    }
                }
            }
        }

        return dragging;
    }
}

namespace e2d::touch_system_impl
{
    void dispatcher::dispatch_all_events(
        collector& collector,
        ecs::registry& owner)
    {
        DEFER([&collector](){
            collector.clear();
        });

        owner.remove_all_components<touchable::clicked>();
        owner.remove_all_components<touchable::pressed>();
        owner.remove_all_components<touchable::released>();

        owner.remove_all_components<touchable::hover_over>();
        owner.remove_all_components<touchable::hover_out>();

        owner.for_each_component<events<touchable_events::event>>([
        ](const ecs::const_entity&, events<touchable_events::event>& es) {
            es.clear();
        });

        gobject target = find_current_target(owner);

        hovering_ = process_hover_target(
            owner,
            target,
            hovering_,
            dragging_);

        for ( const auto& event : collector ) {
            std::visit(utils::overloaded {
                [](std::monostate){},
                [this, &owner](const auto& event){
                    process_hovering_target_event(
                        owner,
                        hovering_,
                        dragging_,
                        event);
                    dragging_ = process_dragging_target_event(
                        owner,
                        hovering_,
                        dragging_,
                        event);
                }
            }, event);
        }
    }
}
