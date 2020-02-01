/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/touchable.hpp>

namespace e2d
{
    const char* factory_loader<touchable>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
        }
    })json";

    bool factory_loader<touchable>::operator()(
        touchable& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<touchable>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<touchable::touched>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<touchable::touched>::operator()(
        touchable::touched& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<touchable::touched>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* factory_loader<touchable::under_mouse>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {}
    })json";

    bool factory_loader<touchable::under_mouse>::operator()(
        touchable::under_mouse& component,
        const fill_context& ctx) const
    {
        E2D_UNUSED(component, ctx);
        return true;
    }

    bool factory_loader<touchable::under_mouse>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<touchable>::title = ICON_FA_FINGERPRINT " touchable";

    void component_inspector<touchable>::operator()(gcomponent<touchable>& c) const {
        if ( bool touched = c.owner().component<touchable::touched>().exists();
            ImGui::Checkbox("touched", &touched) )
        {
            if ( touched ) {
                c.owner().component<touchable::touched>().ensure();
            } else {
                c.owner().component<touchable::touched>().remove();
            }
        }

        if ( bool under_mouse = c.owner().component<touchable::under_mouse>().exists();
            ImGui::Checkbox("under_mouse", &under_mouse) )
        {
            if ( under_mouse ) {
                c.owner().component<touchable::under_mouse>().ensure();
            } else {
                c.owner().component<touchable::under_mouse>().remove();
            }
        }
    }
}
