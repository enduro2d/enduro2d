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
        str title;
        std::mutex mutex;
    public:
        state(const v2u& size, str_view title, bool fullscreen)
        : shared_state(glfw_state::get_shared_state())
        , window(open_window_(size, make_utf8(title), fullscreen))
        , virtual_size(size)
        , title(title)
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
            const v2u& virtual_size, const str& title, bool fullscreen) noexcept
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            if ( !monitor ) {
                return {nullptr, glfwDestroyWindow};
            }
            const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);
            if ( !video_mode ) {
                return {nullptr, glfwDestroyWindow};
            }
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            v2i real_size = fullscreen
                ? make_vec2(video_mode->width, video_mode->height)
                : virtual_size.cast_to<i32>();
            GLFWwindow* w = glfwCreateWindow(
                real_size.x,
                real_size.y,
                title.c_str(),
                fullscreen ? monitor : nullptr,
                nullptr);
            return {w, glfwDestroyWindow};
        }
    };

    window::window(const v2u& size, str_view title, bool fullscreen)
    : state_(new state(size, title, fullscreen)) {}
    window::~window() noexcept = default;

    void window::hide() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        E2D_ASSERT(state_->window);
        glfwHideWindow(state_->window.get());
    }

    void window::show() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        E2D_ASSERT(state_->window);
        glfwShowWindow(state_->window.get());
    }

    void window::restore() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        E2D_ASSERT(state_->window);
        glfwRestoreWindow(state_->window.get());
    }

    void window::minimize() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        E2D_ASSERT(state_->window);
        glfwIconifyWindow(state_->window.get());
    }

    v2u window::real_size() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        E2D_ASSERT(state_->window);
        int w = 0, h = 0;
        glfwGetFramebufferSize(state_->window.get(), &w, &h);
        return make_vec2(w,h).cast_to<u32>();
    }

    v2u window::virtual_size() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->virtual_size;
    }

    const str& window::title() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        return state_->title;
    }

    void window::set_title(str_view title) {
        std::lock_guard<std::mutex> guard(state_->mutex);
        E2D_ASSERT(state_->window);
        state_->title = make_utf8(title);
        glfwSetWindowTitle(
            state_->window.get(), state_->title.c_str());
    }

    bool window::should_close() const noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        E2D_ASSERT(state_->window);
        return glfwWindowShouldClose(state_->window.get());
    }

    void window::set_should_close(bool yesno) noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        E2D_ASSERT(state_->window);
        glfwSetWindowShouldClose(
            state_->window.get(), yesno ? GLFW_TRUE : GLFW_FALSE);
    }

    void window::swap_buffers() noexcept {
        std::lock_guard<std::mutex> guard(state_->mutex);
        E2D_ASSERT(state_->window);
        glfwSwapBuffers(state_->window.get());
    }

    bool window::poll_events() noexcept {
        return glfw_state::poll_events();
    }
}

#endif
