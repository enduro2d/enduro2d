/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include "platform_impl/platform.hpp"

namespace e2d
{
    //
    // platform::internal_state
    //

    class platform::internal_state final : private e2d::noncopyable {
    public:
        internal_state(int argc, char *argv[])
        : impl_(platform_internal_state_impl::create())
        {
            if ( argc > 0 ) {
                command_line_arguments_.reserve(
                    math::numeric_cast<std::size_t>(argc));
                for ( int i = 0; i < argc; ++i ) {
                    command_line_arguments_.emplace_back(argv[i]);
                }
            }
        }
        ~internal_state() noexcept = default;
    public:
        const vector<str>& command_line_arguments() const noexcept {
            return command_line_arguments_;
        }
    private:
        vector<str> command_line_arguments_;
        platform_internal_state_impl_uptr impl_;
    };

    //
    // platform
    //

    platform::platform(int argc, char *argv[])
    : state_(new internal_state(argc, argv)) {}
    platform::~platform() noexcept = default;

    std::size_t platform::command_line_argument_count() const noexcept {
        return state_->command_line_arguments().size();
    }

    const str& platform::command_line_argument(std::size_t index) const noexcept {
        E2D_ASSERT(index < state_->command_line_arguments().size());
        return state_->command_line_arguments()[index];
    }
}
