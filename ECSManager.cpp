//
// Created by Okharev on 27/07/2024.
//

#include "ECSManager.h"
#include "Entity.h"

void ECSManager::deleteEntity(EntityId entityId)
{
    _entities.erase(entityId);
}

Entity& ECSManager::addEntity()
{
    for (const auto entityToDelete : _entitiesToDelete) {
        deleteEntity(entityToDelete);
    }

    _entitiesToDelete.clear();

    auto& entity = _entities[_entityCreated];
    entity.m_id = _entityCreated++;

    return entity;
}

void ECSManager::markForDeletion(EntityId entityId) {
    _entitiesToDelete.push_back(entityId);
}