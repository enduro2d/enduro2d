/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/layout_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/layout.hpp>

#include <3rdparty/yoga/Yoga.h>

namespace
{
    using namespace e2d;

    struct yogo_node final {
        using node_ptr = std::shared_ptr<YGNode>;
        node_ptr as_item{YGNodeNew(), YGNodeFree};
        node_ptr as_root{YGNodeNew(), YGNodeFree};
    };
}

namespace
{
    using namespace e2d;

    void update_yogo_node(const yogo_node& yn, const layout& l, const actor& a) {
        switch ( l.direction() ) {
        case layout::directions::row:
            YGNodeStyleSetFlexDirection(yn.as_root.get(), YGFlexDirectionRow);
            break;
        case layout::directions::row_reversed:
            YGNodeStyleSetFlexDirection(yn.as_root.get(), YGFlexDirectionRowReverse);
            break;
        case layout::directions::column:
            YGNodeStyleSetFlexDirection(yn.as_root.get(), YGFlexDirectionColumn);
            break;
        case layout::directions::column_reversed:
            YGNodeStyleSetFlexDirection(yn.as_root.get(), YGFlexDirectionColumnReverse);
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected layout direction");
            break;
        }

        switch ( l.direction() ) {
        case layout::directions::row:
        case layout::directions::row_reversed:
            switch ( l.halign() ) {
            case layout::haligns::left:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifyFlexStart);
                break;
            case layout::haligns::center:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifyCenter);
                break;
            case layout::haligns::right:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifyFlexEnd);
                break;
            case layout::haligns::space_around:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifySpaceAround);
                break;
            case layout::haligns::space_evenly:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifySpaceEvenly);
                break;
            case layout::haligns::space_between:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifySpaceBetween);
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected layout halign");
                break;
            }

            switch ( l.valign() ) {
            case layout::valigns::top:
            case layout::valigns::space_between:
                YGNodeStyleSetAlignItems(yn.as_root.get(), YGAlignFlexEnd);
                break;
            case layout::valigns::center:
            case layout::valigns::space_around:
            case layout::valigns::space_evenly:
                YGNodeStyleSetAlignItems(yn.as_root.get(), YGAlignCenter);
                break;
            case layout::valigns::bottom:
                YGNodeStyleSetAlignItems(yn.as_root.get(), YGAlignFlexStart);
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected layout valign");
                break;
            }

            break;
        case layout::directions::column:
        case layout::directions::column_reversed:
            switch ( l.valign() ) {
            case layout::valigns::top:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifyFlexEnd);
                break;
            case layout::valigns::center:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifyCenter);
                break;
            case layout::valigns::bottom:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifyFlexStart);
                break;
            case layout::valigns::space_around:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifySpaceAround);
                break;
            case layout::valigns::space_evenly:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifySpaceEvenly);
                break;
            case layout::valigns::space_between:
                YGNodeStyleSetJustifyContent(yn.as_root.get(), YGJustifySpaceBetween);
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected layout valign");
                break;
            }

            switch ( l.halign() ) {
            case layout::haligns::left:
            case layout::haligns::space_between:
                YGNodeStyleSetAlignItems(yn.as_root.get(), YGAlignFlexStart);
                break;
            case layout::haligns::center:
            case layout::haligns::space_around:
            case layout::haligns::space_evenly:
                YGNodeStyleSetAlignItems(yn.as_root.get(), YGAlignCenter);
                break;
            case layout::haligns::right:
                YGNodeStyleSetAlignItems(yn.as_root.get(), YGAlignFlexEnd);
                break;
            default:
                E2D_ASSERT_MSG(false, "unexpected layout halign");
                break;
            }
            break;
        default:
            E2D_ASSERT_MSG(false, "unexpected layout mode");
            break;
        }

        {
            YGNodeStyleSetWidth(yn.as_root.get(), l.size().x);
            YGNodeStyleSetHeight(yn.as_root.get(), l.size().y);

            YGNodeStyleSetPadding(yn.as_root.get(), YGEdgeHorizontal, l.padding().x);
            YGNodeStyleSetPadding(yn.as_root.get(), YGEdgeVertical, l.padding().y);
        }

        {
            const v2f& scale = a.node()
                ? a.node()->scale()
                : v2f::unit();

            YGNodeStyleSetWidth(yn.as_item.get(), l.size().x * scale.x);
            YGNodeStyleSetHeight(yn.as_item.get(), l.size().y * scale.y);

            YGNodeStyleSetMargin(yn.as_item.get(), YGEdgeHorizontal, l.margin().x * scale.x);
            YGNodeStyleSetMargin(yn.as_item.get(), YGEdgeVertical, l.margin().y * scale.y);
        }
    }
}

namespace
{
    using namespace e2d;

    void process_yogo_nodes(ecs::registry& owner) {
        ecsex::remove_all_components_with_disposer<yogo_node>(
            owner,
            [](ecs::entity e, const yogo_node&){
                if ( const actor* a = e.find_component<actor>();
                    a && a->node() && a->node()->owner() )
                {
                    gcomponent<layout> l{a->node()->owner()};
                    layouts::mark_dirty(l);
                    layouts::mark_dirty(layouts::find_parent_layout(l));
                }
            },
            !ecs::exists_all<
                actor,
                layout>() ||
            ecs::exists_any<
                disabled<actor>,
                disabled<layout>>());

        owner.for_joined_components<layout, actor>([](
            ecs::entity e,
            const layout&,
            const actor& a)
        {
            e.ensure_component<yogo_node>();
            if ( a.node() && a.node()->owner() ) {
                gcomponent<layout> l{a.node()->owner()};
                layouts::mark_dirty(l);
                layouts::mark_dirty(layouts::find_parent_layout(l));
            }
        },
        !ecs::exists_any<
            yogo_node,
            disabled<actor>,
            disabled<layout>>());
    }

    void process_dirty_layouts(ecs::registry& owner) {
        owner.for_joined_components<layout::dirty, yogo_node, layout, actor>([](
            const ecs::const_entity&,
            const layout::dirty&,
            const yogo_node& root_yn,
            const layout& root_l,
            const actor& root_a)
        {
            update_yogo_node(root_yn, root_l, root_a);
        });

        owner.for_joined_components<layout::dirty, yogo_node, actor>([](
            const ecs::const_entity&,
            const layout::dirty&,
            const yogo_node& root_yn,
            const actor& root_a)
        {
            static thread_local vector<gcomponent<layout>> items;
            E2D_DEFER([](){ items.clear(); });

            nodes::extract_components_from_children<layout>(
                root_a.node(),
                std::back_inserter(items));

            E2D_DEFER([&root_yn](){
                YGNodeRemoveAllChildren(root_yn.as_root.get());
            });

            for ( const auto& item : items ) {
                if ( const auto& item_yn = item.owner().component<yogo_node>() ) {
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
                YGDirectionLTR);

            for ( const auto& item : items ) {
                gcomponent<actor> item_a = item.owner().component<actor>();
                const_gcomponent<yogo_node> item_yn = item.owner().component<yogo_node>();
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
            process_yogo_nodes(owner);
            process_dirty_layouts(owner);
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
