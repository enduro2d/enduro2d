/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
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

            if ( k.is_key_pressed(keyboard_key::lsuper) && k.is_key_just_released(keyboard_key::enter) ) {
                the<window>().toggle_fullscreen(!the<window>().fullscreen());
            }
        }
    };
    
    class camera_system final : public ecs::system {
    public:
        void process(ecs::registry& owner) override {
            owner.for_joined_components<camera>(
            [](const ecs::const_entity&, camera& cam){
                if ( !cam.target() ) {
                    cam.viewport(
                        the<window>().real_size());
                    cam.projection(math::make_orthogonal_lh_matrix4(
                        the<window>().real_size().cast_to<f32>(), 0.f, 1000.f));
                }
            });
        }
    };

    class game final : public starter::application {
    public:
        bool initialize() final {
            return create_scene()
                && create_camera()
                && create_systems();
        }
    private:
        bool create_scene() {
            auto spine_res = the<library>().load_asset<spine_model_asset>("spine_raptor.json");
            auto spine_mat = the<library>().load_asset<material_asset>("spine_material.json");

            if ( !spine_res || !spine_mat ) {
                return false;
            }
            
            auto scene_i = the<world>().instantiate();

            scene_i->entity_filler()
                .component<scene>()
                .component<actor>(node::create(scene_i));

            node_iptr scene_r = scene_i->get_component<actor>().get().node();
            
        #if 1
            auto spine_i = the<world>().instantiate();
            spine_i->entity_filler()
                .component<actor>(node::create(spine_i, scene_r))
                .component<renderer>(renderer()
                    .materials({spine_mat}))
                .component<spine_renderer>(spine_renderer(spine_res))
                .component<spine_player>(spine_player(spine_res)
                    .set_animation(0, "walk", true)
                    .add_animation(1, "gun-grab", false, secf(2.0f)));
            
            node_iptr spine_n = spine_i->get_component<actor>().get().node();
            spine_n->scale(v3f(0.25f));
            spine_n->translation(v3f{-40.f, -100.f, 0.0f});
        #else
            // performace test
            for ( std::size_t i = 0; i < 20; ++i )
            for ( std::size_t j = 0; j < 40; ++j ) {
                auto spine_i = the<world>().instantiate();
                spine_i->entity_filler()
                    .component<actor>(node::create(spine_i, scene_r))
                    .component<renderer>(renderer()
                        .materials({spine_mat}))
                    .component<spine_renderer>(spine_renderer(spine_res))
                    .component<spine_player>(spine_player(spine_res)
                        .set_animation(0, "walk", true)
                        .add_animation(1, "gun-grab", false, secf(2.0f)));
            
                node_iptr spine_n = spine_i->get_component<actor>().get().node();
                spine_n->scale(v3f(0.05f));
                spine_n->translation(v3f{-400.f, -300.f, 0.0f} + v3f{j * 30.f, i * 30.f, 0});
            }
        #endif
            return true;
        }

        bool create_camera() {
            auto camera_i = the<world>().instantiate();
            camera_i->entity_filler()
                .component<camera>(camera()
                    .background({1.f, 0.4f, 0.f, 1.f}))
                .component<actor>(node::create(camera_i));
            return true;
        }

        bool create_systems() {
            ecs::registry_filler(the<world>().registry())
                .system<game_system>(world::priority_update)
                .system<camera_system>(world::priority_pre_render)
                .system<spine_system>(world::priority_update);
            return true;
        }
    };
}

int e2d_main(int argc, char *argv[]) {
    const auto starter_params = starter::parameters(
        engine::parameters("sample_07", "enduro2d")
            .timer_params(engine::timer_parameters()
                .maximal_framerate(100)));
    modules::initialize<starter>(argc, argv, starter_params).start<game>();
    modules::shutdown<starter>();
    return 0;
}
