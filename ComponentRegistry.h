//
// Created by Okharev on 08/11/2024.
//

#ifndef COMPONENTREGISTRY_H
#define COMPONENTREGISTRY_H

#include <unordered_map>

#include "Components.h"
#include "Entity.h"

template <typename T, typename Variant>
struct is_in_variant;

template <typename T, typename... Types>
struct is_in_variant<T, std::variant<Types...>> : std::disjunction<std::is_same<T, Types>...> {};

// Helper variable template
template <typename T, typename Variant>
constexpr bool is_in_variant_v = is_in_variant<T, Variant>::value;

class ComponentRegistry {

    std::vector<AnyComponents> allComponents;

   public:
    template <class T1>
    requires(is_in_variant_v<T1, AnyComponents>) void Registry(EntityId entityId, T1 component) {
        for(auto && anyComponents : allComponents) {
            std::visit(
                [&](auto&& components) {
                    using T2 = std::decay_t<decltype(components)>;

                    if constexpr (std::is_same_v<T1, T2>) {
                        components[entityId] = component;
                    }
                },
                anyComponents);
        }
    }
    template <class T1>
    requires(is_in_variant_v<T1, AnyComponents>) void Unregister(EntityId entityId) {
        for(auto && anyComponents : allComponents) {
            std::visit(
                [&](auto&& components) {
                    using T2 = std::decay_t<decltype(components)>;

                    if constexpr (std::is_same_v<T1, T2>) {
                        std::erase_if(components, [&](auto&& map) {
                           return std::get<EntityId>(map) == entityId;
                        });
                    }
                },
                anyComponents);
        }
    }

    void Unregister(EntityId entityId) {
        for(auto && anyComponents : allComponents) {
            std::visit(
                [&](auto&& components) {
                    using T2 = std::decay_t<decltype(components)>;

                        std::erase_if(components, [&](auto&& map) {
                           return std::get<EntityId>(map) == entityId;
                        });
                },
                anyComponents);
        }
    }
};

#endif  //COMPONENTREGISTRY_H
