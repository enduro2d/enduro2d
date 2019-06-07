
#include "../common.hpp"
using namespace e2d;

namespace
{
    class game final : public engine::application {
    public:
        bool initialize() final {
            
            auto fs_file = std::make_unique<filesystem_file_source>();
            
            auto sstream1 = the<audio>().preload_stream(fs_file->read("bin/library/Example.ogg"));
            auto sstream2 = the<audio>().create_stream(fs_file->read("bin/library/Example.ogg"));

            if ( !sstream1 || !sstream2 ) {
                return false;
            }

            sound_src1_ = the<audio>().create_source(sstream2);
            sound_src2_ = the<audio>().create_source(sstream1);
            sound_src3_ = the<audio>().create_source(sstream1);
            
            if ( !sound_src1_ || !sound_src2_ || !sound_src3_ ) {
                return false;
            }
            
            sound_src1_->play();
            sound_src1_->looping(true);

            sound_src2_->position(secf(1.0f));
            sound_src2_->looping(true);
            sound_src2_->play();
            
            sound_src3_->position(secf(2.0f));
            sound_src3_->looping(true);
            sound_src3_->play();

            return true;
        }

        bool frame_tick() final {
            const keyboard& k = the<input>().keyboard();

            if ( the<window>().should_close() || k.is_key_just_released(keyboard_key::escape) ) {
                return false;
            }

            if ( k.is_key_just_pressed(keyboard_key::f12) ) {
                the<dbgui>().toggle_visible(!the<dbgui>().visible());
            }

            if ( k.is_key_pressed(keyboard_key::lsuper) && k.is_key_just_released(keyboard_key::enter) ) {
                the<window>().toggle_fullscreen(!the<window>().fullscreen());
            }

            return true;
        }

        void frame_render() final {
            the<render>().execute(render::command_block<64>()
                .add_command(render::viewport_command(
                    the<window>().real_size()))
                .add_command(render::clear_command()
                    .color_value({1.f, 0.4f, 0.f, 1.f})));
        }
    private:
        sound_source_ptr sound_src1_;
        sound_source_ptr sound_src2_;
        sound_source_ptr sound_src3_;
    };
}

int e2d_main(int argc, char *argv[]) {
    auto params = engine::parameters("sample_05", "enduro2d")
        .timer_params(engine::timer_parameters()
            .maximal_framerate(100));
    modules::initialize<engine>(argc, argv, params).start<game>();
    modules::shutdown<engine>();
    return 0;
}
