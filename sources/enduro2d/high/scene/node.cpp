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
        while ( !children_.empty() ) {
            node_iptr child(&children_.back());
            children_.pop_back();
            child->parent_ = nullptr;
            child->on_change_parent_();
        }
        if ( parent_ ) {
            parent_->children_.erase(
                node_children::iterator_to(*this));
            parent_->on_change_children_();
            parent_ = nullptr;
        }
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

    void node::transform(const t3f& transform) noexcept {
        transform_ = transform;
        mark_dirty_local_matrix_();
    }

    const t3f& node::transform() const noexcept {
        return transform_;
    }

    const m4f& node::local_matrix() const noexcept {
        if ( math::check_and_clear_all_flags(flags_, fm_dirty_local_matrix) ) {
            update_local_matrix_();
        }
        return local_matrix_;
    }

    const m4f& node::world_matrix() const noexcept {
        if ( math::check_and_clear_all_flags(flags_, fm_dirty_world_matrix) ) {
            update_world_matrix_();
        }
        return world_matrix_;
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
        parent_->on_change_children_();
        parent_ = nullptr;
        on_change_parent_();
        return true;
    }

    std::size_t node::remove_all_children() noexcept {
        std::size_t count = 0;
        while ( !children_.empty() ) {
            node_iptr child(&children_.back());
            children_.pop_back();
            child->parent_ = nullptr;
            child->on_change_parent_();
            ++count;
        }
        if ( count > 0 ) {
            on_change_children_();
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

        if ( child == first_child() ) {
            return true;
        }

        if ( child->parent_ && child->parent_ == this ) {
            children_.erase(node_children::iterator_to(*child));
            children_.push_front(*child);
            on_change_children_();
            return true;
        }

        if ( child->parent_ ) {
            child->parent_->children_.erase(node_children::iterator_to(*child));
            child->parent_->on_change_children_();
            child->parent_ = nullptr;
        }

        children_.push_front(*child);
        child->parent_ = this;
        child->on_change_parent_();
        on_change_children_();
        return true;
    }

    bool node::add_child_to_front(
        const node_iptr& child) noexcept
    {
        if ( !child ) {
            return false;
        }

        if ( child == last_child() ) {
            return true;
        }

        if ( child->parent_ && child->parent_ == this ) {
            children_.erase(node_children::iterator_to(*child));
            children_.push_back(*child);
            on_change_children_();
            return true;
        }

        if ( child->parent_ ) {
            child->parent_->children_.erase(node_children::iterator_to(*child));
            child->parent_->on_change_children_();
            child->parent_ = nullptr;
        }

        children_.push_back(*child);
        child->parent_ = this;
        child->on_change_parent_();
        on_change_children_();
        return true;
    }

    bool node::add_child_before(
        const node_iptr& before,
        const node_iptr& child) noexcept
    {
        if ( !before || !child || before->parent_ != this ) {
            return false;
        }

        if ( before == child || before->prev_sibling() == child ) {
            return true;
        }

        if ( child->parent_ && child->parent_ == this ) {
            children_.erase(node_children::iterator_to(*child));
            children_.insert(
                node_children::iterator_to(*before),
                *child);
            on_change_children_();
            return true;
        }

        if ( child->parent_ ) {
            child->parent_->children_.erase(node_children::iterator_to(*child));
            child->parent_->on_change_children_();
            child->parent_ = nullptr;
        }

        children_.insert(
            node_children::iterator_to(*before),
            *child);
        child->parent_ = this;
        child->on_change_parent_();
        on_change_children_();
        return true;
    }

    bool node::add_child_after(
        const node_iptr& after,
        const node_iptr& child) noexcept
    {
        if ( !after || !child || after->parent_ != this ) {
            return false;
        }

        if ( after == child || after->next_sibling() == child ) {
            return true;
        }

        if ( child->parent_ && child->parent_ == this ) {
            children_.erase(node_children::iterator_to(*child));
            children_.insert(
                ++node_children::iterator_to(*after),
                *child);
            on_change_children_();
            return true;
        }

        if ( child->parent_ ) {
            child->parent_->children_.erase(node_children::iterator_to(*child));
            child->parent_->on_change_children_();
            child->parent_ = nullptr;
        }

        children_.insert(
            ++node_children::iterator_to(*after),
            *child);
        child->parent_ = this;
        child->on_change_parent_();
        on_change_children_();
        return true;
    }

    bool node::add_sibling_before(const node_iptr& sibling) noexcept {
        return parent_
            ? parent_->add_child_before(this, sibling)
            : false;
    }

    bool node::add_sibling_after(const node_iptr& sibling) noexcept {
        return parent_
            ? parent_->add_child_after(this, sibling)
            : false;
    }

    bool node::send_backward() noexcept {
        node_iptr prev = prev_sibling();
        return prev
            ? prev->add_sibling_before(this)
            : false;
    }

    bool node::bring_to_back() noexcept {
        return parent_
            ? parent_->add_child_to_back(this)
            : false;
    }

    bool node::send_forward() noexcept {
        node_iptr next = next_sibling();
        return next
            ? next->add_sibling_after(this)
            : false;
    }

    bool node::bring_to_front() noexcept {
        return parent_
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

namespace e2d
{
    void node::on_change_parent_() noexcept {
        mark_dirty_world_matrix_();
    }

    void node::on_change_children_() noexcept {
    }
}

namespace e2d
{
    void node::mark_dirty_local_matrix_() noexcept {
        if ( math::check_and_set_any_flags(flags_, fm_dirty_local_matrix) ) {
            mark_dirty_world_matrix_();
        }
    }

    void node::mark_dirty_world_matrix_() noexcept {
        if ( math::check_and_set_any_flags(flags_, fm_dirty_world_matrix) ) {
            for ( node& child : children_ ) {
                child.mark_dirty_world_matrix_();
            }
        }
    }

    void node::update_local_matrix_() const noexcept {
        local_matrix_ = math::make_trs_matrix4(transform_);
    }

    void node::update_world_matrix_() const noexcept {
        world_matrix_ = parent_
            ? parent_->world_matrix() * local_matrix()
            : local_matrix();
    }
}
