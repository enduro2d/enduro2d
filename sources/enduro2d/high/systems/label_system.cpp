/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/systems/label_system.hpp>

#include <enduro2d/high/assets/font_asset.hpp>
#include <enduro2d/high/assets/texture_asset.hpp>

#include <enduro2d/high/components/actor.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/label.hpp>
#include <enduro2d/high/components/model_renderer.hpp>
#include <enduro2d/high/components/renderer.hpp>
#include <enduro2d/high/components/widget.hpp>

namespace
{
    using namespace e2d;

    class geometry_builder {
    public:
        geometry_builder() = default;

        void add_quad(
            const b2f& rect,
            const b2f& texrect,
            const color32& color)
        {
            const u32 start_vertex = math::numeric_cast<u32>(vertices_.size());

            // Y
            // ^
            // | 3 - 2
            // | | / |
            // | 0 - 1
            // +------> X

            {
                const v2f& gp = rect.position;
                const v2f& gs = rect.size;

                vertices_.emplace_back(gp.x + 0.0f, gp.y + 0.0f, 0.f);
                vertices_.emplace_back(gp.x + gs.x, gp.y + 0.0f, 0.f);
                vertices_.emplace_back(gp.x + gs.x, gp.y + gs.y, 0.f);
                vertices_.emplace_back(gp.x + 0.0f, gp.y + gs.y, 0.f);
            }

            {
                const v2f& tp = texrect.position;
                const v2f& ts = texrect.size;

                uvs_.emplace_back(tp.x + 0.0f, tp.y + 0.0f);
                uvs_.emplace_back(tp.x + ts.x, tp.y + 0.0f);
                uvs_.emplace_back(tp.x + ts.x, tp.y + ts.y);
                uvs_.emplace_back(tp.x + 0.0f, tp.y + ts.y);
            }

            {
                indices_.emplace_back(start_vertex + 0u);
                indices_.emplace_back(start_vertex + 1u);
                indices_.emplace_back(start_vertex + 2u);
                indices_.emplace_back(start_vertex + 2u);
                indices_.emplace_back(start_vertex + 3u);
                indices_.emplace_back(start_vertex + 0u);
            }

            {
                colors_.emplace_back(color);
                colors_.emplace_back(color);
                colors_.emplace_back(color);
                colors_.emplace_back(color);
            }
        }

        void update_model(model_renderer& mr) {
            mesh new_mesh;
            new_mesh.set_vertices(vertices_);
            new_mesh.set_indices(0, indices_);
            new_mesh.set_uvs(0, uvs_);
            new_mesh.set_colors(0, colors_);

            model new_model;
            new_model.set_mesh(mesh_asset::create(std::move(new_mesh)));
            new_model.regenerate_geometry(the<render>());

            if ( mr.model() ) {
                mr.model()->fill(std::move(new_model));
            } else {
                mr.model(model_asset::create(std::move(new_model)));
            }
        }

        void clear() noexcept {
            vertices_.clear();
            uvs_.clear();
            indices_.clear();
            colors_.clear();
        }
    private:
        vector<v3f> vertices_;
        vector<v2f> uvs_;
        vector<u32> indices_;
        vector<color32> colors_;
    };
}

namespace
{
    using namespace e2d;

    f32 calculate_halign_offset(
        f32 widget_width,
        label::haligns halign,
        f32 string_width) noexcept
    {
        switch ( halign ) {
        case label::haligns::left:
            return 0.0f;
        case label::haligns::center:
            return 0.5f * widget_width - 0.5f * string_width;
        case label::haligns::right:
            return 1.0f * widget_width - 1.0f * string_width;
        default:
            E2D_ASSERT_MSG(false, "unexpected label halign");
            return 0.f;
        }
    }

    f32 calculate_valign_offset(
        f32 widget_height,
        label::valigns valign,
        f32 leading,
        u32 line_height,
        std::size_t string_count) noexcept
    {
        const f32 label_height = string_count > 1u
            ? line_height + line_height * leading * (string_count - 1u)
            : line_height;

        switch ( valign ) {
        case label::valigns::top:
            return widget_height;
        case label::valigns::center:
            return 0.5f * widget_height + 0.5f * label_height;
        case label::valigns::bottom:
            return 1.0f * label_height;
        default:
            E2D_ASSERT_MSG(false, "unexpected label valign");
            return 0.f;
        }
    }

    std::size_t calculate_new_lines(str32_view text) noexcept {
        std::size_t count{0u};
        for ( std::size_t i = 0; i < text.size(); ++i ) {
            if ( text[i] == '\n' ) {
                ++count;
            }
        }
        return count;
    }
}

namespace
{
    using namespace e2d;

    void update_label_material(const label& l, renderer& r) {
        auto texture_res = l.font() && !l.font()->content().empty()
            ? l.font()->find_nested_asset<texture_asset>(l.font()->content().info().atlas_file)
            : nullptr;

        auto texture = texture_res
            ? texture_res->content()
            : nullptr;

        const f32 glyph_dilate = math::clamp(l.glyph_dilate(), -1.f, 1.0f);
        const f32 outline_width = math::clamp(l.outline_width(), 0.f, 1.f - glyph_dilate);
        const v4f outline_color = make_vec4(color(l.outline_color()));

        r.properties(render::property_block()
            .sampler("u_texture", render::sampler_state()
                .texture(texture)
                .min_filter(render::sampler_min_filter::linear)
                .mag_filter(render::sampler_mag_filter::linear))
            .property("u_glyph_dilate", glyph_dilate)
            .property("u_outline_width", outline_width)
            .property("u_outline_color", outline_color));
    }

    void update_label_geometry(
        const label& l,
        const widget* w,
        model_renderer& mr)
    {
        static thread_local geometry_builder gb;
        DEFER_HPP([](){ gb.clear(); });

        if ( !l.font() || l.font()->content().empty() || l.text().empty() ) {
            gb.update_model(mr);
            return;
        }

        const font& f = l.font()->content();
        const str32& text = make_utf32(l.text());

        const f32 widget_width = w ? w->size().x : 0.f;
        const f32 widget_height = w ? w->size().y : 0.f;

        //
        // update glyphs
        //

        struct glyph_desc {
            const font::glyph_info* glyph{nullptr};
            f32 kerning{0.f};
        };

        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<glyph_desc> glyphs;
        DEFER_HPP([](){ glyphs.clear(); });

        if ( glyphs.capacity() < text.size() ) {
            glyphs.reserve(math::max(glyphs.capacity() * 2u, text.size()));
        }

        for ( std::size_t i = 0, e = text.size(); i < e; ++i ) {
            glyph_desc desc;
            desc.glyph = f.find_glyph(text[i]);
            desc.kerning = i > 0
                ? f.get_kerning(text[i-1], text[i])
                : 0.f;
            glyphs.push_back(std::move(desc));

            if ( !desc.glyph && text[i] != '\n' ) {
                the<debug>().warning("LABEL: Missing font glyph:\n"
                    "--> Code Point: %0",
                    text[i]);
            }
        }

        f32 tracking_width = 0.f;
        if ( const font::glyph_info* sp = f.find_glyph(' '); sp ) {
            tracking_width = sp->advance * l.tracking();
        }

        //
        // update strings
        //

        struct string_desc {
            f32 width{0.f};
            std::size_t start{0};
            std::size_t length{0};

            string_desc(std::size_t start)
            : start(start) {}
        };

        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<string_desc> strings;
        DEFER_HPP([](){ strings.clear(); });

        if ( const std::size_t count = calculate_new_lines(text);
            strings.capacity() < count + 1u )
        {
            strings.reserve(math::max(strings.capacity() * 2u, count));
        }

        f32 last_char_width = 0.f;
        std::size_t last_char_index = std::size_t(-1);

        f32 last_space_width = 0.f;
        std::size_t last_space_index = std::size_t(-1);

        strings.push_back(string_desc(0u));

        for ( std::size_t i = 0, e = text.size(); i < e; ++i ) {
            const u32 code_point = text[i];
            const glyph_desc& glyph = glyphs[i];

            last_char_width = strings.back().width;
            last_char_index = i;

            if ( code_point == ' ' ) {
                last_space_width = strings.back().width;
                last_space_index = i;
            }

            bool new_line = false;

            if ( code_point == '\n' ) {
                new_line = true;
                strings.back().length = i - strings.back().start;
            } else if ( glyph.glyph ) {
                strings.back().width +=
                    glyph.kerning +
                    glyph.glyph->advance +
                    tracking_width;

                switch ( l.wrap() ) {
                case label::wraps::no_wrap:
                    break;
                case label::wraps::wrap_by_chars:
                case label::wraps::wrap_by_spaces: {
                    const bool need_break_line =
                        widget_width > 0.f &&
                        strings.back().width > widget_width;
                    if ( need_break_line ) {
                        if ( last_space_index != std::size_t(-1) ) {
                            new_line = true;
                            strings.back().width = last_space_width;
                            strings.back().length = last_space_index - strings.back().start;
                            i = last_space_index;
                        } else if ( l.wrap() == label::wraps::wrap_by_chars && i - strings.back().start > 0u ) {
                            new_line = true;
                            strings.back().width = last_char_width;
                            strings.back().length = last_char_index - strings.back().start;
                            i = last_char_index - 1u;
                        }
                    }
                }
                break;
                default:
                    E2D_ASSERT_MSG(false, "unexpected label wrap");
                    break;
                }
            }

            if ( i == e - 1 ) {
                new_line = true;
                strings.back().length = i + 1 - strings.back().start;
            }

            if ( new_line ) {
                if ( i < e - 1 ) {
                    strings.push_back(string_desc(i + 1u));
                }

                last_char_width = 0.f;
                last_char_index = std::size_t(-1);

                last_space_width = 0.f;
                last_space_index = std::size_t(-1);
            }
        }

        //
        // update geometry
        //

        v2f cursor = v2f::unit_y() * calculate_valign_offset(
            widget_height,
            l.valign(),
            l.leading(),
            f.info().line_height,
            strings.size());

        for ( std::size_t i = 0, ie = strings.size(); i < ie; ++i ) {
            cursor.x = calculate_halign_offset(
                widget_width,
                l.halign(),
                strings[i].width);

            for ( std::size_t j = strings[i].start, je = strings[i].start + strings[i].length; j < je; ++j ) {
                const glyph_desc& glyph = glyphs[j];
                if ( !glyph.glyph ) {
                    continue;
                }

                cursor.x += glyph.kerning;

                b2f rect = make_rect(cursor, glyph.glyph->tex_rect.size.cast_to<f32>());
                rect.position += glyph.glyph->offset.cast_to<f32>();

                b2f texrect = glyph.glyph->tex_rect.cast_to<f32>();
                texrect.position /= f.info().atlas_size.cast_to<f32>();
                texrect.size /= f.info().atlas_size.cast_to<f32>();

                gb.add_quad(rect, texrect, l.tint());
                cursor.x += glyph.glyph->advance + tracking_width;
            }
            cursor.y -= f.info().line_height * l.leading();
        }

        //
        // update model
        //

        gb.update_model(mr);
    }

    void update_dirty_labels(ecs::registry& owner) {
        owner.for_joined_components<widget::was_dirty, widget, actor>([](
            const ecs::const_entity&,
            const widget::was_dirty&,
            const widget&,
            const actor& a)
        {
            if ( a.node() && a.node()->owner() ) {
                gobject go = a.node()->owner();
                labels::mark_dirty(go);
            }
        }, !ecs::exists_any<
            disabled<actor>,
            disabled<widget>>());

        owner.for_joined_components<label::dirty, label, renderer, model_renderer>([](
            const ecs::const_entity& e,
            const label::dirty&,
            const label& l,
            renderer& r,
            model_renderer& mr
        ){
            update_label_material(l, r);
            update_label_geometry(l, e.find_component<widget>(), mr);
        });
    }

    void update_was_dirty_flags(ecs::registry& owner) {
        RETURN_DEFER_HPP([&owner](){
            owner.remove_all_components<label::dirty>();
        });

        owner.remove_all_components<label::was_dirty>();

        owner.for_joined_components<label::dirty>([
        ](ecs::entity e, const label::dirty&) {
            e.ensure_component<label::was_dirty>();
        });
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

        void process_update(ecs::registry& owner) {
            update_dirty_labels(owner);
            update_was_dirty_flags(owner);
        }
    };

    //
    // label_system
    //

    label_system::label_system()
    : state_(new internal_state()) {}
    label_system::~label_system() noexcept = default;

    void label_system::process(
        ecs::registry& owner,
        const ecs::after<systems::update_event>& trigger)
    {
        E2D_UNUSED(trigger);
        state_->process_update(owner);
    }
}
