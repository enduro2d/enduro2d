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

    template < typename E >
    void dispatch_event(E&& event) {
        gobject target = event.target();
        if ( !target ) {
            return;
        }

        const_gcomponent<actor> target_actor = target.component<actor>();
        const_gcomponent<touchable> target_touchable = target.component<touchable>();
        if ( !target_actor || !target_touchable ) {
            return;
        }

        //
        // parents
        //

        static thread_local std::vector<gcomponent<touchable>> parents;
        DEFER([](){ parents.clear(); });

        nodes::extract_components_from_parents<touchable>(
            target_actor->node(),
            std::back_inserter(parents),
            nodes::options().recursive(true));

        //
        // capturing
        //

        if ( event.capturing() ) {
            for ( auto iter = parents.rbegin(); iter != parents.rend(); ++iter ) {
                const bool parent_disabled = ecs::exists_any<
                    disabled<actor>,
                    disabled<touchable>
                >()(iter->owner().raw_entity());
                if ( !parent_disabled && !(*iter)->capturing() ) {
                    return;
                }
            }
        }

        //
        // targeting
        //

        target.component<events<touchable_events::event>>().ensure().add(event);

        //
        // bubbling
        //

        if ( event.bubbling() && target_touchable->bubbling() ) {
            for ( auto iter = parents.begin(); iter != parents.end(); ++iter ) {
                const bool parent_disabled = ecs::exists_any<
                    disabled<actor>,
                    disabled<touchable>
                >()(iter->owner().raw_entity());
                if ( !parent_disabled ) {
                    iter->component<events<touchable_events::event>>().ensure().add(event);
                    if ( !(*iter)->bubbling() ) {
                        return;
                    }
                }
            }
        }
    }
}

namespace e2d::touch_system_impl
{
    void dispatcher::dispatch_all_events(ecs::registry& owner) {
        DEFER([this](){ events_.clear(); });

        owner.for_each_component<events<touchable_events::event>>([
        ](const ecs::const_entity&, events<touchable_events::event>& es) {
            es.clear();
        });

        if ( events_.empty() ) {
            return;
        }

        gobject target = find_event_target(owner);
        if ( !target ) {
            return;
        }

        for ( const auto& event : events_ ) {
            std::visit(utils::overloaded {
                [](std::monostate){},
                [&target](auto event_copy){
                    dispatch_event(event_copy.target(target));
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
                gobject(),
                touchable_events::mouse_evt::types::pressed,
                button));
            break;
        case mouse_button_action::release:
            events_.push_back(touchable_events::mouse_evt(
                gobject(),
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
