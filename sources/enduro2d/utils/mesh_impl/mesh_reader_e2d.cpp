/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "mesh_impl.hpp"

namespace
{
    using namespace e2d;

    const u32 mesh_file_version = 1u;
    const str_view mesh_file_signature = "e2d_mesh";

    template < typename T >
    input_sequence& iseq_read_vector_pods(input_sequence& iseq, vector<T>& v) {
        return iseq.read(v.data(), v.size() * sizeof(T));
    }

    bool check_signature(const input_stream_uptr& stream) {
        input_sequence iseq{*stream};

        u32 file_version = 0;
        char* file_signature = static_cast<char*>(E2D_CLEAR_ALLOCA(
            mesh_file_signature.size() + 1));

        iseq.read(file_signature, mesh_file_signature.size())
            .read(file_version);

        return iseq.success()
            && mesh_file_signature == file_signature
            && mesh_file_version == file_version;
    }

    bool load_mesh(mesh& dst, const input_stream_uptr& stream) {
        input_sequence iseq{*stream};

        u32 vertices = 0;
        u32 indices = 0;
        u32 uvs_channels = 0;
        u32 colors_channels = 0;
        u32 normals = 0;
        u32 tangents = 0;
        u32 bitangents = 0;

        iseq.read(vertices)
            .read(indices)
            .read(uvs_channels)
            .read(colors_channels)
            .read(normals)
            .read(tangents)
            .read(bitangents);

        if ( !iseq.success() ) {
            return false;
        }

        vector<v3f> vertices_data(vertices);
        vector<u32> indices_data(indices);

        vector<vector<v2f>> uvs_channels_data(
            uvs_channels,
            vector<v2f>(vertices));

        vector<vector<color32>> colors_channels_data(
            colors_channels,
            vector<color32>(vertices));

        vector<v3f> normals_data(normals);
        vector<v3f> tangents_data(tangents);
        vector<v3f> bitangents_data(bitangents);

        iseq_read_vector_pods(iseq, vertices_data);
        iseq_read_vector_pods(iseq, indices_data);

        for ( auto& uvs : uvs_channels_data ) {
            iseq_read_vector_pods(iseq, uvs);
        }

        for ( auto& colors : colors_channels_data ) {
            iseq_read_vector_pods(iseq, colors);
        }

        iseq_read_vector_pods(iseq, normals_data);
        iseq_read_vector_pods(iseq, tangents_data);
        iseq_read_vector_pods(iseq, bitangents_data);

        if ( !iseq.success() || stream->tell() != stream->length() ) {
            return false;
        }

        mesh m;

        m.set_vertices(std::move(vertices_data));
        m.set_indices(0, std::move(indices_data));

        for ( std::size_t i = 0; i < uvs_channels_data.size(); ++i ) {
            m.set_uvs(i, std::move(uvs_channels_data[i]));
        }

        for ( std::size_t i = 0; i < colors_channels_data.size(); ++i ) {
            m.set_colors(i, std::move(colors_channels_data[i]));
        }

        m.set_normals(std::move(normals_data));
        m.set_tangents(std::move(tangents_data));
        m.set_bitangents(std::move(bitangents_data));

        dst = std::move(m);
        return true;
    }
}

namespace e2d::meshes::impl
{
    bool load_mesh_e2d(mesh& dst, buffer_view src) {
        auto stream = make_memory_stream(buffer(src));
        return stream
            && check_signature(stream)
            && load_mesh(dst, stream);
    }
}
