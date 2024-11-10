//
// Created by Okharev on 08/11/2024.
//

#ifndef COMPONENTREGISTRY_H
#define COMPONENTREGISTRY_H

#include <unordered_map>

#include "Components.h"
#include "Entity.h"

template <typename T, typename Variant>
struct is_in_tuple;

template <typename T, typename... Types>
struct is_in_tuple<T, std::tuple<Types...>> : std::disjunction<std::is_same<T, Types>...> {};

// Helper variable template
template <typename T, typename Variant>
constexpr bool is_in_tuple_v = is_in_tuple<T, Variant>::value;

class ComponentRegistry {
   public:
    Components m_components;

    template <class T1>
    requires(is_in_tuple_v<std::unordered_map<EntityId, T1>, Components>)
    void Register(EntityId entityId, T1 component) {
        const auto FindComponent = [&](auto&& hashmap, EntityId entityId)
        {
            using T2 = std::decay_t<decltype(component)>;
            if constexpr (std::is_same_v<std::unordered_map<EntityId, T1>, T2>) {
                hashmap[entityId] = component;
            }
        };

        std::apply([&](auto&&... hashmap) {(FindComponent(hashmap, entityId), ...);}, m_components);
    }

    template <class T1>
    requires(is_in_tuple_v<std::unordered_map<EntityId, T1>, Components>)
    void Unregister(EntityId entityId, T1 component) {

        const auto FindComponent = [&](auto&& hashmap, EntityId entityId)
        {
            using T2 = std::decay_t<decltype(hashmap)>;
            if constexpr (std::is_same_v<std::unordered_map<EntityId, T1>, T2>) {
                hashmap.erase(entityId);
            }
        };

        std::apply([&](auto&&... hashmap) {(FindComponent(hashmap, entityId), ...);}, m_components);
    }

    void Unregister(EntityId entityId) {
        std::apply([&](auto&&... hashmap) {(hashmap.erase(entityId), ...);}, m_components);
    }

    template <class T1>
    requires(is_in_tuple_v<std::unordered_map<EntityId, T1>, Components>)
    T1* Get(EntityId entityId) {

        T1* pointer = nullptr;

        const auto FindComponent = [&](auto&& hashmap, EntityId entityId)
        {
            using T2 = std::decay_t<decltype(hashmap)>;
            if constexpr (std::is_same_v<std::unordered_map<EntityId, T1>, T2>) {
                pointer = &hashmap[entityId];
            }

            return pointer;
        };

        std::apply([&](auto&&... hashmap) {(FindComponent(hashmap, entityId), ...);}, m_components);

        // for(auto && anyComponents : m_components) {
        //     std::visit(
        //         [&](auto&& components) {
        //             using T2 = std::decay_t<decltype(components)>;

        //         if constexpr (std::is_same_v<std::unordered_map<EntityId, T1>, T2>) {
        //                 pointer = &components[entityId];
        //             }
        //         },
        //         anyComponents);

        //     if (pointer) {
        //         return pointer;
        //     }
        // }

        return pointer;
    }

    template<typename ... T>
    auto Query(EntityId entityId) {
        return std::tuple<T*...>(Get<T>(entityId) ...);
    }
};

#endif  //COMPONENTREGISTRY_H
