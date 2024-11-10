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

#include "ComponentRegistry.h"
#include "Entity.h"

auto constexpr MAX_ENTITIES = 2048;

using EntityArray = std::array<Entity, MAX_ENTITIES>;

class ECSManager {

    static constexpr auto INVALID_ENTITY_INDEX = std::numeric_limits<EntityId>::max();

    EntityArray _entities;
    EntityIndex _entityCount;
    EntityIndex _entityCreated;
    std::vector<EntityId> _entitiesToDelete;


    void deleteEntity(EntityIndex entityIndex);

public:
    ComponentRegistry m_componentRegistry;
    ECSManager() : _entityCount(0), _entityCreated(0) {
    }
    Entity* addEntity();
    void markForDeletion(EntityIndex entityId);
    EntityIndex FindEntityIndex(EntityId entityId) const;

    template<typename ... T>
    auto QueryComponents() {
        std::unordered_map<EntityId, std::tuple<T*...>> query;

        for (std::size_t i = 0; i < _entityCount; i++) {
            query[_entities[i].m_id] = m_componentRegistry.Query<T...>(_entities[i].m_id);
        }

        return query;
    }
};

#endif  // ENTITYMANAGER_H