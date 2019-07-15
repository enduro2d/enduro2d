/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "_utils.hpp"

namespace e2d
{
    template < typename Tag >
    class intrusive_list_hook;

    template < typename Tag, typename T, typename TP, typename TR >
    class intrusive_list_iterator;
}

namespace e2d
{
    template < typename Tag >
    class intrusive_list_hook {
    public:
        intrusive_list_hook() = default;

        ~intrusive_list_hook() noexcept {
            if ( is_linked() ) {
                unlink();
            }
        }

        intrusive_list_hook(const intrusive_list_hook& other) noexcept {
            E2D_UNUSED(other);
        }

        intrusive_list_hook& operator=(const intrusive_list_hook& other) noexcept {
            E2D_UNUSED(other);
            return *this;
        }
    private:
        using node_ptr = intrusive_list_hook*;
        using const_node_ptr = const intrusive_list_hook*;

        template < typename U, typename UTag >
        friend class intrusive_list;

        template < typename UTag, typename U, typename UP, typename UR >
        friend class intrusive_list_iterator;
    private:
        intrusive_list_hook(node_ptr prev, node_ptr next) noexcept
        : prev_(prev)
        , next_(next) {}

        bool unique() const noexcept {
            return !next_ || next_ == this;
        }

        bool is_linked() const noexcept {
            return !unique();
        }

        void link_before(node_ptr next) noexcept {
            node_ptr prev = next->prev_;
            prev_ = prev;
            next_ = next;
            next->prev_ = this;
            prev->next_ = this;
        }

        void link_after(node_ptr prev) noexcept {
            node_ptr next = prev->next_;
            next_ = next;
            prev_ = prev;
            prev->next_ = this;
            next->prev_ = this;
        }

        void unlink() noexcept {
            node_ptr next = next_;
            node_ptr prev = prev_;
            prev->next_ = next;
            next->prev_ = prev;
            prev_ = nullptr;
            next_ = nullptr;
        }

        static void swap_prev(node_ptr l, node_ptr r){
            node_ptr tmp = l->prev_;
            l->prev_ = r->prev_;
            r->prev_ = tmp;
        }

        static void swap_next(node_ptr l, node_ptr r){
            node_ptr tmp = l->next_;
            l->next_ = r->next_;
            r->next_ = tmp;
        }

        static void swap_nodes(node_ptr l, node_ptr r) noexcept {
            if ( l != r ) {
                node_ptr prev_l = l->prev_;
                node_ptr next_l = l->next_;
                node_ptr prev_r = r->prev_;
                node_ptr next_r = r->next_;
                swap_prev(next_l, next_r);
                swap_next(prev_l, prev_r);
                swap_prev(l, r);
                swap_next(l, r);
            }
        }
    private:
        node_ptr prev_{nullptr};
        node_ptr next_{nullptr};
    };
}

namespace e2d
{
    template < typename Tag, typename T, typename TP, typename TR >
    class intrusive_list_iterator final {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = ptrdiff_t;
        using pointer = TP;
        using reference = TR&;
    public:
        using self_type = intrusive_list_iterator;

        using node_t = intrusive_list_hook<Tag>;
        using node_ptr = typename node_t::node_ptr;
        using const_node_ptr = typename node_t::const_node_ptr;

        template < typename UTag, typename U, typename UP, typename UR >
        friend class intrusive_list_iterator;
    public:
        intrusive_list_iterator() = default;

        intrusive_list_iterator(const_node_ptr node) noexcept
        : node_(const_cast<node_ptr>(node)) {}

        template < typename UTP, typename UTR >
        intrusive_list_iterator(const intrusive_list_iterator<Tag,T,UTP,UTR>& other) noexcept
        : node_(const_cast<node_ptr>(other.node_)) {}

        node_ptr node() noexcept {
            return node_;
        }

        const_node_ptr node() const noexcept {
            return node_;
        }

        self_type& operator++() noexcept {
            node_ = node_->next_;
            return *this;
        }

        const self_type operator++(int) noexcept {
            self_type tmp(*this);
            node_ = node_->next_;
            return tmp;
        }

        self_type& operator--() noexcept {
            node_ = node_->prev_;
            return *this;
        }

        const self_type operator--(int) noexcept {
            self_type tmp(*this);
            node_ = node_->prev_;
            return tmp;
        }

        TR operator*() const noexcept {
            E2D_ASSERT(node_);
            return static_cast<TR>(*node_);
        }

        TP operator->() const noexcept {
            E2D_ASSERT(node_);
            return static_cast<TP>(node_);
        }
    private:
        node_ptr node_{nullptr};
    };

    template < typename Tag, typename T, typename LTP, typename LTR, typename RTP, typename RTR >
    bool operator==(
        const intrusive_list_iterator<Tag,T,LTP,LTR>& l,
        const intrusive_list_iterator<Tag,T,RTP,RTR>& r) noexcept
    {
        return l.node() == r.node();
    }

    template < typename Tag, typename T, typename LTP, typename LTR, typename RTP, typename RTR >
    bool operator!=(
        const intrusive_list_iterator<Tag,T,LTP,LTR>& l,
        const intrusive_list_iterator<Tag,T,RTP,RTR>& r) noexcept
    {
        return l.node() != r.node();
    }
}

namespace e2d
{
    template < typename T, typename Tag >
    class intrusive_list final {
    public:
        using self_type = intrusive_list;

        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;

        using iterator = intrusive_list_iterator<Tag, T, pointer, reference>;
        using const_iterator = intrusive_list_iterator<Tag, T, const_pointer, const_reference>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    public:
        intrusive_list() = default;
        ~intrusive_list() noexcept;

        iterator begin() noexcept;
        const_iterator begin() const noexcept;
        const_iterator cbegin() const noexcept;

        iterator end() noexcept;
        const_iterator end() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator rbegin() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator crbegin() const noexcept;

        reverse_iterator rend() noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crend() const noexcept;

        T& back() noexcept;
        const T& back() const noexcept;

        T& front() noexcept;
        const T& front() const noexcept;

        std::size_t size() const noexcept;
        bool empty() const noexcept;
        void swap(intrusive_list& other) noexcept;

        template < typename Disposer >
        void clear_and_dispose(Disposer&& disposer);
        void clear() noexcept;

        template < typename Disposer >
        void pop_back_and_dispose(Disposer&& disposer);
        void pop_back() noexcept;

        template < typename Disposer >
        void pop_front_and_dispose(Disposer&& disposer);
        void pop_front() noexcept;

        void push_back(T& v) noexcept;
        void push_front(T& v) noexcept;
        iterator insert(const_iterator pos, T& v) noexcept;

        template < typename Disposer >
        iterator erase_and_dispose(const_iterator pos, Disposer&& disposer);
        iterator erase(const_iterator pos) noexcept;

        static iterator iterator_to(T& v) noexcept;
        static const_iterator iterator_to(const T& v) noexcept;
    private:
        using node_t = intrusive_list_hook<Tag>;
        using node_ptr = typename node_t::node_ptr;
        node_t root_{&root_, &root_};
    };
}

namespace e2d
{
    template < typename T, typename Tag >
    intrusive_list<T,Tag>::~intrusive_list() noexcept {
        clear();
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::iterator
    intrusive_list<T,Tag>::begin() noexcept {
        return iterator(root_.next_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::const_iterator
    intrusive_list<T,Tag>::begin() const noexcept {
        return const_iterator(root_.next_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::const_iterator
    intrusive_list<T,Tag>::cbegin() const noexcept {
        return const_iterator(root_.next_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::iterator
    intrusive_list<T,Tag>::end() noexcept {
        return iterator(&root_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::const_iterator
    intrusive_list<T,Tag>::end() const noexcept {
        return const_iterator(&root_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::const_iterator
    intrusive_list<T,Tag>::cend() const noexcept {
        return const_iterator(&root_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::reverse_iterator
    intrusive_list<T,Tag>::rbegin() noexcept {
        return reverse_iterator(&root_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::const_reverse_iterator
    intrusive_list<T,Tag>::rbegin() const noexcept {
        return const_reverse_iterator(&root_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::const_reverse_iterator
    intrusive_list<T,Tag>::crbegin() const noexcept {
        return const_reverse_iterator(&root_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::reverse_iterator
    intrusive_list<T,Tag>::rend() noexcept {
        return reverse_iterator(root_.next_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::const_reverse_iterator
    intrusive_list<T,Tag>::rend() const noexcept {
        return const_reverse_iterator(root_.next_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::const_reverse_iterator
    intrusive_list<T,Tag>::crend() const noexcept {
        return const_reverse_iterator(root_.next_);
    }

    template < typename T, typename Tag >
    T& intrusive_list<T,Tag>::back() noexcept {
        E2D_ASSERT(!empty());
        return static_cast<T&>(*root_.prev_);
    }

    template < typename T, typename Tag >
    const T& intrusive_list<T,Tag>::back() const noexcept {
        E2D_ASSERT(!empty());
        return static_cast<const T&>(*root_.prev_);
    }

    template < typename T, typename Tag >
    T& intrusive_list<T,Tag>::front() noexcept {
        E2D_ASSERT(!empty());
        return static_cast<T&>(*root_.next_);
    }

    template < typename T, typename Tag >
    const T& intrusive_list<T,Tag>::front() const noexcept {
        E2D_ASSERT(!empty());
        return static_cast<const T&>(*root_.next_);
    }

    template < typename T, typename Tag >
    std::size_t intrusive_list<T,Tag>::size() const noexcept {
        std::size_t result{0u};
        const node_t* node = root_.next_;
        while ( node != &root_ ) {
            ++result;
            node = node->next_;
        }
        return result;
    }

    template < typename T, typename Tag >
    bool intrusive_list<T,Tag>::empty() const noexcept {
        return root_.unique();
    }

    template < typename T, typename Tag >
    void intrusive_list<T,Tag>::swap(intrusive_list<T,Tag>& other) noexcept {
        intrusive_list_hook<Tag>::swap_nodes(&root_, &other.root_);
    }

    template < typename T, typename Tag >
    template < typename Disposer >
    void intrusive_list<T,Tag>::clear_and_dispose(Disposer&& disposer) {
        while ( !empty() ) {
            pop_back_and_dispose(disposer);
        }
    }

    template < typename T, typename Tag >
    void intrusive_list<T,Tag>::clear() noexcept {
        clear_and_dispose(null_disposer());
    }

    template < typename T, typename Tag >
    template < typename Disposer >
    void intrusive_list<T,Tag>::pop_back_and_dispose(Disposer&& disposer) {
        E2D_ASSERT(!empty());
        node_ptr node = root_.prev_;
        node->unlink();
        std::forward<Disposer>(disposer)(static_cast<T*>(node));
    }

    template < typename T, typename Tag >
    void intrusive_list<T,Tag>::pop_back() noexcept {
        pop_back_and_dispose(null_disposer());
    }

    template < typename T, typename Tag >
    template < typename Disposer >
    void intrusive_list<T,Tag>::pop_front_and_dispose(Disposer&& disposer) {
        E2D_ASSERT(!empty());
        node_ptr node = root_.next_;
        node->unlink();
        std::forward<Disposer>(disposer)(static_cast<T*>(node));
    }

    template < typename T, typename Tag >
    void intrusive_list<T,Tag>::pop_front() noexcept {
        pop_front_and_dispose(null_disposer());
    }

    template < typename T, typename Tag >
    void intrusive_list<T,Tag>::push_back(T& v) noexcept {
        node_t& node = static_cast<node_t&>(v);
        E2D_ASSERT(!node.is_linked());
        node.link_before(&root_);
    }

    template < typename T, typename Tag >
    void intrusive_list<T,Tag>::push_front(T& v) noexcept {
        node_t& node = static_cast<node_t&>(v);
        E2D_ASSERT(!node.is_linked());
        node.link_after(&root_);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::iterator intrusive_list<T,Tag>::insert(const_iterator pos, T& v) noexcept {
        node_t& node = static_cast<node_t&>(v);
        E2D_ASSERT(!node.is_linked());
        node.link_before(pos.node());
        return iterator(&node);
    }

    template < typename T, typename Tag >
    template < typename Disposer >
    typename intrusive_list<T,Tag>::iterator intrusive_list<T,Tag>::erase_and_dispose(const_iterator pos, Disposer&& disposer) {
        node_ptr node = pos.node();
        E2D_ASSERT(node != &root_ && node->is_linked());
        ++pos;
        node->unlink();
        std::forward<Disposer>(disposer)(static_cast<T*>(node));
        return iterator(pos.node());
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::iterator intrusive_list<T,Tag>::erase(const_iterator pos) noexcept {
        return erase_and_dispose(pos, null_disposer());
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::iterator intrusive_list<T,Tag>::iterator_to(T& v) noexcept {
        node_t& node = static_cast<node_t&>(v);
        E2D_ASSERT(node.is_linked());
        return iterator(&node);
    }

    template < typename T, typename Tag >
    typename intrusive_list<T,Tag>::const_iterator intrusive_list<T,Tag>::iterator_to(const T& v) noexcept {
        const node_t& node = static_cast<const node_t&>(v);
        E2D_ASSERT(node.is_linked());
        return const_iterator(&node);
    }
}
