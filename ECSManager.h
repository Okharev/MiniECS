//
// Created by Okharev on 27/07/2024.
//

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <array>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "ComponentRegistry.h"
#include "Entity.h"

auto constexpr MAX_ENTITIES = 2048;

template <typename... TComponents>
using ECSQuery = std::unordered_map<EntityId, std::tuple<TComponents*...>>;

class ECSManager {

    std::unordered_map<EntityId, Entity> _entities;
    EntityIndex _entityCreated;
    std::vector<EntityId> _entitiesToDelete;

   private:
    void deleteEntity(EntityId entityIndex);

   public:
    ComponentRegistry m_componentRegistry;

   public:
    inline ECSManager() : _entityCreated(0) {
        _entities.reserve( MAX_ENTITIES );
    }

    Entity& addEntity();
    void markForDeletion(EntityId entityId);

    template<typename ... TComponents>
    inline auto QueryComponents() -> ECSQuery<TComponents...> {
        ECSQuery<TComponents...> query;

        for (auto&& entity : _entities) {
            const auto entityId = entity.second.m_id;
            auto entityQuery = m_componentRegistry.Query<TComponents...>(entityId);
            bool hasAnyPointerNull = false;

            /**
             * Find if any components is null before adding first
             */
            const auto IsValidComponent = [&](auto&& component) {
                if (component == nullptr) {
                    hasAnyPointerNull = true;
                }
            };

            std::apply([&](auto&&... components) {
                    (IsValidComponent(components), ...);
                },
                entityQuery);

            if (hasAnyPointerNull) {
                continue;
            }

            query[entityId] = m_componentRegistry.Query<TComponents...>(entityId);
        }

        return query;
    }

    template<typename ... TComponents>
    inline auto QueryComponents(ECSQuery<TComponents...> &query) {
        query = QueryComponents<TComponents...>();
    }
};

#endif  // ENTITYMANAGER_H