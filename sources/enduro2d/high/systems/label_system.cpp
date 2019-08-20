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
        f32 label_height = f.info().line_height * strings_count;
        f32 top{0};
        switch ( l.valign() ) {
            case label::valigns::top:
                top = label_height;
            break;
            case label::valigns::center:
                top = 0.5f * label_height;
            break;
            case label::valigns::bottom:
                top = 0;
            break;
            case label::valigns::baseline:
                top = label_height - (f.info().line_height - f.info().glyph_ascent);
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
                v2f texture_size = f.info().atlas_size.cast_to<f32>();

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
                            const font::glyph_info* glyph = f.find_glyph(ch);
                            if ( glyph ) {
                                prev_char != 0
                                    ? kerning = f.get_kerning(prev_char, ch)
                                    : kerning = 0;
                                prev_char = ch;
                                f32 char_width = glyph->advance + kerning;
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
                        const font::glyph_info* glyph = f.find_glyph(text[i]);
                        if ( glyph ) {
                            letters_size++;
                            prev_char != 0
                                ? kerning = f.get_kerning(prev_char, text[i])
                                : kerning = 0;
                            strings_width[string_ind] += glyph->advance + kerning;
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
                        y_pos -= f.info().line_height;
                        x_pos = get_x_pos(l, strings_width[str_ind]);
                        prev_char = 0;
                        continue;
                    }
                    const font::glyph_info* glyph = f.find_glyph(text[i]);
                    if ( glyph ) {
                        offset = glyph->offset.cast_to<f32>();
                        prev_char != 0
                            ? kerning = f.get_kerning(prev_char, text[i])
                            : kerning = 0;
                        offset.x += kerning;
                        prev_char = text[i];
                        size_t start_vertices = letters_counter * 4;
                        vertices[start_vertices] = v3f(
                            x_pos + offset.x,
                            y_pos - offset.y - glyph->tex_rect.size.y,
                            0);
                        vertices[start_vertices + 1] = v3f(
                            x_pos + offset.x,
                            y_pos - offset.y,
                            0);
                        vertices[start_vertices + 2] = v3f(
                            x_pos + glyph->tex_rect.size.x + offset.x,
                            y_pos - offset.y,
                            0);
                        vertices[start_vertices + 3] = v3f(
                            x_pos + glyph->tex_rect.size.x + offset.x,
                            y_pos - offset.y - glyph->tex_rect.size.y,
                            0);

                        uvs[start_vertices] = v2f(
                            glyph->tex_rect.position.x / texture_size.x,
                            glyph->tex_rect.position.y / texture_size.y);
                        uvs[start_vertices + 1] = v2f(
                            glyph->tex_rect.position.x / texture_size.x,
                            (glyph->tex_rect.position.y + glyph->tex_rect.size.y) / texture_size.y);
                        uvs[start_vertices + 2] = v2f(
                            (glyph->tex_rect.position.x + glyph->tex_rect.size.x) / texture_size.x,
                            (glyph->tex_rect.position.y + glyph->tex_rect.size.y) / texture_size.y);
                        uvs[start_vertices + 3] = v2f(
                            (glyph->tex_rect.position.x + glyph->tex_rect.size.x) / texture_size.x,
                            glyph->tex_rect.position.y / texture_size.y);

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

                        x_pos += glyph->advance + kerning;
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
                    auto texture_p = the<library>().load_asset<texture_asset>(f.info().atlas_file);
                    if ( !texture_p ) {
                        return;
                    }

                    const f32 glyph_dilate = math::clamp(l.glyph_dilate(), -1.f, 1.0f);
                    const f32 outline_width = math::clamp(l.outline_width(), 0.f, 1.f - glyph_dilate);
                    const v4f outline_color = make_vec4(color(l.outline_color()));

                    r.properties(render::property_block()
                        .sampler("u_texture", render::sampler_state()
                            .texture(texture_p->content())
                            .min_filter(render::sampler_min_filter::linear)
                            .mag_filter(render::sampler_mag_filter::linear))
                        .property("u_glyph_dilate", glyph_dilate)
                        .property("u_outline_width", outline_width)
                        .property("u_outline_color", outline_color));
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
