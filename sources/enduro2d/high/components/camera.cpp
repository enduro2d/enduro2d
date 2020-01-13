/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/components/camera.hpp>

namespace e2d
{
    const char* factory_loader<camera>::schema_source = R"json({
        "type" : "object",
        "required" : [],
        "additionalProperties" : false,
        "properties" : {
            "depth" : { "type" : "number" },
            "viewport" : { "$ref": "#/common_definitions/b2" },
            "projection" : { "$ref": "#/common_definitions/m4" },
            "background" : { "$ref": "#/common_definitions/color" }
        }
    })json";

    bool factory_loader<camera>::operator()(
        camera& component,
        const fill_context& ctx) const
    {
        if ( ctx.root.HasMember("depth") ) {
            auto depth = component.depth();
            if ( !json_utils::try_parse_value(ctx.root["depth"], depth) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'depth' property");
                return false;
            }
            component.depth(depth);
        }

        if ( ctx.root.HasMember("viewport") ) {
            auto viewport = component.viewport();
            if ( !json_utils::try_parse_value(ctx.root["viewport"], viewport) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'viewport' property");
                return false;
            }
            component.viewport(viewport);
        }

        if ( ctx.root.HasMember("projection") ) {
            auto projection = component.projection();
            if ( !json_utils::try_parse_value(ctx.root["projection"], projection) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'projection' property");
                return false;
            }
            component.projection(projection);
        }

        if ( ctx.root.HasMember("target") ) {
            //TODO(BlackMat): add 'target' property parsing
        }

        if ( ctx.root.HasMember("background") ) {
            auto background = component.background();
            if ( !json_utils::try_parse_value(ctx.root["background"], background) ) {
                the<debug>().error("CAMERA: Incorrect formatting of 'background' property");
                return false;
            }
            component.background(background);
        }

        return true;
    }

    bool factory_loader<camera>::operator()(
        asset_dependencies& dependencies,
        const collect_context& ctx) const
    {
        E2D_UNUSED(dependencies, ctx);
        return true;
    }
}

namespace e2d
{
    const char* component_inspector<camera>::title = "camera";

    void component_inspector<camera>::operator()(gcomponent<camera>& c) const {
        if ( i32 depth = c->depth();
            ImGui::DragInt("depth", &depth) )
        {
            c->depth(depth);
        }

        ///TODO(BlackMat): add 'viewport' inspector
        ///TODO(BlackMat): add 'projection' inspector
        ///TODO(BlackMat): add 'target' inspector

        if ( color background = c->background();
            ImGui::ColorEdit4("background", background.data()) )
        {
            c->background(background);
        }
    }
}
