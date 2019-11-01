/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/script_system.hpp>

#include <enduro2d/high/library.hpp>
#include <enduro2d/high/luasol.hpp>
#include <enduro2d/high/world.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/behaviour.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/spine_player_evt.hpp>

namespace
{
    using namespace e2d;

    void init_core_table(sol::state& s) {
        auto e2d_table = s["e2d"].get_or_create<sol::table>();
        e2d_table["debug"] = &the<debug>();
        e2d_table["engine"] = &the<engine>();
        e2d_table["input"] = &the<input>();
        e2d_table["window"] = &the<window>();
    }

    void init_high_table(sol::state& s) {
        auto e2d_table = s["e2d"].get_or_create<sol::table>();
        e2d_table["library"] = &the<library>();
        e2d_table["luasol"] = &the<luasol>();
        e2d_table["world"] = &the<world>();
    }

    void process_spine_player_events(ecs::registry& owner) {
        systems::for_extracted_components<spine_player_evt, behaviour, actor>(owner,
        [](ecs::entity e, const spine_player_evt& spe, behaviour& b, actor& a){
            if ( !a.node() || !a.node()->owner() ) {
                return;
            }
            for ( const spine_player_evt::event& evt : spe.events() ) {
                behaviours::call_result r = behaviours::call_result::success;
                std::visit(utils::overloaded {
                    [&b,&a,&r](const spine_player_evt::custom_evt& e){
                        r = behaviours::call_meta_method(
                            b, "on_event", a.node()->owner(), "spine_player.custom_evt", e);
                    },
                    [&b,&a,&r](const spine_player_evt::end_evt& e){
                        r = behaviours::call_meta_method(
                            b, "on_event", a.node()->owner(), "spine_player.end_evt", e);
                    },
                    [&b,&a,&r](const spine_player_evt::complete_evt& e){
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
            });
        }
        ~internal_state() noexcept = default;

        void update_process(ecs::registry& owner) {
            systems::for_extracted_components<behaviour, actor>(owner,
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
        state_->update_process(owner);
    }

    void script_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        state_->process_events(owner);
    }
}
