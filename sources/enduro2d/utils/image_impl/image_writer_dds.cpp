/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "image_impl.hpp"

namespace
{
    using namespace e2d;

    bool is_save_image_dds_supported(image_data_format data_format) noexcept {
        switch ( data_format ) {
            case image_data_format::g8:
            case image_data_format::ga8:
            case image_data_format::rgb8:
            case image_data_format::rgba8:
            case image_data_format::dxt1:
            case image_data_format::dxt3:
            case image_data_format::dxt5:
                return true;
            default:
                return false;
        }
    }
}

namespace e2d { namespace images { namespace impl
{
    bool try_save_image_dds(const image& src, buffer& dst) noexcept {
        E2D_UNUSED(src, dst);
        if ( is_save_image_dds_supported(src.format()) ) {
            //TODO: implme
            E2D_ASSERT_MSG(false, "implme");
        }
        return false;
    }
}}}