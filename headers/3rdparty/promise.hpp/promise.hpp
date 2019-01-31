/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/promise.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cstdint>
#include <cassert>

#include <new>
#include <tuple>
#include <mutex>
#include <atomic>
#include <chrono>
#include <memory>
#include <vector>
#include <utility>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <functional>
#include <type_traits>
#include <condition_variable>

#include "invoke.hpp"

namespace promise_hpp
{
    //
    // forward declaration
    //

    template < typename T = void >
    class promise;

    //
    // is_promise
    //

    namespace impl
    {
        template < typename T >
        struct is_promise_impl
        : std::false_type {};

        template < typename R >
        struct is_promise_impl<promise<R>>
        : std::true_type {};
    }

    template < typename T >
    struct is_promise
    : impl::is_promise_impl<std::remove_cv_t<T>> {};

    //
    // is_promise_r
    //

    namespace impl
    {
        template < typename R, typename T >
        struct is_promise_r_impl
        : std::false_type {};

        template < typename R, typename PR >
        struct is_promise_r_impl<R, promise<PR>>
        : std::is_convertible<PR, R> {};
    }

    template < typename R, typename T >
    struct is_promise_r
    : impl::is_promise_r_impl<R, std::remove_cv_t<T>> {};

    //
    // detail
    //

    namespace detail
    {
        class noncopyable {
        public:
            noncopyable(const noncopyable&) = delete;
            noncopyable& operator=(const noncopyable&) = delete;
        protected:
            noncopyable() = default;
            ~noncopyable() = default;
        };

        template < typename T >
        class storage final : private noncopyable {
        public:
            storage() = default;

            ~storage()
            noexcept(std::is_nothrow_destructible<T>::value)
            {
                if ( initialized_ ) {
                    ptr_()->~T();
                }
            }

            template < typename U >
            void set(U&& value)
            noexcept(std::is_nothrow_constructible<T,U&&>::value)
            {
                assert(!initialized_);
                ::new(ptr_()) T(std::forward<U>(value));
                initialized_ = true;
            }

            T get()
            noexcept(std::is_nothrow_move_constructible<T>::value)
            {
                assert(initialized_);
                return std::move(*ptr_());
            }

            const T& value() const noexcept {
                assert(initialized_);
                return *ptr_();
            }
        private:
            T* ptr_() noexcept {
                return reinterpret_cast<T*>(&data_);
            }

            const T* ptr_() const noexcept {
                return reinterpret_cast<const T*>(&data_);
            }
        private:
            std::aligned_storage_t<sizeof(T), alignof(T)> data_;
            bool initialized_ = false;
        };
    }

    //
    // promise_wait_status
    //

    enum class promise_wait_status {
        no_timeout,
        timeout
    };

    //
    // promise<T>
    //

    template < typename T >
    class promise final {
    public:
        using value_type = T;
    public:
        promise()
        : state_(std::make_shared<state>()) {}

        promise(const promise&) noexcept = default;
        promise& operator=(const promise&) noexcept = default;

        void swap(promise& other) noexcept {
            state_.swap(other.state_);
        }

        std::size_t hash() const noexcept {
            return std::hash<state*>()(state_.get());
        }

        friend bool operator<(const promise& l, const promise& r) noexcept {
            return l.state_ < r.state_;
        }

        friend bool operator==(const promise& l, const promise& r) noexcept {
            return l.state_ == r.state_;
        }

        friend bool operator!=(const promise& l, const promise& r) noexcept {
            return l.state_ != r.state_;
        }

        template < typename ResolveF
                 , typename ResolveFR = invoke_hpp::invoke_result_t<ResolveF,T> >
        std::enable_if_t<
            is_promise<ResolveFR>::value && std::is_void<typename ResolveFR::value_type>::value,
            promise<typename ResolveFR::value_type>>
        then(ResolveF&& on_resolve) {
            promise<typename ResolveFR::value_type> next;

            then([
                n = next,
                f = std::forward<ResolveF>(on_resolve)
            ](auto&& v) mutable {
                auto np = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f),
                    std::forward<decltype(v)>(v));
                std::move(np).then([n]() mutable {
                    n.resolve();
                }).except([n](std::exception_ptr e) mutable {
                    n.reject(e);
                });
            }).except([n = next](std::exception_ptr e) mutable {
                n.reject(e);
            });

            return next;
        }

        template < typename ResolveF
                 , typename ResolveFR = invoke_hpp::invoke_result_t<ResolveF,T> >
        std::enable_if_t<
            is_promise<ResolveFR>::value && !std::is_void<typename ResolveFR::value_type>::value,
            promise<typename ResolveFR::value_type>>
        then(ResolveF&& on_resolve) {
            promise<typename ResolveFR::value_type> next;

            then([
                n = next,
                f = std::forward<ResolveF>(on_resolve)
            ](auto&& v) mutable {
                auto np = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f),
                    std::forward<decltype(v)>(v));
                std::move(np).then([n](auto&& nv) mutable {
                    n.resolve(std::forward<decltype(nv)>(nv));
                }).except([n](std::exception_ptr e) mutable {
                    n.reject(e);
                });
            }).except([n = next](std::exception_ptr e) mutable {
                n.reject(e);
            });

            return next;
        }

        template < typename ResolveF >
        auto then_all(ResolveF&& on_resolve) {
            return then([
                f = std::forward<ResolveF>(on_resolve)
            ](auto&& v) mutable {
                auto r = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f),
                    std::forward<decltype(v)>(v));
                return make_all_promise(std::move(r));
            });
        }

        template < typename ResolveF >
        auto then_any(ResolveF&& on_resolve) {
            return then([
                f = std::forward<ResolveF>(on_resolve)
            ](auto&& v) mutable {
                auto r = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f),
                    std::forward<decltype(v)>(v));
                return make_any_promise(std::move(r));
            });
        }

        template < typename ResolveF >
        auto then_tuple(ResolveF&& on_resolve) {
            return then([
                f = std::forward<ResolveF>(on_resolve)
            ](auto&& v) mutable {
                auto r = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f),
                    std::forward<decltype(v)>(v));
                return make_tuple_promise(std::move(r));
            });
        }

        template < typename ResolveF
                 , typename RejectF
                 , typename ResolveFR = invoke_hpp::invoke_result_t<ResolveF,T> >
        std::enable_if_t<
            !is_promise<ResolveFR>::value,
            promise<ResolveFR>>
        then(ResolveF&& on_resolve, RejectF&& on_reject) {
            promise<ResolveFR> next;
            state_->attach(
                next,
                std::forward<ResolveF>(on_resolve),
                std::forward<RejectF>(on_reject),
                true);
            return next;
        }

        template < typename ResolveF
                 , typename ResolveFR = invoke_hpp::invoke_result_t<ResolveF,T> >
        std::enable_if_t<
            !is_promise<ResolveFR>::value,
            promise<ResolveFR>>
        then(ResolveF&& on_resolve) {
            promise<ResolveFR> next;
            state_->attach(
                next,
                std::forward<ResolveF>(on_resolve),
                [](std::exception_ptr e) -> ResolveFR {
                    std::rethrow_exception(e);
                },
                false);
            return next;
        }

        template < typename RejectF >
        promise<T> except(RejectF&& on_reject) {
            return then(
                [](auto&& v) { return std::forward<decltype(v)>(v); },
                std::forward<RejectF>(on_reject));
        }

        template < typename U >
        bool resolve(U&& value) {
            return state_->resolve(std::forward<U>(value));
        }

        bool reject(std::exception_ptr e) noexcept {
            return state_->reject(e);
        }

        template < typename E >
        bool reject(E&& e) {
            return state_->reject(
                std::make_exception_ptr(std::forward<E>(e)));
        }

        const T& get() const {
            return state_->get();
        }

        const T& get_or_default(const T& def) const noexcept {
            try {
                return get();
            } catch (...) {
                return def;
            }
        }

        void wait() const noexcept {
            state_->wait();
        }

        template < typename Rep, typename Period >
        promise_wait_status wait_for(
            const std::chrono::duration<Rep, Period>& timeout_duration) const
        {
            return state_->wait_for(timeout_duration);
        }

        template < typename Clock, typename Duration >
        promise_wait_status wait_until(
            const std::chrono::time_point<Clock, Duration>& timeout_time) const
        {
            return state_->wait_until(timeout_time);
        }
    private:
        class state;
        std::shared_ptr<state> state_;
    private:
        class state final : private detail::noncopyable {
        public:
            state() = default;

            template < typename U >
            bool resolve(U&& value) {
                std::lock_guard<std::mutex> guard(mutex_);
                if ( status_ != status::pending ) {
                    return false;
                }
                storage_.set(std::forward<U>(value));
                status_ = status::resolved;
                invoke_resolve_handlers_();
                cond_var_.notify_all();
                return true;
            }

            bool reject(std::exception_ptr e) noexcept {
                std::lock_guard<std::mutex> guard(mutex_);
                if ( status_ != status::pending ) {
                    return false;
                }
                exception_ = e;
                status_ = status::rejected;
                invoke_reject_handlers_();
                cond_var_.notify_all();
                return true;
            }

            const T& get() {
                std::unique_lock<std::mutex> lock(mutex_);
                cond_var_.wait(lock, [this](){
                    return status_ != status::pending;
                });
                if ( status_ == status::rejected ) {
                    std::rethrow_exception(exception_);
                }
                assert(status_ == status::resolved);
                return storage_.value();
            }

            void wait() const noexcept {
                std::unique_lock<std::mutex> lock(mutex_);
                cond_var_.wait(lock, [this](){
                    return status_ != status::pending;
                });
            }

            template < typename Rep, typename Period >
            promise_wait_status wait_for(
                const std::chrono::duration<Rep, Period>& timeout_duration) const
            {
                std::unique_lock<std::mutex> lock(mutex_);
                return cond_var_.wait_for(lock, timeout_duration, [this](){
                    return status_ != status::pending;
                })  ? promise_wait_status::no_timeout
                    : promise_wait_status::timeout;
            }

            template < typename Clock, typename Duration >
            promise_wait_status wait_until(
                const std::chrono::time_point<Clock, Duration>& timeout_time) const
            {
                std::unique_lock<std::mutex> lock(mutex_);
                return cond_var_.wait_until(lock, timeout_time, [this](){
                    return status_ != status::pending;
                })  ? promise_wait_status::no_timeout
                    : promise_wait_status::timeout;
            }

            template < typename U, typename ResolveF, typename RejectF >
            std::enable_if_t<std::is_void<U>::value, void>
            attach(
                promise<U>& next,
                ResolveF&& on_resolve,
                RejectF&& on_reject,
                bool has_reject)
            {
                auto reject_h = [
                    n = next,
                    f = std::forward<RejectF>(on_reject),
                    has_reject
                ](std::exception_ptr e) mutable {
                    if ( has_reject ) {
                        try {
                            invoke_hpp::invoke(
                                std::forward<decltype(f)>(f),
                                e);
                            n.resolve();
                        } catch (...) {
                            n.reject(std::current_exception());
                        }
                    } else {
                        n.reject(e);
                    }
                };

                auto resolve_h = [
                    n = next,
                    f = std::forward<ResolveF>(on_resolve)
                ](auto&& v) mutable {
                    try {
                        invoke_hpp::invoke(
                            std::forward<decltype(f)>(f),
                            std::forward<decltype(v)>(v));
                        n.resolve();
                    } catch (...) {
                        n.reject(std::current_exception());
                    }
                };

                std::lock_guard<std::mutex> guard(mutex_);
                add_handlers_(std::move(resolve_h), std::move(reject_h));
            }

            template < typename U, typename ResolveF, typename RejectF >
            std::enable_if_t<!std::is_void<U>::value, void>
            attach(
                promise<U>& next,
                ResolveF&& on_resolve,
                RejectF&& on_reject,
                bool has_reject)
            {
                auto reject_h = [
                    n = next,
                    f = std::forward<RejectF>(on_reject),
                    has_reject
                ](std::exception_ptr e) mutable {
                    if ( has_reject ) {
                        try {
                            auto r = invoke_hpp::invoke(
                                std::forward<decltype(f)>(f),
                                e);
                            n.resolve(std::move(r));
                        } catch (...) {
                            n.reject(std::current_exception());
                        }
                    } else {
                        n.reject(e);
                    }
                };

                auto resolve_h = [
                    n = next,
                    f = std::forward<ResolveF>(on_resolve)
                ](auto&& v) mutable {
                    try {
                        auto r = invoke_hpp::invoke(
                            std::forward<decltype(f)>(f),
                            std::forward<decltype(v)>(v));
                        n.resolve(std::move(r));
                    } catch (...) {
                        n.reject(std::current_exception());
                    }
                };

                std::lock_guard<std::mutex> guard(mutex_);
                add_handlers_(std::move(resolve_h), std::move(reject_h));
            }
        private:
            template < typename ResolveF, typename RejectF >
            void add_handlers_(ResolveF&& resolve, RejectF&& reject) {
                if ( status_ == status::resolved ) {
                    invoke_hpp::invoke(
                        std::forward<ResolveF>(resolve),
                        storage_.value());
                } else if ( status_ == status::rejected ) {
                    invoke_hpp::invoke(
                        std::forward<RejectF>(reject),
                        exception_);
                } else {
                    handlers_.emplace_back(
                        std::forward<ResolveF>(resolve),
                        std::forward<RejectF>(reject));
                }
            }

            void invoke_resolve_handlers_() noexcept {
                const T& value = storage_.value();
                for ( const auto& h : handlers_ ) {
                    h.resolve_(value);
                }
                handlers_.clear();
            }

            void invoke_reject_handlers_() noexcept {
                for ( const auto& h : handlers_ ) {
                    h.reject_(exception_);
                }
                handlers_.clear();
            }
        private:
            enum class status {
                pending,
                resolved,
                rejected
            };

            status status_{status::pending};
            std::exception_ptr exception_{nullptr};

            mutable std::mutex mutex_;
            mutable std::condition_variable cond_var_;

            struct handler {
                using resolve_t = std::function<void(const T&)>;
                using reject_t = std::function<void(std::exception_ptr)>;

                resolve_t resolve_;
                reject_t reject_;

                template < typename ResolveF, typename RejectF >
                handler(ResolveF&& resolve, RejectF&& reject)
                : resolve_(std::forward<ResolveF>(resolve))
                , reject_(std::forward<RejectF>(reject)) {}
            };

            std::vector<handler> handlers_;
            detail::storage<T> storage_;
        };
    };

    //
    // promise<void>
    //

    template <>
    class promise<void> final {
    public:
        using value_type = void;
    public:
        promise()
        : state_(std::make_shared<state>()) {}

        promise(const promise&) noexcept = default;
        promise& operator=(const promise&) noexcept = default;

        void swap(promise& other) noexcept {
            state_.swap(other.state_);
        }

        std::size_t hash() const noexcept {
            return std::hash<state*>()(state_.get());
        }

        friend bool operator<(const promise& l, const promise& r) noexcept {
            return l.state_ < r.state_;
        }

        friend bool operator==(const promise& l, const promise& r) noexcept {
            return l.state_ == r.state_;
        }

        friend bool operator!=(const promise& l, const promise& r) noexcept {
            return l.state_ != r.state_;
        }

        template < typename ResolveF
                 , typename ResolveFR = invoke_hpp::invoke_result_t<ResolveF> >
        std::enable_if_t<
            is_promise<ResolveFR>::value && std::is_void<typename ResolveFR::value_type>::value,
            promise<typename ResolveFR::value_type>>
        then(ResolveF&& on_resolve) {
            promise<typename ResolveFR::value_type> next;

            then([
                n = next,
                f = std::forward<ResolveF>(on_resolve)
            ]() mutable {
                auto np = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f));
                std::move(np).then([n]() mutable {
                    n.resolve();
                }).except([n](std::exception_ptr e) mutable {
                    n.reject(e);
                });
            }).except([n = next](std::exception_ptr e) mutable {
                n.reject(e);
            });

            return next;
        }

        template < typename ResolveF
                 , typename ResolveFR = invoke_hpp::invoke_result_t<ResolveF> >
        std::enable_if_t<
            is_promise<ResolveFR>::value && !std::is_void<typename ResolveFR::value_type>::value,
            promise<typename ResolveFR::value_type>>
        then(ResolveF&& on_resolve) {
            promise<typename ResolveFR::value_type> next;

            then([
                n = next,
                f = std::forward<ResolveF>(on_resolve)
            ]() mutable {
                auto np = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f));
                std::move(np).then([n](auto&& nv) mutable {
                    n.resolve(std::forward<decltype(nv)>(nv));
                }).except([n](std::exception_ptr e) mutable {
                    n.reject(e);
                });
            }).except([n = next](std::exception_ptr e) mutable {
                n.reject(e);
            });

            return next;
        }

        template < typename ResolveF >
        auto then_all(ResolveF&& on_resolve) {
            return then([
                f = std::forward<ResolveF>(on_resolve)
            ]() mutable {
                auto r = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f));
                return make_all_promise(std::move(r));
            });
        }

        template < typename ResolveF >
        auto then_any(ResolveF&& on_resolve) {
            return then([
                f = std::forward<ResolveF>(on_resolve)
            ]() mutable {
                auto r = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f));
                return make_any_promise(std::move(r));
            });
        }

        template < typename ResolveF >
        auto then_tuple(ResolveF&& on_resolve) {
            return then([
                f = std::forward<ResolveF>(on_resolve)
            ]() mutable {
                auto r = invoke_hpp::invoke(
                    std::forward<decltype(f)>(f));
                return make_tuple_promise(std::move(r));
            });
        }

        template < typename ResolveF
                 , typename RejectF
                 , typename ResolveFR = invoke_hpp::invoke_result_t<ResolveF> >
        std::enable_if_t<
            !is_promise<ResolveFR>::value,
            promise<ResolveFR>>
        then(ResolveF&& on_resolve, RejectF&& on_reject) {
            promise<ResolveFR> next;
            state_->attach(
                next,
                std::forward<ResolveF>(on_resolve),
                std::forward<RejectF>(on_reject),
                true);
            return next;
        }

        template < typename ResolveF
                 , typename ResolveFR = invoke_hpp::invoke_result_t<ResolveF> >
        std::enable_if_t<
            !is_promise<ResolveFR>::value,
            promise<ResolveFR>>
        then(ResolveF&& on_resolve) {
            promise<ResolveFR> next;
            state_->attach(
                next,
                std::forward<ResolveF>(on_resolve),
                [](std::exception_ptr e) -> ResolveFR {
                    std::rethrow_exception(e);
                },
                false);
            return next;
        }

        template < typename RejectF >
        promise<void> except(RejectF&& on_reject) {
            return then(
                []{},
                std::forward<RejectF>(on_reject));
        }

        bool resolve() {
            return state_->resolve();
        }

        bool reject(std::exception_ptr e) noexcept {
            return state_->reject(e);
        }

        template < typename E >
        bool reject(E&& e) {
            return state_->reject(
                std::make_exception_ptr(std::forward<E>(e)));
        }

        void get() const {
            state_->get();
        }

        void get_or_default() const noexcept {
            try {
                return get();
            } catch (...) {
                // nothing
            }
        }

        void wait() const noexcept {
            state_->wait();
        }

        template < typename Rep, typename Period >
        promise_wait_status wait_for(
            const std::chrono::duration<Rep, Period>& timeout_duration) const
        {
            return state_->wait_for(timeout_duration);
        }

        template < typename Clock, typename Duration >
        promise_wait_status wait_until(
            const std::chrono::time_point<Clock, Duration>& timeout_time) const
        {
            return state_->wait_until(timeout_time);
        }
    private:
        class state;
        std::shared_ptr<state> state_;
    private:
        class state final : private detail::noncopyable {
        public:
            state() = default;

            bool resolve() {
                std::lock_guard<std::mutex> guard(mutex_);
                if ( status_ != status::pending ) {
                    return false;
                }
                status_ = status::resolved;
                invoke_resolve_handlers_();
                cond_var_.notify_all();
                return true;
            }

            bool reject(std::exception_ptr e) noexcept {
                std::lock_guard<std::mutex> guard(mutex_);
                if ( status_ != status::pending ) {
                    return false;
                }
                exception_ = e;
                status_ = status::rejected;
                invoke_reject_handlers_();
                cond_var_.notify_all();
                return true;
            }

            void get() {
                std::unique_lock<std::mutex> lock(mutex_);
                cond_var_.wait(lock, [this](){
                    return status_ != status::pending;
                });
                if ( status_ == status::rejected ) {
                    std::rethrow_exception(exception_);
                }
                assert(status_ == status::resolved);
            }

            void wait() const noexcept {
                std::unique_lock<std::mutex> lock(mutex_);
                cond_var_.wait(lock, [this](){
                    return status_ != status::pending;
                });
            }

            template < typename Rep, typename Period >
            promise_wait_status wait_for(
                const std::chrono::duration<Rep, Period>& timeout_duration) const
            {
                std::unique_lock<std::mutex> lock(mutex_);
                return cond_var_.wait_for(lock, timeout_duration, [this](){
                    return status_ != status::pending;
                })  ? promise_wait_status::no_timeout
                    : promise_wait_status::timeout;
            }

            template < typename Clock, typename Duration >
            promise_wait_status wait_until(
                const std::chrono::time_point<Clock, Duration>& timeout_time) const
            {
                std::unique_lock<std::mutex> lock(mutex_);
                return cond_var_.wait_until(lock, timeout_time, [this](){
                    return status_ != status::pending;
                })  ? promise_wait_status::no_timeout
                    : promise_wait_status::timeout;
            }

            template < typename U, typename ResolveF, typename RejectF >
            std::enable_if_t<std::is_void<U>::value, void>
            attach(
                promise<U>& next,
                ResolveF&& on_resolve,
                RejectF&& on_reject,
                bool has_reject)
            {
                auto reject_h = [
                    n = next,
                    f = std::forward<RejectF>(on_reject),
                    has_reject
                ](std::exception_ptr e) mutable {
                    if ( has_reject ) {
                        try {
                            invoke_hpp::invoke(
                                std::forward<decltype(f)>(f),
                                e);
                            n.resolve();
                        } catch (...) {
                            n.reject(std::current_exception());
                        }
                    } else {
                        n.reject(e);
                    }
                };

                auto resolve_h = [
                    n = next,
                    f = std::forward<ResolveF>(on_resolve)
                ]() mutable {
                    try {
                        invoke_hpp::invoke(
                            std::forward<decltype(f)>(f));
                        n.resolve();
                    } catch (...) {
                        n.reject(std::current_exception());
                    }
                };

                std::lock_guard<std::mutex> guard(mutex_);
                add_handlers_(std::move(resolve_h), std::move(reject_h));
            }

            template < typename U, typename ResolveF, typename RejectF >
            std::enable_if_t<!std::is_void<U>::value, void>
            attach(
                promise<U>& next,
                ResolveF&& on_resolve,
                RejectF&& on_reject,
                bool has_reject)
            {
                auto reject_h = [
                    n = next,
                    f = std::forward<RejectF>(on_reject),
                    has_reject
                ](std::exception_ptr e) mutable {
                    if ( has_reject ) {
                        try {
                            auto r = invoke_hpp::invoke(
                                std::forward<decltype(f)>(f),
                                e);
                            n.resolve(std::move(r));
                        } catch (...) {
                            n.reject(std::current_exception());
                        }
                    } else {
                        n.reject(e);
                    }
                };

                auto resolve_h = [
                    n = next,
                    f = std::forward<ResolveF>(on_resolve)
                ]() mutable {
                    try {
                        auto r = invoke_hpp::invoke(
                            std::forward<decltype(f)>(f));
                        n.resolve(std::move(r));
                    } catch (...) {
                        n.reject(std::current_exception());
                    }
                };

                std::lock_guard<std::mutex> guard(mutex_);
                add_handlers_(std::move(resolve_h), std::move(reject_h));
            }
        private:
            template < typename ResolveF, typename RejectF >
            void add_handlers_(ResolveF&& resolve, RejectF&& reject) {
                if ( status_ == status::resolved ) {
                    invoke_hpp::invoke(
                        std::forward<ResolveF>(resolve));
                } else if ( status_ == status::rejected ) {
                    invoke_hpp::invoke(
                        std::forward<RejectF>(reject),
                        exception_);
                } else {
                    handlers_.emplace_back(
                        std::forward<ResolveF>(resolve),
                        std::forward<RejectF>(reject));
                }
            }

            void invoke_resolve_handlers_() noexcept {
                for ( const auto& h : handlers_ ) {
                    h.resolve_();
                }
                handlers_.clear();
            }

            void invoke_reject_handlers_() noexcept {
                for ( const auto& h : handlers_ ) {
                    h.reject_(exception_);
                }
                handlers_.clear();
            }
        private:
            enum class status {
                pending,
                resolved,
                rejected
            };

            status status_{status::pending};
            std::exception_ptr exception_{nullptr};

            mutable std::mutex mutex_;
            mutable std::condition_variable cond_var_;

            struct handler {
                using resolve_t = std::function<void()>;
                using reject_t = std::function<void(std::exception_ptr)>;

                resolve_t resolve_;
                reject_t reject_;

                template < typename ResolveF, typename RejectF >
                handler(ResolveF&& resolve, RejectF&& reject)
                : resolve_(std::forward<ResolveF>(resolve))
                , reject_(std::forward<RejectF>(reject)) {}
            };

            std::vector<handler> handlers_;
        };
    };

    //
    // swap
    //

    template < typename T >
    void swap(promise<T>& l, promise<T>& r) noexcept {
        l.swap(r);
    }

    //
    // make_promise
    //

    template < typename R >
    promise<R> make_promise() {
        return promise<R>();
    }

    template < typename R, typename F >
    promise<R> make_promise(F&& f) {
        promise<R> result;

        auto resolver = [result](auto&& v) mutable {
            return result.resolve(std::forward<decltype(v)>(v));
        };

        auto rejector = [result](auto&& e) mutable {
            return result.reject(std::forward<decltype(e)>(e));
        };

        try {
            invoke_hpp::invoke(
                std::forward<F>(f),
                std::move(resolver),
                std::move(rejector));
        } catch (...) {
            result.reject(std::current_exception());
        }

        return result;
    }

    //
    // make_resolved_promise
    //

    inline promise<void> make_resolved_promise() {
        promise<void> result;
        result.resolve();
        return result;
    }

    template < typename R >
    promise<std::decay_t<R>> make_resolved_promise(R&& v) {
        promise<std::decay_t<R>> result;
        result.resolve(std::forward<R>(v));
        return result;
    }

    //
    // make_rejected_promise
    //

    template < typename E >
    promise<void> make_rejected_promise(E&& e) {
        promise<void> result;
        result.reject(std::forward<E>(e));
        return result;
    }

    template < typename R, typename E >
    promise<R> make_rejected_promise(E&& e) {
        promise<R> result;
        result.reject(std::forward<E>(e));
        return result;
    }

    //
    // make_all_promise
    //

    namespace impl
    {
        template < typename ResultType >
        class all_promise_context_t final : private detail::noncopyable {
        public:
            all_promise_context_t(std::size_t count)
            : results_(count) {}

            template < typename T >
            bool apply_result(std::size_t index, T&& value) {
                results_[index].set(std::forward<T>(value));
                return ++counter_ == results_.size();
            }

            std::vector<ResultType> get_results() {
                std::vector<ResultType> ret;
                ret.reserve(results_.size());
                for ( auto&& v : results_ ) {
                    ret.emplace_back(v.get());
                }
                return ret;
            }
        private:
            std::atomic_size_t counter_{0};
            std::vector<detail::storage<ResultType>> results_;
        };
    }

    template < typename Iter
             , typename SubPromise = typename Iter::value_type
             , typename SubPromiseResult = typename SubPromise::value_type
             , typename ResultPromiseValueType = std::vector<SubPromiseResult> >
    promise<ResultPromiseValueType>
    make_all_promise(Iter begin, Iter end) {
        if ( begin == end ) {
            return make_resolved_promise(ResultPromiseValueType());
        }
        return make_promise<ResultPromiseValueType>([begin, end](auto&& resolver, auto&& rejector){
            std::size_t result_index = 0;
            auto context = std::make_shared<impl::all_promise_context_t<
                SubPromiseResult>>(std::distance(begin, end));
            for ( Iter iter = begin; iter != end; ++iter, ++result_index ) {
                (*iter).then([
                    context,
                    resolver,
                    result_index
                ](auto&& v) mutable {
                    if ( context->apply_result(result_index, std::forward<decltype(v)>(v)) ) {
                        resolver(context->get_results());
                    }
                }).except(rejector);
            }
        });
    }

    template < typename Container >
    auto make_all_promise(Container&& container) {
        return make_all_promise(
            std::begin(container),
            std::end(container));
    }

    //
    // make_any_promise
    //

    template < typename Iter
             , typename SubPromise = typename Iter::value_type
             , typename SubPromiseResult = typename SubPromise::value_type >
    auto make_any_promise(Iter begin, Iter end) {
        if ( begin == end ) {
            throw std::logic_error("at least one input promise must be provided for make_any_promise");
        }
        return make_promise<SubPromiseResult>([begin, end](auto&& resolver, auto&& rejector){
            for ( Iter iter = begin; iter != end; ++iter ) {
                (*iter)
                .then(resolver)
                .except(rejector);
            }
        });
    }

    template < typename Container >
    auto make_any_promise(Container&& container) {
        return make_any_promise(
            std::begin(container),
            std::end(container));
    }

    //
    // make_tuple_promise
    //

    namespace impl
    {
        template < typename Tuple >
        struct tuple_promise_result_impl {};

        template < typename... Args >
        struct tuple_promise_result_impl<std::tuple<promise<Args>...>> {
            using type = std::tuple<Args...>;
        };

        template < typename Tuple >
        struct tuple_promise_result {
            using type = typename tuple_promise_result_impl<std::remove_cv_t<Tuple>>::type;
        };

        template < typename Tuple >
        using tuple_promise_result_t = typename tuple_promise_result<Tuple>::type;

        template < typename... ResultTypes >
        class tuple_promise_context_t {
        public:
            template < std::size_t N, typename T >
            bool apply_result(T&& value) {
                std::get<N>(results_).set(std::forward<T>(value));
                return ++counter_ == sizeof...(ResultTypes);
            }

            std::tuple<ResultTypes...> get_results() {
                return get_results_impl(
                    std::make_index_sequence<sizeof...(ResultTypes)>());
            }
        private:
            template < std::size_t... Is >
            std::tuple<ResultTypes...> get_results_impl(std::index_sequence<Is...>) {
                return std::make_tuple(std::get<Is>(results_).get()...);
            }
        private:
            std::atomic_size_t counter_{0};
            std::tuple<detail::storage<ResultTypes>...> results_;
        };

        template < typename... ResultTypes >
        using tuple_promise_context_ptr = std::shared_ptr<
            tuple_promise_context_t<ResultTypes...>>;

        template < std::size_t I
                 , typename Tuple
                 , typename Resolver
                 , typename Rejector
                 , typename... ResultTypes >
        promise<void> make_tuple_sub_promise_impl(
            Tuple&& tuple,
            Resolver&& resolver,
            Rejector&& rejector,
            const tuple_promise_context_ptr<ResultTypes...>& context)
        {
            return std::get<I>(tuple).then([
                context,
                resolver
            ](auto&& v) mutable {
                if (context->template apply_result<I>(std::forward<decltype(v)>(v))) {
                    resolver(context->get_results());
                }
            }).except(rejector);
        }

        template < typename Tuple
                 , std::size_t... Is
                 , typename ResultTuple = tuple_promise_result_t<std::decay_t<Tuple>> >
        std::enable_if_t<
            sizeof...(Is) == 0,
            promise<ResultTuple>>
        make_tuple_promise_impl(Tuple&&, std::index_sequence<Is...>) {
            return make_resolved_promise(ResultTuple());
        }

        template < typename Tuple
                 , std::size_t... Is
                 , typename ResultTuple = tuple_promise_result_t<std::decay_t<Tuple>> >
        std::enable_if_t<
            sizeof...(Is) != 0,
            promise<ResultTuple>>
        make_tuple_promise_impl(Tuple&& tuple, std::index_sequence<Is...>) {
            auto result = promise<ResultTuple>();

            auto resolver = [result](auto&& v) mutable {
                return result.resolve(std::forward<decltype(v)>(v));
            };

            auto rejector = [result](auto&& e) mutable {
                return result.reject(std::forward<decltype(e)>(e));
            };

            try {
                auto context = std::make_shared<tuple_promise_context_t<
                    std::tuple_element_t<Is, ResultTuple>...>>();
                std::make_tuple(make_tuple_sub_promise_impl<Is>(
                    tuple,
                    resolver,
                    rejector,
                    context)...);
            } catch (...) {
                result.reject(std::current_exception());
            }

            return result;
        }
    }

    template < typename Tuple
             , typename ResultTuple = impl::tuple_promise_result_t<std::decay_t<Tuple>> >
    promise<ResultTuple>
    make_tuple_promise(Tuple&& tuple) {
        return impl::make_tuple_promise_impl(
            std::forward<Tuple>(tuple),
            std::make_index_sequence<std::tuple_size<ResultTuple>::value>());
    }
}

namespace std
{
    template < typename T >
    struct hash<promise_hpp::promise<T>>
        : std::unary_function<promise_hpp::promise<T>, std::size_t>
    {
        std::size_t operator()(const promise_hpp::promise<T>& p) const noexcept {
            return p.hash();
        }
    };
}
