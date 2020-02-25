/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    class module_not_initialized final : public exception {
    public:
        const char* what() const noexcept final {
            return "module not initialized";
        }
    };

    class module_already_initialized final : public exception {
    public:
        const char* what() const noexcept final {
            return "module already initialized";
        }
    };

    template < typename BaseT >
    class module : private noncopyable {
    public:
        using base_type = BaseT;
    public:
        module() noexcept = default;
        virtual ~module() noexcept = default;

        const std::thread::id& main_thread() const noexcept {
            return main_thread_;
        }

        bool is_in_main_thread() const noexcept {
            return std::this_thread::get_id() == main_thread_;
        }
    public:
        template < typename ImplT, typename... Args >
        static ImplT& initialize(Args&&... args) {
            if ( is_initialized() ) {
                throw module_already_initialized();
            }
            instance_ = std::make_unique<ImplT>(std::forward<Args>(args)...);
            return static_cast<ImplT&>(*instance_);
        }

        static void shutdown() noexcept {
            instance_.reset();
        }

        static bool is_initialized() noexcept {
            return !!instance_;
        }

        static BaseT& instance() {
            if ( !is_initialized() ) {
                throw module_not_initialized();
            }
            return *instance_;
        }
    private:
        static std::unique_ptr<BaseT> instance_;
        std::thread::id main_thread_ = std::this_thread::get_id();
    };

    template < typename BaseT >
    std::unique_ptr<BaseT> module<BaseT>::instance_;
}

namespace e2d::modules
{
    template < typename ImplT, typename... Args >
    ImplT& initialize(Args&&... args) {
        using BaseT = typename ImplT::base_type;
        return module<BaseT>::template initialize<ImplT>(std::forward<Args>(args)...);
    }

    template < typename... ImplTs >
    void shutdown() noexcept {
        (... , module<typename ImplTs::base_type>::shutdown());
    }

    template < typename... ImplTs >
    bool is_initialized() noexcept {
        return (... && module<typename ImplTs::base_type>::is_initialized());
    }

    template < typename ImplT >
    ImplT& instance() {
        using BaseT = typename ImplT::base_type;
        return static_cast<ImplT&>(module<BaseT>::instance());
    }
}
