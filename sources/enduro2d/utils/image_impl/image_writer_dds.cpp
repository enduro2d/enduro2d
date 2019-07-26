/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
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
            case image_data_format::rgb_dxt1:
            case image_data_format::rgba_dxt1:
            case image_data_format::rgba_dxt3:
            case image_data_format::rgba_dxt5:
                return true;
            default:
                return false;
        }
    }
}

namespace e2d::images::impl
{
    bool save_image_dds(const image& src, buffer& dst) {
        E2D_UNUSED(src, dst);
        if ( is_save_image_dds_supported(src.format()) ) {
            //TODO(BlackMat): implme
            E2D_ASSERT_MSG(false, "implme");
        }
        return false;
    }
}
