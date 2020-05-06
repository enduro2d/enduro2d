/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "touch_system_dispatcher.hpp"

namespace
{
    using namespace e2d;
    using namespace e2d::touch_system_impl;

    gobject find_event_target(const ecs::registry& owner) {
        static thread_local vector<std::tuple<
            ecs::const_entity,
            scene,
            actor>> scenes;

        E2D_DEFER([](){ scenes.clear(); });
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

    template < typename E >
    bool dispatch_event(gobject target, const E& event);

    void apply_event(gobject target, const touchable_events::mouse_evt& event) {
        target.component<events<touchable_events::event>>().ensure()
            .add(event);

        if ( event.button() != mouse_button::left ) {
            return;
        }

        switch ( event.type() ) {
        case touchable_events::mouse_evt::types::clicked:
            target.component<touchable::clicked>().ensure();
            break;
        case touchable_events::mouse_evt::types::pressed:
            target.component<touchable::pressed>().ensure();
            break;
        case touchable_events::mouse_evt::types::released:
            target.component<touchable::released>().ensure();
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected mouse event type");
            break;
        }
    }

    void apply_event(gobject target, const touchable_events::touch_evt& event) {
        target.component<events<touchable_events::event>>().ensure()
            .add(event);

        if ( event.finger() != 0u ) {
            return;
        }

        switch ( event.type() ) {
        case touchable_events::touch_evt::types::pressed:
            target.component<touchable::pressed>().ensure();
            break;
        case touchable_events::touch_evt::types::released:
            target.component<touchable::released>().ensure();
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected touch event type");
            break;
        }
    }

    void apply_event(gobject target, const touchable_events::hover_evt& event) {
        target.component<events<touchable_events::event>>().ensure()
            .add(event);

        switch ( event.type() ) {
        case touchable_events::hover_evt::types::over:
            target.component<touchable::hover_over>().ensure();
            break;
        case touchable_events::hover_evt::types::out:
            target.component<touchable::hover_out>().ensure();
            break;
        case touchable_events::hover_evt::types::enter:
            target.component<touchable::hover_enter>().ensure();
            break;
        case touchable_events::hover_evt::types::leave:
            target.component<touchable::hover_leave>().ensure();
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected hover event type");
            break;
        }
    }

    bool capture_target(gobject target) {
        if ( !target ) {
            return false;
        }

        const_gcomponent<actor> target_actor = target.component<actor>();
        const_gcomponent<touchable> target_touchable = target.component<touchable>();
        if ( !target_actor || !target_touchable ) {
            return false;
        }

        static thread_local std::vector<const_gcomponent<touchable>> parents;

        const std::size_t begin_index = parents.size();
        E2D_DEFER([begin_index](){
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

        const_gcomponent<actor> target_actor = target.component<actor>();
        const_gcomponent<touchable> target_touchable = target.component<touchable>();
        if ( !target_actor || !target_touchable ) {
            return;
        }

        if ( !target_touchable->bubbling() ) {
            return;
        }

        static thread_local std::vector<const_gcomponent<touchable>> parents;

        const std::size_t begin_index = parents.size();
        E2D_DEFER([begin_index](){
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

namespace e2d::touch_system_impl
{
    void dispatcher::dispatch_all_events(ecs::registry& owner) {
        class touchable_last_hover final {};
        class touchable_next_hover final {};

        E2D_DEFER([this, &owner](){
            events_.clear();
            owner.remove_all_components<touchable_next_hover>();
        });

        owner.remove_all_components<touchable::clicked>();
        owner.remove_all_components<touchable::pressed>();
        owner.remove_all_components<touchable::released>();

        owner.remove_all_components<touchable::hover_over>();
        owner.remove_all_components<touchable::hover_out>();
        owner.remove_all_components<touchable::hover_enter>();
        owner.remove_all_components<touchable::hover_leave>();

        owner.for_each_component<events<touchable_events::event>>([
        ](const ecs::const_entity&, events<touchable_events::event>& es) {
            es.clear();
        });

        //
        //
        //

        gobject target = find_event_target(owner);

        if ( target ) {
            if ( auto target_n = target.component<actor>()->node() ) {
                nodes::for_extracted_components_from_parents<touchable>(target_n, [
                ](const const_gcomponent<touchable>& parent){
                    const bool parent_disabled = ecs::exists_any<
                        disabled<actor>,
                        disabled<touchable>
                    >()(parent.owner().raw_entity());

                    if ( !parent_disabled ) {
                        parent.owner().component<touchable_next_hover>().ensure();
                    }
                }, nodes::options()
                    .recursive(true)
                    .include_root(true));
            }
        }

        owner.for_joined_components<touchable, actor>([
        ](ecs::entity e, const touchable&, const actor& a){
            const auto need_hover_leave =
                ecs::exists<touchable_last_hover>() &&
                !ecs::exists<touchable_next_hover>();

            if ( need_hover_leave(e) && a.node() ) {
                const bool captured = dispatch_event(
                    a.node()->owner(),
                    touchable_events::hover_evt(
                        a.node()->owner(),
                        touchable_events::hover_evt::types::leave));
                if ( captured ) {
                    e.remove_component<touchable_last_hover>();
                }
            }

            const auto need_hover_enter =
                !ecs::exists<touchable_last_hover>() &&
                ecs::exists<touchable_next_hover>();

            if ( need_hover_enter(e) && a.node() ) {
                const bool captured = dispatch_event(
                    a.node()->owner(),
                    touchable_events::hover_evt(
                        a.node()->owner(),
                        touchable_events::hover_evt::types::enter));
                if ( captured ) {
                    e.ensure_component<touchable_last_hover>();
                }
            }
        });

        //
        //
        //

        if ( target != last_hover_target_ ) {
            if ( last_hover_target_ ) {
                dispatch_event(last_hover_target_, touchable_events::hover_evt(
                    last_hover_target_,
                    touchable_events::hover_evt::types::out));
                last_hover_target_.component<touchable::pushing>().remove();
                last_hover_target_.component<touchable::hovering>().remove();
            }

            if ( target ) {
                const bool captured = dispatch_event(target, touchable_events::hover_evt(
                    target,
                    touchable_events::hover_evt::types::over));
                if ( captured ) {
                    target.component<touchable::hovering>().ensure();
                }
            }

            last_hover_target_ = target;
        }

        //
        //
        //

        for ( const auto& event : events_ ) {
            std::visit(utils::overloaded {
                [](std::monostate){},
                [&target](touchable_events::mouse_evt event){
                    const bool captured = dispatch_event(target, event.target(target));

                    if ( !captured || event.button() != mouse_button::left ) {
                        return;
                    }

                    switch ( event.type() ) {
                    case touchable_events::mouse_evt::types::clicked:
                        break;
                    case touchable_events::mouse_evt::types::pressed:
                        target.component<touchable::pushing>().ensure();
                        break;
                    case touchable_events::mouse_evt::types::released:
                        if ( target.component<touchable::pushing>() ) {
                            target.component<touchable::pushing>().remove();
                            dispatch_event(
                                target,
                                touchable_events::mouse_evt(
                                    target,
                                    touchable_events::mouse_evt::types::clicked,
                                    event.button()));
                        }
                        break;
                    default:
                        E2D_ASSERT_MSG(false, "unexpected mouse event type");
                        break;
                    }
                },
                [&target](touchable_events::touch_evt event){
                    const bool captured = dispatch_event(target, event.target(target));

                    if ( !captured || event.finger() != 0u ) {
                        return;
                    }

                    switch ( event.type() ) {
                    case touchable_events::touch_evt::types::clicked:
                        break;
                    case touchable_events::touch_evt::types::pressed:
                        target.component<touchable::pushing>().ensure();
                        break;
                    case touchable_events::touch_evt::types::released:
                        if ( target.component<touchable::pushing>() ) {
                            target.component<touchable::pushing>().remove();
                            dispatch_event(
                                target,
                                touchable_events::touch_evt(
                                    target,
                                    touchable_events::touch_evt::types::clicked,
                                    event.finger()));
                        }
                        break;
                    default:
                        E2D_ASSERT_MSG(false, "unexpected touch event type");
                        break;
                    }
                },
                [&target](auto event){
                    dispatch_event(target, event.target(target));
                }
            }, event);
        }
    }

    void dispatcher::on_mouse_button(
        mouse_button button,
        mouse_button_action action) noexcept
    {
        switch ( action ) {
        case mouse_button_action::press:
            events_.push_back(touchable_events::mouse_evt(
                touchable_events::mouse_evt::types::pressed,
                button));
            break;
        case mouse_button_action::release:
            events_.push_back(touchable_events::mouse_evt(
                touchable_events::mouse_evt::types::released,
                button));
            break;
        case mouse_button_action::unknown:
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected mouse action");
            break;
        }
    }
}
