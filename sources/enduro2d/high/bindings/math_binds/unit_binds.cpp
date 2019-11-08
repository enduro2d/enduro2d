/*******************************************************************************
* This file is part of the "Enduro2D"
* For conditions of distribution and use, see copyright notice in LICENSE.md
* Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
******************************************************************************/

#include "_math_binds.hpp"

namespace
{
    using namespace e2d;

    template < typename T, typename Tag >
    void bind_unit_t(const str& name, sol::state& l) {
        l.new_usertype<unit<T,Tag>>(name,
            sol::constructors<
                unit<T,Tag>(),
                unit<T,Tag>(T),
                unit<T,Tag>(unit<T,Tag>)>(),

            "zero", &unit<T,Tag>::zero,

            "value", &unit<T,Tag>::value,

            sol::meta_function::to_string, [](const unit<T,Tag>& v){
                return strings::rformat("%0", v);
            },

            sol::meta_function::equal_to, sol::resolve<bool(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator<),
            sol::meta_function::less_than_or_equal_to, sol::resolve<bool(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator<=),

            sol::meta_function::unary_minus, sol::resolve<unit<T,Tag>(const unit<T,Tag>&)>(::operator-),

            sol::meta_function::multiplication, sol::overload(
                sol::resolve<unit<T,Tag>(T, const unit<T,Tag>&)>(::operator*),
                sol::resolve<unit<T,Tag>(const unit<T,Tag>&, T)>(::operator*)),

            sol::meta_function::division, sol::overload(
                sol::resolve<unit<T,Tag>(T, const unit<T,Tag>&)>(::operator/),
                sol::resolve<unit<T,Tag>(const unit<T,Tag>&, T)>(::operator/)),

            sol::meta_function::addition, sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator+),
            sol::meta_function::subtraction, sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&)>(::operator-),

            "approximately", [](const unit<T,Tag>& l, const unit<T,Tag>& r){ return math::approximately(l,r); },

            "minimized", sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&)>(&math::minimized),
            "maximized", sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&)>(&math::maximized),
            "clamped", sol::resolve<unit<T,Tag>(const unit<T,Tag>&, const unit<T,Tag>&, const unit<T,Tag>&)>(&math::clamped),
            "saturated", sol::resolve<unit<T,Tag>(const unit<T,Tag>&)>(&math::saturated),

            "contains_nan", sol::resolve<bool(const unit<T,Tag>&)>(&math::contains_nan));
    }
}

namespace e2d::bindings::math
{
    void bind_unit(sol::state& l) {
        bind_unit_t<f32, deg_tag>("degf", l);
        bind_unit_t<f32, rad_tag>("radf", l);
    }
}
