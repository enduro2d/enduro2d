/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/spine_player.hpp>

#include <enduro2d/high/components/commands.hpp>
#include <enduro2d/high/components/events.hpp>

#include <spine/spine.h>

namespace
{
    using namespace e2d;

    std::optional<spine_player_events::custom_evt> parse_custom_evt(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        spine_player_events::custom_evt evt;

        if ( root.HasMember("name") ) {
            str evt_name = evt.name();
            if ( !json_utils::try_parse_value(root["name"], evt_name) ) {
                the<debug>().error("SPINE_PLAYER_EVENTS: Incorrect formatting of 'custom_evt.name' property");
                return std::nullopt;
            }
            evt.name(std::move(evt_name));
        }

        if ( root.HasMember("int_value") ) {
            i32 evt_int_value = evt.int_value();
            if ( !json_utils::try_parse_value(root["int_value"], evt_int_value) ) {
                the<debug>().error("SPINE_PLAYER_EVENTS: Incorrect formatting of 'custom_evt.int_value' property");
                return std::nullopt;
            }
            evt.int_value(evt_int_value);
        }

        if ( root.HasMember("float_value") ) {
            f32 evt_float_value = evt.float_value();
            if ( !json_utils::try_parse_value(root["float_value"], evt_float_value) ) {
                the<debug>().error("SPINE_PLAYER_EVENTS: Incorrect formatting of 'custom_evt.float_value' property");
                return std::nullopt;
            }
            evt.float_value(evt_float_value);
        }

        if ( root.HasMember("string_value") ) {
            str evt_string_value = evt.string_value();
            if ( !json_utils::try_parse_value(root["string_value"], evt_string_value) ) {
                the<debug>().error("SPINE_PLAYER_EVENTS: Incorrect formatting of 'custom_evt.string_value' property");
                return std::nullopt;
            }
            evt.string_value(std::move(evt_string_value));
        }

        return evt;
    }

    std::optional<spine_player_events::end_evt> parse_end_evt(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        spine_player_events::end_evt evt;

        if ( root.HasMember("message") ) {
            str evt_message = evt.message();
            if ( !json_utils::try_parse_value(root["message"], evt_message) ) {
                the<debug>().error("SPINE_PLAYER_EVENTS: Incorrect formatting of 'end_evt.message' property");
                return std::nullopt;
            }
            evt.message(std::move(evt_message));
        }

        return evt;
    }

    std::optional<spine_player_events::complete_evt> parse_complete_evt(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        spine_player_events::complete_evt evt;

        if ( root.HasMember("message") ) {
            str evt_message = evt.message();
            if ( !json_utils::try_parse_value(root["message"], evt_message) ) {
                the<debug>().error("SPINE_PLAYER_EVENTS: Incorrect formatting of 'complete_evt.message' property");
                return std::nullopt;
            }
            evt.message(std::move(evt_message));
        }

        return evt;
    }

    std::optional<spine_player_events::event> parse_event(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        str_hash command_type;
        E2D_ASSERT(root.HasMember("type") && root["type"].IsString());
        if ( !json_utils::try_parse_value(root["type"], command_type) ) {
            the<debug>().error("SPINE_PLAYER_EVENTS: Incorrect formatting of 'event.type' property");
            return std::nullopt;
        }

        E2D_ASSERT(root.HasMember("desc") && root["desc"].IsObject());
        const auto& event_desc = root["desc"];

        if ( command_type == make_hash("custom_evt") ) {
            auto evt = parse_custom_evt(event_desc);
            return evt ? evt : std::nullopt;
        } else if ( command_type == make_hash("end_evt") ) {
            auto evt = parse_end_evt(event_desc);
            return evt ? evt : std::nullopt;
        } else if ( command_type == make_hash("complete_evt") ) {
            auto evt = parse_complete_evt(event_desc);
            return evt ? evt : std::nullopt;
        } else {
            the<debug>().error("SPINE_PLAYER_EVENTS: Incorrect formatting of 'event.type' property");
            return std::nullopt;
        }
    }

    std::optional<vector<spine_player_events::event>> parse_events(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsArray());

        vector<spine_player_events::event> evts;
        evts.reserve(root.Size());

        for ( rapidjson::SizeType i = 0; i < root.Size(); ++i ) {
            auto evt = parse_event(root[i]);
            if ( !evt ) {
                return std::nullopt;
            }
            evts.push_back(std::move(*evt));
        }

        return evts;
    }
}

namespace
{
    using namespace e2d;

    std::optional<spine_player_commands::clear_track_cmd> parse_clear_track_cmd(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        u32 cmd_track{0u};
        if ( !json_utils::try_parse_value(root["track"], cmd_track) ) {
            the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'clear_track_cmd.track' property");
            return std::nullopt;
        }

        return spine_player_commands::clear_track_cmd(cmd_track);
    }

    std::optional<spine_player_commands::set_anim_cmd> parse_set_anim_cmd(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        u32 cmd_track{0u};
        E2D_ASSERT(root.HasMember("track"));
        if ( !json_utils::try_parse_value(root["track"], cmd_track) ) {
            the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'set_anim_cmd.track' property");
            return std::nullopt;
        }

        str cmd_name;
        E2D_ASSERT(root.HasMember("name"));
        if ( !json_utils::try_parse_value(root["name"], cmd_name) ) {
            the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'set_anim_cmd.name' property");
            return std::nullopt;
        }

        spine_player_commands::set_anim_cmd cmd(cmd_track, std::move(cmd_name));

        if ( root.HasMember("loop") ) {
            bool cmd_loop = cmd.loop();
            if ( !json_utils::try_parse_value(root["loop"], cmd_loop) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'set_anim_cmd.loop' property");
                return std::nullopt;
            }
            cmd.loop(cmd_loop);
        }

        if ( root.HasMember("end_message") ) {
            str cmd_end_message = cmd.end_message();
            if ( !json_utils::try_parse_value(root["end_message"], cmd_end_message) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'set_anim_cmd.end_message' property");
                return std::nullopt;
            }
            cmd.end_message(std::move(cmd_end_message));
        }

        if ( root.HasMember("complete_message") ) {
            str cmd_complete_message = cmd.complete_message();
            if ( !json_utils::try_parse_value(root["complete_message"], cmd_complete_message) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'set_anim_cmd.complete_message' property");
                return std::nullopt;
            }
            cmd.complete_message(std::move(cmd_complete_message));
        }

        return cmd;
    }

    std::optional<spine_player_commands::add_anim_cmd> parse_add_anim_cmd(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        u32 cmd_track{0u};
        E2D_ASSERT(root.HasMember("track"));
        if ( !json_utils::try_parse_value(root["track"], cmd_track) ) {
            the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_anim_cmd.track' property");
            return std::nullopt;
        }

        str cmd_name;
        E2D_ASSERT(root.HasMember("name"));
        if ( !json_utils::try_parse_value(root["name"], cmd_name) ) {
            the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_anim_cmd.name' property");
            return std::nullopt;
        }

        spine_player_commands::add_anim_cmd cmd(cmd_track, std::move(cmd_name));

        if ( root.HasMember("loop") ) {
            bool cmd_loop = cmd.loop();
            if ( !json_utils::try_parse_value(root["loop"], cmd_loop) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_anim_cmd.loop' property");
                return std::nullopt;
            }
            cmd.loop(cmd_loop);
        }

        if ( root.HasMember("delay") ) {
            f32 cmd_delay = cmd.delay();
            if ( !json_utils::try_parse_value(root["delay"], cmd_delay) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_anim_cmd.delay' property");
                return std::nullopt;
            }
            cmd.delay(cmd_delay);
        }

        if ( root.HasMember("end_message") ) {
            str cmd_end_message = cmd.end_message();
            if ( !json_utils::try_parse_value(root["end_message"], cmd_end_message) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_anim_cmd.end_message' property");
                return std::nullopt;
            }
            cmd.end_message(std::move(cmd_end_message));
        }

        if ( root.HasMember("complete_message") ) {
            str cmd_complete_message = cmd.complete_message();
            if ( !json_utils::try_parse_value(root["complete_message"], cmd_complete_message) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_anim_cmd.complete_message' property");
                return std::nullopt;
            }
            cmd.complete_message(std::move(cmd_complete_message));
        }

        return cmd;
    }

    std::optional<spine_player_commands::set_empty_anim_cmd> parse_set_empty_anim_cmd(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        u32 cmd_track{0u};
        E2D_ASSERT(root.HasMember("track"));
        if ( !json_utils::try_parse_value(root["track"], cmd_track) ) {
            the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'set_empty_anim_cmd.track' property");
            return std::nullopt;
        }

        spine_player_commands::set_empty_anim_cmd cmd(cmd_track);

        if ( root.HasMember("mix_duration") ) {
            f32 cmd_mix_duration = cmd.mix_duration();
            if ( !json_utils::try_parse_value(root["mix_duration"], cmd_mix_duration) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'set_empty_anim_cmd.mix_duration' property");
                return std::nullopt;
            }
            cmd.mix_duration(cmd_mix_duration);
        }

        if ( root.HasMember("end_message") ) {
            str cmd_end_message = cmd.end_message();
            if ( !json_utils::try_parse_value(root["end_message"], cmd_end_message) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'set_empty_anim_cmd.end_message' property");
                return std::nullopt;
            }
            cmd.end_message(std::move(cmd_end_message));
        }

        if ( root.HasMember("complete_message") ) {
            str cmd_complete_message = cmd.complete_message();
            if ( !json_utils::try_parse_value(root["complete_message"], cmd_complete_message) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'set_empty_anim_cmd.complete_message' property");
                return std::nullopt;
            }
            cmd.complete_message(std::move(cmd_complete_message));
        }

        return cmd;
    }

    std::optional<spine_player_commands::add_empty_anim_cmd> parse_add_empty_anim_cmd(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        u32 cmd_track{0u};
        E2D_ASSERT(root.HasMember("track"));
        if ( !json_utils::try_parse_value(root["track"], cmd_track) ) {
            the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_empty_anim_cmd.track' property");
            return std::nullopt;
        }

        spine_player_commands::add_empty_anim_cmd cmd(cmd_track);

        if ( root.HasMember("delay") ) {
            f32 cmd_delay = cmd.delay();
            if ( !json_utils::try_parse_value(root["delay"], cmd_delay) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_empty_anim_cmd.delay' property");
                return std::nullopt;
            }
            cmd.delay(cmd_delay);
        }

        if ( root.HasMember("mix_duration") ) {
            f32 cmd_mix_duration = cmd.mix_duration();
            if ( !json_utils::try_parse_value(root["mix_duration"], cmd_mix_duration) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_empty_anim_cmd.mix_duration' property");
                return std::nullopt;
            }
            cmd.mix_duration(cmd_mix_duration);
        }

        if ( root.HasMember("end_message") ) {
            str cmd_end_message = cmd.end_message();
            if ( !json_utils::try_parse_value(root["end_message"], cmd_end_message) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_empty_anim_cmd.end_message' property");
                return std::nullopt;
            }
            cmd.end_message(std::move(cmd_end_message));
        }

        if ( root.HasMember("complete_message") ) {
            str cmd_complete_message = cmd.complete_message();
            if ( !json_utils::try_parse_value(root["complete_message"], cmd_complete_message) ) {
                the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'add_empty_anim_cmd.complete_message' property");
                return std::nullopt;
            }
            cmd.complete_message(std::move(cmd_complete_message));
        }

        return cmd;
    }

    std::optional<spine_player_commands::command> parse_command(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsObject());

        str_hash command_type;
        E2D_ASSERT(root.HasMember("type") && root["type"].IsString());
        if ( !json_utils::try_parse_value(root["type"], command_type) ) {
            the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'command.type' property");
            return std::nullopt;
        }

        E2D_ASSERT(root.HasMember("desc") && root["desc"].IsObject());
        const auto& command_desc = root["desc"];

        if ( command_type == make_hash("clear_track_cmd") ) {
            auto cmd = parse_clear_track_cmd(command_desc);
            return cmd ? cmd : std::nullopt;
        } else if ( command_type == make_hash("set_anim_cmd") ) {
            auto cmd = parse_set_anim_cmd(command_desc);
            return cmd ? cmd : std::nullopt;
        } else if ( command_type == make_hash("add_anim_cmd") ) {
            auto cmd = parse_add_anim_cmd(command_desc);
            return cmd ? cmd : std::nullopt;
        } else if ( command_type == make_hash("set_empty_anim_cmd") ) {
            auto cmd = parse_set_empty_anim_cmd(command_desc);
            return cmd ? cmd : std::nullopt;
        } else if ( command_type == make_hash("add_empty_anim_cmd") ) {
            auto cmd = parse_add_empty_anim_cmd(command_desc);
            return cmd ? cmd : std::nullopt;
        } else {
            the<debug>().error("SPINE_PLAYER_COMMANDS: Incorrect formatting of 'command.type' property");
            return std::nullopt;
        }
    }

    std::optional<vector<spine_player_commands::command>> parse_commands(const rapidjson::Value& root) {
        E2D_ASSERT(root.IsArray());

        vector<spine_player_commands::command> cmds;
        cmds.reserve(root.Size());

        for ( rapidjson::SizeType i = 0; i < root.Size(); ++i ) {
            auto cmd = parse_command(root[i]);
            if ( !cmd ) {
                return std::nullopt;
            }
            cmds.push_back(std::move(*cmd));
        }

        return cmds;
    }
}

namespace e2d
{
    spine_player::spine_player(const spine_asset::ptr& spine) {
        this->spine(spine);
    }

    spine_player& spine_player::spine(const spine_asset::ptr& value) {
        clipping_ptr new_clipping = clipping_ptr(
            spSkeletonClipping_create(),
            spSkeletonClipping_dispose);
        if ( !new_clipping ) {
            throw std::bad_alloc();
        }

        skeleton_ptr new_skeleton;
        if ( value && value->content().skeleton() ) {
            new_skeleton = skeleton_ptr(
                spSkeleton_create(value->content().skeleton().get()),
                spSkeleton_dispose);
            if ( !new_skeleton ) {
                throw std::bad_alloc();
            }
        }

        animation_ptr new_animation;
        if ( value && value->content().animation() ) {
            new_animation = animation_ptr(
                spAnimationState_create(value->content().animation().get()),
                spAnimationState_dispose);
            if ( !new_animation ) {
                throw std::bad_alloc();
            }
        }

        spine_ = value;
        clipping_ = std::move(new_clipping);
        skeleton_ = std::move(new_skeleton);
        animation_ = std::move(new_animation);
        return *this;
    }

    bool spine_player::skin(str_view name) {
        if ( !skeleton_ ) {
            return false;
        }

        static thread_local str skin_name;
        skin_name = name;

        return !!spSkeleton_setSkinByName(
            skeleton_.get(), skin_name.c_str());
    }

    bool spine_player::attachment(str_view slot, str_view name) {
        if ( !skeleton_ ) {
            return false;
        }

        static thread_local str slot_name;
        slot_name = slot;

        static thread_local str attachment_name;
        attachment_name = name;

        return !!spSkeleton_setAttachment(
            skeleton_.get(), slot_name.c_str(), attachment_name.c_str());
    }

    bool spine_player::has_skin(str_view name) const noexcept {
        if ( !skeleton_ ) {
            return false;
        }

        for ( int i = 0; i < skeleton_->data->skinsCount; ++i ) {
            if ( name == skeleton_->data->skins[i]->name ) {
                return true;
            }
        }

        return false;
    }

    bool spine_player::has_animation(str_view name) const noexcept {
        if ( !skeleton_ ) {
            return false;
        }

        for ( int i = 0; i < skeleton_->data->animationsCount; ++i ) {
            if ( name == skeleton_->data->animations[i]->name ) {
                return true;
            }
        }

        return false;
    }

    const spine_asset::ptr& spine_player::spine() const noexcept {
        return spine_;
    }

    const spine_player::clipping_ptr& spine_player::clipper() const noexcept {
        return clipping_;
    }

    const spine_player::skeleton_ptr& spine_player::skeleton() const noexcept {
        return skeleton_;
    }

    const spine_player::animation_ptr& spine_player::animation() const noexcept {
        return animation_;
    }

    spine_player& spine_player::materials(flat_map<str_hash, material_asset::ptr> value) noexcept {
        materials_ = std::move(value);
        return *this;
    }

    const flat_map<str_hash, material_asset::ptr>& spine_player::materials() const noexcept {
        return materials_;
    }

    material_asset::ptr spine_player::find_material(str_hash name) const noexcept {
        const auto iter = materials_.find(name);
        return iter != materials_.end()
            ? iter->second
            : nullptr;
    }
}

namespace e2d
{
    const char* factory_loader<spine_player>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "spine" : { "$ref": "#/common_definitions/address" },
            "materials" : { "$ref": "#/definitions/materials" },
            "skin" : { "$ref": "#/common_definitions/name" },
            "attachments" : { "$ref": "#/definitions/attachments" }
        },
        "definitions" : {
            "materials" : {
                "type" : "object",
                "required" : [],
                "additionalProperties" : false,
                "properties" : {
                    "normal" : { "$ref": "#/common_definitions/address" },
                    "additive" : { "$ref": "#/common_definitions/address" },
                    "multiply" : { "$ref": "#/common_definitions/address" },
                    "screen" : { "$ref": "#/common_definitions/address" }
                }
            },
            "attachments" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/attachment" }
            },
            "attachment" : {
                "type" : "object",
                "required" : [ "slot", "name" ],
                "additionalProperties" : false,
                "properties" : {
                    "slot" : { "$ref": "#/common_definitions/name" },
                    "name" : { "$ref": "#/common_definitions/name" }
                }
            }
        }
    })json";

    bool factory_loader<spine_player>::operator()(
        spine_player& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("spine") ) {
            auto spine = ctx.dependencies.find_asset<spine_asset>(
                path::combine(ctx.parent_address, ctx.root["spine"].GetString()));
            if ( !spine ) {
                the<debug>().error("SPINE_PLAYER: Dependency 'spine' is not found:\n"
                    "--> Parent address: %0\n"
                    "--> Dependency address: %1",
                    ctx.parent_address,
                    ctx.root["spine"].GetString());
                return false;
            }
            component.spine(spine);
        }

        if ( ctx.root.HasMember("materials") ) {
            const rapidjson::Value& materials_root = ctx.root["materials"];
            flat_map<str_hash, material_asset::ptr> materials;
            materials.reserve(materials_root.MemberCount());
            for ( rapidjson::Value::ConstMemberIterator material_root = materials_root.MemberBegin();
                material_root != materials_root.MemberEnd();
                ++material_root )
            {
                str_hash material_name;
                if ( !json_utils::try_parse_value(material_root->name, material_name) ) {
                    the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'material.name' property");
                    return false;
                }

                str material_address;
                if ( !json_utils::try_parse_value(material_root->value, material_address) ) {
                    the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'material.address' property");
                    return false;
                }

                auto material = ctx.dependencies.find_asset<material_asset>(
                    path::combine(ctx.parent_address, material_address));
                if ( !material ) {
                    the<debug>().error("SPINE_PLAYER: Dependency 'material' is not found:\n"
                        "--> Parent address: %0\n"
                        "--> Dependency address: %1",
                        ctx.parent_address,
                        material_address);
                    return false;
                }
                materials.emplace(material_name, material);
            }
            component.materials(std::move(materials));
        }

        if ( ctx.root.HasMember("skin") ) {
            str skin;
            if ( !json_utils::try_parse_value(ctx.root["skin"], skin) ) {
                the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'skin' property");
                return false;
            }
            component.skin(skin);
        }

        if ( ctx.root.HasMember("attachments") ) {
            const auto& attachments_json = ctx.root["attachments"];
            E2D_ASSERT(attachments_json.IsArray());

            for ( rapidjson::SizeType i = 0; i < attachments_json.Size(); ++i ) {
                E2D_ASSERT(attachments_json[i].IsObject());
                const auto& attachment_json = attachments_json[i];

                str slot;
                E2D_ASSERT(attachment_json.HasMember("slot"));
                if ( !json_utils::try_parse_value(attachment_json["slot"], slot) ) {
                    the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'attachment.slot' property");
                    return false;
                }

                str name;
                E2D_ASSERT(attachment_json.HasMember("name"));
                if ( !json_utils::try_parse_value(attachment_json["name"], name) ) {
                    the<debug>().error("SPINE_PLAYER: Incorrect formatting of 'attachment.name' property");
                    return false;
                }

                component.attachment(slot, name);
            }
        }

        return true;
    }

    bool factory_loader<spine_player>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        if ( ctx.root.HasMember("spine") ) {
            dependencies.add_dependency<spine_asset>(
                path::combine(ctx.parent_address, ctx.root["spine"].GetString()));
        }

        if ( ctx.root.HasMember("materials") ) {
            const rapidjson::Value& materials_root = ctx.root["materials"];
            for ( rapidjson::Value::ConstMemberIterator material_root = materials_root.MemberBegin();
                material_root != materials_root.MemberEnd();
                ++material_root )
            {
                dependencies.add_dependency<material_asset>(
                    path::combine(ctx.parent_address, material_root->value.GetString()));
            }
        }

        return true;
    }
}

namespace e2d
{
    const char* factory_loader<events<spine_player_events::event>>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "events" : { "$ref": "#/definitions/events" }
        },
        "definitions" : {
            "events" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/event" }
            },
            "event" : {
                "type" : "object",
                "required" : [ "type", "desc" ],
                "additionalProperties" : false,
                "properties" : {
                    "type" : { "$ref": "#/common_definitions/name" },
                    "desc" : {
                        "anyOf" : [{
                            "$ref" : "#/definitions/custom_evt"
                        }, {
                            "$ref" : "#/definitions/end"
                        }, {
                            "$ref" : "#/definitions/complete"
                        }]
                    }
                }
            },
            "custom_evt" : {
                "type" : "object",
                "required" : [],
                "additionalProperties" : false,
                "properties" : {
                    "name" : { "type" : "string" },
                    "int_value" : { "type" : "integer" },
                    "float_value" : { "type" : "number" },
                    "string_value" : { "type" : "string" }
                }
            },
            "end" : {
                "type" : "object",
                "required" : [],
                "additionalProperties" : false,
                "properties" : {
                    "message" : { "type" : "string" }
                }
            },
            "complete" : {
                "type" : "object",
                "required" : [],
                "additionalProperties" : false,
                "properties" : {
                    "message" : { "type" : "string" }
                }
            }
        }
    })json";

    bool factory_loader<events<spine_player_events::event>>::operator()(
        events<spine_player_events::event>& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("events") ) {
            auto evts = parse_events(ctx.root["events"]);

            if ( !evts ) {
                return false;
            }

            for ( auto& evt : *evts ) {
                if ( evt.valueless_by_exception() ) {
                    return false;
                }
            }

            component.set(std::move(*evts));
        }

        return true;
    }

    bool factory_loader<events<spine_player_events::event>>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<commands<spine_player_commands::command>>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "commands" : { "$ref": "#/definitions/commands" }
        },
        "definitions" : {
            "commands" : {
                "type" : "array",
                "items" : { "$ref": "#/definitions/command" }
            },
            "command" : {
                "type" : "object",
                "required" : [ "type", "desc" ],
                "additionalProperties" : false,
                "properties" : {
                    "type" : { "$ref": "#/common_definitions/name" },
                    "desc" : {
                        "anyOf" : [{
                            "$ref" : "#/definitions/clear_track_cmd"
                        }, {
                            "$ref" : "#/definitions/set_anim_cmd"
                        }, {
                            "$ref" : "#/definitions/add_anim_cmd"
                        }, {
                            "$ref" : "#/definitions/set_empty_anim_cmd"
                        }, {
                            "$ref" : "#/definitions/add_empty_anim_cmd"
                        }]
                    }
                }
            },
            "clear_track_cmd" : {
                "type" : "object",
                "required" : [ "track" ],
                "additionalProperties" : false,
                "properties" : {
                    "track" : { "type" : "integer", "minimum" : 0 }
                }
            },
            "set_anim_cmd" : {
                "type" : "object",
                "required" : [ "track", "name" ],
                "additionalProperties" : false,
                "properties" : {
                    "track" : { "type" : "integer", "minimum" : 0 },
                    "name" : { "$ref": "#/common_definitions/name" },
                    "loop" : { "type" : "boolean" },
                    "end_message" : { "type" : "string" },
                    "complete_message" : { "type" : "string" }
                }
            },
            "add_anim_cmd" : {
                "type" : "object",
                "required" : [ "track", "name" ],
                "additionalProperties" : false,
                "properties" : {
                    "track" : { "type" : "integer", "minimum" : 0 },
                    "name" : { "$ref": "#/common_definitions/name" },
                    "loop" : { "type" : "boolean" },
                    "delay" : { "type" : "number" },
                    "end_message" : { "type" : "string" },
                    "complete_message" : { "type" : "string" }
                }
            },
            "set_empty_anim_cmd" : {
                "type" : "object",
                "required" : [ "track" ],
                "additionalProperties" : false,
                "properties" : {
                    "track" : { "type" : "integer", "minimum" : 0 },
                    "mix_duration" : { "type" : "number" },
                    "end_message" : { "type" : "string" },
                    "complete_message" : { "type" : "string" }
                }
            },
            "add_empty_anim_cmd" : {
                "type" : "object",
                "required" : [ "track" ],
                "additionalProperties" : false,
                "properties" : {
                    "track" : { "type" : "integer", "minimum" : 0 },
                    "delay" : { "type" : "number" },
                    "mix_duration" : { "type" : "number" },
                    "end_message" : { "type" : "string" },
                    "complete_message" : { "type" : "string" }
                }
            }
        }
    })json";

    bool factory_loader<commands<spine_player_commands::command>>::operator()(
        commands<spine_player_commands::command>& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("commands") ) {
            auto cmds = parse_commands(ctx.root["commands"]);

            if ( !cmds ) {
                return false;
            }

            for ( auto& cmd : *cmds ) {
                if ( cmd.valueless_by_exception() ) {
                    return false;
                }
            }

            component.set(std::move(*cmds));
        }

        return true;
    }

    bool factory_loader<commands<spine_player_commands::command>>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<spine_player>::title = ICON_FA_PARAGRAPH " spine_player";

    void component_inspector<spine_player>::operator()(gcomponent<spine_player>& c) const {
        E2D_UNUSED(c);
        ///TODO(BlackMat): add 'spine' inspector
        ///TODO(BlackMat): add 'materials' inspector
    }
}
