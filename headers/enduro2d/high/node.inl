/*******************************************************************************
 * This file is part of the "Enduro2D"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2018-2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include "node.hpp"

namespace e2d::nodes::impl
{
    template < typename F, typename... Args >
    bool invoke_with_force_bool(F&& f, Args&&... args) {
        if constexpr ( std::is_invocable_r_v<bool, F, Args...> ) {
            return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
        } else {
            std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
            return true;
        }
    }
}

namespace e2d::nodes
{
    template < typename Node, typename F >
    bool for_each_child(
        const intrusive_ptr<Node>& root,
        F&& f,
        const options& opts)
    {
        if ( !root ) {
            return true;
        }

        if ( opts.reversed() ) {
            for ( auto child = root->last_child(); child; child = child->prev_sibling() ) {
                if ( opts.recursive() && !for_each_child(child, f, options(opts).include_root(false)) ) {
                    return false;
                }

                if ( !impl::invoke_with_force_bool(f, child) ) {
                    return false;
                }
            }

            if ( opts.include_root() && !impl::invoke_with_force_bool(f, root) ) {
                return false;
            }
        } else {
            if ( opts.include_root() && !impl::invoke_with_force_bool(f, root) ) {
                return false;
            }

            for ( auto child = root->first_child(); child; child = child->next_sibling() ) {
                if ( !impl::invoke_with_force_bool(f, child) ) {
                    return false;
                }

                if ( opts.recursive() && !for_each_child(child, f, options(opts).include_root(false)) ) {
                    return false;
                }
            }
        }

        return true;
    }

    template < typename Node, typename F >
    bool for_each_parent(
        const intrusive_ptr<Node>& root,
        F&& f,
        const options& opts)
    {
        if ( !root ) {
            return true;
        }

        if ( opts.reversed() ) {
            if ( root->has_parent() ) {
                if ( opts.recursive() && !for_each_parent(root->parent(), f, options(opts).include_root(false)) ) {
                    return false;
                }

                if ( !impl::invoke_with_force_bool(f, root->parent()) ) {
                    return false;
                }
            }

            if ( opts.include_root() && !impl::invoke_with_force_bool(f, root) ) {
                return false;
            }
        } else {
            if ( opts.include_root() && !impl::invoke_with_force_bool(f, root) ) {
                return false;
            }

            if ( root->has_parent() ) {
                if ( !impl::invoke_with_force_bool(f, root->parent()) ) {
                    return false;
                }

                if ( opts.recursive() && !for_each_parent(root->parent(), f, options(opts).include_root(false)) ) {
                    return false;
                }
            }
        }

        return true;
    }
}

namespace e2d::nodes
{
    template < typename Node, typename Iter >
    std::size_t extract_parents(
        const intrusive_ptr<Node>& root,
        Iter iter,
        const options& opts)
    {
        std::size_t count{0u};
        for_each_parent(root, [&count, &iter](const auto& parent){
            ++count;
            iter++ = parent;
        }, opts);
        return count;
    }

    template < typename Node, typename Iter >
    std::size_t extract_children(
        const intrusive_ptr<Node>& root,
        Iter iter,
        const options& opts)
    {
        std::size_t count{0u};
        for_each_child(root, [&count, &iter](const auto& child){
            ++count;
            iter++ = child;
        }, opts);
        return count;
    }
}

namespace e2d::nodes
{
    template < typename Node, typename F >
    bool for_extracted_parents(
        const intrusive_ptr<Node>& root,
        F&& f,
        const options& opts)
    {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<intrusive_ptr<Node>> parents;

        const std::size_t begin_index = parents.size();
        E2D_DEFER([begin_index](){
            parents.erase(
                parents.begin() + begin_index,
                parents.end());
        });

        extract_parents(
            root,
            std::back_inserter(parents),
            opts);

        const std::size_t end_index = parents.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            if ( !impl::invoke_with_force_bool(f, parents[i]) ) {
                return false;
            }
        }

        return true;
    }

    template < typename Node, typename F >
    bool for_extracted_children(
        const intrusive_ptr<Node>& root,
        F&& f,
        const options& opts)
    {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<intrusive_ptr<Node>> children;

        const std::size_t begin_index = children.size();
        E2D_DEFER([begin_index](){
            children.erase(
                children.begin() + begin_index,
                children.end());
        });

        extract_children(
            root,
            std::back_inserter(children),
            opts);

        const std::size_t end_index = children.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            if ( !impl::invoke_with_force_bool(f, children[i]) ) {
                return false;
            }
        }

        return true;
    }
}

namespace e2d::nodes
{
    template < typename Component, typename Node, typename Iter >
    std::size_t extract_components_from_parents(
        const intrusive_ptr<Node>& root,
        Iter iter,
        const options& opts)
    {
        std::size_t count{0u};
        for_each_parent(root, [&count, &iter](const auto& parent){
            if ( auto component = parent->owner().template component<Component>() ) {
                ++count;
                iter++ = component;
            }
        }, opts);
        return count;
    }

    template < typename Component, typename Node, typename Iter >
    std::size_t extract_components_from_children(
        const intrusive_ptr<Node>& root,
        Iter iter,
        const options& opts)
    {
        std::size_t count{0u};
        for_each_child(root, [&count, &iter](const auto& child){
            if ( auto component = child->owner().template component<Component>() ) {
                ++count;
                iter++ = component;
            }
        }, opts);
        return count;
    }
}

namespace e2d::nodes
{
    template < typename Component, typename Node, typename F >
    bool for_extracted_components_from_parents(
        const intrusive_ptr<Node>& root,
        F&& f,
        const options& opts)
    {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<gcomponent<Component>> components;

        const std::size_t begin_index = components.size();
        E2D_DEFER([begin_index](){
            components.erase(
                components.begin() + begin_index,
                components.end());
        });

        extract_components_from_parents<Component>(
            root,
            std::back_inserter(components),
            opts);

        const std::size_t end_index = components.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            if ( !impl::invoke_with_force_bool(f, components[i]) ) {
                return false;
            }
        }

        return true;
    }

    template < typename Component, typename Node, typename F >
    bool for_extracted_components_from_children(
        const intrusive_ptr<Node>& root,
        F&& f,
        const options& opts)
    {
        //TODO(BlackMat): replace it to frame allocator
        static thread_local vector<gcomponent<Component>> components;

        const std::size_t begin_index = components.size();
        E2D_DEFER([begin_index](){
            components.erase(
                components.begin() + begin_index,
                components.end());
        });

        extract_components_from_children<Component>(
            root,
            std::back_inserter(components),
            opts);

        const std::size_t end_index = components.size();
        for ( std::size_t i = begin_index; i < end_index; ++i ) {
            if ( !impl::invoke_with_force_bool(f, components[i]) ) {
                return false;
            }
        }

        return true;
    }
}

namespace e2d::nodes
{
    template < typename Component, typename Node >
    gcomponent<Component> find_component_from_parents(
        const intrusive_ptr<Node>& root,
        const options& opts)
    {
        gcomponent<Component> component;
        for_each_parent(root, [&component](const auto& child){
            if ( auto child_component = child->owner().template component<Component>() ) {
                component = child_component;
                return false;
            }
            return true;
        }, opts);
        return component;
    }

    template < typename Component, typename Node >
    gcomponent<Component> find_component_from_children(
        const intrusive_ptr<Node>& root,
        const options& opts)
    {
        gcomponent<Component> component;
        for_each_child(root, [&component](const auto& child){
            if ( auto child_component = child->owner().template component<Component>() ) {
                component = child_component;
                return false;
            }
            return true;
        }, opts);
        return component;
    }
}
