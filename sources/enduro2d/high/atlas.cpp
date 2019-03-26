/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019 Matvey Cherevko
 ******************************************************************************/

#include <enduro2d/high/atlas.hpp>

namespace e2d
{
    atlas::atlas() = default;
    atlas::~atlas() noexcept = default;

    atlas::atlas(atlas&& other) noexcept {
        assign(std::move(other));
    }

    atlas& atlas::operator=(atlas&& other) noexcept {
        return assign(std::move(other));
    }

    atlas::atlas(const atlas& other) {
        assign(other);
    }

    atlas& atlas::operator=(const atlas& other) {
        return assign(other);
    }

    void atlas::clear() noexcept {
        texture_.reset();
        regions_.clear();
        shape_regions_.clear();
    }

    void atlas::swap(atlas& other) noexcept {
        using std::swap;
        swap(texture_, other.texture_);
        swap(regions_, other.regions_);
        swap(shape_regions_, other.shape_regions_);
    }

    atlas& atlas::assign(atlas&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    atlas& atlas::assign(const atlas& other) {
        if ( this != &other ) {
            atlas s;
            s.texture_ = other.texture_;
            s.regions_ = other.regions_;
            s.shape_regions_ = other.shape_regions_;
            swap(s);
        }
        return *this;
    }

    atlas& atlas::set_texture(const texture_asset::ptr& texture) noexcept {
        texture_ = texture;
        return *this;
    }

    const texture_asset::ptr& atlas::texture() const noexcept {
        return texture_;
    }

    atlas& atlas::set_regions(vector<region>&& regions) noexcept {
        regions_ = std::move(regions);
        std::sort(
            regions_.begin(), regions_.end(),
            [](const atlas::region& l, const atlas::region& r) noexcept {
                return l.name < r.name;
            });
        return *this;
    }

    atlas& atlas::set_regions(const vector<region>& regions) {
        return set_regions(vector<region>(regions));
    }

    const vector<atlas::region>& atlas::regions() const noexcept {
        return regions_;
    }

    const atlas::region* atlas::find_region(str_hash name) const noexcept {
        const auto iter = std::lower_bound(
            regions_.begin(), regions_.end(), name,
            [](const atlas::region& l, str_hash r) noexcept {
                return l.name < r;
            });
        return iter != regions_.end() && iter->name == name
            ? &*iter
            : nullptr;
    }

    atlas& atlas::set_shape_regions(vector<shape_region>&& regions) noexcept {
        shape_regions_ = std::move(regions);
        std::sort(
            shape_regions_.begin(), shape_regions_.end(),
            [](const atlas::shape_region& l, const atlas::shape_region& r) noexcept {
                return l.name < r.name;
            });
        return *this;
    }

    atlas& atlas::set_shape_regions(const vector<shape_region>& regions) {
        return set_shape_regions(vector<shape_region>(regions));
    }

    const vector<atlas::shape_region>& atlas::shape_regions() const noexcept {
        return shape_regions_;
    }

    const atlas::shape_region* atlas::find_shape_region(str_hash name) const noexcept {
        const auto iter = std::lower_bound(
            shape_regions_.begin(), shape_regions_.end(), name,
            [](const atlas::shape_region& l, str_hash r) noexcept {
                return l.name < r;
            });
        return iter != shape_regions_.end() && iter->name == name
            ? &*iter
            : nullptr;
    }
}

namespace e2d
{
    void swap(atlas& l, atlas& r) noexcept {
        l.swap(r);
    }

    bool operator==(const atlas& l, const atlas& r) noexcept {
        return l.texture() == r.texture()
            && l.regions() == r.regions()
            && l.shape_regions() == r.shape_regions();
    }

    bool operator!=(const atlas& l, const atlas& r) noexcept {
        return !(l == r);
    }

    void swap(atlas::region& l, atlas::region& r) noexcept {
        using std::swap;
        swap(l.name, r.name);
        swap(l.pivot, r.pivot);
        swap(l.texrect, r.texrect);
    }

    bool operator==(const atlas::region& l, const atlas::region& r) noexcept {
        return l.name == r.name
            && l.pivot == r.pivot
            && l.texrect == r.texrect;
    }

    bool operator!=(const atlas::region& l, const atlas::region& r) noexcept {
        return !(l == r);
    }

    void swap(atlas::shape_region& l, atlas::shape_region& r) noexcept {
        using std::swap;
        swap(l.name, r.name);
        swap(l.pivot, r.pivot);
        swap(l.points, r.points);
    }

    bool operator==(const atlas::shape_region& l, const atlas::shape_region& r) noexcept {
        return l.name == r.name
            && l.pivot == r.pivot
            && l.points == r.points;
    }

    bool operator!=(const atlas::shape_region& l, const atlas::shape_region& r) noexcept {
        return !(l == r);
    }
}
