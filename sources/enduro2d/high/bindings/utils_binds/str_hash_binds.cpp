/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include "_utils_binds.hpp"

using namespace e2d;

namespace e2d::bindings::utils
{
    void bind_str_hash(sol::state& l) {
        l.new_usertype<str_hash>("str_hash",
            sol::constructors<
                str_hash(),
                str_hash(const char*)
            >(),

            sol::meta_function::equal_to, sol::resolve<bool(str_hash, str_hash)>(::operator==),
            sol::meta_function::less_than, sol::resolve<bool(str_hash, str_hash)>(::operator<),

            "clear", &str_hash::clear,
            "empty", sol::property(&str_hash::empty),
            "hash", sol::property(&str_hash::hash));
    }
}
