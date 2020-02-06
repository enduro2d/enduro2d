/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/named.hpp>

namespace e2d
{
    const char* factory_loader<named>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "name" : { "$ref": "#/common_definitions/name" }
        }
    })json";

    bool factory_loader<named>::operator()(
        named& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("name") ) {
            str name = component.name();
            if ( !json_utils::try_parse_value(ctx.root["name"], name) ) {
                the<debug>().error("NAMED: Incorrect formatting of 'name' property");
                return false;
            }
            component.name(std::move(name));
        }

        return true;
    }

    bool factory_loader<named>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<named>::title = ICON_FA_SIGNATURE " named";

    void component_inspector<named>::operator()(gcomponent<named>& c) const {
        if ( str name = c->name();
            ImGui::InputText("name", &name) )
        {
            c->name(std::move(name));
        }
    }
}
