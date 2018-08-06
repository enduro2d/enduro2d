/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/utils/jobber.hpp>

namespace e2d
{
    jobber::jobber(u32 threads) {
        try {
            threads_.resize(math::max(1u, threads));
            for ( std::thread& thread : threads_ ) {
                thread = std::thread(&jobber::worker_main_, this);
            }
        } catch (...) {
            shutdown_();
            throw;
        }
    }

    jobber::~jobber() noexcept {
        shutdown_();
    }

    void jobber::pause() noexcept {
        paused_.store(true);
    }

    void jobber::resume() noexcept {
        paused_.store(false);
        {
            std::lock_guard<std::mutex> guard(tasks_mutex_);
            cond_var_.notify_all();
        }
    }

    bool jobber::is_paused() const noexcept {
        return paused_;
    }

    void jobber::wait_all() const noexcept {
        while ( active_task_count_ ) {
            std::this_thread::yield();
        }
    }

    void jobber::active_wait_all() noexcept {
        while ( active_task_count_ ) {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            process_task_(std::move(lock));
        }
    }

    void jobber::push_task_(priority priority, task_ptr task) {
        tasks_.emplace_back(priority, std::move(task));
        std::push_heap(tasks_.begin(), tasks_.end());
        ++active_task_count_;
        cond_var_.notify_one();
    }

    jobber::task_ptr jobber::pop_task_() noexcept {
        if ( !tasks_.empty() ) {
            std::pop_heap(tasks_.begin(), tasks_.end());
            task_ptr task = std::move(tasks_.back().second);
            tasks_.pop_back();
            return task;
        }
        return nullptr;
    }

    void jobber::shutdown_() noexcept {
        cancelled_.store(true);
        {
            std::lock_guard<std::mutex> guard(tasks_mutex_);
            cond_var_.notify_all();
        }
        for ( std::thread& thread : threads_ ) {
            if ( thread.joinable() ) {
                thread.join();
            }
        }
    }

    void jobber::worker_main_() noexcept {
        for (;;) {
            std::unique_lock<std::mutex> lock(tasks_mutex_);
            cond_var_.wait(lock, [this](){
                return cancelled_ || (!paused_ && !tasks_.empty());
            });
            if ( cancelled_ ) {
                break;
            }
            process_task_(std::move(lock));
        }
    }

    void jobber::process_task_(std::unique_lock<std::mutex> lock) noexcept {
        E2D_ASSERT(lock.owns_lock());
        task_ptr task = pop_task_();
        if ( task ) {
            lock.unlock();
            task->run();
            --active_task_count_;
        }
    }
}
