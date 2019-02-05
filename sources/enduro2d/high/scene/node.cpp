/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/scene/node.hpp>

namespace e2d
{
    node::node() = default;

    node::~node() noexcept {
        remove_all_children();
        remove_from_parent();
    }

    node_iptr node::create() {
        return node_iptr(new node());
    }

    node_iptr node::create(const node_iptr& parent) {
        node_iptr child = create();
        if ( parent ) {
            parent->add_child(child);
        }
        return child;
    }

    node_iptr node::root() noexcept {
        node* n = this;
        while ( n->parent_ ) {
            n = n->parent_;
        }
        return node_iptr(n);
    }

    const_node_iptr node::root() const noexcept {
        const node* n = this;
        while ( n->parent_ ) {
            n = n->parent_;
        }
        return const_node_iptr(n);
    }

    node_iptr node::parent() noexcept {
        return node_iptr(parent_);
    }

    const_node_iptr node::parent() const noexcept {
        return const_node_iptr(parent_);
    }

    bool node::has_parent() const noexcept {
        return !!parent_;
    }

    bool node::has_parent(const const_node_iptr& parent) const noexcept {
        if ( !parent ) {
            return has_parent();
        }
        const node* n = this;
        while ( n ) {
            if ( n->parent_ == parent ) {
                return true;
            }
            n = n->parent_;
        }
        return false;
    }

    bool node::remove_from_parent() noexcept {
        if ( !parent_ ) {
            return false;
        }
        parent_->children_.erase(
            node_children::iterator_to(*this));
        parent_ = nullptr;
        return true;
    }

    std::size_t node::remove_all_children() noexcept {
        std::size_t count = 0;
        while ( !children_.empty() ) {
            children_.back().remove_from_parent();
            ++count;
        }
        return count;
    }

    std::size_t node::child_count() const noexcept {
        return children_.size();
    }

    std::size_t node::child_count_recursive() const noexcept {
        std::size_t count = child_count();
        for ( const node& child : children_ ) {
            count += child.child_count_recursive();
        }
        return count;
    }

    bool node::add_child(const node_iptr& child) noexcept {
        return add_child_to_front(child);
    }

    bool node::add_child_to_back(
        const node_iptr& child) noexcept
    {
        if ( !child ) {
            return false;
        }
        child->remove_from_parent();
        children_.push_front(*child);
        child->parent_ = this;
        return true;
    }

    bool node::add_child_to_front(
        const node_iptr& child) noexcept
    {
        if ( !child ) {
            return false;
        }
        child->remove_from_parent();
        children_.push_back(*child);
        child->parent_ = this;
        return true;
    }

    bool node::add_child_before(
        const const_node_iptr& before,
        const node_iptr& child) noexcept
    {
        if ( !before ) {
            return add_child_to_back(child);
        }

        if ( !child || before->parent_ != this ) {
            return false;
        }

        child->remove_from_parent();
        const auto iter = node_children::iterator_to(*before);
        children_.insert(iter, *child);
        child->parent_ = this;
        return true;
    }

    bool node::add_child_after(
        const const_node_iptr& after,
        const node_iptr& child) noexcept
    {
        if ( !after ) {
            return add_child_to_front(child);
        }

        if ( !child || after->parent_ != this ) {
            return false;
        }

        child->remove_from_parent();
        const auto iter = ++node_children::iterator_to(*after);
        children_.insert(iter, *child);
        child->parent_ = this;
        return true;
    }

    bool node::send_backward() noexcept {
        const_node_iptr prev = prev_sibling();
        return prev
            ? parent_->add_child_before(prev, this)
            : false;
    }

    bool node::bring_to_back() noexcept {
        const_node_iptr prev = prev_sibling();
        return prev
            ? parent_->add_child_to_back(this)
            : false;
    }

    bool node::send_forward() noexcept {
        const_node_iptr next = next_sibling();
        return next
            ? parent_->add_child_after(next, this)
            : false;
    }

    bool node::bring_to_front() noexcept {
        const_node_iptr next = next_sibling();
        return next
            ? parent_->add_child_to_front(this)
            : false;
    }

    node_iptr node::first_child() noexcept {
        return !children_.empty()
            ? node_iptr(&children_.front())
            : nullptr;
    }

    const_node_iptr node::first_child() const noexcept {
        return !children_.empty()
            ? const_node_iptr(&children_.front())
            : nullptr;
    }

    node_iptr node::last_child() noexcept {
        return !children_.empty()
            ? node_iptr(&children_.back())
            : nullptr;
    }

    const_node_iptr node::last_child() const noexcept {
        return !children_.empty()
            ? const_node_iptr(&children_.back())
            : nullptr;
    }

    node_iptr node::prev_sibling() noexcept {
        if ( !parent_ ) {
            return nullptr;
        }
        auto iter = node_children::iterator_to(*this);
        if ( iter == parent_->children_.begin() ) {
            return nullptr;
        }
        --iter;
        return node_iptr(&*iter);
    }

    const_node_iptr node::prev_sibling() const noexcept {
        if ( !parent_ ) {
            return nullptr;
        }
        auto iter = node_children::iterator_to(*this);
        if ( iter == parent_->children_.begin() ) {
            return nullptr;
        }
        --iter;
        return const_node_iptr(&*iter);
    }

    node_iptr node::next_sibling() noexcept {
        if ( !parent_ ) {
            return nullptr;
        }
        auto iter = node_children::iterator_to(*this);
        if ( ++iter == parent_->children_.end() ) {
            return nullptr;
        }
        return node_iptr(&*iter);
    }

    const_node_iptr node::next_sibling() const noexcept {
        if ( !parent_ ) {
            return nullptr;
        }
        auto iter = node_children::iterator_to(*this);
        if ( ++iter == parent_->children_.end() ) {
            return nullptr;
        }
        return const_node_iptr(&*iter);
    }
}
