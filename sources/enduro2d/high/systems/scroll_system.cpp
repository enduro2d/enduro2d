/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/scroll_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/events.hpp>
#include <enduro2d/high/components/scroll.hpp>
#include <enduro2d/high/components/scroll_content.hpp>
#include <enduro2d/high/components/touchable.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;

    struct scroll_hiden_state {
        gcomponent<actor> content_a;
        gcomponent<widget> content_w;
        gcomponent<scroll_content> content_h;
        const_gcomponent<widget> content_area_w;

        bool dragging{false};

        v2f velocity{v2f::zero()};
        v2f local_offset{v2f::zero()};
        v2f prev_translation{v2f::zero()};

        bool check_handles() const noexcept {
            return content_a
                && content_w
                && content_h
                && content_area_w;
        }

        void reset_handles() noexcept {
            content_a = gcomponent<actor>();
            content_w = gcomponent<widget>();
            content_h = gcomponent<scroll_content>();
            content_area_w = const_gcomponent<widget>();
        }
    };
}

namespace
{
    using namespace e2d;

    f32 calculate_rubber_delta(
        f32 offset,
        f32 view_size) noexcept
    {
        return (1.f - (1.f / ((math::abs(offset) * 0.55f / view_size) + 1.f)))
            * view_size
            * math::sign(offset);
    }

    f32 calculate_smooth_damp(
        f32 from,
        f32 to,
        f32& velocity,
        f32 smooth_time,
        f32 delta_time) noexcept
    {
        const f32 omega = 2.f / math::max(0.0001f, smooth_time);
        const f32 x = omega * delta_time;
        const f32 exp = 1.f / (1.f + x + 0.48f * x * x + 0.235f * x * x * x);
        const f32 change = from - to;

        const f32 temp = (velocity + omega * change) * delta_time;
        velocity = (velocity - omega * temp) * exp;
        f32 result = to + (change + temp) * exp;

        if ( to - from > 0.f == result > to ) {
            result = to;
            velocity = (result - to) / delta_time;
        }

        return result;
    }

    v2f calculate_content_offset(
        const v2f& delta,
        const v2f& content_size,
        const v2f& content_area_size,
        const t2f& content_transform) noexcept
    {
        const v2f& content_min =
            content_transform.translation +
            delta;

        const v2f& content_max =
            content_transform.translation +
            content_size * content_transform.scale +
            delta;

        const v2f& content_area_min = v2f::zero();
        const v2f& content_area_max = content_area_size;

        const v2f& min_offset = content_area_min - content_min;
        const v2f& max_offset = content_area_max - content_max;

        v2f offset = v2f::zero();

        if ( min_offset.x < -0.01f ) {
            offset.x = min_offset.x;
        } else if ( max_offset.x > 0.01f ) {
            offset.x = max_offset.x;
        }

        if ( min_offset.y < -0.01f ) {
            offset.y = min_offset.y;
        } else if ( max_offset.y > 0.01f ) {
            offset.y = max_offset.y;
        }

        return offset;
    };

    v2f move_scroll_content(
        scroll& s,
        const v2f& delta,
        gcomponent<actor> content_a)
    {
        if ( delta == v2f::zero() || !content_a || !content_a->node() ) {
            return v2f::zero();
        }

        v2f real_delta = delta;

        if ( !s.horizontal() ) {
            real_delta.x = 0.f;
        }

        if ( !s.vertical() ) {
            real_delta.y = 0.f;
        }

        content_a->node()->translation(
            content_a->node()->translation() +
            real_delta);

        return real_delta;
    }
}

namespace
{
    using namespace e2d;

    void update_scroll_states(ecs::registry& owner) {
        owner.for_joined_components<scroll, actor>([](
            const ecs::const_entity&,
            scroll&,
            const actor& a)
        {
            if ( !a.node() || !a.node()->owner() ) {
                return;
            }

            scroll_hiden_state& state = a.node()->owner().component<scroll_hiden_state>().ensure();

            gcomponent<scroll_content> content_h =
                nodes::find_component_from_children<scroll_content>(
                    a.node(),
                    nodes::options().recursive(true));

            if ( !content_h ) {
                state.reset_handles();
                return;
            }

            gcomponent<actor> content_a = content_h.component<actor>();
            if ( !content_a || !content_a->node() ) {
                state.reset_handles();
                return;
            }

            gcomponent<widget> content_w = content_h.component<widget>();
            if ( !content_w ) {
                state.reset_handles();
                return;
            }

            gcomponent<widget> content_area_w = content_a->node()->parent()
                ? content_a->node()->parent()->owner().component<widget>()
                : gcomponent<widget>();

            if ( !content_area_w ) {
                state.reset_handles();
                return;
            }

            state.content_a = content_a;
            state.content_w = content_w;
            state.content_h = content_h;
            state.content_area_w = content_area_w;

            state.prev_translation = content_a->node()->translation();
        }, !ecs::exists<disabled<scroll>>());
    }

    void update_scroll_inputs(ecs::registry& owner) {
        owner.for_joined_components<scroll, events<touchable_events::event>, actor>([](
            const ecs::const_entity&,
            scroll& s,
            const events<touchable_events::event>& events,
            const actor& a)
        {
            if ( events.empty() ) {
                return;
            }

            if ( !a.node() || !a.node()->owner() ) {
                return;
            }

            scroll_hiden_state& state = a.node()->owner().component<scroll_hiden_state>().ensure();
            if ( !state.check_handles() ) {
                return;
            }

            for ( const touchable_events::event& evt : events.get() ) {

                //
                // mouse_drag_evt
                //

                if ( auto mouse_evt = std::get_if<touchable_events::mouse_drag_evt>(&evt) ) {
                    const v2f& local_offset = v2f(state.content_a->node()->world_to_local(
                        v4f(mouse_evt->world_point(), 0.f, 1.f)));

                    if ( mouse_evt->type() == touchable_events::mouse_drag_evt::types::start ) {
                        state.dragging = false;
                        state.velocity = v2f::zero();
                        state.local_offset = local_offset;
                    }

                    if ( mouse_evt->type() == touchable_events::mouse_drag_evt::types::move ) {
                        const v2f& delta = local_offset - state.local_offset;
                        if ( state.dragging ) {
                            const v2f curr_translation = state.content_a->node()->translation();
                            v2f next_translation = curr_translation + delta;

                            const v2f& offset = calculate_content_offset(
                                next_translation - curr_translation,
                                state.content_w->size(),
                                state.content_area_w->size(),
                                state.content_a->node()->transform());

                            next_translation += offset;

                            if ( !math::is_near_zero(s.elasticity()) ) {
                                for ( std::size_t i = 0; i < 2; ++i ) {
                                    if ( offset[i] != 0.f ) {
                                        next_translation[i] -= calculate_rubber_delta(
                                            offset[i],
                                            state.content_area_w->size()[i]);
                                    }
                                }
                            }

                            move_scroll_content(s, next_translation - curr_translation, state.content_a);
                        } else if ( math::length(delta) > 7.f ) {
                            state.dragging = true;
                            state.local_offset = local_offset;
                        }
                    }

                    if ( mouse_evt->type() == touchable_events::mouse_drag_evt::types::end ) {
                        state.dragging = false;
                    }
                }

                //
                // mouse_scroll_evt
                //

                if ( auto mouse_evt = std::get_if<touchable_events::mouse_scroll_evt>(&evt) ) {
                    state.velocity = v2f::zero();

                    const v2f curr_translation = state.content_a->node()->translation();
                    v2f next_translation = curr_translation;

                    const v2f& delta = s.sensitivity() * mouse_evt->delta();
                    next_translation += delta;

                    if ( math::is_near_zero(s.elasticity()) ) {
                        next_translation += calculate_content_offset(
                            next_translation - curr_translation,
                            state.content_w->size(),
                            state.content_area_w->size(),
                            state.content_a->node()->transform());
                    }

                    move_scroll_content(s, next_translation - curr_translation, state.content_a);
                }
            }
        }, !ecs::exists<disabled<scroll>>());
    }

    void update_scroll_inertia(f32 dt, ecs::registry& owner) {
        owner.for_joined_components<scroll, actor>([dt](
            const ecs::const_entity&,
            scroll& s,
            const actor& a)
        {
            if ( !a.node() || !a.node()->owner() ) {
                return;
            }

            scroll_hiden_state& state = a.node()->owner().component<scroll_hiden_state>().ensure();
            if ( !state.check_handles() ) {
                return;
            }

            v2f offset = calculate_content_offset(
                v2f::zero(),
                state.content_w->size(),
                state.content_area_w->size(),
                state.content_a->node()->transform());

            if ( !state.dragging && (offset != v2f::zero() || state.velocity != v2f::zero()) ) {
                const v2f curr_translation = state.content_a->node()->translation();
                v2f next_translation = curr_translation;

                for ( std::size_t i = 0; i < 2u; ++i ) {
                    if ( s.sensitivity() > 0.f && offset[i] != 0.f ) {
                        f32 velocity = state.velocity[i];
                        next_translation[i] = calculate_smooth_damp(
                            curr_translation[i],
                            curr_translation[i] + offset[i],
                            velocity,
                            s.elasticity(),
                            dt);
                        if ( math::abs(velocity) < 1.f ) {
                            velocity = 0.f;
                        }
                        state.velocity[i] = velocity;
                    } else if ( s.inertia() > 0.f ) {
                        f32 velocity = state.velocity[i];
                        next_translation[i] += velocity * dt;
                        velocity *= math::pow(s.inertia(), dt);
                        if ( math::abs(velocity) < 1.f ) {
                            velocity = 0.f;
                        }
                        state.velocity[i] = velocity;
                    } else {
                        state.velocity[i] = 0.f;
                    }
                }

                if ( math::is_near_zero(s.elasticity()) ) {
                    offset = calculate_content_offset(
                        next_translation - curr_translation,
                        state.content_w->size(),
                        state.content_area_w->size(),
                        state.content_a->node()->transform());
                    next_translation += offset;
                }

                move_scroll_content(s, next_translation - curr_translation, state.content_a);
            }

            if ( state.dragging && s.inertia() > 0.f ) {
                const v2f& last_delta =
                    state.content_a->node()->translation() -
                    state.prev_translation;
                state.velocity = math::lerp(state.velocity, last_delta / dt, dt * 10.f);
            }
        }, !ecs::exists<disabled<scroll>>());
    }
}

namespace e2d
{
    //
    // scroll_system::internal_state
    //

    class scroll_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(f32 dt, ecs::registry& owner) {
            update_scroll_states(owner);
            update_scroll_inputs(owner);
            update_scroll_inertia(dt, owner);
        }
    };

    //
    // scroll_system
    //

    scroll_system::scroll_system()
    : state_(new internal_state()) {}
    scroll_system::~scroll_system() noexcept = default;

    void scroll_system::process(
        ecs::registry& owner,
        const ecs::before<systems::update_event>& trigger)
    {
        E2D_PROFILER_SCOPE("scroll_system.process_update");
        state_->process_update(trigger.event.dt, owner);
    }
}
