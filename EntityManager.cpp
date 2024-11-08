//
// Created by Okharev on 27/07/2024.
//

#include "EntityManager.h"

void EntityManager::deleteEntity(EntityIndex entityIndex)
{
    decltype(_entities) tmpEntities = {};

    for (EntityIndex i = 0; i < entityIndex; ++i) {
        tmpEntities[i] = _entities[i];
    }

    for (EntityIndex i = entityIndex + 1; i < MAX_ENTITIES; ++i) {
        tmpEntities[i - 1] = _entities[i];
    }

    _entityCount--;

    for (EntityIndex i = 0; i < MAX_ENTITIES; i++) {
        _entities[i] = tmpEntities[i];
    }

    // compnents à faire ...
}

Entity* EntityManager::addEntity()
{
    for (const auto entityToDelete:_entitiesToDelete) {
        auto entityIndex = FindEntityIndex(entityToDelete);
        if ( entityIndex == INVALID_ENTITY_INDEX) {
            continue;
        }
        deleteEntity(entityIndex);
    }

    auto entity = &_entities[_entityCount++];
    entity->m_id = _entityCreated++;
    return entity;
}

void EntityManager::markForDeletion(const EntityIndex entityId) {
    _entitiesToDelete.push_back(entityId);
}

EntityIndex EntityManager::FindEntityIndex(EntityId entityId) {
    for (EntityIndex i = 0; i < _entityCount;i++) {
        if (_entities[i].m_id == entityId) {
            return i;
        }
    }

    return INVALID_ENTITY_INDEX;
}