/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#include "_utils.hpp"
using namespace e2d;

TEST_CASE("shape") {
    str resources;
    REQUIRE(filesystem::extract_predef_path(
        resources,
        filesystem::predef_path::resources));
    {
        shape s;
        REQUIRE(shapes::try_load_shape(
            s,
            make_read_file(path::combine(resources, "bin/gnome/gnome.obj.gnome.e2d_shape"))));

        REQUIRE(s.vertices().size() == 397);

        REQUIRE(s.indices_subshape_count() == 1);
        REQUIRE(s.indices(0).size() == 2028);

        REQUIRE(s.uvs_channel_count() == 1);
        REQUIRE(s.uvs(0).size() == 397);

        REQUIRE(s.colors_channel_count() == 0);
    }
    {
        shape s;
        REQUIRE(shapes::try_load_shape(
            s,
            make_read_file(path::combine(resources, "bin/gnome/gnome.obj.yad.e2d_shape"))));

        REQUIRE(s.vertices().size() == 46);

        REQUIRE(s.indices_subshape_count() == 1);
        REQUIRE(s.indices(0).size() == 264);

        REQUIRE(s.uvs_channel_count() == 1);
        REQUIRE(s.uvs(0).size() == 46);

        REQUIRE(s.colors_channel_count() == 0);
    }
}
