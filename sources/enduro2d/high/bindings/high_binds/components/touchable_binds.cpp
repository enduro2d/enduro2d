/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "../_high_binds.hpp"

#include <enduro2d/high/gobject.hpp>
#include <enduro2d/high/components/disabled.hpp>
#include <enduro2d/high/components/touchable.hpp>

namespace e2d::bindings::high
{
    void bind_touchable(sol::state& l) {
        l.new_usertype<gcomponent<touchable>>("touchable",
            sol::no_constructor,

            "enable", [](gcomponent<touchable>& c){
                c.component<disabled<touchable>>().remove();
            },

            "disable", [](gcomponent<touchable>& c){
                c.component<disabled<touchable>>().ensure();
            },

            "enabled", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return !c.component<disabled<touchable>>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<touchable>>().remove();
                    } else {
                        c.component<disabled<touchable>>().ensure();
                    }
                }
            ),

            "disabled", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<disabled<touchable>>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<disabled<touchable>>().ensure();
                    } else {
                        c.component<disabled<touchable>>().remove();
                    }
                }
            ),

            "pushing", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<touchable::pushing>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<touchable::pushing>().ensure();
                    } else {
                        c.component<touchable::pushing>().remove();
                    }
                }
            ),

            "hovering", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<touchable::hovering>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<touchable::hovering>().ensure();
                    } else {
                        c.component<touchable::hovering>().remove();
                    }
                }
            ),

            "clicked", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<touchable::clicked>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<touchable::clicked>().ensure();
                    } else {
                        c.component<touchable::clicked>().remove();
                    }
                }
            ),

            "pressed", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<touchable::pressed>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<touchable::pressed>().ensure();
                    } else {
                        c.component<touchable::pressed>().remove();
                    }
                }
            ),

            "released", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<touchable::released>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<touchable::released>().ensure();
                    } else {
                        c.component<touchable::released>().remove();
                    }
                }
            ),

            "hover_over", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<touchable::hover_over>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<touchable::hover_over>().ensure();
                    } else {
                        c.component<touchable::hover_over>().remove();
                    }
                }
            ),

            "hover_out", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<touchable::hover_out>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<touchable::hover_out>().ensure();
                    } else {
                        c.component<touchable::hover_out>().remove();
                    }
                }
            ),

            "hover_enter", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<touchable::hover_enter>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<touchable::hover_enter>().ensure();
                    } else {
                        c.component<touchable::hover_enter>().remove();
                    }
                }
            ),

            "hover_leave", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c.component<touchable::hover_leave>().exists();
                },
                [](gcomponent<touchable>& c, bool yesno){
                    if ( yesno ) {
                        c.component<touchable::hover_leave>().ensure();
                    } else {
                        c.component<touchable::hover_leave>().remove();
                    }
                }
            ),

            "bubbling", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c->bubbling();
                },
                [](gcomponent<touchable>& c, bool v){
                    c->bubbling(v);
                }),

            "capturing", sol::property(
                [](const gcomponent<touchable>& c) -> bool {
                    return c->capturing();
                },
                [](gcomponent<touchable>& c, bool v){
                    c->capturing(v);
                })
        );

        //
        // events
        //

        l["touchable"].get_or_create<sol::table>()
        .new_usertype<touchable_events::mouse_move_evt>("mouse_move_evt",
            sol::no_constructor,

            "target", sol::property(
                [](const touchable_events::mouse_move_evt& c) -> gobject {
                    return c.target();
                }),

            "bubbling", sol::property(
                [](const touchable_events::mouse_move_evt& c) -> bool {
                    return c.bubbling();
                }),

            "local_point", sol::property(
                [](const touchable_events::mouse_move_evt& c) -> v2f {
                    return c.local_point();
                }),

            "world_point", sol::property(
                [](const touchable_events::mouse_move_evt& c) -> v2f {
                    return c.world_point();
                })
        );

        l["touchable"].get_or_create<sol::table>()
        .new_usertype<touchable_events::mouse_hover_evt>("mouse_hover_evt",
            sol::no_constructor,

            "target", sol::property(
                [](const touchable_events::mouse_hover_evt& c) -> gobject {
                    return c.target();
                }),

            "bubbling", sol::property(
                [](const touchable_events::mouse_hover_evt& c) -> bool {
                    return c.bubbling();
                }),

            "type", sol::property(
                [](const touchable_events::mouse_hover_evt& c) -> str {
                    return str(enum_hpp::to_string_or_throw(c.type()));
                })
        );

        l["touchable"].get_or_create<sol::table>()
        .new_usertype<touchable_events::mouse_button_evt>("mouse_button_evt",
            sol::no_constructor,

            "target", sol::property(
                [](const touchable_events::mouse_button_evt& c) -> gobject {
                    return c.target();
                }),

            "bubbling", sol::property(
                [](const touchable_events::mouse_button_evt& c) -> bool {
                    return c.bubbling();
                }),

            "type", sol::property(
                [](const touchable_events::mouse_button_evt& c) -> str {
                    return str(enum_hpp::to_string_or_throw(c.type()));
                }),

            "button", sol::property(
                [](const touchable_events::mouse_button_evt& c) -> str {
                    return str(enum_hpp::to_string_or_throw(c.button()));
                }),

            "local_point", sol::property(
                [](const touchable_events::mouse_button_evt& c) -> v2f {
                    return c.local_point();
                }),

            "world_point", sol::property(
                [](const touchable_events::mouse_button_evt& c) -> v2f {
                    return c.world_point();
                })
        );
    }
}
