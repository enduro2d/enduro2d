/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "../_high.hpp"

namespace e2d
{
    class prefab final {
    public:
        prefab() = default;
        ~prefab() noexcept = default;

        prefab(prefab&& other) noexcept;
        prefab& operator=(prefab&& other) noexcept;

        prefab(const prefab& other);
        prefab& operator=(const prefab& other);

        void clear() noexcept;
        void swap(prefab& other) noexcept;

        prefab& assign(prefab&& other) noexcept;
        prefab& assign(const prefab& other);

        prefab& set_uuid(str&& uuid) noexcept;
        prefab& set_uuid(const str& uuid);

        prefab& set_children(vector<prefab>&& children) noexcept;
        prefab& set_children(const vector<prefab>& children);

        prefab& set_prototype(ecs::prototype&& prototype) noexcept;
        prefab& set_prototype(const ecs::prototype& prototype);

        str& uuid() noexcept;
        const str& uuid() const noexcept;

        vector<prefab>& children() noexcept;
        const vector<prefab>& children() const noexcept;

        ecs::prototype& prototype() noexcept;
        const ecs::prototype& prototype() const noexcept;
    private:
        str uuid_;
        vector<prefab> children_;
        ecs::prototype prototype_;
    };

    void swap(prefab& l, prefab& r) noexcept;
    bool operator==(const prefab& l, const prefab& r) = delete;
    bool operator!=(const prefab& l, const prefab& r) = delete;
}
