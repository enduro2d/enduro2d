/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/node.hpp>
#include <enduro2d/high/world.hpp>

namespace
{
    using namespace e2d;

    struct iptr_release_disposer {
        void operator()(node* n) const noexcept {
            intrusive_ptr_release(n);
        }
    };
}

namespace e2d
{
    node::node(world& world)
    : entity_(world.registry().create_entity()) {}

    node::~node() noexcept {
        E2D_ASSERT(!parent_);
        while ( !children_.empty() ) {
            node_iptr child(&children_.back());
            children_.pop_back_and_dispose(iptr_release_disposer());
            child->parent_ = nullptr;
            child->on_change_parent_();
        }
    }

    node_iptr node::create(world& world) {
        return node_iptr(new node(world));
    }

    node_iptr node::create(world& world, const node_iptr& parent) {
        node_iptr child = create(world);
        if ( parent ) {
            parent->add_child(child);
        }
        return child;
    }

    ecs::entity node::entity() noexcept {
        return entity_;
    }

    ecs::const_entity node::entity() const noexcept {
        return entity_;
    }

    void node::transform(const t3f& transform) noexcept {
        transform_ = transform;
        mark_dirty_local_matrix_();
    }

    const t3f& node::transform() const noexcept {
        return transform_;
    }

    void node::translation(const v3f& translation) noexcept {
        transform_.translation = translation;
        mark_dirty_local_matrix_();
    }

    const v3f& node::translation() const noexcept {
        return transform_.translation;
    }

    void node::rotation(const q4f& rotation) noexcept {
        transform_.rotation = rotation;
        mark_dirty_local_matrix_();
    }

    const q4f& node::rotation() const noexcept {
        return transform_.rotation;
    }

    void node::scale(const v3f& scale) noexcept {
        transform_.scale = scale;
        mark_dirty_local_matrix_();
    }

    const v3f& node::scale() const noexcept {
        return transform_.scale;
    }

    const m4f& node::local_matrix() const noexcept {
        if ( math::check_and_clear_any_flags(flags_, fm_dirty_local_matrix) ) {
            update_local_matrix_();
        }
        return local_matrix_;
    }

    const m4f& node::world_matrix() const noexcept {
        if ( math::check_and_clear_any_flags(flags_, fm_dirty_world_matrix) ) {
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
        const node* p = parent_;
        while ( p && p != parent ) {
            p = p->parent_;
        }
        return !!p;
    }

    bool node::remove_from_parent() noexcept {
        if ( !parent_ ) {
            return false;
        }
        node_iptr self(this);
        parent_->children_.erase_and_dispose(
            node_children::iterator_to(*this),
            iptr_release_disposer());
        parent_->on_change_children_();
        parent_ = nullptr;
        on_change_parent_();
        return true;
    }

    std::size_t node::remove_all_children() noexcept {
        std::size_t count = 0;
        while ( !children_.empty() ) {
            node_iptr child(&children_.back());
            children_.pop_back_and_dispose(iptr_release_disposer());
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
            child->parent_->children_.erase_and_dispose(
                node_children::iterator_to(*child),
                iptr_release_disposer());
            child->parent_->on_change_children_();
            child->parent_ = nullptr;
        }

        intrusive_ptr_add_ref(child.get());
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
            child->parent_->children_.erase_and_dispose(
                node_children::iterator_to(*child),
                iptr_release_disposer());
            child->parent_->on_change_children_();
            child->parent_ = nullptr;
        }

        intrusive_ptr_add_ref(child.get());
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
            child->parent_->children_.erase_and_dispose(
                node_children::iterator_to(*child),
                iptr_release_disposer());
            child->parent_->on_change_children_();
            child->parent_ = nullptr;
        }

        intrusive_ptr_add_ref(child.get());
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
            child->parent_->children_.erase_and_dispose(
                node_children::iterator_to(*child),
                iptr_release_disposer());
            child->parent_->on_change_children_();
            child->parent_ = nullptr;
        }

        intrusive_ptr_add_ref(child.get());
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
