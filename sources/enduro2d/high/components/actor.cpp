/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/actor.hpp>

namespace e2d
{
    const char* factory_loader<actor>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "translation" : { "$ref": "#/common_definitions/v3" },
            "rotation" : { "$ref": "#/common_definitions/q4" },
            "scale" : { "$ref": "#/common_definitions/v3" }
        }
    })json";

    bool factory_loader<actor>::operator()(
        actor& component,
        const fill_context& ctx) const
    {
        if ( !component.node() ) {
            component.node(node::create());
        }

        if ( ctx.root.HasMember("translation") ) {
            auto translation = component.node()->translation();
            if ( !json_utils::try_parse_value(ctx.root["translation"], translation) ) {
                the<debug>().error("ACTOR: Incorrect formatting of 'translation' property");
                return false;
            }
            component.node()->translation(translation);
        }

        if ( ctx.root.HasMember("rotation") ) {
            auto rotation = component.node()->rotation();
            if ( !json_utils::try_parse_value(ctx.root["rotation"], rotation) ) {
                the<debug>().error("ACTOR: Incorrect formatting of 'rotation' property");
                return false;
            }
            component.node()->rotation(rotation);
        }

        if ( ctx.root.HasMember("scale") ) {
            auto scale = component.node()->scale();
            if ( !json_utils::try_parse_value(ctx.root["scale"], scale) ) {
                the<debug>().error("ACTOR: Incorrect formatting of 'scale' property");
                return false;
            }
            component.node()->scale(scale);
        }

        return true;
    }

    bool factory_loader<actor>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}
