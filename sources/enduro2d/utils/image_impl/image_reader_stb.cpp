/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "image_impl.hpp"

#define STBI_NO_STDIO
#define STBI_NO_FAILURE_STRINGS

#define STBI_NO_HDR
#define STBI_NO_LINEAR

#define STBI_ONLY_PNG
#define STBI_ONLY_TGA
#define STBI_ONLY_JPEG

#define STBI_FREE(ptr) std::free(ptr)
#define STBI_MALLOC(size) std::malloc(size)
#define STBI_REALLOC(ptr,nsize) std::realloc(ptr, nsize)

#define STBI_ASSERT(expr) E2D_ASSERT(expr)

#define STB_IMAGE_IMPLEMENTATION
#include <3rdparty/stb/stb_image.h>

namespace
{
    using namespace e2d;

    using stbi_img_uptr = std::unique_ptr<stbi_uc, decltype(&stbi_image_free)>;

    stbi_img_uptr load_stb_image(const buffer& data, v2u& out_size, u32& out_channels) noexcept {
        int img_w = 0, img_h = 0, img_c = 0;
        stbi_uc* img = stbi_load_from_memory(
            static_cast<const stbi_uc*>(data.data()),
            math::numeric_cast<int>(data.size()),
            &img_w, &img_h, &img_c, 0);
        stbi_img_uptr img_ptr(img, stbi_image_free);
        if ( img_ptr && img_w > 0 && img_h > 0 && img_c >= 1 && img_c <= 4 ) {
            out_size = v2u(math::numeric_cast<u32>(img_w), math::numeric_cast<u32>(img_h));
            out_channels = math::numeric_cast<u32>(img_c);
            return img_ptr;
        }
        return stbi_img_uptr(nullptr, stbi_image_free);
    }

    image_data_format image_format_from_stb_channels(u32 channels) noexcept {
        switch ( channels ) {
            case 1: return image_data_format::g8;
            case 2: return image_data_format::ga8;
            case 3: return image_data_format::rgb8;
            case 4: return image_data_format::rgba8;
            default:
                E2D_ASSERT_MSG(false, "unexpected stb channel count");
                return image_data_format::rgba8;
        }
    }
}

namespace e2d::images::impl
{
    bool load_image_stb(image& dst, const buffer& src) {
        v2u img_size;
        u32 img_channels = 0;
        const stbi_img_uptr img_ptr = load_stb_image(src, img_size, img_channels);
        if ( !img_ptr || !img_size.x || !img_size.y ) {
            return false;
        }

        const image_data_format img_format = image_format_from_stb_channels(img_channels);
        const std::size_t img_buffer_size = img_size.x * img_size.y * img_channels;

        dst = image(img_size, img_format, buffer(img_ptr.get(), img_buffer_size));
        return true;
    }
}
