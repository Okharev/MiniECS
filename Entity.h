//
// Created by Okharev on 27/07/2024.
//

#ifndef ENTITY_H
#define ENTITY_H
#include <cstdint>
#include <memory>
#include <optional>

#include "Components.h"

enum TagFlags : uint8_t {
    Enemy = 0b00000001,
    Player = 0b00000010,
    Bullet = 0b00000100,
    Prop = 0b00001000,
};

class Entity final {
    TagFlags m_flags;
    bool m_active;
    size_t m_id = 0;

public:
    std::optional<CTransform> cTransform = std::nullopt;
    std::optional<CShape> cShape = std::nullopt;
    std::optional<CCollision> cCollison = std::nullopt;
    std::optional<CInput> cInput = std::nullopt;
    std::optional<CScore> cScore = std::nullopt;
    std::optional<CLifespan> cLifespan = std::nullopt;
    std::optional<CFireCooldown> cFireCooldown = std::nullopt;

    Entity(const TagFlags flags, const size_t id) : m_flags(flags), m_active(true), m_id(id) {};

    ~Entity() = default;

    void destroy() { m_active = false; }
    [[nodiscard]] TagFlags getFlags() const { return m_flags; }
    [[nodiscard]] size_t& getId() { return m_id; }
    [[nodiscard]] bool isActive() const { return m_active; }
};

#endif  // ENTITY_H