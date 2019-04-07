/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"

#define STBI_WRITE_NO_STDIO

#define STBIW_FREE(ptr)          std::free(ptr)
#define STBIW_MALLOC(size)       std::malloc(size)
#define STBIW_REALLOC(ptr,nsize) std::realloc(ptr, nsize)

#define STBIW_ASSERT(expr)       E2D_ASSERT(expr)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <3rdparty/stb/stb_image_write.h>

namespace
{
    using namespace e2d;

    int stb_channels_from_image_format(image_data_format format) noexcept {
        switch ( format ) {
            case image_data_format::g8:    return 1;
            case image_data_format::ga8:   return 2;
            case image_data_format::rgb8:  return 3;
            case image_data_format::rgba8: return 4;
            default:                       return 0;
        }
    }

    void stb_write_callback(void *context, void *data, int size) {
        E2D_ASSERT(context && data && size > 0);
        vector<u8>& ctx = *static_cast<vector<u8>*>(context);
        ctx.insert(
            ctx.cend(),
            reinterpret_cast<u8*>(data),
            reinterpret_cast<u8*>(data) + math::numeric_cast<std::size_t>(size));
    }
}

namespace e2d { namespace images { namespace impl
{
    bool try_save_image_jpg(const image& src, buffer& dst) noexcept {
        int img_w = math::numeric_cast<int>(src.size().x);
        int img_h = math::numeric_cast<int>(src.size().y);
        int img_c = stb_channels_from_image_format(src.format());
        try {
            vector<u8> data;
            if ( 0 != stbi_write_jpg_to_func(
                stb_write_callback, &data, img_w, img_h, img_c, src.data().data(), 80) )
            {
                dst.assign(data.data(), data.size());
                return true;
            }
        } catch (...) {
            // nothing
        }
        return false;
    }

    bool try_save_image_png(const image& src, buffer& dst) noexcept {
        int img_w = math::numeric_cast<int>(src.size().x);
        int img_h = math::numeric_cast<int>(src.size().y);
        int img_c = stb_channels_from_image_format(src.format());
        try {
            vector<u8> data;
            if ( 0 != stbi_write_png_to_func(
                stb_write_callback, &data, img_w, img_h, img_c, src.data().data(), img_w * img_c) )
            {
                dst.assign(data.data(), data.size());
                return true;
            }
        } catch (...) {
            // nothing
        }
        return false;
    }

    bool try_save_image_tga(const image& src, buffer& dst) noexcept {
        int img_w = math::numeric_cast<int>(src.size().x);
        int img_h = math::numeric_cast<int>(src.size().y);
        int img_c = stb_channels_from_image_format(src.format());
        try {
            vector<u8> data;
            if ( 0 != stbi_write_tga_to_func(
                stb_write_callback, &data, img_w, img_h, img_c, src.data().data()) )
            {
                dst.assign(data.data(), data.size());
                return true;
            }
        } catch (...) {
            // nothing
        }
        return false;
    }
}}}
