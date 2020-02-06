/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../assets/script_asset.hpp"

namespace e2d
{
    class behaviour final {
    public:
        behaviour() = default;

        behaviour& meta(sol::table value) noexcept;
        behaviour& script(const script_asset::ptr& value) noexcept;

        [[nodiscard]] sol::table& meta() noexcept;
        [[nodiscard]] const sol::table& meta() const noexcept;
        [[nodiscard]] const script_asset::ptr& script() const noexcept;
    private:
        sol::table meta_;
        script_asset::ptr script_;
    };
}

namespace e2d
{
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
    template <>
    class component_inspector<behaviour> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<behaviour>& c) const;
    };
}


namespace e2d
{
    inline behaviour& behaviour::meta(sol::table value) noexcept {
        meta_ = std::move(value);
        return *this;
    }

    inline behaviour& behaviour::script(const script_asset::ptr& value) noexcept {
        script_ = value;
        return *this;
    }

    inline sol::table& behaviour::meta() noexcept {
        return meta_;
    }

    inline const sol::table& behaviour::meta() const noexcept {
        return meta_;
    }

    inline const script_asset::ptr& behaviour::script() const noexcept {
        return script_;
    }
}

namespace e2d::behaviours
{
    ENUM_HPP_CLASS_DECL(fill_result, u8,
        (failed)
        (success))

    inline fill_result fill_meta_table(behaviour& behaviour) {
        if ( !behaviour.script() ) {
            return fill_result::failed;
        }

        sol::protected_function_result meta = behaviour.script()->content().call();

        if ( !meta.valid() ) {
            sol::error err = meta;
            the<debug>().error("BEHAVIOUR: Behaviour script error:\n"
                "--> Error: %0",
                err.what());
            return fill_result::failed;
        }

        if ( meta.get_type() != sol::type::table ) {
            the<debug>().error("BEHAVIOUR: Behaviour script must return a meta table");
            return fill_result::failed;
        }

        behaviour.meta(std::move(meta));
        return fill_result::success;
    }

    ENUM_HPP_CLASS_DECL(call_result, u8,
        (failed)
        (success)
        (method_not_found))

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

ENUM_HPP_REGISTER_TRAITS(e2d::behaviours::fill_result)
ENUM_HPP_REGISTER_TRAITS(e2d::behaviours::call_result)
