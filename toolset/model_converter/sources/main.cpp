/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <cassert>
#include <cstring>
#include <cstdint>

#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <algorithm>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace
{
    const std::uint32_t mesh_file_version = 1;
    const std::string mesh_file_signature = "e2d_mesh";

    struct opts {
        bool timers = false;
        bool verbose = false;

        opts(int argc, char *argv[]) {
            timers = has_flag("-t", argc, argv) || has_flag("--timers", argc, argv);
            verbose = has_flag("-v", argc, argv) || has_flag("--verbose", argc, argv);
        }

    private:
        static bool has_flag(const char* flag, int argc, char *argv[]) noexcept {
            for ( int i = 0; i < argc; ++i ) {
                if ( 0 == std::strcmp(argv[i], flag) ) {
                    return true;
                }
            }
            return false;
        }
    };

    class timer {
    public:
        timer()
        : tp_(std::chrono::high_resolution_clock::now()) {}

        void done() const {
            const auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - tp_);
            std::cout << duration_us.count() << "us" << std::endl;
        }
    private:
        std::chrono::high_resolution_clock::time_point tp_;
    };

    struct v2f {
        float x = 0.f;
        float y = 0.f;

        v2f(float nx, float ny)
        : x(nx), y(ny) {}
    };

    struct v3f {
        float x = 0.f;
        float y = 0.f;
        float z = 0.f;

        v3f(float nx, float ny, float nz)
        : x(nx), y(ny), z(nz) {}
    };

    struct mesh {
        std::vector<v3f> vertices;
        std::vector<std::uint32_t> indices;

        std::vector<std::vector<v2f>> uvs_channels;
        std::vector<std::vector<std::uint32_t>> colors_channels;

        std::vector<v3f> normals;
        std::vector<v3f> tangents;
        std::vector<v3f> bitangents;
    };

    template < typename T >
    T saturate(T v) noexcept {
        return std::min(std::max(v, T(0)), T(1));
    }

    std::uint8_t pack_color_component(float c) noexcept {
        return static_cast<std::uint8_t>(std::round(saturate(c) * 255.f));
    }

    std::uint32_t pack_color(float r, float g, float b, float a) noexcept {
        std::uint8_t rr = pack_color_component(r);
        std::uint8_t gg = pack_color_component(g);
        std::uint8_t bb = pack_color_component(b);
        std::uint8_t aa = pack_color_component(a);
        return
            static_cast<std::uint32_t>(aa) << 24 |
            static_cast<std::uint32_t>(rr) << 16 |
            static_cast<std::uint32_t>(gg) <<  8 |
            static_cast<std::uint32_t>(bb) <<  0;
    }

    void write_u32_to_ofstream(std::ofstream& s, const std::uint32_t v) {
        s.write(
            reinterpret_cast<const char*>(&v),
            sizeof(v));
    }

    void write_str_to_ofstream(std::ofstream& s, const std::string& v) {
        s.write(v.data(), static_cast<std::ptrdiff_t>(v.length()));
    }

    template < typename T >
    std::size_t write_vector_to_ofstream(std::ofstream& s, const std::vector<T>& v) {
        if ( !v.empty() ) {
            std::size_t data_size = v.size() * sizeof(T);
            s.write(
                reinterpret_cast<const char*>(v.data()),
                static_cast<std::ptrdiff_t>(data_size));
            return data_size;
        }
        return 0;
    }

    bool validate_mesh(const mesh& mesh) noexcept {
        if ( mesh.vertices.empty() ) {
            return false;
        }
        if ( mesh.indices.empty() ) {
            return false;
        }
        for ( const auto& uvs : mesh.uvs_channels ) {
            if ( uvs.size() != mesh.vertices.size() ) {
                return false;
            }
        }
        for ( const auto& colors : mesh.colors_channels ) {
            if ( colors.size() != mesh.vertices.size() ) {
                return false;
            }
        }
        if ( !mesh.normals.empty() && mesh.normals.size() != mesh.vertices.size() ) {
            return false;
        }
        if ( !mesh.tangents.empty() && mesh.tangents.size() != mesh.vertices.size() ) {
            return false;
        }
        if ( !mesh.bitangents.empty() && mesh.bitangents.size() != mesh.vertices.size() ) {
            return false;
        }
        return true;
    }

    bool save_mesh(const mesh& mesh, const std::string& out_path, const opts& opts) {
        timer save_timer;

        if ( !validate_mesh(mesh) ) {
            std::cerr << "Failed to validate out mesh: " << out_path << std::endl;
            return false;
        }

        std::ofstream stream(out_path, std::ofstream::out | std::ofstream::binary);
        if ( !stream.is_open() ) {
            std::cerr << "Failed to open out file stream: " << out_path << std::endl;
            return false;
        }

        write_str_to_ofstream(stream, mesh_file_signature);
        write_u32_to_ofstream(stream, mesh_file_version);

        write_u32_to_ofstream(stream, static_cast<std::uint32_t>(mesh.vertices.size()));
        write_u32_to_ofstream(stream, static_cast<std::uint32_t>(mesh.indices.size()));

        write_u32_to_ofstream(stream, static_cast<std::uint32_t>(mesh.uvs_channels.size()));
        write_u32_to_ofstream(stream, static_cast<std::uint32_t>(mesh.colors_channels.size()));

        write_u32_to_ofstream(stream, static_cast<std::uint32_t>(mesh.normals.size()));
        write_u32_to_ofstream(stream, static_cast<std::uint32_t>(mesh.tangents.size()));
        write_u32_to_ofstream(stream, static_cast<std::uint32_t>(mesh.bitangents.size()));

        std::size_t vertices_bytes = write_vector_to_ofstream(stream, mesh.vertices);
        std::size_t indices_bytes = write_vector_to_ofstream(stream, mesh.indices);

        std::size_t uvs_bytes = 0;
        for ( const auto& uvs : mesh.uvs_channels ) {
            uvs_bytes += write_vector_to_ofstream(stream, uvs);
        }

        std::size_t colors_bytes = 0;
        for ( const auto& colors : mesh.colors_channels ) {
            colors_bytes += write_vector_to_ofstream(stream, colors);
        }

        std::size_t normals_bytes = write_vector_to_ofstream(stream, mesh.normals);
        std::size_t tangents_bytes = write_vector_to_ofstream(stream, mesh.tangents);
        std::size_t bitangents_bytes = write_vector_to_ofstream(stream, mesh.bitangents);

        if ( opts.timers ) {
            std::cout << "> save mesh: ";
            save_timer.done();
            std::cout << " - " << out_path << std::endl;
        }

        if ( opts.verbose ) {
            std::cout
                << std::endl
                << "> mesh info:" << std::endl
                << "-> vertices: " << mesh.vertices.size() << ", " << vertices_bytes << " B" << std::endl
                << "-> indices: " << mesh.indices.size() << ", " << indices_bytes << " B" << std::endl
                << "-> uvs: " << mesh.uvs_channels.size() << ", " << uvs_bytes << " B" << std::endl
                << "-> colors: " << mesh.colors_channels.size() << ", " << colors_bytes << " B" << std::endl
                << "-> normals: " << mesh.normals.size() << ", " << normals_bytes << " B" << std::endl
                << "-> tangents: " << mesh.tangents.size() << ", " << tangents_bytes << " B" << std::endl
                << "-> bitangents: " << mesh.bitangents.size() << ", " << bitangents_bytes << " B" << std::endl;
        }

        return true;
    }

    bool convert_mesh(const aiMesh* ai_mesh, const std::string& out_path, const opts& opts) {
        mesh out_mesh;
        timer convert_timer;

        if ( ai_mesh->HasPositions() ) {
            out_mesh.vertices.reserve(ai_mesh->mNumVertices);
            std::transform(
                ai_mesh->mVertices,
                ai_mesh->mVertices + ai_mesh->mNumVertices,
                std::back_inserter(out_mesh.vertices),
                [](const aiVector3D& v) noexcept {
                    return v3f{v.x, v.y, v.z};
                });
        }

        if ( ai_mesh->HasFaces() ) {
            out_mesh.indices.reserve(ai_mesh->mNumFaces * 3u);
            std::for_each(
                ai_mesh->mFaces,
                ai_mesh->mFaces + ai_mesh->mNumFaces,
                [&out_mesh](const aiFace& f) {
                    if ( f.mNumIndices != 3 ) {
                        throw std::logic_error("invalide face index count");
                    }
                    out_mesh.indices.insert(
                        out_mesh.indices.end(),
                        f.mIndices,
                        f.mIndices + f.mNumIndices);
                });
        }

        for ( unsigned int channel = 0; channel < ai_mesh->GetNumUVChannels(); ++channel ) {
            std::vector<v2f> uvs;
            uvs.reserve(ai_mesh->mNumVertices);
            std::transform(
                ai_mesh->mTextureCoords[channel],
                ai_mesh->mTextureCoords[channel] + ai_mesh->mNumVertices,
                std::back_inserter(uvs),
                [](const aiVector3D& v) noexcept {
                    return v2f{v.x, v.y};
                });
            out_mesh.uvs_channels.emplace_back(std::move(uvs));
        }

        for ( unsigned int channel = 0; channel < ai_mesh->GetNumColorChannels(); ++channel ) {
            std::vector<std::uint32_t> colors;
            colors.reserve(ai_mesh->mNumVertices);
            std::transform(
                ai_mesh->mColors[channel],
                ai_mesh->mColors[channel] + ai_mesh->mNumVertices,
                std::back_inserter(colors),
                [](const aiColor4D& v) noexcept {
                    return pack_color(v.r, v.g, v.b, v.a);
                });
            out_mesh.colors_channels.emplace_back(std::move(colors));
        }

        if ( ai_mesh->HasNormals() ) {
            out_mesh.normals.reserve(ai_mesh->mNumVertices);
            std::transform(
                ai_mesh->mNormals,
                ai_mesh->mNormals + ai_mesh->mNumVertices,
                std::back_inserter(out_mesh.normals),
                [](const aiVector3D& v) noexcept {
                    return v3f{v.x, v.y, v.z};
                });
        }

        if ( ai_mesh->HasTangentsAndBitangents() ) {
            out_mesh.tangents.reserve(ai_mesh->mNumVertices);
            std::transform(
                ai_mesh->mTangents,
                ai_mesh->mTangents + ai_mesh->mNumVertices,
                std::back_inserter(out_mesh.tangents),
                [](const aiVector3D& v) noexcept {
                    return v3f{v.x, v.y, v.z};
                });

            out_mesh.bitangents.reserve(ai_mesh->mNumVertices);
            std::transform(
                ai_mesh->mBitangents,
                ai_mesh->mBitangents + ai_mesh->mNumVertices,
                std::back_inserter(out_mesh.bitangents),
                [](const aiVector3D& v) noexcept {
                    return v3f{v.x, v.y, v.z};
                });
        }

        if ( opts.timers ) {
            std::cout << std::endl << "> convert mesh: ";
            convert_timer.done();
            std::cout << " - " << out_path << std::endl;
        }

        return save_mesh(out_mesh, out_path, opts);
    }

    bool convert(const std::string& path, const opts& opts) {
        timer convert_timer;
        timer importer_timer;

        Assimp::Importer importer;

        if ( opts.timers ) {
            std::cout << "> prepare importer: ";
            importer_timer.done();
        }

        const unsigned int importer_flags =
            aiProcess_Triangulate |
            aiProcess_MakeLeftHanded |
            aiProcess_OptimizeMeshes |
            aiProcess_JoinIdenticalVertices;

        timer import_timer;

        const aiScene* scene = importer.ReadFile(path, importer_flags);
        if ( !scene ) {
            std::cerr << "Failed to import model: " << path << std::endl;
            std::cerr << "Error: " << importer.GetErrorString() << std::endl;
            return false;
        }

        if ( opts.timers ) {
            std::cout << "> import model: ";
            import_timer.done();
        }

        for ( unsigned int mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index ) {
            const aiMesh* mesh = scene->mMeshes[mesh_index];

            const std::string mesh_name = mesh->mName.length
                ? mesh->mName.C_Str()
                : "mesh_" + std::to_string(mesh_index);

            std::string mesh_out_path = std::string()
                .append(path)
                .append(".")
                .append(mesh_name)
                .append(".e2d_mesh");

            if ( opts.verbose ) {
                std::cout
                    << std::endl
                    << ">> Mesh("
                    << mesh_name
                    << ") converting..."
                    << std::endl;
            }

            if ( !convert_mesh(mesh, mesh_out_path, opts) ) {
                std::cerr << "Failed!" << std::endl;
                return false;
            }

            if ( opts.verbose ) {
                std::cout << "OK. " << std::endl;
            }
        }

        if ( opts.timers ) {
            std::cout << std::endl << "=====" << std::endl;
            convert_timer.done();
        }

        return true;
    }
}

int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        std::cout << "USAGE: model_converter mesh.obj" << std::endl;
        return 0;
    }
    return convert(argv[1], opts(argc, argv)) ? 0 : 1;
}
