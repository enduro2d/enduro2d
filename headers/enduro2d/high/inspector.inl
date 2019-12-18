/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "inspector.hpp"

namespace e2d
{
    //
    // typed_inspector_drawer
    //

    namespace impl
    {
        template < typename Component >
        void typed_inspector_drawer<Component>::operator()(gobject& go) const {
            gcomponent<Component> co = go.component<Component>();
            if ( !co ) {
                return;
            }

            ImGui::PushID(co.find());
            E2D_DEFER([](){ ImGui::PopID(); });

            if ( ImGui::CollapsingHeader(component_inspector<Component>::title) ) {
                inspector_(co);
            }
        }
    }

    //
    // inspector
    //

    template < typename Component >
    inspector& inspector::register_component(str_hash type) {
        std::lock_guard<std::mutex> guard(mutex_);
        if ( drawers_.count(type) > 0 ) {
            throw bad_inspector_operation();
        }
        impl::inspector_drawer_iptr drawer(new impl::typed_inspector_drawer<Component>());
        drawers_.emplace(type, std::move(drawer));
        return *this;
    }
}
