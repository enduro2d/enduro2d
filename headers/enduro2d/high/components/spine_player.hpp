/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../assets/spine_asset.hpp"
#include "../assets/material_asset.hpp"

struct spSkeleton;
struct spSkeletonClipping;
struct spAnimationState;

namespace e2d::spine_player_events
{
    class custom_evt;
    class end_evt;
    class complete_evt;

    using event = std::variant<
        custom_evt,
        end_evt,
        complete_evt>;
}

namespace e2d::spine_player_commands
{
    class clear_track_cmd;
    class set_anim_cmd;
    class add_anim_cmd;
    class set_empty_anim_cmd;
    class add_empty_anim_cmd;

    using command = std::variant<
        clear_track_cmd,
        set_anim_cmd,
        add_anim_cmd,
        set_empty_anim_cmd,
        add_empty_anim_cmd>;
}

namespace e2d
{
    class bad_spine_player_access final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad spine player access";
        }
    };

    class spine_player final {
    public:
        using clipping_ptr = std::shared_ptr<spSkeletonClipping>;
        using skeleton_ptr = std::shared_ptr<spSkeleton>;
        using animation_ptr = std::shared_ptr<spAnimationState>;
    public:
        spine_player() = default;
        spine_player(const spine_asset::ptr& spine);

        spine_player& spine(
            const spine_asset::ptr& value);

        bool skin(str_view name);
        bool attachment(str_view slot, str_view name);

        bool has_skin(str_view name) const noexcept;
        bool has_animation(str_view name) const noexcept;

        const spine_asset::ptr& spine() const noexcept;
        const clipping_ptr& clipper() const noexcept;
        const skeleton_ptr& skeleton() const noexcept;
        const animation_ptr& animation() const noexcept;

        spine_player& materials(flat_map<str_hash, material_asset::ptr> value) noexcept;
        const flat_map<str_hash, material_asset::ptr>& materials() const noexcept;
        material_asset::ptr find_material(str_hash name) const noexcept;
    private:
        spine_asset::ptr spine_;
        clipping_ptr clipping_;
        skeleton_ptr skeleton_;
        animation_ptr animation_;
        flat_map<str_hash, material_asset::ptr> materials_;
    };
}

namespace e2d
{
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

    template <>
    class factory_loader<events<spine_player_events::event>> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            events<spine_player_events::event>& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };

    template <>
    class factory_loader<commands<spine_player_commands::command>> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            commands<spine_player_commands::command>& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<spine_player> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<spine_player>& c) const;
    };
}

namespace e2d::spine_player_events
{
    class custom_evt final {
    public:
        custom_evt() = default;

        custom_evt(str name)
        : name_(std::move(name)) {}

        custom_evt& name(str value) noexcept {
            name_ = std::move(value);
            return *this;
        }

        custom_evt& int_value(i32 value) noexcept {
            int_value_ = value;
            return *this;
        }

        custom_evt& float_value(f32 value) noexcept {
            float_value_ = value;
            return *this;
        }

        custom_evt& string_value(str value) noexcept {
            string_value_ = std::move(value);
            return *this;
        }

        [[nodiscard]] const str& name() const noexcept { return name_; }
        [[nodiscard]] i32 int_value() const noexcept { return int_value_; }
        [[nodiscard]] f32 float_value() const noexcept { return float_value_; }
        [[nodiscard]] const str& string_value() const noexcept { return string_value_; }
    private:
        str name_;
        i32 int_value_{0};
        f32 float_value_{0.f};
        str string_value_;
    };

    class end_evt final {
    public:
        end_evt() = default;

        end_evt(str message)
        : message_(std::move(message)) {}

        end_evt& message(str value) noexcept {
            message_ = std::move(value);
            return *this;
        }

        [[nodiscard]] const str& message() const noexcept { return message_; }
    private:
        str message_;
    };

    class complete_evt final {
    public:
        complete_evt() = default;

        complete_evt(str message)
        : message_(std::move(message)) {}

        complete_evt& message(str value) noexcept {
            message_ = std::move(value);
            return *this;
        }

        [[nodiscard]] const str& message() const noexcept { return message_; }
    private:
        str message_;
    };
}

namespace e2d::spine_player_commands
{
    class clear_track_cmd final {
    public:
        clear_track_cmd(u32 track)
        : track_(track) {}

        [[nodiscard]] u32 track() const noexcept { return track_; }
    private:
        u32 track_{0u};
    };

    class set_anim_cmd final {
    public:
        set_anim_cmd(u32 track, str name)
        : track_(track)
        , name_(std::move(name)) {}

        set_anim_cmd& loop(bool value) noexcept {
            loop_ = value;
            return *this;
        }

        set_anim_cmd& end_message(str value) noexcept {
            end_message_ = std::move(value);
            return *this;
        }

        set_anim_cmd& complete_message(str value) noexcept {
            complete_message_ = std::move(value);
            return *this;
        }

        [[nodiscard]] u32 track() const noexcept { return track_; }
        [[nodiscard]] const str& name() const noexcept { return name_; }
        [[nodiscard]] bool loop() const noexcept { return loop_; }
        [[nodiscard]] const str& end_message() const noexcept { return complete_message_; }
        [[nodiscard]] const str& complete_message() const noexcept { return complete_message_; }
    private:
        u32 track_{0u};
        str name_;
        bool loop_{false};
        str end_message_;
        str complete_message_;
    };

    class add_anim_cmd final {
    public:
        add_anim_cmd(u32 track, str name)
        : track_(track)
        , name_(std::move(name)) {}

        add_anim_cmd& loop(bool value) noexcept {
            loop_ = value;
            return *this;
        }

        add_anim_cmd& delay(f32 value) noexcept {
            delay_ = value;
            return *this;
        }

        add_anim_cmd& end_message(str value) noexcept {
            end_message_ = std::move(value);
            return *this;
        }

        add_anim_cmd& complete_message(str value) noexcept {
            complete_message_ = std::move(value);
            return *this;
        }

        [[nodiscard]] u32 track() const noexcept { return track_; }
        [[nodiscard]] const str& name() const noexcept { return name_; }
        [[nodiscard]] bool loop() const noexcept { return loop_; }
        [[nodiscard]] f32 delay() const noexcept { return delay_; }
        [[nodiscard]] const str& end_message() const noexcept { return end_message_; }
        [[nodiscard]] const str& complete_message() const noexcept { return complete_message_; }
    private:
        u32 track_{0u};
        str name_;
        bool loop_{false};
        f32 delay_{0.f};
        str end_message_;
        str complete_message_;
    };

    class set_empty_anim_cmd final {
    public:
        set_empty_anim_cmd(u32 track)
        : track_(track) {}

        set_empty_anim_cmd& mix_duration(f32 value) noexcept {
            mix_duration_ = value;
            return *this;
        }

        set_empty_anim_cmd& end_message(str value) noexcept {
            end_message_ = std::move(value);
            return *this;
        }

        set_empty_anim_cmd& complete_message(str value) noexcept {
            complete_message_ = std::move(value);
            return *this;
        }

        [[nodiscard]] u32 track() const noexcept { return track_; }
        [[nodiscard]] f32 mix_duration() const noexcept { return mix_duration_; }
        [[nodiscard]] const str& end_message() const noexcept { return end_message_; }
        [[nodiscard]] const str& complete_message() const noexcept { return complete_message_; }
    private:
        u32 track_{0u};
        f32 mix_duration_{0.f};
        str end_message_;
        str complete_message_;
    };

    class add_empty_anim_cmd final {
    public:
        add_empty_anim_cmd(u32 track)
        : track_(track) {}

        add_empty_anim_cmd& delay(f32 value) noexcept {
            delay_ = value;
            return *this;
        }

        add_empty_anim_cmd& mix_duration(f32 value) noexcept {
            mix_duration_ = value;
            return *this;
        }

        add_empty_anim_cmd& end_message(str value) noexcept {
            end_message_ = std::move(value);
            return *this;
        }

        add_empty_anim_cmd& complete_message(str value) noexcept {
            complete_message_ = std::move(value);
            return *this;
        }

        [[nodiscard]] u32 track() const noexcept { return track_; }
        [[nodiscard]] f32 delay() const noexcept { return delay_; }
        [[nodiscard]] f32 mix_duration() const noexcept { return mix_duration_; }
        [[nodiscard]] const str& end_message() const noexcept { return end_message_; }
        [[nodiscard]] const str& complete_message() const noexcept { return complete_message_; }
    private:
        u32 track_{0u};
        f32 delay_{0.f};
        f32 mix_duration_{0.f};
        str end_message_;
        str complete_message_;
    };
}
