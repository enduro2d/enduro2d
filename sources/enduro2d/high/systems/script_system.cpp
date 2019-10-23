/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/script_system.hpp>

#include <enduro2d/high/library.hpp>
#include <enduro2d/high/luasol.hpp>
#include <enduro2d/high/world.hpp>

namespace
{
    using namespace e2d;

    void init_core_table(sol::state& s) {
        auto e2d_table = s["e2d"].get_or_create<sol::table>();
        e2d_table["core"] = s.create_table_with(
            "debug", &the<debug>(),
            "engine", &the<engine>(),
            "input", &the<input>(),
            "window", &the<window>());
    }

    void init_high_table(sol::state& s) {
        auto e2d_table = s["e2d"].get_or_create<sol::table>();
        e2d_table["high"] = s.create_table_with(
            "library", &the<library>(),
            "luasol", &the<luasol>(),
            "world", &the<world>());
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
            E2D_UNUSED(owner);
        }

        void finalize_process(ecs::registry& owner) {
            E2D_UNUSED(owner);
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
        const ecs::after<systems::frame_finalize_event>& event)
    {
        E2D_UNUSED(event);
        state_->finalize_process(owner);
    }
}
