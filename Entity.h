//
// Created by Okharev on 27/07/2024.
//

#ifndef ENTITY_H
#define ENTITY_H
#include <cstdint>
#include <memory>
#include <optional>

using EntityIndex = std::size_t;
using EntityId = std::size_t;

class Entity final {
public:
    size_t m_id;

    Entity(const size_t id) : m_id(id) {};
    Entity() = default;
};

#endif  // ENTITY_H