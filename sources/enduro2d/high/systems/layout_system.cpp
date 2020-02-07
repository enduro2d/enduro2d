/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/layout_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/layout.hpp>

namespace
{
    using namespace e2d;

    bool is_layout_item_enabled(const const_gcomponent<layout_item>& item) noexcept {
        const auto filter = !ecs::exists_any<
            disabled<actor>,
            disabled<layout_item>
        >() && ecs::exists_all<
            actor,
            layout_item
        >();

        return item
            || filter(item.owner().raw_entity())
            || item.owner().component<actor>()->node();
    }

    void update_layout_items(const layout& layout, const const_node_iptr& layout_node) {
        static thread_local vector<gcomponent<layout_item>> items;
        E2D_DEFER([](){ items.clear(); });

        nodes::extract_components_from_children<layout_item>(
            layout_node,
            std::back_inserter(items));

        //
        // layout size
        //

        v2f layout_size = v2f::zero();

        for ( std::size_t i = 0; i < items.size(); ++i ) {
            const gcomponent<layout_item>& item = items[i];
            if ( !is_layout_item_enabled(item) ) {
                continue;
            }
            layout_size += item->size();
            if ( i > 0u ) {
                layout_size += layout.spacing();
            }
        }

        //
        // cursor offsets
        //

        v2f item_cursor = v2f::zero();
        v2f size_offset_mul = v2f::zero();
        v2f cursor_offset_mul = v2f::zero();

        if ( layout.mode() == layout::modes::horizontal ) {
            cursor_offset_mul = v2f::unit_x();

            switch ( layout.halign() ) {
            case layout::haligns::left:
                item_cursor = 0.0f * layout_size.x * cursor_offset_mul;
                break;
            case layout::haligns::center:
                item_cursor = -0.5f * layout_size.x * cursor_offset_mul;
                break;
            case layout::haligns::right:
                item_cursor = -1.0f * layout_size.x * cursor_offset_mul;
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected layout halign");
            }

            switch ( layout.valign() ) {
            case layout::valigns::top:
                size_offset_mul = -1.0f * v2f::unit_y();
                break;
            case layout::valigns::center:
                size_offset_mul = -0.5f * v2f::unit_y();
                break;
            case layout::valigns::bottom:
                size_offset_mul = 0.0f * v2f::unit_y();
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected layout valign");
            }
        } else if ( layout.mode() == layout::modes::vertical ) {
            cursor_offset_mul = v2f::unit_y();

            switch ( layout.valign() ) {
            case layout::valigns::top:
                item_cursor = -1.0f * layout_size.y * cursor_offset_mul;
                break;
            case layout::valigns::center:
                item_cursor = -0.5f * layout_size.y * cursor_offset_mul;
                break;
            case layout::valigns::bottom:
                item_cursor = 0.0f * layout_size.y * cursor_offset_mul;
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected layout valign");
            }

            switch ( layout.halign() ) {
            case layout::haligns::left:
                size_offset_mul = 0.0f * v2f::unit_x();
                break;
            case layout::haligns::center:
                size_offset_mul = -0.5f * v2f::unit_x();
                break;
            case layout::haligns::right:
                size_offset_mul = -1.0f * v2f::unit_x();
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected layout halign");
            }
        } else {
            E2D_ASSERT_MSG(false, "unexpected layout mode");
        }

        //
        // placing
        //

        for ( std::size_t i = 0; i < items.size(); ++i ) {
            const gcomponent<layout_item>& item = items[i];
            if ( !is_layout_item_enabled(item) ) {
                continue;
            }
            const node_iptr& item_node = item.owner().component<actor>()->node();
            item_node->translation(item_cursor + item->size() * size_offset_mul);
            item_cursor += (item->size() + layout.spacing()) * cursor_offset_mul;
        }
    }
}

namespace e2d
{
    //
    // layout_system::internal_state
    //

    class layout_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process_update(ecs::registry& owner) {
            owner.for_joined_components<layout, actor>([](
                const ecs::const_entity&,
                const layout& layout,
                const actor& layout_actor)
            {
                update_layout_items(layout, layout_actor.node());
            }, !ecs::exists_any<
                disabled<actor>,
                disabled<layout>>());
        }
    };

    //
    // layout_system
    //

    layout_system::layout_system()
    : state_(new internal_state()) {}
    layout_system::~layout_system() noexcept = default;

    void layout_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        E2D_PROFILER_SCOPE("layout_system.process_update");
        state_->process_update(owner);
    }
}
