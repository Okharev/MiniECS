//
// Created by Okharev on 27/07/2024.
//

#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::unordered_map<TagFlags, std::vector<std::shared_ptr<Entity>>> EntityMapVec;

class EntityManager final {
    std::vector<std::shared_ptr<Entity>> m_entities;
    std::unordered_map<TagFlags, EntityVec> m_entitiesMap;
    EntityVec m_toAdd;
    size_t m_entitiescount = 0;

public:
    void setCapacity() {
        m_entities.reserve(2500);
        for (auto& [tag, entities] : m_entitiesMap) {
            entities.reserve(900);
        }
    }

    void update();

    std::shared_ptr<Entity> addEntity(TagFlags tag);

    EntityVec& getEntities();

    EntityVec& getEntities(TagFlags flags);
};

#endif  // ENTITYMANAGER_H