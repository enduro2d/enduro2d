/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../common.hpp"
using namespace e2d;

namespace
{
    struct node_rotator {
    };

    struct renderer_rotator {
        v3f delta;
    };

    class game_system final : public systems::update_system {
    public:
        void process(
            ecs::registry& owner,
            const systems::update_event& event) override
        {
            E2D_UNUSED(owner, event);
            const keyboard& k = the<input>().keyboard();

            if ( k.is_key_just_released(keyboard_key::f1) ) {
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

    class rotator_system final : public systems::update_system {
    public:
        void process(
            ecs::registry& owner,
            const systems::update_event& event) override
        {
            owner.for_joined_components<node_rotator, actor>(
            [&event](const ecs::const_entity&, const node_rotator&, actor& act){
                const node_iptr node = act.node();
                if ( node ) {
                    node->rotation(event.time);
                }
            });

            owner.for_joined_components<renderer_rotator, renderer>(
            [&event](const ecs::const_entity&, const renderer_rotator& rot, renderer& r){
                r.rotation(r.rotation() + rot.delta * event.dt);
            });
        }
    };

    class game final : public starter::application {
    public:
        bool initialize() final {
            return create_scene()
                && create_systems();
        }
    private:
        bool create_scene() {
            auto model_res = the<library>().load_asset<model_asset>("models/gnome/gnome_model.json");
            auto model_mat = the<library>().load_asset<material_asset>("models/gnome/gnome_material.json");
            auto sprite_res = the<library>().load_asset<sprite_asset>("sprites/ship_sprite.json");
            auto sprite_mat = the<library>().load_asset<material_asset>("materials/sprite_material_normal.json");
            auto flipbook_res = the<library>().load_asset<flipbook_asset>("sprites/cube_flipbook.json");

            if ( !model_res || !model_mat || !sprite_res || !sprite_mat || !flipbook_res ) {
                return false;
            }

            auto scene_i = the<world>().instantiate();
            scene_i.component<scene>().assign();
            scene_i.component<named>().assign("scene");

            {
                prefab prefab;
                prefab.prototype()
                    .component<named>(named()
                        .name("camera"))
                    .component<camera>(camera()
                        .background({1.f, 0.4f, 0.f, 1.f}))
                    .component<camera::gizmos>();

                the<world>().instantiate(
                    prefab,
                    scene_i.component<actor>()->node());
            }

            {
                prefab prefab;
                prefab.prototype()
                    .component<named>(named()
                        .name("gnome"))
                    .component<renderer_rotator>(v3f::unit_y())
                    .component<renderer>(renderer()
                        .materials({model_mat})
                        .translation({0.f, 0.f, 5.f}))
                    .component<model_renderer>(model_res);

                the<world>().instantiate(
                    prefab,
                    scene_i.component<actor>()->node(),
                    make_trs2(v2f{0,50.f}, 0.f, v2f{20.f}));
            }

            {
                prefab sprite_prefab;
                sprite_prefab.prototype()
                    .component<named>(named()
                        .name("sprite"))
                    .component<actor>(actor()
                        .node(node::create(math::make_translation_trs2(v2f{-33.f,-56.5f}))))
                    .component<renderer>()
                    .component<sprite_renderer>(sprite_renderer(sprite_res)
                        .materials({{"normal", sprite_mat}}));

                prefab prefab;
                prefab.prototype()
                    .component<named>(named()
                        .name("ship"))
                    .component<node_rotator>();
                prefab.set_children({sprite_prefab});

                the<world>().instantiate(
                    prefab,
                    scene_i.component<actor>()->node(),
                    math::make_translation_trs2(v2f{0,-50.f}));
            }

            {
                prefab sprite_prefab;
                sprite_prefab.prototype()
                    .component<named>(named()
                        .name("sprite"))
                    .component<actor>(actor()
                        .node(node::create(math::make_translation_trs2(v2f{-12.f,-12.f}))))
                    .component<renderer>()
                    .component<sprite_renderer>(sprite_renderer()
                        .filtering(false)
                        .materials({{"normal", sprite_mat}}))
                    .component<flipbook_player>(flipbook_player(flipbook_res)
                        .play("idle")
                        .looped(true));

                prefab child_prefab;
                child_prefab.prototype()
                    .component<named>(named()
                        .name("child"))
                    .component<actor>(actor()
                        .node(node::create(make_trs2(
                            v2f{20.f,0.f},
                            0.f,
                            v2f{0.3f,0.3f}))))
                    .component<node_rotator>();
                child_prefab.set_children({sprite_prefab});

                prefab root_prefab;
                root_prefab.prototype()
                    .component<named>(named()
                        .name("root"))
                    .component<node_rotator>();
                root_prefab.set_children({sprite_prefab, child_prefab});

                for ( std::size_t i = 0; i < 2; ++i )
                for ( std::size_t j = 0; j < 5; ++j ) {
                    t2f trans{
                        {-80.f + j * 40.f, -200.f + i * 40.f},
                        0.f,
                        {2.f,2.f}};
                    the<world>().instantiate(
                        root_prefab,
                        scene_i.component<actor>()->node(),
                        trans);
                }
            }

            return true;
        }

        bool create_systems() {
            ecs::registry_filler(the<world>().registry())
            .feature<struct game_feature>(ecs::feature()
                .add_system<game_system>()
                .add_system<rotator_system>());
            return true;
        }
    };
}

int e2d_main(int argc, char *argv[]) {
    const auto starter_params = starter::parameters(
        engine::parameters("sample_03", "enduro2d")
            .window_params(engine::window_parameters()
                .size({1024, 768}))
            .timer_params(engine::timer_parameters()
                .maximal_framerate(100)));
    modules::initialize<starter>(argc, argv, starter_params).start<game>();
    modules::shutdown<starter>();
    return 0;
}
