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
    requires(is_in_tuple_v<Component<T1>, decltype(m_components)>)
    void Register(EntityId entityId, T1 component) {
        std::get<Component<T1>>(m_components)[entityId] = component;
    }

    template <class... T>
    void Register(EntityId entityId, T&&... components) {
        (Register(entityId, components), ...);
    }

    template <class T1>
    requires(is_in_tuple_v<Component<T1>, decltype(m_components)>)
    void Unregister(EntityId entityId, T1 component) {
        std::get<Component<T1>>(m_components).erase(entityId);
    }

    void Unregister(EntityId entityId) {
        std::apply([&](auto&&... components) {
            (components.erase(entityId), ...);
        }, m_components);
    }

    template <class T1>
    requires(is_in_tuple_v<Component<T1>, decltype(m_components)>)
    T1* Get(EntityId entityId) {

        auto& hashmap = std::get<Component<T1>>(m_components);

        if (hashmap.find(entityId) == hashmap.end()) {
            return nullptr;
        }
    
        return &hashmap[entityId];
    }

    template<typename ... T>
    auto Query(EntityId entityId) {
        return std::tuple<T*...>(Get<T>(entityId) ...);
    }
};

#endif  //COMPONENTREGISTRY_H
