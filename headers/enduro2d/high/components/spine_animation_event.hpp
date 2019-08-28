/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"

namespace e2d
{
    class spine_animation_event final {
    public:
        struct clear_track {
            u32 track;
        };

        struct set_anim {
            u32 track;
            str name;
            bool loop;
            str on_complete;
        };

        struct add_anim {
            u32 track;
            str name;
            bool loop;
            secf delay;
            str on_complete;
        };

        struct empty_anim {
            u32 track;
            secf duration;
            secf delay;
            str on_complete;
        };

        using commands_t = stdex::variant<
            clear_track,
            set_anim,
            add_anim,
            empty_anim>;
    public:
        spine_animation_event() = default;

        std::vector<commands_t>& commands() noexcept { return commands_; }
        const std::vector<commands_t>& commands() const noexcept { return commands_; }
    private:
        std::vector<commands_t> commands_;
    };
    
    template <>
    class factory_loader<spine_animation_event> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            spine_animation_event& component,
            const fill_context& ctx) const;
            
        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}
