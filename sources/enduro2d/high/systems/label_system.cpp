/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/label_system.hpp>

#include <enduro2d/high/components/label.hpp>
#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/model_renderer.hpp>
#include <enduro2d/high/assets/texture_asset.hpp>
#include <enduro2d/high/assets/shader_asset.hpp>

namespace
{
    using namespace e2d;

    f32 get_x_pos(const label& l, f32 string_width) {
        f32 x_pos{0};
        switch ( l.halign() ) {
            case label::haligns::left :
                x_pos = 0;
            break;
            case label::haligns::center :
                x_pos = -0.5f * string_width;
            break;
            case label::haligns::right :
                x_pos = -string_width;
            break;
            default:
                E2D_ASSERT_MSG(false,"label_system: uncknow horizontal align flag");
            break;
        }

        return x_pos;
    }

    f32 get_top_pos(const label& l, u32 strings_count) {
        const auto& f = l.font()->content();
        f32 label_height = f.common().line * strings_count;
        f32 top{0};
        switch ( l.valign() ) {
            case label::valigns::top :
                top = label_height;
            break;
            case label::valigns::center :
                top = 0.5f * label_height;
            break;
            case label::valigns::bottom :
                top = 0;
            break;
            case label::valigns::baseline :
                top = label_height - (f.common().line - f.common().base);
            break;
            default:
                E2D_ASSERT_MSG(false,"label_system: uncknow vertical align flag");
            break;
        }

        return top;
    }
}

namespace e2d
{
    //
    // label_system::internal_state
    //

    class label_system::internal_state final : private noncopyable {
    public:
        internal_state() = default;
        ~internal_state() noexcept = default;

        void process(ecs::registry& owner) {
            owner.for_joined_components<label::dirty, label, renderer, model_renderer>([](
                const ecs::const_entity&,
                const label::dirty&,
                const label& l,
                renderer& r,
                model_renderer& mr)
            {
                vector<v3f> vertices;
                vector<u32> indices;
                vector<v2f> uvs;
                vector<v3f> normals;
                vector<color32> colors;
                v2f offset;
                f32 kerning{0};
                u32 prev_char{0};
                str32 text = l.text();
                if (text.empty()) {
                    return;
                }
                const auto& f = l.font()->content();
                auto common = f.common();
                v2f texture_size = common.atlas_size.cast_to<f32>();

                if ( l.width() != 0 ) {
                    f32 word_width{0};
                    f32 string_width{0};
                    i32 last_space{-1};
                    for ( size_t i = 0; i < text.size(); i++ ) {
                        const auto& ch = text[i];
                        if ( ch == '\n' ) {
                            word_width = 0;
                            string_width = 0;
                            last_space = -1;
                        } else {
                            auto data = f.find_char(ch);
                            if ( data ) {
                                prev_char != 0
                                    ? kerning = f.get_kerning(prev_char, data->id)
                                    : kerning = 0;
                                prev_char = ch;
                                f32 char_width = data->advance + kerning;
                                if ( ch == ' ' ) {
                                    if ( string_width + word_width < l.width() ) {
                                        if ( string_width + word_width + char_width < l.width() ) {
                                            string_width += word_width + char_width;
                                            word_width = 0;
                                            last_space = i;
                                        } else {
                                            text[i] = '\n';
                                            word_width = 0;
                                            string_width = 0;
                                            last_space = -1;
                                        }
                                    } else {
                                        if ( last_space != -1 ) {
                                            text[last_space] = '\n';
                                            last_space = -1;
                                            string_width = 0;
                                            --i;
                                        } else {
                                            text[i] = '\n';
                                            word_width = 0;
                                            string_width = 0;
                                        }
                                    }
                                } else {
                                    word_width += char_width;
                                    if ( i == text.size() - 1) {
                                        if ( string_width + word_width > l.width() ) {
                                            if ( last_space != -1 ) {
                                                text[last_space] = '\n';
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                size_t strings_count{1};
                for ( size_t i = 0; i < text.size(); i++ ) {
                    if ( text[i] == '\n' && i != text.size() - 1) {
                        strings_count++;
                    }
                }
                size_t letters_size{0};
                vector<f32> strings_width(strings_count);
                std::fill(strings_width.begin(), strings_width.end(), 0);
                prev_char = 0;
                for ( size_t i = 0, string_ind = 0; i < text.size(); i++ ) {
                    if ( text[i] != '\n' ) {
                        auto data = f.find_char(text[i]);
                        if ( data ) {
                            letters_size++;
                            prev_char != 0
                                ? kerning = f.get_kerning(prev_char, data->id)
                                : kerning = 0;
                            strings_width[string_ind] += data->advance + kerning;
                        }
                    } else {
                        string_ind++;
                    }
                }

                vertices.resize(letters_size * 4);
                uvs.resize(vertices.size());
                colors.resize(vertices.size());
                indices.resize(text.size() * 6);
                f32 x_pos = get_x_pos(l, strings_width[0]);
                f32 y_pos = get_top_pos(l, strings_count);
                u32 letters_counter{0};
                prev_char = 0;
                for ( size_t i = 0, str_ind = 0; i < text.size(); i++ ) {
                    if ( text[i] == '\n' ) {
                        str_ind++;
                        y_pos -= common.line;
                        x_pos = get_x_pos(l, strings_width[str_ind]);
                        prev_char = 0;
                        continue;
                    }
                    auto data = f.find_char(text[i]);
                    if ( data ) {
                        offset = data->offset.cast_to<f32>();
                        prev_char != 0
                            ? kerning = f.get_kerning(prev_char, data->id)
                            : kerning = 0;
                        offset.x += kerning;
                        prev_char = data->id;
                        size_t start_vertices = letters_counter * 4;
                        vertices[start_vertices] = v3f(
                            x_pos + offset.x,
                            y_pos - offset.y - data->rect.size.y,
                            0);
                        vertices[start_vertices + 1] = v3f(
                            x_pos + offset.x,
                            y_pos - offset.y,
                            0);
                        vertices[start_vertices + 2] = v3f(
                            x_pos + data->rect.size.x + offset.x,
                            y_pos - offset.y,
                            0);
                        vertices[start_vertices + 3] = v3f(
                            x_pos + data->rect.size.x + offset.x,
                            y_pos - offset.y - data->rect.size.y,
                            0);

                        uvs[start_vertices] = v2f(
                            data->rect.position.x / texture_size.x,
                            data->rect.position.y / texture_size.y);
                        uvs[start_vertices + 1] = v2f(
                            data->rect.position.x / texture_size.x,
                            (data->rect.position.y + data->rect.size.y) / texture_size.y);
                        uvs[start_vertices + 2] = v2f(
                            (data->rect.position.x + data->rect.size.x) / texture_size.x,
                            (data->rect.position.y + data->rect.size.y) / texture_size.y);
                        uvs[start_vertices + 3] = v2f(
                            (data->rect.position.x + data->rect.size.x) / texture_size.x,
                            data->rect.position.y / texture_size.y);

                        colors[start_vertices]     = l.tint();
                        colors[start_vertices + 1] = l.tint();
                        colors[start_vertices + 2] = l.tint();
                        colors[start_vertices + 3] = l.tint();

                        size_t start_indices = i * 6;
                        indices[start_indices]     = start_vertices;
                        indices[start_indices + 1] = start_vertices + 1;
                        indices[start_indices + 2] = start_vertices + 2;
                        indices[start_indices + 3] = start_vertices + 2;
                        indices[start_indices + 4] = start_vertices + 3;
                        indices[start_indices + 5] = start_vertices;

                        x_pos += data->advance + kerning;
                        letters_counter++;
                    }
                }

                mesh m;
                m.set_vertices(std::move(vertices));
                m.set_indices(0, std::move(indices));
                m.set_uvs(0, std::move(uvs));
                m.set_colors(0, std::move(colors));

                model content;
                content.set_mesh(mesh_asset::create(m));
                content.regenerate_geometry(the<render>());

                if ( !mr.model() ) {
                    mr.model(model_asset::create(content));
                } else {
                    mr.model()->fill(content);
                }

                if ( r.materials().size() > 0 &&
                     r.materials().front()->content().properties().sampler_count() == 0 ) {
                    str page_file = f.find_page(0)->file;
                    auto texture_p = the<library>().load_asset<texture_asset>(page_file);
                    if ( !texture_p ) {
                        return;
                    }

                    vec2 shadow_offset = l.shadow_offset();
                    shadow_offset.x /= texture_p->content()->size().x;
                    shadow_offset.y /= texture_p->content()->size().y;
                    shadow_offset.x = -shadow_offset.x;
                    color oc(l.outline_color());
                    color sc(l.shadow_color());
                    r.properties(render::property_block()
                        .sampler("u_texture", render::sampler_state()
                            .texture(texture_p->content())
                            .min_filter(render::sampler_min_filter::linear)
                            .mag_filter(render::sampler_mag_filter::linear))
                        .property("u_char_width", l.char_width())
                        .property("u_char_edge", l.char_edge())
                        .property("u_outline_width", l.outline_width())
                        .property("u_outline_edge", l.outline_edge())
                        .property("u_outline_color", make_vec4(oc.r, oc.g, oc.b, oc.a))
                        .property("u_shadow_width", l.shadow_width())
                        .property("u_shadow_edge", l.shadow_edge())
                        .property("u_shadow_offset", shadow_offset)
                        .property("u_shadow_color", make_vec4(sc.r, sc.g, sc.b, sc.a)));
                }
            });

            owner.remove_all_components<label::dirty>();
        }
    };

    //
    // label_system
    //

    label_system::label_system()
    : state_(new internal_state()) {}
    label_system::~label_system() noexcept = default;

    void label_system::process(ecs::registry& owner) {
        state_->process(owner);
    }
}
