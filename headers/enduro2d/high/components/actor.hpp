/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_components.hpp"

#include "../node.hpp"

namespace e2d
{
    class actor final {
    public:
        actor() = default;
        actor(const node_iptr& node);

        actor& node(const node_iptr& value) noexcept;

        [[nodiscard]] node_iptr node() noexcept;
        [[nodiscard]] const_node_iptr node() const noexcept;
    private:
        node_iptr node_;
    };
}

namespace e2d
{
    template <>
    class factory_loader<actor> final : factory_loader<> {
    public:
        static const char* schema_source;

        bool operator()(
            actor& component,
            const fill_context& ctx) const;

        bool operator()(
            asset_dependencies& dependencies,
            const collect_context& ctx) const;
    };
}

namespace e2d
{
    template <>
    class component_inspector<actor> final : component_inspector<> {
    public:
        static const char* title;

        void operator()(gcomponent<actor>& c) const;
    };
}

namespace e2d
{
    inline actor::actor(const node_iptr& node)
    : node_(node) {}

    inline actor& actor::node(const node_iptr& value) noexcept {
        node_ = value;
        return *this;
    }

    inline node_iptr actor::node() noexcept {
        return node_;
    }

    inline const_node_iptr actor::node() const noexcept {
        return node_;
    }
}
