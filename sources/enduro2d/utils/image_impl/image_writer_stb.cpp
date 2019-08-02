/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"

#define STBI_WRITE_NO_STDIO

#define STBIW_FREE(ptr) std::free(ptr)
#define STBIW_MALLOC(size) std::malloc(size)
#define STBIW_REALLOC(ptr,nsize) std::realloc(ptr, nsize)

#define STBIW_ASSERT(expr) E2D_ASSERT(expr)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <3rdparty/stb/stb_image_write.h>

namespace
{
    using namespace e2d;

    struct stb_write_context {
        vector<u8> data;
        std::exception_ptr exception{nullptr};
    };

    void stb_write_callback(void *context, void *data, int size) {
        E2D_ASSERT(context && data && size > 0);
        stb_write_context& ctx = *static_cast<stb_write_context*>(context);
        if ( ctx.exception ) {
            return;
        }
        try {
            ctx.data.insert(
                ctx.data.cend(),
                reinterpret_cast<u8*>(data),
                reinterpret_cast<u8*>(data) + math::numeric_cast<std::size_t>(size));
        } catch (...) {
            ctx.exception = std::current_exception();
        }
    }

    int stb_channels_from_image_format(image_data_format format) noexcept {
        switch ( format ) {
            case image_data_format::g8:    return 1;
            case image_data_format::ga8:   return 2;
            case image_data_format::rgb8:  return 3;
            case image_data_format::rgba8: return 4;
            default:                       return 0;
        }
    }
}

namespace e2d::images::impl
{
    bool save_image_jpg(const image& src, buffer& dst) {
        int img_w = math::numeric_cast<int>(src.size().x);
        int img_h = math::numeric_cast<int>(src.size().y);
        int img_c = stb_channels_from_image_format(src.format());
        const auto write_ctx = std::make_unique<stb_write_context>();

        if ( !stbi_write_jpg_to_func(
            stb_write_callback,
            write_ctx.get(),
            img_w,
            img_h,
            img_c,
            src.data().data(),
            80) )
        {
            return false;
        }

        if ( write_ctx->exception ) {
            std::rethrow_exception(write_ctx->exception);
        }

        dst.assign(write_ctx->data.data(), write_ctx->data.size());
        return true;
    }

    bool save_image_png(const image& src, buffer& dst) {
        int img_w = math::numeric_cast<int>(src.size().x);
        int img_h = math::numeric_cast<int>(src.size().y);
        int img_c = stb_channels_from_image_format(src.format());
        const auto write_ctx = std::make_unique<stb_write_context>();

        if ( !stbi_write_png_to_func(
            stb_write_callback,
            write_ctx.get(),
            img_w,
            img_h,
            img_c,
            src.data().data(),
            img_w * img_c) )
        {
            return false;
        }

        if ( write_ctx->exception ) {
            std::rethrow_exception(write_ctx->exception);
        }

        dst.assign(write_ctx->data.data(), write_ctx->data.size());
        return true;
    }

    bool save_image_tga(const image& src, buffer& dst) {
        int img_w = math::numeric_cast<int>(src.size().x);
        int img_h = math::numeric_cast<int>(src.size().y);
        int img_c = stb_channels_from_image_format(src.format());
        const auto write_ctx = std::make_unique<stb_write_context>();

        vector<u8> data;
        if ( !stbi_write_tga_to_func(
            stb_write_callback,
            write_ctx.get(),
            img_w,
            img_h,
            img_c,
            src.data().data()) )
        {
            return false;
        }

        if ( write_ctx->exception ) {
            std::rethrow_exception(write_ctx->exception);
        }

        dst.assign(write_ctx->data.data(), write_ctx->data.size());
        return true;
    }
}
