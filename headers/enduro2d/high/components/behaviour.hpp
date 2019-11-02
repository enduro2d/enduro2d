/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

#include "../factory.hpp"

namespace e2d
{
    class behaviour final {
    public:
        behaviour() = default;

        behaviour& meta(sol::table value) noexcept;

        [[nodiscard]] sol::table& meta() noexcept;
        [[nodiscard]] const sol::table& meta() const noexcept;
    private:
        str name_;
        sol::table meta_;
    };

    template <>
    class factory_loader<behaviour> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            behaviour& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    inline behaviour& behaviour::meta(sol::table value) noexcept {
        meta_ = std::move(value);
        return *this;
    }

    inline sol::table& behaviour::meta() noexcept {
        return meta_;
    }

    inline const sol::table& behaviour::meta() const noexcept {
        return meta_;
    }
}

namespace e2d::behaviours
{
    enum class call_result {
        failed,
        success,
        method_not_found,
    };

    template < typename... Args >
    call_result call_meta_method(behaviour& behaviour, str_view method, Args&&... args) {
        if ( method.empty() || !behaviour.meta() || !behaviour.meta().valid() ) {
            return call_result::method_not_found;
        }

        sol::optional<sol::protected_function> f = behaviour.meta()[method];
        if ( !f ) {
            return call_result::method_not_found;
        }

        sol::protected_function_result r = f->call(
            behaviour.meta(),
            std::forward<Args>(args)...);
        if ( !r.valid() ) {
            sol::error err = r;
            the<debug>().error("BEHAVIOUR: Behaviour method error:\n"
                "--> Method: %0\n"
                "--> Error: %1",
                method,
                err.what());
            return call_result::failed;
        }

        return call_result::success;
    }
}
