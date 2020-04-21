/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/resources/prefab.hpp>

namespace e2d
{
    prefab::prefab(prefab&& other) noexcept {
        assign(std::move(other));
    }

    prefab& prefab::operator=(prefab&& other) noexcept {
        return assign(std::move(other));
    }

    prefab::prefab(const prefab& other) {
        assign(other);
    }

    prefab& prefab::operator=(const prefab& other) {
        return assign(other);
    }

    void prefab::clear() noexcept {
        uuid_.clear();
        children_.clear();
        prototype_.clear();
    }

    void prefab::swap(prefab& other) noexcept {
        using std::swap;
        swap(uuid_, other.uuid_);
        swap(children_, other.children_);
        swap(prototype_, other.prototype_);
    }

    prefab& prefab::assign(prefab&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    prefab& prefab::assign(const prefab& other) {
        if ( this != &other ) {
            prefab s;
            s.uuid_ = other.uuid_;
            s.children_ = other.children_;
            s.prototype_ = other.prototype_;
            swap(s);
        }
        return *this;
    }

    prefab& prefab::set_uuid(str&& uuid) noexcept {
        uuid_ = std::move(uuid);
        return *this;
    }

    prefab& prefab::set_uuid(const str& uuid) {
        uuid_ = uuid;
        return *this;
    }

    prefab& prefab::set_children(vector<prefab>&& children) noexcept {
        children_ = std::move(children);
        return *this;
    }

    prefab& prefab::set_children(const vector<prefab>& children) {
        children_ = children;
        return *this;
    }

    prefab& prefab::set_prototype(ecs::prototype&& prototype) noexcept {
        prototype_ = std::move(prototype);
        return *this;
    }

    prefab& prefab::set_prototype(const ecs::prototype& prototype) {
        prototype_ = prototype;
        return *this;
    }

    str& prefab::uuid() noexcept {
        return uuid_;
    }

    const str& prefab::uuid() const noexcept {
        return uuid_;
    }

    vector<prefab>& prefab::children() noexcept {
        return children_;
    }

    const vector<prefab>& prefab::children() const noexcept {
        return children_;
    }

    ecs::prototype& prefab::prototype() noexcept {
        return prototype_;
    }

    const ecs::prototype& prefab::prototype() const noexcept {
        return prototype_;
    }
}

namespace e2d
{
    void swap(prefab& l, prefab& r) noexcept {
        l.swap(r);
    }
}
