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

#include "Entity.h"

auto constexpr MAX_ENTITIES = 2048;

using EntityArray = std::array<Entity, MAX_ENTITIES>;

class EntityManager final {

    static constexpr auto INVALID_ENTITY_INDEX = std::numeric_limits<EntityId>::max();

    EntityArray _entities;
    EntityIndex _entityCount;
    EntityIndex _entityCreated;
    std::vector<EntityId> _entitiesToDelete;


    void deleteEntity(EntityIndex entityIndex);

public:
    EntityManager() : _entities({}), _entityCount({}), _entityCreated(0), _entitiesToDelete({}) {}
    Entity* addEntity();
    void markForDeletion(const EntityIndex entityId);
    EntityIndex FindEntityIndex(EntityId entityId);
};

#endif  // ENTITYMANAGER_H