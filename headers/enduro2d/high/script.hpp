/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_high.hpp"

namespace e2d
{
    class bad_script_access final : public exception {
    public:
        const char* what() const noexcept final {
            return "bad script access";
        }
    };

    class script final {
    public:
        script() = default;
        ~script() noexcept = default;

        script(script&& other) noexcept;
        script& operator=(script&& other) noexcept;

        script(const script& other);
        script& operator=(const script& other);

        script(sol::protected_function&& func) noexcept;
        script(const sol::protected_function& func);

        script& assign(script&& other) noexcept;
        script& assign(const script& other);

        script& assign(sol::protected_function&& func) noexcept;
        script& assign(const sol::protected_function& func);
        
        void clear() noexcept;
        void swap(script& other) noexcept;
        bool empty() const noexcept;

        template < typename... Ret, typename... Args >
        decltype(auto) call(Args&&... args) const;
    private:
        std::optional<sol::protected_function> func_;
    };

    void swap(script& l, script& r) noexcept;
}

namespace e2d
{
    template < typename... Ret, typename... Args >
    decltype(auto) script::call(Args&&... args) const {
        if ( !func_ ) {
            throw bad_script_access();
        }
        return func_->call<Ret...>(std::forward<Args>(args)...);
    }
}
