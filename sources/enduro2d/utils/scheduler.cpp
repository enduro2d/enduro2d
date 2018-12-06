/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/utils/scheduler.hpp>

namespace
{
    using namespace e2d;
}

namespace e2d
{
    scheduler::scheduler() = default;
    scheduler::~scheduler() noexcept = default;

    void scheduler::process_all_tasks() noexcept {
        while ( active_task_count_ ) {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            process_task_(std::move(lock));
        }
    }

    void scheduler::push_task_(priority priority, task_ptr task) {
        tasks_.emplace_back(priority, std::move(task));
        std::push_heap(tasks_.begin(), tasks_.end());
        ++active_task_count_;
    }

    scheduler::task_ptr scheduler::pop_task_() noexcept {
        if ( !tasks_.empty() ) {
            std::pop_heap(tasks_.begin(), tasks_.end());
            task_ptr task = std::move(tasks_.back().second);
            tasks_.pop_back();
            return task;
        }
        return nullptr;
    }

    void scheduler::process_task_(std::unique_lock<std::mutex> lock) noexcept {
        E2D_ASSERT(lock.owns_lock());
        task_ptr task = pop_task_();
        if ( task ) {
            lock.unlock();
            task->run();
            --active_task_count_;
        }
    }
}
