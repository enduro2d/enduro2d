#pragma once

#include <enduro2d/math/vec2.hpp>
#include <enduro2d/math/vec4.hpp>

#define IM_ASSERT(_EXPR) E2D_ASSERT(_EXPR)

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

#define IMGUI_DISABLE_DEMO_WINDOWS
#define IMGUI_DISABLE_DEBUG_TOOLS

#define IMGUI_DISABLE_WIN32_DEFAULT_CLIPBOARD_FUNCTIONS
#define IMGUI_DISABLE_WIN32_DEFAULT_IME_FUNCTIONS
#define IMGUI_DISABLE_WIN32_FUNCTIONS
#define IMGUI_ENABLE_OSX_DEFAULT_CLIPBOARD_FUNCTIONS

#define IM_VEC2_CLASS_EXTRA                                             \
    ImVec2(const e2d::v2f& f) { x = f.x; y = f.y; }                     \
    operator e2d::v2f() const { return e2d::v2f(x,y); }

#define IM_VEC4_CLASS_EXTRA                                             \
    ImVec4(const e2d::v4f& f) { x = f.x; y = f.y; z = f.z; w = f.w; }   \
    operator e2d::v4f() const { return e2d::v4f(x,y,z,w); }
