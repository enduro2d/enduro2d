/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "window.hpp"

#if defined(E2D_WINDOW_MODE) && E2D_WINDOW_MODE == E2D_WINDOW_MODE_GLFW

#include <3rdparty/glfw/glfw3.h>

namespace
{
    using namespace e2d;

    class glfw_state;
    using glfw_state_ptr = std::shared_ptr<glfw_state>;

    class glfw_state : private noncopyable {
    public:
        glfw_state() {
            if ( !glfwInit() ) {
                throw bad_window_operation();
            }
        }

        ~glfw_state() noexcept {
            glfwTerminate();
        }
    public:
        static bool poll_events() noexcept {
            std::lock_guard<std::mutex> guard(mutex_);
            if ( shared_state_ ) {
                glfwPollEvents();
                return true;
            }
            return false;
        }

        static glfw_state_ptr get_shared_state() {
            std::lock_guard<std::mutex> guard(mutex_);
            if ( !shared_state_ ) {
                shared_state_ = std::make_shared<glfw_state>();
            }
            return shared_state_;
        }
    private:
        static std::mutex mutex_;
        static std::shared_ptr<glfw_state> shared_state_;
    };

    std::mutex glfw_state::mutex_;
    std::shared_ptr<glfw_state> glfw_state::shared_state_;
}

namespace e2d
{
    class window::state final : private e2d::noncopyable {
    public:
        using window_uptr = std::unique_ptr<
            GLFWwindow, void(*)(GLFWwindow*)>;
        glfw_state_ptr shared_state;
        window_uptr window;
        v2u virtual_size;
    public:
        state(const v2u& size, str_view title, bool fullscreen)
        : shared_state(glfw_state::get_shared_state())
        , window(open_window_(size, make_utf8(title), fullscreen))
        , virtual_size(size)
        {
            if ( !window ) {
                throw bad_window_operation();
            }
        }

        ~state() noexcept {
            // reset window before shared state
            window.reset();
        }
    private:
        static window_uptr open_window_(
            const v2u& size, const str& title, bool fullscreen) noexcept
        {
            if ( fullscreen ) {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                if ( !monitor ) {
                    return {nullptr, glfwDestroyWindow};
                }
                const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);
                if ( !video_mode ) {
                    return {nullptr, glfwDestroyWindow};
                }
                GLFWwindow* w = glfwCreateWindow(
                    video_mode->width,
                    video_mode->height,
                    title.c_str(),
                    monitor,
                    nullptr);
                return {w, glfwDestroyWindow};
            } else {
                GLFWwindow* w = glfwCreateWindow(
                    math::numeric_cast<int>(size.x),
                    math::numeric_cast<int>(size.y),
                    title.c_str(),
                    nullptr,
                    nullptr);
                return {w, glfwDestroyWindow};
            }
        }
    };

    window::window(const v2u& size, str_view title, bool fullscreen)
    : state_(new state(size, title, fullscreen)) {}
    window::~window() noexcept = default;

    v2u window::real_size() const noexcept {
        E2D_ASSERT(state_->window);
        int w = 0, h = 0;
        glfwGetWindowSize(state_->window.get(), &w, &h);
        return make_vec2(w,h).cast_to<u32>();
    }

    v2u window::virtual_size() const noexcept {
        return state_->virtual_size;
    }

    bool window::should_close() const noexcept {
        E2D_ASSERT(state_->window);
        return glfwWindowShouldClose(state_->window.get());
    }

    void window::swap_buffers() noexcept {
        E2D_ASSERT(state_->window);
        glfwSwapBuffers(state_->window.get());
    }

    bool window::poll_events() noexcept {
        return glfw_state::poll_events();
    }
}

#endif
