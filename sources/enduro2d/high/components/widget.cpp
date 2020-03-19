/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/widget.hpp>

namespace e2d
{
    const char* factory_loader<widget>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "size" : { "$ref": "#/common_definitions/v2" },
            "pivot" : { "$ref": "#/common_definitions/v2" }
        }
    })json";

    bool factory_loader<widget>::operator()(
        widget& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("size") ) {
            v2f size = component.size();
            if ( !json_utils::try_parse_value(ctx.root["size"], size) ) {
                the<debug>().error("WIDGET: Incorrect formatting of 'size' property");
                return false;
            }
            component.size(size);
        }

        if ( ctx.root.HasMember("pivot") ) {
            v2f pivot = component.pivot();
            if ( !json_utils::try_parse_value(ctx.root["pivot"], pivot) ) {
                the<debug>().error("WIDGET: Incorrect formatting of 'pivot' property");
                return false;
            }
            component.pivot(pivot);
        }

        return true;
    }

    bool factory_loader<widget>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<widget>::title = ICON_FA_VECTOR_SQUARE " widget";

    void component_inspector<widget>::operator()(gcomponent<widget>& c) const {
        if ( v2f size = c->size();
            ImGui::DragFloat2("size", size.data(), 1.f) )
        {
            c->size(size);
        }

        if ( v2f pivot = c->pivot();
            ImGui::DragFloat2("pivot", pivot.data(), 0.01f) )
        {
            c->pivot(pivot);
        }
    }
}
