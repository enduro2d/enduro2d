/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/ecs.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <tuple>
#include <memory>
#include <vector>
#include <limits>
#include <utility>
#include <iterator>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include <type_traits>

// -----------------------------------------------------------------------------
//
// config
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    class entity;
    class const_entity;

    template < typename T >
    class component;
    template < typename T >
    class const_component;

    class prototype;

    class system;
    class registry;

    class entity_filler;
    class registry_filler;

    using family_id = std::uint16_t;
    using entity_id = std::uint32_t;
    using priority_t = std::int32_t;

    constexpr std::size_t entity_id_index_bits = 22u;
    constexpr std::size_t entity_id_version_bits = 10u;

    static_assert(
        std::is_unsigned<family_id>::value,
        "ecs_hpp (family_id must be an unsigned integer)");

    static_assert(
        std::is_unsigned<entity_id>::value,
        "ecs_hpp (entity_id must be an unsigned integer)");

    static_assert(
        entity_id_index_bits > 0u &&
        entity_id_version_bits > 0u &&
        sizeof(entity_id) == (entity_id_index_bits + entity_id_version_bits) / 8u,
        "ecs_hpp (invalid entity id index and version bits)");
}

// -----------------------------------------------------------------------------
//
// utilities
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    namespace detail
    {
        //
        // as_const
        //

        template < typename T >
        constexpr std::add_const_t<T>& as_const(T& t) noexcept {
            return t;
        }

        //
        // hash_combine
        //

        constexpr std::size_t hash_combine(std::size_t l, std::size_t r) noexcept {
            return l ^ (r + 0x9e3779b9 + (l << 6) + (l >> 2));
        }

        //
        // tuple_tail
        //

        namespace impl
        {
            template < typename T, typename... Ts, std::size_t... Is >
            std::tuple<Ts...> tuple_tail_impl(
                std::tuple<T, Ts...>&& t,
                std::index_sequence<Is...> iseq)
            {
                (void)iseq;
                return std::make_tuple(std::move(std::get<Is + 1u>(t))...);
            }

            template < typename T, typename... Ts, std::size_t... Is >
            std::tuple<Ts...> tuple_tail_impl(
                const std::tuple<T, Ts...>& t,
                std::index_sequence<Is...> iseq)
            {
                (void)iseq;
                return std::make_tuple(std::get<Is + 1u>(t)...);
            }
        }

        template < typename T, typename... Ts >
        std::tuple<Ts...> tuple_tail(std::tuple<T, Ts...>&& t) {
            return impl::tuple_tail_impl(
                std::move(t),
                std::make_index_sequence<sizeof...(Ts)>());
        }

        template < typename T, typename... Ts >
        std::tuple<Ts...> tuple_tail(const std::tuple<T, Ts...>& t) {
            return impl::tuple_tail_impl(
                t,
                std::make_index_sequence<sizeof...(Ts)>());
        }

        //
        // tuple_contains
        //

        namespace impl
        {
            template < size_t I, typename V, typename... Ts >
            std::enable_if_t<I == sizeof...(Ts), bool>
            tuple_contains_impl(const std::tuple<Ts...>& t, const V& v) {
                (void)t;
                (void)v;
                return false;
            }

            template < size_t I, typename V, typename... Ts >
            std::enable_if_t<I != sizeof...(Ts), bool>
            tuple_contains_impl(const std::tuple<Ts...>& t, const V& v) {
                if ( std::get<I>(t) == v ) {
                    return true;
                }
                return tuple_contains_impl<I + 1>(t, v);
            }
        }

        template < typename V, typename... Ts >
        bool tuple_contains(const std::tuple<Ts...>& t, const V& v) {
            return impl::tuple_contains_impl<0>(t, v);
        }

        //
        // tiny_tuple_apply
        //

        namespace impl
        {
            template < typename F, typename Tuple, std::size_t... I >
            void tiny_tuple_apply_impl(F&& f, Tuple&& args, std::index_sequence<I...>) {
                std::forward<F>(f)(std::get<I>(std::forward<Tuple>(args))...);
            }
        }

        template < typename F, typename Tuple >
        void tiny_tuple_apply(F&& f, Tuple&& args) {
            impl::tiny_tuple_apply_impl(
                std::forward<F>(f),
                std::forward<Tuple>(args),
                std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>());
        }

        //
        // next_capacity_size
        //

        inline std::size_t next_capacity_size(
            std::size_t cur_size,
            std::size_t min_size,
            std::size_t max_size)
        {
            if ( min_size > max_size ) {
                throw std::length_error("ecs_hpp::next_capacity_size");
            }
            if ( cur_size >= max_size / 2u ) {
                return max_size;
            }
            return std::max(cur_size * 2u, min_size);
        }

        //
        // entity_id index/version
        //

        constexpr std::size_t entity_id_index_mask = (1u << entity_id_index_bits) - 1u;
        constexpr std::size_t entity_id_version_mask = (1u << entity_id_version_bits) - 1u;

        constexpr inline entity_id entity_id_index(entity_id id) noexcept {
            return id & entity_id_index_mask;
        }

        constexpr inline entity_id entity_id_version(entity_id id) noexcept {
            return (id >> entity_id_index_bits) & entity_id_version_mask;
        }

        constexpr inline entity_id entity_id_join(entity_id index, entity_id version) noexcept {
            return index | (version << entity_id_index_bits);
        }

        constexpr inline entity_id upgrade_entity_id(entity_id id) noexcept {
            return entity_id_join(
                entity_id_index(id),
                entity_id_version(id) + 1u);
        }
    }
}

// -----------------------------------------------------------------------------
//
// detail::type_family
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    namespace detail
    {
        template < typename Void = void >
        class type_family_base {
            static_assert(
                std::is_void<Void>::value,
                "unexpected internal error");
        protected:
            static family_id last_id_;
        };

        template < typename T >
        class type_family final : public type_family_base<> {
        public:
            static family_id id() noexcept {
                static family_id self_id = ++last_id_;
                assert(self_id > 0u && "ecs_hpp::family_id overflow");
                return self_id;
            }
        };

        template < typename Void >
        family_id type_family_base<Void>::last_id_ = 0u;
    }
}

// -----------------------------------------------------------------------------
//
// detail::sparse_indexer
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    namespace detail
    {
        template < typename T
                 , bool = std::is_unsigned<T>::value
                    && sizeof(T) <= sizeof(std::size_t) >
        struct sparse_unsigned_indexer {
            std::size_t operator()(const T v) const noexcept {
                return static_cast<std::size_t>(v);
            }
        };

        template < typename T >
        struct sparse_unsigned_indexer<T, false> {};

        template < typename T >
        struct sparse_indexer final
        : public sparse_unsigned_indexer<T> {};
    }
}

// -----------------------------------------------------------------------------
//
// detail::sparse_set
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    namespace detail
    {
        template < typename T
                 , typename Indexer = sparse_indexer<T> >
        class sparse_set final {
        public:
            using iterator = typename std::vector<T>::iterator;
            using const_iterator = typename std::vector<T>::const_iterator;
        public:
            static_assert(
                noexcept(std::declval<Indexer>()(std::declval<T>())),
                "unsupported sparse_set indexer");
            static_assert(
                std::is_nothrow_destructible<T>::value &&
                std::is_nothrow_move_assignable<T>::value &&
                noexcept(std::declval<T>() == std::declval<T>()),
                "unsupported sparse_set value type");
        public:
            iterator begin() noexcept {
                return dense_.begin();
            }

            iterator end() noexcept {
                return dense_.end();
            }

            const_iterator begin() const noexcept {
                return dense_.begin();
            }

            const_iterator end() const noexcept {
                return dense_.end();
            }

            const_iterator cbegin() const noexcept {
                return dense_.cbegin();
            }

            const_iterator cend() const noexcept {
                return dense_.cend();
            }
        public:
            sparse_set(const Indexer& indexer = Indexer())
            : indexer_(indexer) {}

            sparse_set(const sparse_set& other) = default;
            sparse_set& operator=(const sparse_set& other) = default;

            sparse_set(sparse_set&& other) noexcept = default;
            sparse_set& operator=(sparse_set&& other) noexcept = default;

            void swap(sparse_set& other) noexcept {
                using std::swap;
                swap(dense_, other.dense_);
                swap(sparse_, other.sparse_);
            }

            template < typename UT >
            bool insert(UT&& v) {
                if ( has(v) ) {
                    return false;
                }
                const std::size_t vi = indexer_(v);
                if ( vi >= sparse_.size() ) {
                    sparse_.resize(next_capacity_size(
                        sparse_.size(), vi + 1u, sparse_.max_size()));
                }
                dense_.push_back(std::forward<UT>(v));
                sparse_[vi] = dense_.size() - 1u;
                return true;
            }

            template < typename... Args >
            bool emplace(Args&&... args) {
                return insert(T(std::forward<Args>(args)...));
            }

            bool unordered_erase(const T& v) noexcept {
                if ( !has(v) ) {
                    return false;
                }
                const std::size_t vi = indexer_(v);
                const std::size_t dense_index = sparse_[vi];
                if ( dense_index != dense_.size() - 1 ) {
                    dense_[dense_index] = std::move(dense_.back());
                    sparse_[indexer_(dense_[dense_index])] = dense_index;
                }
                dense_.pop_back();
                return true;
            }

            void clear() noexcept {
                dense_.clear();
            }

            bool has(const T& v) const noexcept {
                const std::size_t vi = indexer_(v);
                return vi < sparse_.size()
                    && sparse_[vi] < dense_.size()
                    && dense_[sparse_[vi]] == v;
            }

            const_iterator find(const T& v) const noexcept {
                return has(v)
                    ? begin() + sparse_[indexer_(v)]
                    : end();
            }

            std::size_t get_dense_index(const T& v) const {
                const auto p = find_dense_index(v);
                if ( p.second ) {
                    return p.first;
                }
                throw std::logic_error("ecs_hpp::sparse_set (value not found)");
            }

            std::pair<std::size_t,bool> find_dense_index(const T& v) const noexcept {
                return has(v)
                    ? std::make_pair(sparse_[indexer_(v)], true)
                    : std::make_pair(std::size_t(-1), false);
            }

            bool empty() const noexcept {
                return dense_.empty();
            }

            std::size_t size() const noexcept {
                return dense_.size();
            }

            std::size_t memory_usage() const noexcept {
                return dense_.capacity() * sizeof(dense_[0])
                    + sparse_.capacity() * sizeof(sparse_[0]);
            }
        private:
            Indexer indexer_;
            std::vector<T> dense_;
            std::vector<std::size_t> sparse_;
        };

        template < typename T
                 , typename Indexer >
        void swap(
            sparse_set<T, Indexer>& l,
            sparse_set<T, Indexer>& r) noexcept
        {
            l.swap(r);
        }
    }
}

// -----------------------------------------------------------------------------
//
// detail::sparse_map
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    namespace detail
    {
        template < typename K
                 , typename T
                 , typename Indexer = sparse_indexer<K> >
        class sparse_map final {
        public:
            using iterator = typename std::vector<K>::iterator;
            using const_iterator = typename std::vector<K>::const_iterator;
        public:
            static_assert(
                std::is_nothrow_destructible<T>::value &&
                std::is_nothrow_move_assignable<T>::value,
                "unsupported sparse_map value type");
        public:
            iterator begin() noexcept {
                return keys_.begin();
            }

            iterator end() noexcept {
                return keys_.end();
            }

            const_iterator begin() const noexcept {
                return keys_.begin();
            }

            const_iterator end() const noexcept {
                return keys_.end();
            }

            const_iterator cbegin() const noexcept {
                return keys_.cbegin();
            }

            const_iterator cend() const noexcept {
                return keys_.cend();
            }
        public:
            sparse_map(const Indexer& indexer = Indexer())
            : keys_(indexer) {}

            sparse_map(const sparse_map& other) = default;
            sparse_map& operator=(const sparse_map& other) = default;

            sparse_map(sparse_map&& other) noexcept = default;
            sparse_map& operator=(sparse_map&& other) noexcept = default;

            void swap(sparse_map& other) noexcept {
                using std::swap;
                swap(keys_, other.keys_);
                swap(values_, other.values_);
            }

            template < typename UK, typename UT >
            bool insert(UK&& k, UT&& v) {
                if ( keys_.has(k) ) {
                    return false;
                }
                values_.push_back(std::forward<UT>(v));
                try {
                    return keys_.insert(std::forward<UK>(k));
                } catch (...) {
                    values_.pop_back();
                    throw;
                }
            }

            template < typename UK, typename... Args >
            bool emplace(UK&& k, Args&&... args) {
                if ( keys_.has(k) ) {
                    return false;
                }
                values_.emplace_back(std::forward<Args>(args)...);
                try {
                    return keys_.insert(std::forward<UK>(k));
                } catch (...) {
                    values_.pop_back();
                    throw;
                }
            }

            template < typename UK, typename UT >
            bool insert_or_assign(UK&& k, UT&& v) {
                if ( keys_.has(k) ) {
                    get(k) = std::forward<UT>(v);
                    return false;
                } else {
                    insert(std::forward<UK>(k), std::forward<UT>(v));
                    return true;
                }
            }

            bool unordered_erase(const K& k) noexcept {
                const auto value_index_p = keys_.find_dense_index(k);
                if ( !value_index_p.second ) {
                    return false;
                }
                if ( value_index_p.first != values_.size() - 1 ) {
                    values_[value_index_p.first] = std::move(values_.back());
                }
                values_.pop_back();
                keys_.unordered_erase(k);
                return true;
            }

            void clear() noexcept {
                keys_.clear();
                values_.clear();
            }

            bool has(const K& k) const noexcept {
                return keys_.has(k);
            }

            T& get(const K& k) {
                return values_[keys_.get_dense_index(k)];
            }

            const T& get(const K& k) const {
                return values_[keys_.get_dense_index(k)];
            }

            T* find(const K& k) noexcept {
                const auto value_index_p = keys_.find_dense_index(k);
                return value_index_p.second
                    ? &values_[value_index_p.first]
                    : nullptr;
            }

            const T* find(const K& k) const noexcept {
                const auto value_index_p = keys_.find_dense_index(k);
                return value_index_p.second
                    ? &values_[value_index_p.first]
                    : nullptr;
            }

            bool empty() const noexcept {
                return values_.empty();
            }

            std::size_t size() const noexcept {
                return values_.size();
            }

            std::size_t memory_usage() const noexcept {
                return keys_.memory_usage()
                    + values_.capacity() * sizeof(values_[0]);
            }
        private:
            sparse_set<K, Indexer> keys_;
            std::vector<T> values_;
        };

        template < typename K
                 , typename T
                 , typename Indexer >
        void swap(
            sparse_map<K, T, Indexer>& l,
            sparse_map<K, T, Indexer>& r) noexcept
        {
            l.swap(r);
        }
    }
}

// -----------------------------------------------------------------------------
//
// detail::entity_id_indexer
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    namespace detail
    {
        struct entity_id_indexer final {
            std::size_t operator()(entity_id id) const noexcept {
                return entity_id_index(id);
            }
        };
    }
}

// -----------------------------------------------------------------------------
//
// detail::component_storage
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    namespace detail
    {
        class component_storage_base {
        public:
            virtual ~component_storage_base() = default;
            virtual bool remove(entity_id id) noexcept = 0;
            virtual bool has(entity_id id) const noexcept = 0;
            virtual void clone(entity_id from, entity_id to) = 0;
            virtual std::size_t memory_usage() const noexcept = 0;
        };

        template < typename T, bool E = std::is_empty<T>::value >
        class component_storage final : public component_storage_base {
        public:
            component_storage(registry& owner)
            : owner_(owner) {}

            template < typename... Args >
            T& assign(entity_id id, Args&&... args) {
                components_.insert_or_assign(id, T(std::forward<Args>(args)...));
                return components_.get(id);
            }

            bool exists(entity_id id) const noexcept {
                return components_.has(id);
            }

            bool remove(entity_id id) noexcept override {
                return components_.unordered_erase(id);
            }

            T* find(entity_id id) noexcept {
                return components_.find(id);
            }

            const T* find(entity_id id) const noexcept {
                return components_.find(id);
            }

            std::size_t count() const noexcept {
                return components_.size();
            }

            bool has(entity_id id) const noexcept override {
                return components_.has(id);
            }

            void clone(entity_id from, entity_id to) override {
                const T* c = components_.find(from);
                if ( c ) {
                    components_.insert_or_assign(to, *c);
                }
            }

            template < typename F >
            void for_each_component(F&& f) {
                for ( const entity_id id : components_ ) {
                    f(id, components_.get(id));
                }
            }

            template < typename F >
            void for_each_component(F&& f) const {
                for ( const entity_id id : components_ ) {
                    f(id, components_.get(id));
                }
            }

            std::size_t memory_usage() const noexcept override {
                return components_.memory_usage();
            }
        private:
            registry& owner_;
            detail::sparse_map<entity_id, T, entity_id_indexer> components_;
        };

        template < typename T >
        class component_storage<T, true> final : public component_storage_base {
        public:
            component_storage(registry& owner)
            : owner_(owner) {}

            template < typename... Args >
            T& assign(entity_id id, Args&&...) {
                components_.insert(id);
                return empty_value_;
            }

            bool exists(entity_id id) const noexcept {
                return components_.has(id);
            }

            bool remove(entity_id id) noexcept override {
                return components_.unordered_erase(id);
            }

            T* find(entity_id id) noexcept {
                return components_.has(id)
                    ? &empty_value_
                    : nullptr;
            }

            const T* find(entity_id id) const noexcept {
                return components_.has(id)
                    ? &empty_value_
                    : nullptr;
            }

            std::size_t count() const noexcept {
                return components_.size();
            }

            bool has(entity_id id) const noexcept override {
                return components_.has(id);
            }

            void clone(entity_id from, entity_id to) override {
                if ( components_.has(from) ) {
                    components_.insert(to);
                }
            }

            template < typename F >
            void for_each_component(F&& f) {
                for ( const entity_id id : components_ ) {
                    f(id, empty_value_);
                }
            }

            template < typename F >
            void for_each_component(F&& f) const {
                for ( const entity_id id : components_ ) {
                    f(id, empty_value_);
                }
            }

            std::size_t memory_usage() const noexcept override {
                return components_.memory_usage();
            }
        private:
            registry& owner_;
            static T empty_value_;
            detail::sparse_set<entity_id, entity_id_indexer> components_;
        };

        template < typename T >
        T component_storage<T, true>::empty_value_;
    }
}

// -----------------------------------------------------------------------------
//
// entity
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    class entity final {
    public:
        entity(registry& owner) noexcept;
        entity(registry& owner, entity_id id) noexcept;

        entity(const entity&) = default;
        entity& operator=(const entity&) = default;

        entity(entity&&) noexcept = default;
        entity& operator=(entity&&) noexcept = default;

        registry& owner() noexcept;
        const registry& owner() const noexcept;

        entity_id id() const noexcept;

        entity clone() const;
        void destroy() noexcept;
        bool valid() const noexcept;

        template < typename T, typename... Args >
        T& assign_component(Args&&... args);

        template < typename T >
        bool remove_component();

        template < typename T >
        bool exists_component() const noexcept;

        std::size_t remove_all_components() noexcept;

        template < typename T >
        T& get_component();

        template < typename T >
        const T& get_component() const;

        template < typename T >
        T* find_component() noexcept;

        template < typename T >
        const T* find_component() const noexcept;

        template < typename... Ts >
        std::tuple<Ts&...> get_components();
        template < typename... Ts >
        std::tuple<const Ts&...> get_components() const;

        template < typename... Ts >
        std::tuple<Ts*...> find_components() noexcept;
        template < typename... Ts >
        std::tuple<const Ts*...> find_components() const noexcept;

        std::size_t component_count() const noexcept;
    private:
        registry* owner_{nullptr};
        entity_id id_{0u};
    };

    bool operator<(const entity& l, const entity& r) noexcept;

    bool operator==(const entity& l, const entity& r) noexcept;
    bool operator==(const entity& l, const const_entity& r) noexcept;

    bool operator!=(const entity& l, const entity& r) noexcept;
    bool operator!=(const entity& l, const const_entity& r) noexcept;
}

namespace std
{
    template <>
    struct hash<ecs_hpp::entity> final
        : std::unary_function<const ecs_hpp::entity&, std::size_t>
    {
        std::size_t operator()(const ecs_hpp::entity& ent) const noexcept {
            return ecs_hpp::detail::hash_combine(
                std::hash<const ecs_hpp::registry*>()(&ent.owner()),
                std::hash<ecs_hpp::entity_id>()(ent.id()));
        }
    };
}

// -----------------------------------------------------------------------------
//
// const_entity
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    class const_entity final {
    public:
        const_entity(const entity& ent) noexcept;

        const_entity(const registry& owner) noexcept;
        const_entity(const registry& owner, entity_id id) noexcept;

        const_entity(const const_entity&) = default;
        const_entity& operator=(const const_entity&) = default;

        const_entity(const_entity&&) noexcept = default;
        const_entity& operator=(const_entity&&) noexcept = default;

        const registry& owner() const noexcept;
        entity_id id() const noexcept;

        bool valid() const noexcept;

        template < typename T >
        bool exists_component() const noexcept;

        template < typename T >
        const T& get_component() const;

        template < typename T >
        const T* find_component() const noexcept;

        template < typename... Ts >
        std::tuple<const Ts&...> get_components() const;

        template < typename... Ts >
        std::tuple<const Ts*...> find_components() const noexcept;

        std::size_t component_count() const noexcept;
    private:
        const registry* owner_{nullptr};
        entity_id id_{0u};
    };

    bool operator<(const const_entity& l, const const_entity& r) noexcept;

    bool operator==(const const_entity& l, const entity& r) noexcept;
    bool operator==(const const_entity& l, const const_entity& r) noexcept;

    bool operator!=(const const_entity& l, const entity& r) noexcept;
    bool operator!=(const const_entity& l, const const_entity& r) noexcept;
}

namespace std
{
    template <>
    struct hash<ecs_hpp::const_entity> final
        : std::unary_function<const ecs_hpp::const_entity&, std::size_t>
    {
        std::size_t operator()(const ecs_hpp::const_entity& ent) const noexcept {
            return ecs_hpp::detail::hash_combine(
                std::hash<const ecs_hpp::registry*>()(&ent.owner()),
                std::hash<ecs_hpp::entity_id>()(ent.id()));
        }
    };
}

// -----------------------------------------------------------------------------
//
// component
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    template < typename T >
    class component final {
    public:
        component(const entity& owner) noexcept;

        component(const component&) = default;
        component& operator=(const component&) = default;

        component(component&&) noexcept = default;
        component& operator=(component&&) noexcept = default;

        entity& owner() noexcept;
        const entity& owner() const noexcept;

        bool remove();
        bool exists() const noexcept;

        template < typename... Args >
        T& assign(Args&&... args);

        T& get();
        const T& get() const;

        T* find() noexcept;
        const T* find() const noexcept;
    private:
        entity owner_;
    };

    template < typename T >
    bool operator<(const component<T>& l, const component<T>& r) noexcept;

    template < typename T >
    bool operator==(const component<T>& l, const component<T>& r) noexcept;
    template < typename T >
    bool operator==(const component<T>& l, const const_component<T>& r) noexcept;

    template < typename T >
    bool operator!=(const component<T>& l, const component<T>& r) noexcept;
    template < typename T >
    bool operator!=(const component<T>& l, const const_component<T>& r) noexcept;
}

namespace std
{
    template < typename T >
    struct hash<ecs_hpp::component<T>>
        : std::unary_function<const ecs_hpp::component<T>&, std::size_t>
    {
        std::size_t operator()(const ecs_hpp::component<T>& comp) const noexcept {
            return std::hash<ecs_hpp::entity>()(comp.owner());
        }
    };
}

// -----------------------------------------------------------------------------
//
// const_component
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    template < typename T >
    class const_component final {
    public:
        const_component(const component<T>& comp) noexcept;
        const_component(const const_entity& owner) noexcept;

        const_component(const const_component&) = default;
        const_component& operator=(const const_component&) = default;

        const_component(const_component&&) noexcept = default;
        const_component& operator=(const_component&&) noexcept = default;

        const const_entity& owner() const noexcept;

        bool exists() const noexcept;

        const T& get() const;
        const T* find() const noexcept;
    private:
        const_entity owner_;
    };

    template < typename T >
    bool operator<(const const_component<T>& l, const const_component<T>& r) noexcept;

    template < typename T >
    bool operator==(const const_component<T>& l, const component<T>& r) noexcept;
    template < typename T >
    bool operator==(const const_component<T>& l, const const_component<T>& r) noexcept;

    template < typename T >
    bool operator!=(const const_component<T>& l, const component<T>& r) noexcept;
    template < typename T >
    bool operator!=(const const_component<T>& l, const const_component<T>& r) noexcept;
}

namespace std
{
    template < typename T >
    struct hash<ecs_hpp::const_component<T>>
        : std::unary_function<const ecs_hpp::const_component<T>&, std::size_t>
    {
        std::size_t operator()(const ecs_hpp::const_component<T>& comp) const noexcept {
            return std::hash<ecs_hpp::const_entity>()(comp.owner());
        }
    };
}

// -----------------------------------------------------------------------------
//
// prototype
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    namespace detail
    {
        class component_applier_base;
        using component_applier_uptr = std::unique_ptr<component_applier_base>;

        class component_applier_base {
        public:
            virtual ~component_applier_base() = default;
            virtual component_applier_uptr clone() const = 0;
            virtual void apply_to(entity& ent, bool override) const = 0;
        };

        template < typename T, typename... Args >
        class component_applier final : public component_applier_base {
        public:
            component_applier(std::tuple<Args...>&& args);
            component_applier(const std::tuple<Args...>& args);
            component_applier_uptr clone() const override;
            void apply_to(entity& ent, bool override) const override;
        private:
            std::tuple<Args...> args_;
        };
    }

    class prototype final {
    public:
        prototype() = default;
        ~prototype() noexcept = default;

        prototype(const prototype& other);
        prototype& operator=(const prototype& other);

        prototype(prototype&& other) noexcept;
        prototype& operator=(prototype&& other) noexcept;

        void clear() noexcept;
        bool empty() const noexcept;
        void swap(prototype& other) noexcept;

        template < typename T >
        bool has_component() const noexcept;

        template < typename T, typename... Args >
        prototype& component(Args&&... args) &;
        template < typename T, typename... Args >
        prototype&& component(Args&&... args) &&;

        prototype& merge_with(const prototype& other, bool override) &;
        prototype&& merge_with(const prototype& other, bool override) &&;

        void apply_to(entity& ent, bool override) const;
    private:
        detail::sparse_map<
            family_id,
            detail::component_applier_uptr> appliers_;
    };

    void swap(prototype& l, prototype& r) noexcept;
}

// -----------------------------------------------------------------------------
//
// system
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    class system {
    public:
        virtual ~system() = default;
        virtual void process(registry& owner) = 0;
    };
}

// -----------------------------------------------------------------------------
//
// registry
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    class registry final {
    private:
        class uentity {
        public:
            uentity(registry& owner, entity_id id) noexcept;

            uentity(entity_id ent) noexcept;
            uentity(entity ent) noexcept;

            operator entity_id() const noexcept;
            operator entity() const noexcept;
            operator const_entity() const noexcept;

            entity_id id() const noexcept;
            registry* owner() noexcept;
            const registry* owner() const noexcept;

            bool check_owner(const registry* owner) const noexcept;
        private:
            entity_id id_{0u};
            registry* owner_{nullptr};
        };

        class const_uentity {
        public:
            const_uentity(const registry& owner, entity_id id) noexcept;

            const_uentity(entity_id ent) noexcept;
            const_uentity(entity ent) noexcept;
            const_uentity(const_entity ent) noexcept;
            const_uentity(const uentity& ent) noexcept;

            operator entity_id() const noexcept;
            operator const_entity() const noexcept;

            entity_id id() const noexcept;
            const registry* owner() const noexcept;

            bool check_owner(const registry* owner) const noexcept;
        private:
            entity_id id_{0u};
            const registry* owner_{nullptr};
        };
    public:
        registry() = default;
        ~registry() noexcept = default;

        registry(const registry& other) = delete;
        registry& operator=(const registry& other) = delete;

        registry(registry&& other) noexcept = default;
        registry& operator=(registry&& other) noexcept = default;

        entity wrap_entity(const const_uentity& ent) noexcept;
        const_entity wrap_entity(const const_uentity& ent) const noexcept;

        template < typename T >
        component<T> wrap_component(const const_uentity& ent) noexcept;
        template < typename T >
        const_component<T> wrap_component(const const_uentity& ent) const noexcept;

        entity create_entity();
        entity create_entity(const prototype& proto);
        entity create_entity(const const_uentity& proto);

        void destroy_entity(const uentity& ent) noexcept;
        bool valid_entity(const const_uentity& ent) const noexcept;

        template < typename T, typename... Args >
        T& assign_component(const uentity& ent, Args&&... args);

        template < typename T >
        bool remove_component(const uentity& ent);

        template < typename T >
        bool exists_component(const const_uentity& ent) const noexcept;

        std::size_t remove_all_components(const uentity& ent) noexcept;

        template < typename T >
        T& get_component(const uentity& ent);
        template < typename T >
        const T& get_component(const const_uentity& ent) const;

        template < typename T >
        T* find_component(const uentity& ent) noexcept;
        template < typename T >
        const T* find_component(const const_uentity& ent) const noexcept;

        template < typename... Ts >
        std::tuple<Ts&...> get_components(const uentity& ent);
        template < typename... Ts >
        std::tuple<const Ts&...> get_components(const const_uentity& ent) const;

        template < typename... Ts >
        std::tuple<Ts*...> find_components(const uentity& ent) noexcept;
        template < typename... Ts >
        std::tuple<const Ts*...> find_components(const const_uentity& ent) const noexcept;

        template < typename T >
        std::size_t component_count() const noexcept;
        std::size_t entity_count() const noexcept;
        std::size_t entity_component_count(const const_uentity& ent) const noexcept;

        template < typename F >
        void for_each_entity(F&& f);
        template < typename F >
        void for_each_entity(F&& f) const;

        template < typename T, typename F >
        void for_each_component(F&& f);
        template < typename T, typename F >
        void for_each_component(F&& f) const;

        template < typename... Ts, typename F >
        void for_joined_components(F&& f);
        template < typename... Ts, typename F >
        void for_joined_components(F&& f) const;

        template < typename T, typename... Args >
        void add_system(priority_t priority, Args&&... args);

        void process_all_systems();
        void process_systems_above(priority_t min);
        void process_systems_below(priority_t max);
        void process_systems_in_range(priority_t min, priority_t max);

        struct memory_usage_info {
            std::size_t entities{0u};
            std::size_t components{0u};
        };
        memory_usage_info memory_usage() const noexcept;

        template < typename T >
        std::size_t component_memory_usage() const noexcept;
    private:
        template < typename T >
        detail::component_storage<T>* find_storage_() noexcept;

        template < typename T >
        const detail::component_storage<T>* find_storage_() const noexcept;

        template < typename T >
        detail::component_storage<T>& get_or_create_storage_();

        template < typename T
                 , typename... Ts
                 , typename F
                 , std::size_t I
                 , std::size_t... Is >
        void for_joined_components_impl_(
            F&& f,
            std::index_sequence<I, Is...> iseq);

        template < typename T
                 , typename... Ts
                 , typename F
                 , std::size_t I
                 , std::size_t... Is >
        void for_joined_components_impl_(
            F&& f,
            std::index_sequence<I, Is...> iseq) const;

        template < typename T
                 , typename... Ts
                 , typename F
                 , typename Ss
                 , typename... Cs >
        void for_joined_components_impl_(
            const uentity& e,
            const F& f,
            const Ss& ss,
            Cs&... cs);

        template < typename T
                 , typename... Ts
                 , typename F
                 , typename Ss
                 , typename... Cs >
        void for_joined_components_impl_(
            const const_uentity& e,
            const F& f,
            const Ss& ss,
            const Cs&... cs) const;

        template < typename F, typename... Cs >
        void for_joined_components_impl_(
            const uentity& e,
            const F& f,
            const std::tuple<>& ss,
            Cs&... cs);

        template < typename F, typename... Cs >
        void for_joined_components_impl_(
            const const_uentity& e,
            const F& f,
            const std::tuple<>& ss,
            const Cs&... cs) const;
    private:
        entity_id last_entity_id_{0u};
        std::vector<entity_id> free_entity_ids_;
        detail::sparse_set<entity_id, detail::entity_id_indexer> entity_ids_;

        using storage_uptr = std::unique_ptr<detail::component_storage_base>;
        detail::sparse_map<family_id, storage_uptr> storages_;

        using system_uptr = std::unique_ptr<system>;
        std::vector<std::pair<priority_t, system_uptr>> systems_;
    };
}

// -----------------------------------------------------------------------------
//
// fillers
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    class entity_filler final {
    public:
        entity_filler(entity& entity) noexcept
        : entity_(entity) {}

        template < typename T, typename... Args >
        entity_filler& component(Args&&... args) {
            entity_.assign_component<T>(std::forward<Args>(args)...);
            return *this;
        }
    private:
        entity& entity_;
    };

    class registry_filler final {
    public:
        registry_filler(registry& registry) noexcept
        : registry_(registry) {}

        template < typename T, typename... Args >
        registry_filler& system(priority_t priority, Args&&... args) {
            registry_.add_system<T>(
                priority,
                std::forward<Args>(args)...);
            return *this;
        }
    private:
        registry& registry_;
    };
}

// -----------------------------------------------------------------------------
//
// entity impl
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    inline entity::entity(registry& owner) noexcept
    : owner_(&owner) {}

    inline entity::entity(registry& owner, entity_id id) noexcept
    : owner_(&owner)
    , id_(id) {}

    inline registry& entity::owner() noexcept {
        return *owner_;
    }

    inline const registry& entity::owner() const noexcept {
        return *owner_;
    }

    inline entity_id entity::id() const noexcept {
        return id_;
    }

    inline entity entity::clone() const {
        return (*owner_).create_entity(id_);
    }

    inline void entity::destroy() noexcept {
        (*owner_).destroy_entity(id_);
    }

    inline bool entity::valid() const noexcept {
        return detail::as_const(*owner_).valid_entity(id_);
    }

    template < typename T, typename... Args >
    T& entity::assign_component(Args&&... args) {
        return (*owner_).assign_component<T>(
            id_,
            std::forward<Args>(args)...);
    }

    template < typename T >
    bool entity::remove_component() {
        return (*owner_).remove_component<T>(id_);
    }

    template < typename T >
    bool entity::exists_component() const noexcept {
        return detail::as_const(*owner_).exists_component<T>(id_);
    }

    inline std::size_t entity::remove_all_components() noexcept {
        return (*owner_).remove_all_components(id_);
    }

    template < typename T >
    T& entity::get_component() {
        return (*owner_).get_component<T>(id_);
    }

    template < typename T >
    const T& entity::get_component() const {
        return detail::as_const(*owner_).get_component<T>(id_);
    }

    template < typename T >
    T* entity::find_component() noexcept {
        return (*owner_).find_component<T>(id_);
    }

    template < typename T >
    const T* entity::find_component() const noexcept {
        return detail::as_const(*owner_).find_component<T>(id_);
    }

    template < typename... Ts >
    std::tuple<Ts&...> entity::get_components() {
        return (*owner_).get_components<Ts...>(id_);
    }

    template < typename... Ts >
    std::tuple<const Ts&...> entity::get_components() const {
        return detail::as_const(*owner_).get_components<Ts...>(id_);
    }

    template < typename... Ts >
    std::tuple<Ts*...> entity::find_components() noexcept {
        return (*owner_).find_components<Ts...>(id_);
    }

    template < typename... Ts >
    std::tuple<const Ts*...> entity::find_components() const noexcept {
        return detail::as_const(*owner_).find_components<Ts...>(id_);
    }

    inline std::size_t entity::component_count() const noexcept {
        return detail::as_const(*owner_).entity_component_count(id_);
    }

    inline bool operator<(const entity& l, const entity& r) noexcept {
        return (&l.owner() < &r.owner())
            || (&l.owner() == &r.owner() && l.id() < r.id());
    }

    inline bool operator==(const entity& l, const entity& r) noexcept {
        return &l.owner() == &r.owner()
            && l.id() == r.id();
    }

    inline bool operator==(const entity& l, const const_entity& r) noexcept {
        return &l.owner() == &r.owner()
            && l.id() == r.id();
    }

    inline bool operator!=(const entity& l, const entity& r) noexcept {
        return !(l == r);
    }

    inline bool operator!=(const entity& l, const const_entity& r) noexcept {
        return !(l == r);
    }
}

// -----------------------------------------------------------------------------
//
// const_entity impl
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    inline const_entity::const_entity(const entity& ent) noexcept
    : owner_(&ent.owner())
    , id_(ent.id()) {}

    inline const_entity::const_entity(const registry& owner) noexcept
    : owner_(&owner) {}

    inline const_entity::const_entity(const registry& owner, entity_id id) noexcept
    : owner_(&owner)
    , id_(id) {}

    inline const registry& const_entity::owner() const noexcept {
        return *owner_;
    }

    inline entity_id const_entity::id() const noexcept {
        return id_;
    }

    inline bool const_entity::valid() const noexcept {
        return (*owner_).valid_entity(id_);
    }

    template < typename T >
    bool const_entity::exists_component() const noexcept {
        return (*owner_).exists_component<T>(id_);
    }

    template < typename T >
    const T& const_entity::get_component() const {
        return (*owner_).get_component<T>(id_);
    }

    template < typename T >
    const T* const_entity::find_component() const noexcept {
        return (*owner_).find_component<T>(id_);
    }

    template < typename... Ts >
    std::tuple<const Ts&...> const_entity::get_components() const {
        return (*owner_).get_components<Ts...>(id_);
    }

    template < typename... Ts >
    std::tuple<const Ts*...> const_entity::find_components() const noexcept {
        return (*owner_).find_components<Ts...>(id_);
    }

    inline std::size_t const_entity::component_count() const noexcept {
        return (*owner_).entity_component_count(id_);
    }

    inline bool operator<(const const_entity& l, const const_entity& r) noexcept {
        return (&l.owner() < &r.owner())
            || (&l.owner() == &r.owner() && l.id() < r.id());
    }

    inline bool operator==(const const_entity& l, const entity& r) noexcept {
        return &l.owner() == &r.owner()
            && l.id() == r.id();
    }

    inline bool operator==(const const_entity& l, const const_entity& r) noexcept {
        return &l.owner() == &r.owner()
            && l.id() == r.id();
    }

    inline bool operator!=(const const_entity& l, const entity& r) noexcept {
        return !(l == r);
    }

    inline bool operator!=(const const_entity& l, const const_entity& r) noexcept {
        return !(l == r);
    }
}

// -----------------------------------------------------------------------------
//
// component impl
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    template < typename T >
    component<T>::component(const entity& owner) noexcept
    : owner_(owner) {}

    template < typename T >
    entity& component<T>::owner() noexcept {
        return owner_;
    }

    template < typename T >
    const entity& component<T>::owner() const noexcept {
        return owner_;
    }

    template < typename T >
    bool component<T>::remove() {
        return owner_.remove_component<T>();
    }

    template < typename T >
    bool component<T>::exists() const noexcept {
        return owner_.exists_component<T>();
    }

    template < typename T >
    template < typename... Args >
    T& component<T>::assign(Args&&... args) {
        return owner_.assign_component<T>(
            std::forward<Args>(args)...);
    }

    template < typename T >
    T& component<T>::get() {
        return owner_.get_component<T>();
    }

    template < typename T >
    const T& component<T>::get() const {
        return detail::as_const(owner_).template get_component<T>();
    }

    template < typename T >
    T* component<T>::find() noexcept {
        return owner_.find_component<T>();
    }

    template < typename T >
    const T* component<T>::find() const noexcept {
        return detail::as_const(owner_).template find_component<T>();
    }

    template < typename T >
    bool operator<(const component<T>& l, const component<T>& r) noexcept {
        return l.owner() < r.owner();
    }

    template < typename T >
    bool operator==(const component<T>& l, const component<T>& r) noexcept {
        return l.owner() == r.owner();
    }

    template < typename T >
    bool operator==(const component<T>& l, const const_component<T>& r) noexcept {
        return l.owner() == r.owner();
    }

    template < typename T >
    bool operator!=(const component<T>& l, const component<T>& r) noexcept {
        return !(l == r);
    }

    template < typename T >
    bool operator!=(const component<T>& l, const const_component<T>& r) noexcept {
        return !(l == r);
    }
}

// -----------------------------------------------------------------------------
//
// const_component impl
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    template < typename T >
    const_component<T>::const_component(const component<T>& comp) noexcept
    : owner_(comp.owner()) {}

    template < typename T >
    const_component<T>::const_component(const const_entity& owner) noexcept
    : owner_(owner) {}

    template < typename T >
    const const_entity& const_component<T>::owner() const noexcept {
        return owner_;
    }

    template < typename T >
    bool const_component<T>::exists() const noexcept {
        return detail::as_const(owner_).template exists_component<T>();
    }

    template < typename T >
    const T& const_component<T>::get() const {
        return detail::as_const(owner_).template get_component<T>();
    }

    template < typename T >
    const T* const_component<T>::find() const noexcept {
        return detail::as_const(owner_).template find_component<T>();
    }

    template < typename T >
    bool operator<(const const_component<T>& l, const const_component<T>& r) noexcept {
        return l.owner() < r.owner();
    }

    template < typename T >
    bool operator==(const const_component<T>& l, const component<T>& r) noexcept {
        return l.owner() == r.owner();
    }

    template < typename T >
    bool operator==(const const_component<T>& l, const const_component<T>& r) noexcept {
        return l.owner() == r.owner();
    }

    template < typename T >
    bool operator!=(const const_component<T>& l, const component<T>& r) noexcept {
        return !(l == r);
    }

    template < typename T >
    bool operator!=(const const_component<T>& l, const const_component<T>& r) noexcept {
        return !(l == r);
    }
}

// -----------------------------------------------------------------------------
//
// prototype impl
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    namespace detail
    {
        template < typename T, typename... Args >
        component_applier<T,Args...>::component_applier(std::tuple<Args...>&& args)
        : args_(std::move(args)) {}

        template < typename T, typename... Args >
        component_applier<T,Args...>::component_applier(const std::tuple<Args...>& args)
        : args_(args) {}

        template < typename T, typename... Args >
        component_applier_uptr component_applier<T,Args...>::clone() const {
            return std::make_unique<component_applier>(args_);
        }

        template < typename T, typename... Args >
        void component_applier<T,Args...>::apply_to(entity& ent, bool override) const {
            detail::tiny_tuple_apply([&ent, override](const Args&... args){
                if ( override || !ent.exists_component<T>() ) {
                    ent.assign_component<T>(args...);
                }
            }, args_);
        }
    }

    inline prototype::prototype(const prototype& other) {
        for ( const family_id id : other.appliers_ ) {
            appliers_.insert(id, other.appliers_.get(id)->clone());
        }
    }

    inline prototype& prototype::operator=(const prototype& other) {
        if ( this != &other ) {
            prototype p(other);
            swap(p);
        }
        return *this;
    }

    inline prototype::prototype(prototype&& other) noexcept
    : appliers_(std::move(other.appliers_)) {}

    inline prototype& prototype::operator=(prototype&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    inline void prototype::clear() noexcept {
        appliers_.clear();
    }

    inline bool prototype::empty() const noexcept {
        return appliers_.empty();
    }

    inline void prototype::swap(prototype& other) noexcept {
        using std::swap;
        swap(appliers_, other.appliers_);
    }

    template < typename T >
    bool prototype::has_component() const noexcept {
        const auto family = detail::type_family<T>::id();
        return appliers_.has(family);
    }

    template < typename T, typename... Args >
    prototype& prototype::component(Args&&... args) & {
        using applier_t = detail::component_applier<
            T,
            std::decay_t<Args>...>;
        auto applier = std::make_unique<applier_t>(
            std::make_tuple(std::forward<Args>(args)...));
        const auto family = detail::type_family<T>::id();
        appliers_.emplace(family, std::move(applier));
        return *this;
    }

    template < typename T, typename... Args >
    prototype&& prototype::component(Args&&... args) && {
        component<T>(std::forward<Args>(args)...);
        return std::move(*this);
    }

    inline prototype& prototype::merge_with(const prototype& other, bool override) & {
        for ( const auto family_id : other.appliers_ ) {
            if ( override || !appliers_.has(family_id) ) {
                appliers_.insert_or_assign(
                    family_id,
                    other.appliers_.get(family_id)->clone());
            }
        }
        return *this;
    }

    inline prototype&& prototype::merge_with(const prototype& other, bool override) && {
        merge_with(other, override);
        return std::move(*this);
    }

    inline void prototype::apply_to(entity& ent, bool override) const {
        for ( const auto family_id : appliers_ ) {
            appliers_.get(family_id)->apply_to(ent, override);
        }
    }

    inline void swap(prototype& l, prototype& r) noexcept {
        l.swap(r);
    }
}

// -----------------------------------------------------------------------------
//
// registry impl
//
// -----------------------------------------------------------------------------

namespace ecs_hpp
{
    //
    // registry::uentity
    //

    inline registry::uentity::uentity(registry& owner, entity_id id) noexcept
    : id_(id)
    , owner_(&owner) {}

    inline registry::uentity::uentity(entity_id ent) noexcept
    : id_(ent) {}

    inline registry::uentity::uentity(entity ent) noexcept
    : id_(ent.id())
    , owner_(&ent.owner()) {}

    inline registry::uentity::operator entity_id() const noexcept {
        return id_;
    }

    inline registry::uentity::operator entity() const noexcept {
        assert(owner_);
        return {*owner_, id_};
    }

    inline registry::uentity::operator const_entity() const noexcept {
        assert(owner_);
        return {*owner_, id_};
    }

    inline entity_id registry::uentity::id() const noexcept {
        return id_;
    }

    inline registry* registry::uentity::owner() noexcept {
        return owner_;
    }

    inline const registry* registry::uentity::owner() const noexcept {
        return owner_;
    }

    inline bool registry::uentity::check_owner(const registry* owner) const noexcept {
        return !owner_ || owner_ == owner;
    }

    //
    // registry::const_uentity
    //

    inline registry::const_uentity::const_uentity(const registry& owner, entity_id id) noexcept
    : id_(id)
    , owner_(&owner) {}

    inline registry::const_uentity::const_uentity(entity_id ent) noexcept
    : id_(ent) {}

    inline registry::const_uentity::const_uentity(entity ent) noexcept
    : id_(ent.id())
    , owner_(&ent.owner()) {}

    inline registry::const_uentity::const_uentity(const_entity ent) noexcept
    : id_(ent.id())
    , owner_(&ent.owner()) {}

    inline registry::const_uentity::const_uentity(const uentity& ent) noexcept
    : id_(ent.id())
    , owner_(ent.owner()) {}

    inline registry::const_uentity::operator entity_id() const noexcept {
        return id_;
    }

    inline registry::const_uentity::operator const_entity() const noexcept {
        assert(owner_);
        return {*owner_, id_};
    }

    inline entity_id registry::const_uentity::id() const noexcept {
        return id_;
    }

    inline const registry* registry::const_uentity::owner() const noexcept {
        return owner_;
    }

    inline bool registry::const_uentity::check_owner(const registry* owner) const noexcept {
        return !owner_ || owner_ == owner;
    }

    //
    // registry
    //

    inline entity registry::wrap_entity(const const_uentity& ent) noexcept {
        return {*this, ent.id()};
    }

    inline const_entity registry::wrap_entity(const const_uentity& ent) const noexcept {
        return {*this, ent.id()};
    }

    template < typename T >
    component<T> registry::wrap_component(const const_uentity& ent) noexcept {
        return {wrap_entity(ent)};
    }

    template < typename T >
    const_component<T> registry::wrap_component(const const_uentity& ent) const noexcept {
        return {wrap_entity(ent)};
    }

    inline entity registry::create_entity() {
        if ( !free_entity_ids_.empty() ) {
            const auto free_ent_id = free_entity_ids_.back();
            const auto new_ent_id = detail::upgrade_entity_id(free_ent_id);
            entity_ids_.insert(new_ent_id);
            free_entity_ids_.pop_back();
            return wrap_entity(new_ent_id);

        }
        if ( last_entity_id_ >= detail::entity_id_index_mask ) {
            throw std::logic_error("ecs_hpp::registry (entity index overlow)");
        }
        if ( free_entity_ids_.capacity() <= entity_ids_.size() ) {
            // ensure free entity ids capacity for safe (noexcept) entity destroying
            free_entity_ids_.reserve(detail::next_capacity_size(
                free_entity_ids_.capacity(),
                entity_ids_.size() + 1,
                free_entity_ids_.max_size()));
        }
        entity_ids_.insert(last_entity_id_ + 1);
        return wrap_entity(++last_entity_id_);
    }

    inline entity registry::create_entity(const prototype& proto) {
        auto ent = create_entity();
        try {
            proto.apply_to(ent, true);
        } catch (...) {
            destroy_entity(ent);
            throw;
        }
        return ent;
    }

    inline entity registry::create_entity(const const_uentity& proto) {
        assert(valid_entity(proto));
        entity ent = create_entity();
        try {
            for ( const auto family_id : storages_ ) {
                storages_.get(family_id)->clone(proto, ent.id());
            }
        } catch (...) {
            destroy_entity(ent);
            throw;
        }
        return ent;
    }

    inline void registry::destroy_entity(const uentity& ent) noexcept {
        assert(valid_entity(ent));
        remove_all_components(ent);
        if ( entity_ids_.unordered_erase(ent) ) {
            assert(free_entity_ids_.size() < free_entity_ids_.capacity());
            free_entity_ids_.push_back(ent);
        }
    }

    inline bool registry::valid_entity(const const_uentity& ent) const noexcept {
        assert(ent.check_owner(this));
        return entity_ids_.has(ent);
    }

    template < typename T, typename... Args >
    T& registry::assign_component(const uentity& ent, Args&&... args) {
        assert(valid_entity(ent));
        return get_or_create_storage_<T>().assign(
            ent,
            std::forward<Args>(args)...);
    }

    template < typename T >
    bool registry::remove_component(const uentity& ent) {
        assert(valid_entity(ent));
        detail::component_storage<T>* storage = find_storage_<T>();
        return storage
            ? storage->remove(ent)
            : false;
    }

    template < typename T >
    bool registry::exists_component(const const_uentity& ent) const noexcept {
        assert(valid_entity(ent));
        const detail::component_storage<T>* storage = find_storage_<T>();
        return storage
            ? storage->exists(ent)
            : false;
    }

    inline std::size_t registry::remove_all_components(const uentity& ent) noexcept {
        assert(valid_entity(ent));
        std::size_t removed_count = 0u;
        for ( const auto family_id : storages_ ) {
            if ( storages_.get(family_id)->remove(ent) ) {
                ++removed_count;
            }
        }
        return removed_count;
    }

    template < typename T >
    T& registry::get_component(const uentity& ent) {
        assert(valid_entity(ent));
        T* component = find_component<T>(ent);
        if ( component ) {
            return *component;
        }
        throw std::logic_error("ecs_hpp::registry (component not found)");
    }

    template < typename T >
    const T& registry::get_component(const const_uentity& ent) const {
        assert(valid_entity(ent));
        const T* component = find_component<T>(ent);
        if ( component ) {
            return *component;
        }
        throw std::logic_error("ecs_hpp::registry (component not found)");
    }

    template < typename T >
    T* registry::find_component(const uentity& ent) noexcept {
        assert(valid_entity(ent));
        detail::component_storage<T>* storage = find_storage_<T>();
        return storage
            ? storage->find(ent)
            : nullptr;
    }

    template < typename T >
    const T* registry::find_component(const const_uentity& ent) const noexcept {
        assert(valid_entity(ent));
        const detail::component_storage<T>* storage = find_storage_<T>();
        return storage
            ? storage->find(ent)
            : nullptr;
    }

    template < typename... Ts >
    std::tuple<Ts&...> registry::get_components(const uentity& ent) {
        assert(valid_entity(ent));
        return std::make_tuple(std::ref(get_component<Ts>(ent))...);
    }

    template < typename... Ts >
    std::tuple<const Ts&...> registry::get_components(const const_uentity& ent) const {
        assert(valid_entity(ent));
        return std::make_tuple(std::cref(get_component<Ts>(ent))...);
    }

    template < typename... Ts >
    std::tuple<Ts*...> registry::find_components(const uentity& ent) noexcept {
        assert(valid_entity(ent));
        return std::make_tuple(find_component<Ts>(ent)...);
    }

    template < typename... Ts >
    std::tuple<const Ts*...> registry::find_components(const const_uentity& ent) const noexcept {
        assert(valid_entity(ent));
        return std::make_tuple(find_component<Ts>(ent)...);
    }

    template < typename T >
    std::size_t registry::component_count() const noexcept {
        const detail::component_storage<T>* storage = find_storage_<T>();
        return storage
            ? storage->count()
            : 0u;
    }

    inline std::size_t registry::entity_count() const noexcept {
        return entity_ids_.size();
    }

    inline std::size_t registry::entity_component_count(const const_uentity& ent) const noexcept {
        assert(valid_entity(ent));
        std::size_t component_count = 0u;
        for ( const auto family_id : storages_ ) {
            if ( storages_.get(family_id)->has(ent) ) {
                ++component_count;
            }
        }
        return component_count;
    }

    template < typename F >
    void registry::for_each_entity(F&& f) {
        for ( const auto id : entity_ids_ ) {
            f({*this, id});
        }
    }

    template < typename F >
    void registry::for_each_entity(F&& f) const {
        for ( const auto id : entity_ids_ ) {
            f({*this, id});
        }
    }

    template < typename T, typename F >
    void registry::for_each_component(F&& f) {
        detail::component_storage<T>* storage = find_storage_<T>();
        if ( storage ) {
            storage->for_each_component([this, &f](const entity_id e, T& t){
                f(uentity{*this, e}, t);
            });
        }
    }

    template < typename T, typename F >
    void registry::for_each_component(F&& f) const {
        const detail::component_storage<T>* storage = find_storage_<T>();
        if ( storage ) {
            storage->for_each_component([this, &f](const entity_id e, const T& t){
                f(const_uentity{*this, e}, t);
            });
        }
    }

    template < typename... Ts, typename F >
    void registry::for_joined_components(F&& f) {
        for_joined_components_impl_<Ts...>(
            std::forward<F>(f),
            std::make_index_sequence<sizeof...(Ts)>());
    }

    template < typename... Ts, typename F >
    void registry::for_joined_components(F&& f) const {
        for_joined_components_impl_<Ts...>(
            std::forward<F>(f),
            std::make_index_sequence<sizeof...(Ts)>());
    }

    template < typename T, typename... Args >
    void registry::add_system(priority_t priority, Args&&... args) {
        auto iter = std::upper_bound(
            systems_.begin(), systems_.end(), priority,
            [](priority_t pr, const auto& r){
                return pr < r.first;
            });
        systems_.emplace(
            iter,
            priority,
            std::make_unique<T>(std::forward<Args>(args)...));
    }

    inline void registry::process_all_systems() {
        process_systems_in_range(
            std::numeric_limits<priority_t>::min(),
            std::numeric_limits<priority_t>::max());
    }

    inline void registry::process_systems_above(priority_t min) {
        process_systems_in_range(
            min,
            std::numeric_limits<priority_t>::max());
    }

    inline void registry::process_systems_below(priority_t max) {
        process_systems_in_range(
            std::numeric_limits<priority_t>::min(),
            max);
    }

    inline void registry::process_systems_in_range(priority_t min, priority_t max) {
        const auto first = std::lower_bound(
            systems_.begin(), systems_.end(), min,
            [](const auto& p, priority_t pr) noexcept {
                return p.first < pr;
            });
        for ( auto iter = first; iter != systems_.end() && iter->first <= max; ++iter ) {
            iter->second->process(*this);
        }
    }

    inline registry::memory_usage_info registry::memory_usage() const noexcept {
        memory_usage_info info;
        info.entities += free_entity_ids_.capacity() * sizeof(free_entity_ids_[0]);
        info.entities += entity_ids_.memory_usage();
        for ( const auto family_id : storages_ ) {
            info.components += storages_.get(family_id)->memory_usage();
        }
        return info;
    }

    template < typename T >
    std::size_t registry::component_memory_usage() const noexcept {
        const detail::component_storage<T>* storage = find_storage_<T>();
        return storage
            ? storage->memory_usage()
            : 0u;
    }

    template < typename T >
    detail::component_storage<T>* registry::find_storage_() noexcept {
        const auto family = detail::type_family<T>::id();
        using raw_storage_ptr = detail::component_storage<T>*;
        const storage_uptr* storage_uptr_ptr = storages_.find(family);
        return storage_uptr_ptr && *storage_uptr_ptr
            ? static_cast<raw_storage_ptr>(storage_uptr_ptr->get())
            : nullptr;
    }

    template < typename T >
    const detail::component_storage<T>* registry::find_storage_() const noexcept {
        const auto family = detail::type_family<T>::id();
        using raw_storage_ptr = const detail::component_storage<T>*;
        const storage_uptr* storage_uptr_ptr = storages_.find(family);
        return storage_uptr_ptr && *storage_uptr_ptr
            ? static_cast<raw_storage_ptr>(storage_uptr_ptr->get())
            : nullptr;
    }

    template < typename T >
    detail::component_storage<T>& registry::get_or_create_storage_() {
        detail::component_storage<T>* storage = find_storage_<T>();
        if ( storage ) {
            return *storage;
        }
        const auto family = detail::type_family<T>::id();
        storages_.emplace(
            family,
            std::make_unique<detail::component_storage<T>>(*this));
        return *static_cast<detail::component_storage<T>*>(
            storages_.get(family).get());
    }

    template < typename T
             , typename... Ts
             , typename F
             , std::size_t I
             , std::size_t... Is >
    void registry::for_joined_components_impl_(
        F&& f,
        std::index_sequence<I, Is...> iseq)
    {
        (void)iseq;
        const auto ss = std::make_tuple(find_storage_<Ts>()...);
        if ( !detail::tuple_contains(ss, nullptr) ) {
            for_each_component<T>([this, &f, &ss](const uentity& e, T& t) {
                for_joined_components_impl_<Ts...>(e, f, ss, t);
            });
        }
    }

    template < typename T
             , typename... Ts
             , typename F
             , std::size_t I
             , std::size_t... Is >
    void registry::for_joined_components_impl_(
        F&& f,
        std::index_sequence<I, Is...> iseq) const
    {
        (void)iseq;
        const auto ss = std::make_tuple(find_storage_<Ts>()...);
        if ( !detail::tuple_contains(ss, nullptr) ) {
            for_each_component<T>([this, &f, &ss](const const_uentity& e, const T& t) {
                detail::as_const(*this).for_joined_components_impl_<Ts...>(e, f, ss, t);
            });
        }
    }

    template < typename T
             , typename... Ts
             , typename F
             , typename Ss
             , typename... Cs >
    void registry::for_joined_components_impl_(
        const uentity& e,
        const F& f,
        const Ss& ss,
        Cs&... cs)
    {
        T* c = std::get<0>(ss)->find(e);
        if ( c ) {
            for_joined_components_impl_<Ts...>(
                e,
                f,
                detail::tuple_tail(ss),
                cs...,
                *c);
        }
    }

    template < typename T
             , typename... Ts
             , typename F
             , typename Ss
             , typename... Cs >
    void registry::for_joined_components_impl_(
        const const_uentity& e,
        const F& f,
        const Ss& ss,
        const Cs&... cs) const
    {
        const T* c = std::get<0>(ss)->find(e);
        if ( c ) {
            for_joined_components_impl_<Ts...>(
                e,
                f,
                detail::tuple_tail(ss),
                cs...,
                *c);
        }
    }

    template < typename F, typename... Cs >
    void registry::for_joined_components_impl_(
        const uentity& e,
        const F& f,
        const std::tuple<>& ss,
        Cs&... cs)
    {
        (void)ss;
        f(e, cs...);
    }

    template < typename F, typename... Cs >
    void registry::for_joined_components_impl_(
        const const_uentity& e,
        const F& f,
        const std::tuple<>& ss,
        const Cs&... cs) const
    {
        (void)ss;
        f(e, cs...);
    }
}
