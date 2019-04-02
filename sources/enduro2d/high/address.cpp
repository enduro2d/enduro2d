/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/address.hpp>

namespace
{
    using namespace e2d;

    const str_view address_separator = ":/";

    str_view::iterator str_view_search(str_view str, str_view s_str) noexcept {
        return std::search(str.begin(), str.end(), s_str.begin(), s_str.end());
    }
}

namespace e2d { namespace address
{
    str parent(str_view address) {
        const auto sep_e = str_view_search(address, address_separator);
        if ( sep_e == address.end() ) {
            return address;
        }
        const auto sep_d = std::distance(address.begin(), sep_e);
        return address.substr(0, static_cast<std::size_t>(sep_d));
    }

    str nested(str_view address) {
        const auto sep_e = str_view_search(address, address_separator);
        if ( sep_e == address.end() ) {
            return str();
        }
        const auto sep_d = std::distance(address.begin(), sep_e);
        return address.substr(static_cast<std::size_t>(sep_d) + address_separator.size());
    }
}}
