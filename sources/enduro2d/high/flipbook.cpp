/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enduro2d/high/flipbook.hpp>

namespace e2d
{
    flipbook::flipbook(flipbook&& other) noexcept {
        assign(std::move(other));
    }

    flipbook& flipbook::operator=(flipbook&& other) noexcept {
        return assign(std::move(other));
    }

    flipbook::flipbook(const flipbook& other) {
        assign(other);
    }

    flipbook& flipbook::operator=(const flipbook& other) {
        return assign(other);
    }

    void flipbook::clear() noexcept {
        frames_.clear();
        sequences_.clear();
    }

    void flipbook::swap(flipbook& other) noexcept {
        using std::swap;
        swap(frames_, other.frames_);
        swap(sequences_, other.sequences_);
    }

    flipbook& flipbook::assign(flipbook&& other) noexcept {
        if ( this != &other ) {
            swap(other);
            other.clear();
        }
        return *this;
    }

    flipbook& flipbook::assign(const flipbook& other) {
        if ( this != &other ) {
            flipbook s;
            s.frames_ = other.frames_;
            s.sequences_ = other.sequences_;
            swap(s);
        }
        return *this;
    }

    flipbook& flipbook::set_frames(vector<frame>&& frames) noexcept {
        frames_ = std::move(frames);
        return *this;
    }

    flipbook& flipbook::set_frames(const vector<frame>& frames) {
        return set_frames(vector<frame>(frames));
    }

    const vector<flipbook::frame>& flipbook::frames() const noexcept {
        return frames_;
    }

    const flipbook::frame* flipbook::find_frame(std::size_t index) const noexcept {
        return index < frames_.size()
            ? &frames_[index]
            : nullptr;
    }

    flipbook& flipbook::set_sequences(vector<sequence>&& sequences) noexcept {
        sequences_ = std::move(sequences);
        std::sort(
            sequences_.begin(), sequences_.end(),
            [](const flipbook::sequence& l, const flipbook::sequence& r) noexcept {
                return l.name < r.name;
            });
        return *this;
    }

    flipbook& flipbook::set_sequences(const vector<sequence>& sequences) {
        return set_sequences(vector<sequence>(sequences));
    }

    const vector<flipbook::sequence>& flipbook::sequences() const noexcept {
        return sequences_;
    }

    const flipbook::sequence* flipbook::find_sequence(str_hash name) const noexcept {
        const auto iter = std::lower_bound(
            sequences_.begin(), sequences_.end(), name,
            [](const flipbook::sequence& l, str_hash r) noexcept {
                return l.name < r;
            });
        return iter != sequences_.end() && iter->name == name
            ? &*iter
            : nullptr;
    }
}

namespace e2d
{
    void swap(flipbook& l, flipbook& r) noexcept {
        l.swap(r);
    }

    bool operator==(const flipbook& l, const flipbook& r) noexcept {
        if ( l.frames().size() != r.frames().size() ) {
            return false;
        }
        if ( l.sequences().size() != r.sequences().size() ) {
            return false;
        }
        return l.frames() == r.frames()
            && l.sequences() == r.sequences();
    }

    bool operator!=(const flipbook& l, const flipbook& r) noexcept {
        return !(l == r);
    }

    void swap(flipbook::frame& l, flipbook::frame& r) noexcept {
        using std::swap;
        swap(l.sprite, r.sprite);
    }

    bool operator==(const flipbook::frame& l, const flipbook::frame& r) noexcept {
        return l.sprite == r.sprite;
    }

    bool operator!=(const flipbook::frame& l, const flipbook::frame& r) noexcept {
        return !(l == r);
    }

    void swap(flipbook::sequence& l, flipbook::sequence& r) noexcept {
        using std::swap;
        swap(l.fps, r.fps);
        swap(l.name, r.name);
        swap(l.frames, r.frames);
    }

    bool operator==(const flipbook::sequence& l, const flipbook::sequence& r) noexcept {
        return math::approximately(l.fps, r.fps)
            && l.name == r.name
            && l.frames == r.frames;
    }

    bool operator!=(const flipbook::sequence& l, const flipbook::sequence& r) noexcept {
        return !(l == r);
    }
}
