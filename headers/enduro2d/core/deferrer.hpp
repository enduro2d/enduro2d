/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#pragma once

#include "_core.hpp"

namespace e2d
{
    class deferrer final : public module<deferrer> {
    public:
        deferrer();
        ~deferrer() noexcept final;

        stdex::jobber& worker() noexcept;
        const stdex::jobber& worker() const noexcept;

        stdex::scheduler& scheduler() noexcept;
        const stdex::scheduler& scheduler() const noexcept;

        template < typename F
                 , typename... Args
                 , typename R = stdex::scheduler::schedule_invoke_result_t<F, Args...> >
        stdex::promise<R> do_in_main_thread(F&& f, Args&&... args);

        template < typename F
                 , typename... Args
                 , typename R = stdex::jobber::async_invoke_result_t<F, Args...> >
        stdex::promise<R> do_in_worker_thread(F&& f, Args&&... args);
    private:
        stdex::jobber worker_;
        stdex::scheduler scheduler_;
    };
}

namespace e2d
{
    template < typename F , typename... Args , typename R >
    stdex::promise<R> deferrer::do_in_main_thread(F&& f, Args&&... args) {
        return scheduler_.schedule(std::forward<F>(f), std::forward<Args>(args)...);
    }

    template < typename F , typename... Args , typename R >
    stdex::promise<R> deferrer::do_in_worker_thread(F&& f, Args&&... args) {
        return worker_.async(std::forward<F>(f), std::forward<Args>(args)...);
    }
}
