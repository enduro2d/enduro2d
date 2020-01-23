/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/gizmos_system.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/camera.hpp>

#include <enduro2d/high/editor.hpp>
#include <enduro2d/high/inspector.hpp>

namespace
{
    using namespace e2d;

    class imgui_gizmos_context final : public component_inspector<>::gizmos_context {
    public:
        imgui_gizmos_context(editor& e, inspector& i)
        : editor_(e)
        , inspector_(i) {}

        bool setup(const ecs::const_entity& cam_e) {
            if ( !cam_e.valid() || !ecs::exists_all<actor, camera, camera::gizmos>()(cam_e) ) {
                return false;
            }

            const camera& cam = cam_e.get_component<camera>();
            const actor& cam_a = cam_e.get_component<actor>();

            const const_node_iptr& cam_n = cam_a.node();
            if ( !cam_n ) {
                return false;
            }

            camera_vp_ =
                math::inversed(cam_n->world_matrix()).first *
                cam.projection() *
                math::make_scale_matrix4(0.5f, -0.5f) *
                math::make_translation_matrix4(0.5f, 0.5f) *
                math::make_scale_matrix4(v2f(ImGui::GetIO().DisplaySize));

            return true;
        }

        bool show_for(const ecs::const_entity& e) {
            if ( !e.valid() || !ecs::exists_all<actor>()(e) ) {
                return false;
            }

            const actor& e_a = e.get_component<actor>();

            const const_node_iptr& e_n = e_a.node();
            if ( !e_n ) {
                return false;
            }

            gobject e_go = e_n->owner();
            if ( !e_go ) {
                return false;
            }

            go_matrix_ = e_n->world_matrix() * camera_vp_;
            go_selected_ = e_go == editor_.selection();
            inspector_.show_for(e_go, *this);
            return true;
        }
    public:
        void draw_line(
            const v2f& p1,
            const v2f& p2,
            const color32& color) final
        {
            const v2f pp1 = v2f(v4f(p1, 0.f, 1.f) * go_matrix_);
            const v2f pp2 = v2f(v4f(p2, 0.f, 1.f) * go_matrix_);

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddLine(pp1, pp2, colors::pack_color32(color));
        }

        void draw_rect(
            const v2f& p1,
            const v2f& p2,
            const v2f& p3,
            const v2f& p4,
            const color32& color) final
        {
            const v2f pp1 = v2f(v4f(p1, 0.f, 1.f) * go_matrix_);
            const v2f pp2 = v2f(v4f(p2, 0.f, 1.f) * go_matrix_);
            const v2f pp3 = v2f(v4f(p3, 0.f, 1.f) * go_matrix_);
            const v2f pp4 = v2f(v4f(p4, 0.f, 1.f) * go_matrix_);

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddQuadFilled(pp1, pp2, pp3, pp4, colors::pack_color32(color));
        }

        void draw_wire_rect(
            const v2f& p1,
            const v2f& p2,
            const v2f& p3,
            const v2f& p4,
            const color32& color) final
        {
            const v2f pp1 = v2f(v4f(p1, 0.f, 1.f) * go_matrix_);
            const v2f pp2 = v2f(v4f(p2, 0.f, 1.f) * go_matrix_);
            const v2f pp3 = v2f(v4f(p3, 0.f, 1.f) * go_matrix_);
            const v2f pp4 = v2f(v4f(p4, 0.f, 1.f) * go_matrix_);

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddQuad(pp1, pp2, pp3, pp4, colors::pack_color32(color));
        }

        void draw_circle(
            const v2f& center,
            f32 radius,
            u32 segments,
            const color32& color) final
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            for ( u32 i = 0, e = math::max(6u, segments); i < e; ++i ) {
                const radf a =
                    math::two_pi<f32>() /
                    math::numeric_cast<f32>(e) *
                    math::numeric_cast<f32>(i);
                const v2f p =
                    center +
                    v2f(math::cos(a), math::sin(a)) *
                    radius;
                draw_list->PathLineTo(v2f(v4f(p, 0.f, 1.f) * go_matrix_));
            }
            draw_list->PathFillConvex(colors::pack_color32(color));
        }

        void draw_wire_circle(
            const v2f& center,
            f32 radius,
            u32 segments,
            const color32& color) final
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            for ( u32 i = 0, e = math::max(6u, segments); i < e; ++i ) {
                const radf a =
                    math::two_pi<f32>() /
                    math::numeric_cast<f32>(e) *
                    math::numeric_cast<f32>(i);
                const v2f p =
                    center +
                    v2f(math::cos(a), math::sin(a)) *
                    radius;
                draw_list->PathLineTo(v2f(v4f(p, 0.f, 1.f) * go_matrix_));
            }
            draw_list->PathStroke(colors::pack_color32(color), true);
        }

        bool selected() const noexcept final {
            return go_selected_;
        }
    private:
        editor& editor_;
        inspector& inspector_;
        m4f camera_vp_ = m4f::identity();
        m4f go_matrix_ = m4f::identity();
        bool go_selected_ = false;
    };
}

namespace e2d
{
    //
    // gizmos_system::internal_state
    //

    class gizmos_system::internal_state final : private noncopyable {
    public:
        internal_state(dbgui& d, editor& e, inspector& i)
        : dbgui_(d)
        , gcontext_(e, i) {}
        ~internal_state() noexcept = default;

        void process_render(const ecs::const_entity& cam_e, ecs::registry& owner) {
            if ( !dbgui_.visible() || !gcontext_.setup(cam_e) ) {
                return;
            }
            imgui_utils::with_fullscreen_window("e2d_gizmos_system_window", [this, &owner](){
                owner.for_joined_components<actor>([this](const ecs::const_entity& e, const actor&){
                    gcontext_.show_for(e);
                });
            });
        }
    private:
        dbgui& dbgui_;
        imgui_gizmos_context gcontext_;
    };

    //
    // gizmos_system
    //

    gizmos_system::gizmos_system()
    : state_(new internal_state(the<dbgui>(), the<editor>(), the<inspector>())) {}
    gizmos_system::~gizmos_system() noexcept = default;

    void gizmos_system::process(
        ecs::registry& owner,
        const ecs::after<systems::render_event>& trigger)
    {
        E2D_PROFILER_SCOPE("gizmos_system.process_render");
        state_->process_render(trigger.event.cam_e, owner);
    }
}