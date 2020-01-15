/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/rigid_body.hpp>

namespace e2d
{
    const char* factory_loader<rigid_body>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "type" : { "$ref": "#/definitions/types" }
        },
        "definitions" : {
            "types" : {
                "type" : "string",
                "enum" : [
                    "dynamic",
                    "kinematic"
                ]
            }
        }
    })json";

    bool factory_loader<rigid_body>::operator()(
        rigid_body& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("type") ) {
            rigid_body::types type = component.type();
            if ( !json_utils::try_parse_value(ctx.root["type"], type) ) {
                the<debug>().error("RIGID_BODY: Incorrect formatting of 'type' property");
                return false;
            }
            component.type(type);
        }

        return true;
    }

    bool factory_loader<rigid_body>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<rigid_body>::title = "rigid_body";

    void component_inspector<rigid_body>::operator()(gcomponent<rigid_body>& c) const {
        if ( rigid_body::types type = c->type();
            imgui_utils::show_enum_combo_box("type", &type) )
        {
            c->type(type);
        }
    }
}
