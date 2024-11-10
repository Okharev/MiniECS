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
template <typename T, typename Tuple>
constexpr bool is_in_tuple_v = is_in_tuple<T, Tuple>::value;

class ComponentRegistry {
   public:
    Components<CEnemy, CPlayer, CBullet, CProp, CTransform, CShape, CCollision, CInput, CScore, CFireCooldown, CLifespan> m_components;

    template <class T1>
    requires(is_in_tuple_v<std::unordered_map<EntityId, T1>, decltype(m_components)>)
    void Register(EntityId entityId, T1 component) {
        const auto RegisterComponent = [&](auto&& hashmap, EntityId entityId) {
            using T2 = std::decay_t<decltype(hashmap)>;
            if constexpr (std::is_same_v<std::unordered_map<EntityId, T1>, T2>) {
                hashmap[entityId] = component;
            }
        };

        std::apply([&](auto&&... components) {
           (RegisterComponent(components, entityId), ...);
        }, m_components);
    }

    template <class... T>
    void Register(EntityId entityId, T&&... components) {
        (Register(entityId, components), ...);
    }

    template <class T1>
    requires(is_in_tuple_v<std::unordered_map<EntityId, T1>, decltype(m_components)>)
    void Unregister(EntityId entityId, T1 component) {
        const auto UnregisterComponent = [&](auto&& hashmap, EntityId entityId) {
            using T2 = std::decay_t<decltype(hashmap)>;
            if constexpr (std::is_same_v<std::unordered_map<EntityId, T1>, T2>) {
                hashmap.erase(entityId);
            }
        };

        std::apply([&](auto&&... components) {
            (UnregisterComponent(components, entityId), ...);
        }, m_components);
    }

    void Unregister(EntityId entityId) {
        std::apply([&](auto&&... components) {
            (components.erase(entityId), ...);
        }, m_components);
    }

    template <class T1>
    requires(is_in_tuple_v<std::unordered_map<EntityId, T1>, decltype(m_components)>)
    T1* Get(EntityId entityId) {

        T1* pointer = nullptr;

        const auto FindComponent = [&](auto&& hashmap, EntityId entityId) {
            using T2 = std::decay_t<decltype(hashmap)>;
            if constexpr (std::is_same_v<std::unordered_map<EntityId, T1>, T2>) {
                if (hashmap.find(entityId) == hashmap.end()) {
                    return;
                }
                pointer = &hashmap[entityId];
            }
        };

        std::apply([&](auto&&... components) {
            (FindComponent(components, entityId), ...);
        },
        m_components);

        return pointer;
    }

    template<typename ... T>
    auto Query(EntityId entityId) {
        return std::tuple<T*...>(Get<T>(entityId) ...);
    }
};

#endif  //COMPONENTREGISTRY_H
