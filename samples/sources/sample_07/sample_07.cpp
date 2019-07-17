/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
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
            auto model_res = the<library>().load_asset<model_asset>("gnome_model.json");
            auto font_mat = the<library>().load_asset<material_asset>("font_material.json");
            auto bmfont_res = the<library>().load_asset<font_asset>("bmruen.fnt");

            if ( !model_res || !font_mat || !bmfont_res ) {
                return false;
            }

            auto scene_i = the<world>().instantiate();

            scene_i->entity_filler()
                .component<scene>()
                .component<actor>(node::create(scene_i));

            node_iptr scene_r = scene_i->get_component<actor>().get().node();

            {
                auto label_i = the<world>().instantiate();

                label l;
                l.font(bmfont_res);
                l.text(make_utf32("{Съешь же ещё\nthis мягких french\nбулок, да drink tea!}"));

                label_i->entity_filler()
                .component<actor>(node::create(label_i, scene_r))
                .component<label>(l)
                .component<renderer>(renderer()
                                     .materials({font_mat}))
                .component<model_renderer>(model_res);

                node_iptr label_n = label_i->get_component<actor>().get().node();
                label_n->translation(v3f{-300.f,0.f,0.f});
            }

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
                .system<label_system>(world::priority_update)
                .system<camera_system>(world::priority_pre_render);
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
