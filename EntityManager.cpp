//
// Created by Okharev on 27/07/2024.
//

#include "EntityManager.h"

void EntityManager::update() {
    for (const auto& e : m_toAdd) {
        m_entities.push_back(e);
        m_entitiesMap[e->getFlags()].push_back(e);
    }
    m_toAdd.clear();

    std::erase_if(m_entities, [](const std::shared_ptr<Entity>& e) { return !e->isActive(); });

    for (auto& [key, value] : m_entitiesMap) {
        std::erase_if(value, [](const std::shared_ptr<Entity>& e) { return !e->isActive(); });
    }
}

std::shared_ptr<Entity> EntityManager::addEntity(const TagFlags tag) {
    const auto entity = std::make_shared<Entity>(tag, m_entitiescount++);
    m_toAdd.push_back(entity);
    return entity;
}

EntityVec& EntityManager::getEntities() {
    return m_entities;
}

EntityVec& EntityManager::getEntities(const TagFlags flags) {
    return m_entitiesMap[flags];
}