/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/script_system.hpp>

#include <enduro2d/high/editor.hpp>
#include <enduro2d/high/library.hpp>
#include <enduro2d/high/luasol.hpp>
#include <enduro2d/high/world.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/behaviour.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/events.hpp>
#include <enduro2d/high/components/spine_player.hpp>
#include <enduro2d/high/components/touchable.hpp>

namespace
{
    using namespace e2d;

    void init_core_table(sol::state& s) {
        s["the_dbgui"] = &the<dbgui>();
        s["the_debug"] = &the<debug>();
        s["the_engine"] = &the<engine>();
        s["the_input"] = &the<input>();
        s["the_keyboard"] = &the<input>().keyboard();
        s["the_mouse"] = &the<input>().mouse();
        s["the_window"] = &the<window>();
    }

    void init_high_table(sol::state& s) {
        s["the_editor"] = &the<editor>();
        s["the_library"] = &the<library>();
        s["the_luasol"] = &the<luasol>();
        s["the_world"] = &the<world>();
    }

    void init_base_functions(sol::state& s) {
        s["require"] = [](const char* package){
            the<debug>().error(
                "LUASOL: Require function is not allowed:\n"
                "--> Package: %0",
                package);
        };
    }

    void process_spine_player_events(ecs::registry& owner) {
        ecsex::for_extracted_components<events<spine_player_events::event>, behaviour, actor>(owner,
        [](ecs::entity e, const events<spine_player_events::event>& es, behaviour& b, actor& a){
            if ( !a.node() || !a.node()->owner() ) {
                return;
            }
            for ( const spine_player_events::event& evt : es.get() ) {
                behaviours::call_result r = behaviours::call_result::success;
                std::visit(utils::overloaded {
                    [&b,&a,&r](const spine_player_events::custom_evt& e){
                        r = behaviours::call_meta_method(
                            b, "on_event", a.node()->owner(), "spine_player.custom_evt", e);
                    },
                    [&b,&a,&r](const spine_player_events::end_evt& e){
                        r = behaviours::call_meta_method(
                            b, "on_event", a.node()->owner(), "spine_player.end_evt", e);
                    },
                    [&b,&a,&r](const spine_player_events::complete_evt& e){
                        r = behaviours::call_meta_method(
                            b, "on_event", a.node()->owner(), "spine_player.complete_evt", e);
                    }
                }, evt);
                if ( r == behaviours::call_result::failed ) {
                    e.assign_component<disabled<behaviour>>();
                }
            }
        }, !ecs::exists<disabled<behaviour>>());
    }

    void process_touchable_events(ecs::registry& owner) {
        ecsex::for_extracted_components<events<touchable_events::event>, behaviour, actor>(owner,
        [](ecs::entity e, const events<touchable_events::event>& es, behaviour& b, actor& a){
            if ( !a.node() || !a.node()->owner() ) {
                return;
            }
            for ( const touchable_events::event& evt : es.get() ) {
                behaviours::call_result r = behaviours::call_result::success;
                std::visit(utils::overloaded {
                    [](std::monostate){},
                    [&b,&a,&r](const touchable_events::mouse_move_evt& e){
                        r = behaviours::call_meta_method(
                            b, "on_event", a.node()->owner(), "touchable.mouse_move_evt", e);
                    },
                    [&b,&a,&r](const touchable_events::mouse_hover_evt& e){
                        r = behaviours::call_meta_method(
                            b, "on_event", a.node()->owner(), "touchable.mouse_hover_evt", e);
                    },
                    [&b,&a,&r](const touchable_events::mouse_button_evt& e){
                        r = behaviours::call_meta_method(
                            b, "on_event", a.node()->owner(), "touchable.mouse_button_evt", e);
                    }
                }, evt);
                if ( r == behaviours::call_result::failed ) {
                    e.assign_component<disabled<behaviour>>();
                }
            }
        }, !ecs::exists<disabled<behaviour>>());
    }
}

namespace e2d
{
    //
    // script_system::internal_state
    //

    class script_system::internal_state final : private noncopyable {
    public:
        internal_state() {
            the<luasol>().with_state([](sol::state& s){
                init_core_table(s);
                init_high_table(s);
                init_base_functions(s);
            });
        }
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            ecsex::for_extracted_components<behaviour, actor>(owner,
            [](ecs::entity e, behaviour& b, actor& a){
                if ( !a.node() || !a.node()->owner() ) {
                    return;
                }
                const auto result = behaviours::call_meta_method(b, "on_update", a.node()->owner());
                if ( result == behaviours::call_result::failed ) {
                    e.assign_component<disabled<behaviour>>();
                }
            }, !ecs::exists<disabled<behaviour>>());
        }

        void process_events(ecs::registry& owner) {
            process_spine_player_events(owner);
            process_touchable_events(owner);
        }
    };

    //
    // script_system
    //

    script_system::script_system()
    : state_(new internal_state()) {}
    script_system::~script_system() noexcept = default;

    void script_system::process(
        ecs::registry& owner,
        const systems::update_event& event)
    {
        E2D_UNUSED(event);
        E2D_PROFILER_SCOPE("script_system.process_update");
        state_->process_update(owner);
    }

    void script_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("script_system.process_events");
        state_->process_events(owner);
    }
}
