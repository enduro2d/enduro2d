/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    struct rotator {
        v3f axis;
    };

    class game_system final : public ecs::system {
    public:
        void process(ecs::registry& owner) override {
            E2D_UNUSED(owner);
            const keyboard& k = the<input>().keyboard();
            if ( k.is_key_just_released(keyboard_key::f12) ) {
                the<dbgui>().toggle_visible(!the<dbgui>().visible());
            }
            if ( k.is_key_just_released(keyboard_key::escape) ) {
                the<window>().set_should_close(true);
            }
        }
    };

    class rotator_system final : public ecs::system {
    public:
        void process(ecs::registry& owner) override {
            const f32 time = the<engine>().time();
            owner.for_joined_components<rotator, actor>(
                [&time](const ecs::const_entity&, const rotator& rot, actor& act){
                    const node_iptr node = act.node();
                    if ( node ) {
                        const q4f q = math::make_quat_from_axis_angle(make_rad(time), rot.axis);
                        node->rotation(q);
                    }
                });
        }
    };

    class game final : public high_application {
    public:
        bool initialize() final {
            if ( !create_scene() || !create_camera() ) {
                return false;
            }
            ecs::registry_filler(the<world>().registry())
                .system<game_system>(world::priority_update)
                .system<rotator_system>(world::priority_update);
            return true;
        }
    private:
        bool create_scene() {
            auto model_res = the<library>().load_asset<model_asset>("gnome_model.json");
            auto sprite_res = the<library>().load_asset<sprite_asset>("ship_sprite.json");
            if ( !model_res || !sprite_res ) {
                return false;
            }

            ecs::entity scene_e = the<world>().registry().create_entity();
            scene_e.assign_component<scene>(node::create(the<world>()));
            node_iptr scene_r = scene_e.get_component<scene>().root();

            {
                ecs::entity model_e = the<world>().registry().create_entity();

                ecs::entity_filler(model_e)
                    .component<rotator>(rotator{v3f::unit_y()})
                    .component<actor>(node::create(model_e, scene_r))
                    .component<renderer>()
                    .component<model_renderer>(model_res);

                node_iptr model_n = model_e.get_component<actor>().node();
                model_n->scale(v3f{20.f});
                model_n->translation(v3f{0.f,50.f,0.f});
            }

            {
                ecs::entity sprite_e = the<world>().registry().create_entity();

                ecs::entity_filler(sprite_e)
                    .component<rotator>(rotator{v3f::unit_z()})
                    .component<actor>(node::create(sprite_e, scene_r))
                    .component<renderer>()
                    .component<sprite_renderer>(sprite_res);

                node_iptr sprite_n = sprite_e.get_component<actor>().node();
                sprite_n->translation(v3f{0,-50.f,0});
            }

            return true;
        }

        bool create_camera() {
            const auto camera_c = camera()
                .background({1.f, 0.4f, 0.f, 1.f})
                .viewport(the<window>().real_size())
                .projection(math::make_orthogonal_lh_matrix4(
                    the<window>().real_size().cast_to<f32>(), 0.f, 1000.f));
            ecs::entity camera_e = the<world>().registry().create_entity();
            camera_e.assign_component<camera>(camera_c);
            camera_e.assign_component<actor>(node::create(camera_e));
            return true;
        }
    };
}

int e2d_main(int argc, char *argv[]) {
    const auto starter_params = starter::parameters(
        engine::parameters("sample_03", "enduro2d")
            .timer_params(engine::timer_parameters()
                .maximal_framerate(100)));
    modules::initialize<starter>(argc, argv, starter_params).start<game>();
    modules::shutdown<starter>();
    return 0;
}
