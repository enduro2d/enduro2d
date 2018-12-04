/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("mesh") {
    str resources;
    REQUIRE(filesystem::extract_predef_path(
        resources,
        filesystem::predef_path::resources));
    {
        mesh m;
        REQUIRE(meshes::try_load_mesh(
            m,
            make_read_file(path::combine(resources, "bin/gnome/gnome.obj.gnome.e2d_mesh"))));

        REQUIRE(m.vertices().size() == 397);

        REQUIRE(m.indices_submesh_count() == 1);
        REQUIRE(m.indices(0).size() == 2028);

        REQUIRE(m.uvs_channel_count() == 1);
        REQUIRE(m.uvs(0).size() == 397);

        REQUIRE(m.colors_channel_count() == 0);
        REQUIRE(m.normals().size() == 397);
        REQUIRE(m.tangents().empty());
        REQUIRE(m.bitangents().empty());
    }
    {
        mesh m;
        REQUIRE(meshes::try_load_mesh(
            m,
            make_read_file(path::combine(resources, "bin/gnome/gnome.obj.yad.e2d_mesh"))));

        REQUIRE(m.vertices().size() == 46);

        REQUIRE(m.indices_submesh_count() == 1);
        REQUIRE(m.indices(0).size() == 264);

        REQUIRE(m.uvs_channel_count() == 1);
        REQUIRE(m.uvs(0).size() == 46);

        REQUIRE(m.colors_channel_count() == 0);
        REQUIRE(m.normals().size() == 46);
        REQUIRE(m.tangents().empty());
        REQUIRE(m.bitangents().empty());
    }
}
