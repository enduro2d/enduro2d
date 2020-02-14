/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/mask.hpp>

namespace e2d
{
    const char* factory_loader<mask>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "visible" : { "type" : "boolean" }
        }
    })json";

    bool factory_loader<mask>::operator()(
        mask& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("visible") ) {
            bool visible = component.visible();
            if ( !json_utils::try_parse_value(ctx.root["visible"], visible) ) {
                the<debug>().error("MASK: Incorrect formatting of 'visible' property");
                return false;
            }
            component.visible(visible);
        }

        return true;
    }

    bool factory_loader<mask>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<mask>::title = ICON_FA_MASK " mask";

    void component_inspector<mask>::operator()(gcomponent<mask>& c) const {
        if ( bool visible = c->visible();
            ImGui::Checkbox("visible", &visible) )
        {
            c->visible(visible);
        }
    }
}
