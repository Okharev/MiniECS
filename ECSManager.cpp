//
// Created by Okharev on 27/07/2024.
//

#include "ECSManager.h"

void ECSManager::deleteEntity(EntityIndex entityIndex)
{
    decltype(_entities) tmpEntities = {};

    // components à faire ...
    m_componentRegistry.Unregister(_entities[entityIndex].m_id);

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
}

Entity* ECSManager::addEntity()
{
    for (const auto entityToDelete : _entitiesToDelete) {
        const auto entityIndex = FindEntityIndex(entityToDelete);
        if (entityIndex == INVALID_ENTITY_INDEX) {
            continue;
        }
        deleteEntity(entityIndex);
    }

    const auto entity = &_entities[_entityCount++];
    entity->m_id = _entityCreated++;
    return entity;
}

void ECSManager::markForDeletion(EntityIndex entityId) {
    _entitiesToDelete.push_back(entityId);
}

EntityIndex ECSManager::FindEntityIndex(EntityId entityId) const {
    for (EntityIndex i = 0; i < _entityCount;i++) {
        if (_entities[i].m_id == entityId) {
            return i;
        }
    }

    return INVALID_ENTITY_INDEX;
}