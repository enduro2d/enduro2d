/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
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
        virtual ~module() noexcept = default;
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
    };

    template < typename BaseT >
    std::unique_ptr<BaseT> module<BaseT>::instance_;
}

namespace e2d { namespace modules
{
    template < typename ImplT, typename... Args >
    ImplT& initialize(Args&&... args) {
        using BaseT = typename ImplT::base_type;
        return module<BaseT>::template initialize<ImplT>(std::forward<Args>(args)...);
    }

    template < typename ImplT >
    void shutdown() noexcept {
        using BaseT = typename ImplT::base_type;
        module<BaseT>::shutdown();
    }

    template < typename ImplT >
    bool is_initialized() noexcept {
        using BaseT = typename ImplT::base_type;
        return module<BaseT>::is_initialized();
    }

    template < typename ImplT >
    ImplT& instance() {
        using BaseT = typename ImplT::base_type;
        return static_cast<ImplT&>(module<BaseT>::instance());
    }
}}
