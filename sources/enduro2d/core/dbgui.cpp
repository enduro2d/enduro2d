/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "dbgui_impl/dbgui.hpp"

#include "dbgui_impl/widgets/console_widget.hpp"
#include "dbgui_impl/widgets/engine_widget.hpp"
#include "dbgui_impl/widgets/window_widget.hpp"

namespace e2d
{
    class dbgui::internal_state final : private e2d::noncopyable {
    public:
        using context_uptr = std::unique_ptr<
            ImGuiContext, void(*)(ImGuiContext*)>;
    public:
        internal_state(debug& d, input& i, render& r, window& w)
        : debug_(d)
        , input_(i)
        , render_(r)
        , window_(w)
        , context_(ImGui::CreateContext(), ImGui::DestroyContext)
        , listener_(w.register_event_listener<imgui::imgui_event_listener>(bind_context_(), w))
        {
            ImGuiIO& io = bind_context_();
            setup_key_map_(io);
            setup_config_flags_(io);
            setup_internal_resources_(io);
        }

        ~internal_state() noexcept {
            window_.unregister_event_listener(listener_);
        }
    public:
        void register_menu_widget(str menu, str item, widget_uptr widget) {
            E2D_ASSERT(!menu.empty() && !item.empty() && widget);
            std::lock_guard<std::recursive_mutex> guard(rmutex_);

            widget_desc desc{std::move(item), std::move(widget), false};
            if ( widgets_.contains(menu) ) {
                widgets_[menu].push_back(std::move(desc));
            } else {
                vector<widget_desc> descs;
                descs.push_back(std::move(desc));
                widgets_.emplace(std::move(menu), std::move(descs));
            }
        }

        bool visible() const noexcept {
            std::lock_guard<std::recursive_mutex> guard(rmutex_);
            return visible_;
        }

        void toggle_visible(bool yesno) noexcept {
            std::lock_guard<std::recursive_mutex> guard(rmutex_);
            visible_ = yesno;
        }

        void frame_tick() {
            std::lock_guard<std::recursive_mutex> guard(rmutex_);

            ImGuiIO& io = bind_context_();
            const mouse& m = input_.mouse();
            const keyboard& k = input_.keyboard();

            io.MouseDown[0] =
                m.is_button_just_pressed(mouse_button::left) ||
                m.is_button_pressed(mouse_button::left);

            io.MouseDown[1] =
                m.is_button_just_pressed(mouse_button::right) ||
                m.is_button_pressed(mouse_button::right);

            io.KeyCtrl =
                k.is_key_just_pressed(keyboard_key::lcontrol) ||
                k.is_key_just_pressed(keyboard_key::rcontrol) ||
                k.is_key_pressed(keyboard_key::lcontrol) ||
                k.is_key_pressed(keyboard_key::rcontrol);

            io.KeyShift =
                k.is_key_just_pressed(keyboard_key::lshift) ||
                k.is_key_just_pressed(keyboard_key::rshift) ||
                k.is_key_pressed(keyboard_key::lshift) ||
                k.is_key_pressed(keyboard_key::rshift);

            io.KeyAlt =
                k.is_key_just_pressed(keyboard_key::lalt) ||
                k.is_key_just_pressed(keyboard_key::ralt) ||
                k.is_key_pressed(keyboard_key::lalt) ||
                k.is_key_pressed(keyboard_key::ralt);

            io.KeySuper =
                k.is_key_just_pressed(keyboard_key::lsuper) ||
                k.is_key_just_pressed(keyboard_key::rsuper) ||
                k.is_key_pressed(keyboard_key::lsuper) ||
                k.is_key_pressed(keyboard_key::rsuper);

            io.DisplaySize =
                window_.real_size().cast_to<f32>();

            io.DisplayFramebufferScale = io.DisplaySize.x > 0.f && io.DisplaySize.y > 0.f
                ? window_.framebuffer_size().cast_to<f32>() / v2f(io.DisplaySize)
                : v2f(1.f, 1.f);

            window_.set_cursor_shape(
                imgui::convert_mouse_cursor(
                    ImGui::GetMouseCursor()));

            if ( ImGui::GetFrameCount() > 0 ) {
                ImGui::EndFrame();
            }

            ImGui::NewFrame();
        }

        void frame_render() {
            std::lock_guard<std::recursive_mutex> guard(rmutex_);

            ImGui::Render();
            ImDrawData* draw_data = ImGui::GetDrawData();
            if ( !draw_data ) {
                return;
            }

            const v2f display_size = draw_data->DisplaySize;
            const v2f framebuffer_size = display_size * v2f(draw_data->FramebufferScale);

            const m4f projection =
                math::make_orthographic_lh_no_matrix4(display_size, 0.f, 1.f) *
                math::make_translation_matrix4(-1.f, 1.f);

            for ( int i = 0; i < draw_data->CmdListsCount; ++i ) {
                const ImDrawList* cmd_list = draw_data->CmdLists[i];

                update_index_buffer_({
                    cmd_list->IdxBuffer.Data,
                    math::numeric_cast<std::size_t>(cmd_list->IdxBuffer.Size) * sizeof(ImDrawIdx)});

                update_vertex_buffer_({
                    cmd_list->VtxBuffer.Data,
                    math::numeric_cast<std::size_t>(cmd_list->VtxBuffer.Size) * sizeof(ImDrawVert)});

                const auto geometry = render::geometry()
                    .indices(index_buffer_)
                    .add_vertices(vertex_buffer_);

                std::size_t first_index = 0;
                for ( int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; ++cmd_i ) {
                    const ImDrawCmd& pcmd = cmd_list->CmdBuffer[cmd_i];

                    const b2f clip_r(
                        (pcmd.ClipRect.x) * draw_data->FramebufferScale.x,
                        (display_size.y - pcmd.ClipRect.w) * draw_data->FramebufferScale.y,
                        (pcmd.ClipRect.z - pcmd.ClipRect.x) * draw_data->FramebufferScale.x,
                        (pcmd.ClipRect.w - pcmd.ClipRect.y) * draw_data->FramebufferScale.y);

                    if ( math::minimum(clip_r.position) >= 0.f
                        && clip_r.position.x + clip_r.size.x <= framebuffer_size.x
                        && clip_r.position.y + clip_r.size.y <= framebuffer_size.y )
                    {
                        texture_ptr texture = pcmd.TextureId
                            ? *static_cast<texture_ptr*>(pcmd.TextureId)
                            : texture_ptr();

                        mprops_
                            .property("u_MVP", projection)
                            .sampler("u_texture", render::sampler_state()
                                .texture(texture)
                                .min_filter(render::sampler_min_filter::linear)
                                .mag_filter(render::sampler_mag_filter::linear));

                        render_.execute(render::command_block<8>()
                            .add_command(render::viewport_command(
                                framebuffer_size.cast_to<i32>(),
                                clip_r.cast_to<i32>()))
                            .add_command(render::draw_command(material_, geometry, mprops_)
                                .index_range(first_index, pcmd.ElemCount)));
                    }

                    first_index += pcmd.ElemCount;
                }
            }
        }

        void show_main_dock_space() {
            imgex::with_main_dock_space([this](){
                show_widget_menu_bar_();
                show_widget_windows_();
            });
        }
    private:
        ImGuiIO& bind_context_() noexcept {
            ImGui::SetCurrentContext(context_.get());
            return ImGui::GetIO();
        }

        void setup_key_map_(ImGuiIO& io) noexcept {
            const auto map_key = [&io](ImGuiKey_ imgui_key, keyboard_key key) noexcept {
                E2D_ASSERT(imgui_key < ImGuiKey_COUNT);
                io.KeyMap[imgui_key] = utils::enum_to_underlying(key);
            };

            map_key(ImGuiKey_Tab, keyboard_key::tab);

            map_key(ImGuiKey_LeftArrow, keyboard_key::left);
            map_key(ImGuiKey_RightArrow, keyboard_key::right);
            map_key(ImGuiKey_UpArrow, keyboard_key::up);
            map_key(ImGuiKey_DownArrow, keyboard_key::down);

            map_key(ImGuiKey_PageUp, keyboard_key::page_up);
            map_key(ImGuiKey_PageDown, keyboard_key::page_down);

            map_key(ImGuiKey_Home, keyboard_key::home);
            map_key(ImGuiKey_End, keyboard_key::end);
            map_key(ImGuiKey_Insert, keyboard_key::insert);
            map_key(ImGuiKey_Delete, keyboard_key::del);

            map_key(ImGuiKey_Backspace, keyboard_key::backspace);
            map_key(ImGuiKey_Space, keyboard_key::space);
            map_key(ImGuiKey_Enter, keyboard_key::enter);
            map_key(ImGuiKey_Escape, keyboard_key::escape);

            map_key(ImGuiKey_A, keyboard_key::a);
            map_key(ImGuiKey_C, keyboard_key::c);
            map_key(ImGuiKey_V, keyboard_key::v);
            map_key(ImGuiKey_X, keyboard_key::x);
            map_key(ImGuiKey_Y, keyboard_key::y);
            map_key(ImGuiKey_Z, keyboard_key::z);
        }

        void setup_config_flags_(ImGuiIO& io) noexcept {
            io.IniFilename = nullptr;
            io.LogFilename = nullptr;

            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

            io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        }

        void setup_internal_resources_(ImGuiIO& io) {
            {
                shader_ = render_.create_shader(
                    imgui::vertex_source_cstr(),
                    imgui::fragment_source_cstr());

                if ( !shader_ ) {
                    throw bad_dbgui_operation();
                }
            }
            {
                unsigned char* font_pixels;
                int font_width, font_height;
                io.Fonts->GetTexDataAsRGBA32(&font_pixels, &font_width, &font_height);

                texture_ = render_.create_texture(image(
                    make_vec2(font_width, font_height).cast_to<u32>(),
                    image_data_format::rgba8,
                    buffer(
                        font_pixels,
                        math::numeric_cast<std::size_t>(font_width * font_height) * sizeof(u32))));

                if ( !texture_ ) {
                    throw bad_dbgui_operation();
                }

                io.Fonts->TexID = &texture_;
            }
            {
                material_ = render::material()
                    .add_pass(render::pass_state()
                        .states(render::state_block()
                            .capabilities(render::capabilities_state()
                                .blending(true))
                            .blending(render::blending_state()
                                .src_factor(render::blending_factor::src_alpha)
                                .dst_factor(render::blending_factor::one_minus_src_alpha)))
                        .shader(shader_));
            }
        }

        static std::size_t calculate_new_buffer_size_(
            std::size_t esize, std::size_t osize, std::size_t nsize)
        {
            std::size_t msize = std::size_t(-1) / esize * esize;
            if ( nsize > msize ) {
                throw bad_dbgui_operation();
            }
            if ( osize >= msize / 2u ) {
                return msize;
            }
            return math::max(osize * 2u, nsize);
        }

        void update_index_buffer_(buffer_view indices) {
            if ( index_buffer_ && index_buffer_->buffer_size() >= indices.size() ) {
                render_.update_buffer(index_buffer_, indices, 0u);
                return;
            }

            const std::size_t new_buffer_size = calculate_new_buffer_size_(
                sizeof(ImDrawIdx),
                index_buffer_ ? index_buffer_->buffer_size() : 0u,
                indices.size());

            buffer new_buffer_data(new_buffer_size);
            std::memcpy(new_buffer_data.data(), indices.data(), indices.size());

            index_buffer_ = render_.create_index_buffer(
                new_buffer_data,
                index_declaration::index_type::unsigned_short,
                index_buffer::usage::dynamic_draw);

            if ( !index_buffer_ ) {
                debug_.error("DBGUI: Failed to create index buffer:\n"
                    "--> Size: %0",
                    new_buffer_size);
            }
        }

        void update_vertex_buffer_(buffer_view vertices) {
            if ( vertex_buffer_ && vertex_buffer_->buffer_size() >= vertices.size() ) {
                render_.update_buffer(vertex_buffer_, vertices, 0u);
                return;
            }

            const std::size_t new_buffer_size = calculate_new_buffer_size_(
                sizeof(ImDrawVert),
                vertex_buffer_ ? vertex_buffer_->buffer_size() : 0u,
                vertices.size());

            buffer new_buffer_data(new_buffer_size);
            std::memcpy(new_buffer_data.data(), vertices.data(), vertices.size());

            vertex_buffer_ = render_.create_vertex_buffer(
                new_buffer_data,
                vertex_declaration()
                    .add_attribute<v2f>("a_position")
                    .add_attribute<v2f>("a_uv")
                    .add_attribute<color32>("a_color").normalized(),
                vertex_buffer::usage::dynamic_draw);

            if ( !vertex_buffer_ ) {
                debug_.error("DBGUI: Failed to create vertex buffer:\n"
                    "--> Size: %0",
                    new_buffer_size);
            }
        }

        void show_widget_menu_bar_() {
            imgex::with_menu_bar([this](){
                for ( auto& p : widgets_ ) {
                    imgex::with_menu(p.first, [&p](){
                        for ( widget_desc& desc : p.second ) {
                            ImGui::MenuItem(desc.item.c_str(), nullptr, &desc.opened);
                        }
                    });
                }
            });
        }

        void show_widget_windows_() {
            for ( auto& p : widgets_ ) {
                for ( widget_desc& desc : p.second ) {
                    if ( !desc.opened ) {
                        continue;
                    }

                    const str& title = desc.widget->desc().title
                        ? *desc.widget->desc().title
                        : desc.item;

                    if ( desc.widget->desc().first_size ) {
                        ImGui::SetNextWindowSize(
                            *desc.widget->desc().first_size,
                            ImGuiCond_FirstUseEver);
                    }

                    imgex::with_window(title, &desc.opened, ImGuiWindowFlags_None, [&desc](){
                        if ( !desc.widget->show() ) {
                            desc.opened = false;
                        }
                    });
                }
            }
        }
    private:
        debug& debug_;
        input& input_;
        render& render_;
        window& window_;
        context_uptr context_;
        window::event_listener& listener_;
    private:
        shader_ptr shader_;
        texture_ptr texture_;
        render::material material_;
        render::property_block mprops_;
        index_buffer_ptr index_buffer_;
        vertex_buffer_ptr vertex_buffer_;
    private:
        struct widget_desc {
            str item;
            widget_uptr widget;
            bool opened = false;
        };
        mutable std::recursive_mutex rmutex_;
        bool visible_{false};
        flat_map<str, vector<widget_desc>> widgets_;
    };
}

namespace e2d
{
    dbgui::dbgui(debug& d, input& i, render& r, window& w)
    : state_(new internal_state(d, i, r, w)) {
        register_menu_widget<dbgui_widgets::console_widget>("Debug", "Console...", d);
        register_menu_widget<dbgui_widgets::engine_widget>("Debug", "Engine...");
        register_menu_widget<dbgui_widgets::window_widget>("Debug", "Window...");
    }
    dbgui::~dbgui() noexcept = default;

    void dbgui::register_menu_widget(str menu, str item, widget_uptr widget) {
        state_->register_menu_widget(std::move(menu), std::move(item), std::move(widget));
    }

    bool dbgui::visible() const noexcept {
        return state_->visible();
    }

    void dbgui::toggle_visible(bool yesno) noexcept {
        state_->toggle_visible(yesno);
    }

    void dbgui::frame_tick() {
        E2D_PROFILER_SCOPE("dbgui.frame_tick");
        state_->frame_tick();

        if ( visible() ) {
            state_->show_main_dock_space();
        }
    }

    void dbgui::frame_render() {
        E2D_PROFILER_SCOPE("dbgui.frame_render");
        state_->frame_render();
    }
}
