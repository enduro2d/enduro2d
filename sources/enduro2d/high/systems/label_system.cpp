/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/label_system.hpp>


#include <enduro2d/high/components/label.hpp>
#include <enduro2d/high/components/model_renderer.hpp>


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
            owner.for_joined_components<label, label::label_dirty, model_renderer>([](
                const ecs::const_entity&,
                label& l,
                label::label_dirty&,
                model_renderer& mr)
            {
                vector<v3f> vertices;
                vector<u32> indices;
                vector<v2f> uvs;
                vector<v3f> normals;
                vector<color32> colors;
                f32 xoffset{0};
                f32 yoffset{0};
                str32 text = l.text();
                const auto& f = l.font()->content();
                auto common = f.common();
                v2f texture_size;
                texture_size.x = common.atlas_width;
                texture_size.y = common.atlas_height;

                size_t letters_size = text.size();
                for ( size_t i = 0; i < text.size(); i++ ) {
                    if ( text[i] == '\n' ) {
                        letters_size--;
                    }
                }

                vertices.resize(letters_size * 4);
                uvs.resize(vertices.size());
                colors.resize(vertices.size());
                indices.resize(text.size() * 6);
                f32 x_pos{-l.pivot().x};
                f32 y_pos{-l.pivot().y};
                f32 kerning{0};
                u32 prev_char{0};
                u32 letters_counter{0};
                for ( size_t i = 0; i < text.size(); i++ ) {
                    if ( text[i] == '\n' ) {
                        y_pos -= f.common().line_height;
                        x_pos = -l.pivot().x;
                        prev_char = 0;
                        continue;
                    }
                    auto data = f.find_char(text[i]);
                    if ( data ) {
                        yoffset = data->yoffset;
                        xoffset = data->xoffset;
                        if ( prev_char != 0 ) {
                            kerning = f.find_kerning(prev_char, data->id);
                        } else {
                            kerning = 0;
                        }
                        xoffset += kerning;
                        prev_char = data->id;
                        size_t start_vertices = letters_counter * 4;
                        vertices[start_vertices] = v3f(
                            x_pos + xoffset,
                            y_pos + yoffset,
                            0);
                        vertices[start_vertices + 1] = v3f(
                            x_pos + xoffset,
                            y_pos + data->rect.size.y + yoffset,
                            0);
                        vertices[start_vertices + 2] = v3f(
                            x_pos + data->rect.size.x + xoffset,
                            y_pos + data->rect.size.y + yoffset,
                            0);
                        vertices[start_vertices + 3] = v3f(
                            x_pos + data->rect.size.x + xoffset,
                            y_pos + yoffset,
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

                        x_pos += data->xadvance + kerning;
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
                mr.model()->fill(content);
            });

            owner.remove_all_components<label::label_dirty>();
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
