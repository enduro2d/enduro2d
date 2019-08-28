/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/spine_system.hpp>

#include <enduro2d/high/components/spine_player.hpp>
#include <enduro2d/high/components/spine_animation_event.hpp>

#include <spine/spine.h>

namespace
{
    // spine will allocate storage for each track, so keep number of track as small as possible
    constexpr e2d::u32 max_track_count = 8;
}

namespace e2d
{
    //
    // internal_state
    //

    class spine_system::internal_state final {
    public:
        void clear_events(ecs::registry& owner) const;
        void process_new_animations(ecs::registry& owner);
        void update_animations(ecs::registry& owner);

        void add_event(spTrackEntry* entry, const str& ev_name);
    private:
        struct animation_visitor_;
        static void event_listener_(spAnimationState* state, spEventType type, spTrackEntry* entry, spEvent* event);
    private:
        flat_map<const spTrackEntry*, str> anim_events_;
        ecs::registry* owner_ = nullptr;
    };
    
    //
    // animation_visitor_
    //

    struct spine_system::internal_state::animation_visitor_ {
        animation_visitor_(
            spSkeletonData* skeleton,
            spAnimationState* anim_state,
            internal_state& state)
        : skeleton_(skeleton)
        , anim_state_(anim_state)
        , state_(state) {}

        void operator()(const spine_animation_event::clear_track& cmd) const noexcept {
            E2D_ASSERT(cmd.track < max_track_count);
            spAnimationState_clearTrack(anim_state_, cmd.track);
        }

        void operator()(const spine_animation_event::set_anim& cmd) const noexcept {
            E2D_ASSERT(cmd.track < max_track_count);

            spAnimation* anim = spSkeletonData_findAnimation(skeleton_, cmd.name.c_str());
            if ( !anim ) {
                the<debug>().error("SPINE_PLAYER: animation '%0' is not found", cmd.name);
                return;
            }
            spTrackEntry* entry = spAnimationState_setAnimation(anim_state_, cmd.track, anim, cmd.loop);
            if ( entry && !cmd.on_complete.empty() ) {
                state_.add_event(entry, cmd.on_complete);
            }
        }
        
        void operator()(const spine_animation_event::add_anim& cmd) const noexcept {
            E2D_ASSERT(cmd.track < max_track_count);
            
            spAnimation* anim = spSkeletonData_findAnimation(skeleton_, cmd.name.c_str());
            if ( !anim ) {
                the<debug>().error("SPINE_PLAYER: animation '%0' is not found", cmd.name);
                return;
            }
            spTrackEntry* entry = spAnimationState_addAnimation(anim_state_, cmd.track, anim, cmd.loop, cmd.delay.value);
            if ( entry && !cmd.on_complete.empty() ) {
                state_.add_event(entry, cmd.on_complete);
            }
        }

        void operator()(const spine_animation_event::empty_anim& cmd) const noexcept {
            E2D_ASSERT(cmd.track < max_track_count);
            spTrackEntry* entry = spAnimationState_addEmptyAnimation(anim_state_, cmd.track, cmd.duration.value, cmd.delay.value);
            if ( entry && !cmd.on_complete.empty() ) {
                state_.add_event(entry, cmd.on_complete);
            }
        }
    private:
        spSkeletonData* skeleton_;
        spAnimationState* anim_state_;
        internal_state& state_;
    };
    
    void spine_system::internal_state::add_event(spTrackEntry* entry, const str& ev_name) {
        anim_events_.insert_or_assign(entry, ev_name);
        entry->listener = &event_listener_;
        entry->userData = this;
    }
    
    void spine_system::internal_state::event_listener_(spAnimationState* state, spEventType type, spTrackEntry* entry, spEvent* event) {
        if ( !entry || !entry->userData || type == SP_ANIMATION_DISPOSE ) {
            return;
        }

        internal_state* self = reinterpret_cast<internal_state*>(entry->userData);
        auto entry_to_event = self->anim_events_.find(entry);
        if ( entry_to_event == self->anim_events_.end() ) {
            return;
        }

        ecs::entity_id id(size_t(state->userData));
        ecs::entity ent(*self->owner_, id);

        switch ( type ) {
            case SP_ANIMATION_COMPLETE: {
                auto* comp = ent.find_component<spine_player::on_complete_event>();
                if ( !comp ) {
                    comp = &ent.assign_component<spine_player::on_complete_event>();
                }
                comp->completed().push_back(std::move(entry_to_event->second));

                self->anim_events_.erase(entry_to_event);
                break;
            }
        }
    }
    
    void spine_system::internal_state::clear_events(ecs::registry& owner) const {
        owner.remove_all_components<spine_player::on_complete_event>();
    }

    void spine_system::internal_state::process_new_animations(ecs::registry& owner) {
        owner_ = &owner;
        owner.for_joined_components<spine_animation_event, spine_player>(
        [this, &owner](
            ecs::entity_id id,
            const spine_animation_event& events,
            spine_player& player)
        {
            spSkeleton* skeleton = player.skeleton().get();
            spAnimationState* anim_state = player.animation().get();
            
            if ( !skeleton || !anim_state ) {
                return;
            }

            anim_state->userData = reinterpret_cast<void*>(size_t(id));

            animation_visitor_ vis(skeleton->data, anim_state, *this);
            for ( auto& ev : events.commands() ) {
                stdex::visit(vis, ev);
            }
        });
        owner.remove_all_components<spine_animation_event>();
        owner_ = nullptr;
    }

    void spine_system::internal_state::update_animations(ecs::registry& owner) {
        const float dt = the<engine>().delta_time();
        owner_ = &owner;
        owner.for_each_component<spine_player>(
        [dt](
            ecs::entity_id id,
            spine_player& player)
        {
            spSkeleton* skeleton = player.skeleton().get();
            spAnimationState* anim_state = player.animation().get();
            
            if ( !skeleton || !anim_state ) {
                return;
            }

            spSkeleton_update(skeleton, dt);
            spAnimationState_update(anim_state, dt);
            spAnimationState_apply(anim_state, skeleton);
            spSkeleton_updateWorldTransform(skeleton);
        });
        owner_ = nullptr;
    }

    spine_system::spine_system()
    : state_(new internal_state()) {}

    spine_system::~spine_system() noexcept {
        spAnimationState_disposeStatics();
    }

    void spine_system::process(ecs::registry& owner) {
        state_->clear_events(owner);
        state_->process_new_animations(owner);
        state_->update_animations(owner);
    }
}
