/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/layout_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/layout.hpp>
#include <enduro2d/high/components/widget.hpp>

#include <3rdparty/yoga/Yoga.h>

namespace
{
    using namespace e2d;

    struct yogo_node final {
        using node_ptr = std::shared_ptr<YGNode>;
        node_ptr as_item{YGNodeNew(), YGNodeFree};
        node_ptr as_root{YGNodeNew(), YGNodeFree};
    };

    YGDirection convert_to_yogo_direction(layout::directions direction) noexcept {
        switch ( direction ) {
        case layout::directions::ltr:
            return YGDirectionLTR;
        case layout::directions::rtl:
            return YGDirectionRTL;
        default:
            E2D_ASSERT_MSG(false, "unexpected layout direction");
            return YGDirectionLTR;
        }
    }

    YGAlign convert_to_yogo_align(layout::align_modes align_mode) noexcept {
        switch ( align_mode ) {
        case layout::align_modes::flex_start:
            return YGAlignFlexStart;
        case layout::align_modes::center:
            return YGAlignCenter;
        case layout::align_modes::flex_end:
            return YGAlignFlexEnd;
        case layout::align_modes::space_between:
            return YGAlignSpaceBetween;
        case layout::align_modes::space_around:
            return YGAlignSpaceAround;
        default:
            E2D_ASSERT_MSG(false, "unexpected layout align mode");
            return YGAlignFlexStart;
        }
    }

    YGJustify convert_to_yogo_justify(layout::justify_modes justify_mode) noexcept {
        switch ( justify_mode ) {
        case layout::justify_modes::flex_start:
            return YGJustifyFlexStart;
        case layout::justify_modes::center:
            return YGJustifyCenter;
        case layout::justify_modes::flex_end:
            return YGJustifyFlexEnd;
        case layout::justify_modes::space_between:
            return YGJustifySpaceBetween;
        case layout::justify_modes::space_around:
            return YGJustifySpaceAround;
        case layout::justify_modes::space_evenly:
            return YGJustifySpaceEvenly;
        default:
            E2D_ASSERT_MSG(false, "unexpected layout justify mode");
            return YGJustifyFlexStart;
        }
    }

    YGWrap convert_to_yogo_flex_wrap(layout::flex_wraps flex_wrap) noexcept {
        switch ( flex_wrap ) {
        case layout::flex_wraps::no_wrap:
            return YGWrapNoWrap;
        case layout::flex_wraps::wrap:
            return YGWrapWrap;
        case layout::flex_wraps::wrap_reversed:
            return YGWrapWrapReverse;
        default:
            E2D_ASSERT_MSG(false, "unexpected layout flex wrap");
            return YGWrapNoWrap;
        }
    }

    YGFlexDirection convert_to_yogo_flex_direction(layout::flex_directions flex_direction) noexcept {
        switch ( flex_direction ) {
        case layout::flex_directions::row:
            return YGFlexDirectionRow;
        case layout::flex_directions::row_reversed:
            return YGFlexDirectionRowReverse;
        case layout::flex_directions::column:
            return YGFlexDirectionColumn;
        case layout::flex_directions::column_reversed:
            return YGFlexDirectionColumnReverse;
        default:
            E2D_ASSERT_MSG(false, "unexpected layout flex direction");
            return YGFlexDirectionRow;
        }
    }

    void update_yogo_layout(const yogo_node& yn, const layout& l, const widget& w) {
        YGNodeStyleSetWidth(yn.as_root.get(), w.size().x);
        YGNodeStyleSetHeight(yn.as_root.get(), w.size().y);

        YGNodeStyleSetPadding(yn.as_root.get(), YGEdgeHorizontal, w.padding().x);
        YGNodeStyleSetPadding(yn.as_root.get(), YGEdgeVertical, w.padding().y);

        YGNodeStyleSetAlignItems(yn.as_root.get(), convert_to_yogo_align(l.align_items()));
        YGNodeStyleSetAlignContent(yn.as_root.get(), convert_to_yogo_align(l.align_content()));
        YGNodeStyleSetJustifyContent(yn.as_root.get(), convert_to_yogo_justify(l.justify_content()));
        YGNodeStyleSetFlexWrap(yn.as_root.get(), convert_to_yogo_flex_wrap(l.flex_wrap()));
        YGNodeStyleSetFlexDirection(yn.as_root.get(), convert_to_yogo_flex_direction(l.flex_direction()));
    }

    void update_yogo_widget(const yogo_node& yn, const widget& w, const actor& a) {
        const v2f& scale = a.node()
            ? a.node()->scale()
            : v2f::unit();

        YGNodeStyleSetWidth(yn.as_item.get(), w.size().x * scale.x);
        YGNodeStyleSetHeight(yn.as_item.get(), w.size().y * scale.y);

        YGNodeStyleSetMargin(yn.as_item.get(), YGEdgeHorizontal, w.margin().x * scale.x);
        YGNodeStyleSetMargin(yn.as_item.get(), YGEdgeVertical, w.margin().y * scale.y);
    }
}

namespace
{
    using namespace e2d;

    void update_yogo_nodes(ecs::registry& owner) {
        ecsex::remove_all_components_with_disposer<yogo_node>(
            owner,
            [](ecs::entity e, const yogo_node&){
                if ( const actor* a = e.find_component<actor>();
                    a && a->node() && a->node()->owner() )
                {
                    gcomponent<layout> l{a->node()->owner()};
                    gcomponent<widget> w{a->node()->owner()};
                    layouts::mark_dirty(l);
                    layouts::mark_dirty(widgets::find_parent_layout(w));
                }
            }, !ecs::exists_all<
                actor,
                widget>()
            || ecs::exists_any<
                disabled<actor>,
                disabled<widget>>());

        owner.for_joined_components<widget, actor>([](
            ecs::entity e,
            const widget&,
            const actor& a)
        {
            e.ensure_component<yogo_node>();
            if ( a.node() && a.node()->owner() ) {
                gcomponent<layout> l{a.node()->owner()};
                gcomponent<widget> w{a.node()->owner()};
                layouts::mark_dirty(l);
                layouts::mark_dirty(widgets::find_parent_layout(w));
            }
        }, !ecs::exists_any<
            yogo_node,
            disabled<actor>,
            disabled<widget>>());
    }

    void update_dirty_layouts(ecs::registry& owner) {
        owner.for_joined_components<layout::dirty, yogo_node, layout, widget, actor>([](
            const ecs::const_entity&,
            const layout::dirty&,
            const yogo_node& root_yn,
            const layout& root_l,
            const widget& root_w,
            const actor& root_a)
        {
            static thread_local vector<gcomponent<widget>> item_ws;
            DEFER_HPP([](){ item_ws.clear(); });

            nodes::extract_components_from_children<widget>(
                root_a.node(),
                std::back_inserter(item_ws));

            DEFER_HPP([&root_yn](){
                YGNodeRemoveAllChildren(root_yn.as_root.get());
            });

            update_yogo_layout(root_yn, root_l, root_w);

            for ( const auto& item_w : item_ws ) {
                const_gcomponent<actor> item_a{item_w.owner()};
                const_gcomponent<yogo_node> item_yn{item_w.owner()};
                if ( item_a && item_yn ) {
                    update_yogo_widget(
                        *item_yn,
                        *item_w,
                        *item_a);
                    YGNodeInsertChild(
                        root_yn.as_root.get(),
                        item_yn->as_item.get(),
                        YGNodeGetChildCount(root_yn.as_root.get()));
                }
            }

            YGNodeCalculateLayout(
                root_yn.as_root.get(),
                YGUndefined,
                YGUndefined,
                convert_to_yogo_direction(root_l.direction()));

            for ( const auto& item_w : item_ws ) {
                gcomponent<actor> item_a{item_w.owner()};
                const_gcomponent<yogo_node> item_yn{item_w.owner()};
                if ( item_a && item_a->node() && item_yn && item_yn->as_item ) {
                    item_a->node()->translation(v2f(
                        YGNodeLayoutGetLeft(item_yn->as_item.get()),
                        YGNodeLayoutGetTop(item_yn->as_item.get())));
                }
            }
        });

        owner.remove_all_components<layout::dirty>();
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
            update_yogo_nodes(owner);
            update_dirty_layouts(owner);
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
        state_->process_update(owner);
    }
}
