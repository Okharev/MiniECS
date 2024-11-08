//
// Created by Okharev on 27/07/2024.
//

#ifndef ENTITY_H
#define ENTITY_H
#include <cstdint>
#include <memory>
#include <optional>

#include "Components.h"


using EntityIndex = std::size_t;
using EntityId = std::size_t;

class Entity final {
public:
    size_t m_id;

    std::optional<CTransform> cTransform = std::nullopt;
    std::optional<CShape> cShape = std::nullopt;
    std::optional<CCollision> cCollison = std::nullopt;
    std::optional<CInput> cInput = std::nullopt;
    std::optional<CScore> cScore = std::nullopt;
    std::optional<CLifespan> cLifespan = std::nullopt;
    std::optional<CFireCooldown> cFireCooldown = std::nullopt;

    Entity(const size_t id) : m_id(id) {};

    ~Entity() = default;

    void destroy() { m_active = false; }
    [[nodiscard]] TagFlags getFlags() const { return m_flags; }
    [[nodiscard]] size_t& getId() { return m_id; }
    [[nodiscard]] bool isActive() const { return m_active; }
};

#endif  // ENTITY_H