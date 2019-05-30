/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "shape_impl.hpp"

namespace
{
    using namespace e2d;

    const u32 mesh_file_version = 1u;
    const str_view shape_file_signature = "e2d_shape";

    template < typename T >
    input_sequence& iseq_read_vector_pods(input_sequence& iseq, vector<T>& v) {
        return iseq.read(v.data(), v.size() * sizeof(T));
    }

    bool check_signature(const input_stream_uptr& stream) {
        input_sequence iseq{*stream};

        u32 file_version = 0;
        char* file_signature = static_cast<char*>(E2D_CLEAR_ALLOCA(
            shape_file_signature.length() + 1));

        iseq.read(file_signature, shape_file_signature.length())
            .read(file_version);

        return iseq.success()
            && shape_file_signature == file_signature
            && mesh_file_version == file_version;
    }

    bool load_shape(shape& dst, const input_stream_uptr& stream) {
        input_sequence iseq{*stream};

        u32 vertices = 0;
        u32 indices = 0;
        u32 uvs_channels = 0;
        u32 colors_channels = 0;

        iseq.read(vertices)
            .read(indices)
            .read(uvs_channels)
            .read(colors_channels);

        if ( !iseq.success() ) {
            return false;
        }

        vector<v2f> vertices_data(vertices);
        vector<u32> indices_data(indices);

        vector<vector<v2f>> uvs_channels_data(
            uvs_channels,
            vector<v2f>(vertices));

        vector<vector<color32>> colors_channels_data(
            colors_channels,
            vector<color32>(vertices));

        iseq_read_vector_pods(iseq, vertices_data);
        iseq_read_vector_pods(iseq, indices_data);

        for ( auto& uvs : uvs_channels_data ) {
            iseq_read_vector_pods(iseq, uvs);
        }

        for ( auto& colors : colors_channels_data ) {
            iseq_read_vector_pods(iseq, colors);
        }

        if ( !iseq.success() || stream->tell() != stream->length() ) {
            return false;
        }

        shape m;

        m.set_vertices(std::move(vertices_data));
        m.set_indices(0, std::move(indices_data));

        for ( std::size_t i = 0; i < uvs_channels_data.size(); ++i ) {
            m.set_uvs(i, std::move(uvs_channels_data[i]));
        }

        for ( std::size_t i = 0; i < colors_channels_data.size(); ++i ) {
            m.set_colors(i, std::move(colors_channels_data[i]));
        }

        dst = std::move(m);
        return true;
    }
}

namespace e2d::shapes::impl
{
    bool try_load_shape_e2d(shape& dst, const buffer& src) noexcept {
        try {
            auto stream = make_memory_stream(src);
            return stream
                && check_signature(stream)
                && load_shape(dst, stream);
        } catch (...) {
            return false;
        }
    }
}
