//
// Created by Okharev on 27/07/2024.
//

#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <variant>

#include "SFML/Graphics/CircleShape.hpp"
#include "Vec2.h"

#include "Entity.h"

#include <tuple>
#include <unordered_map>

struct CEnemy {};
struct CPlayer {};
struct CBullet {};
struct CProp {};

struct CTransform final {
    Vec2 position{0.0, 0.0};
    Vec2 direction{0.0, 0.0};
    float speed, angle{};

    CTransform(const Vec2 position, const Vec2 direction, const float speed, const float angle) : position(position),
                                                                                                  direction(direction), speed(speed), angle(angle) {};
};

struct CShape final {
    sf::CircleShape circle;

    CShape(const float radius, const int points, const sf::Color& fill, const float outlineThickness, const sf::Color& outline) : circle(radius, points) {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(outlineThickness);
        circle.setOrigin(radius, radius);
    }
};

struct CCollision final {
    float radius;

    explicit CCollision(const float radius) : radius(radius) {};
};

struct CInput final {
    bool left{false};
    bool right{false};
    bool up{false};
    bool down{false};

    CInput() = default;
};

struct CScore final {
    unsigned int value;

    explicit CScore(const int value) : value(value) {};
};

struct CFireCooldown final {
    float remaining{};
    float totalLifespan{};

    explicit CFireCooldown(const float cooldown) : remaining(cooldown), totalLifespan(cooldown) {};
};

struct CLifespan final {
    float remaining{};
    float totalLifespan{};

    explicit CLifespan(const float lifeSpan) : remaining(lifeSpan), totalLifespan(lifeSpan) {};
};

using AnyComponents = std::variant<
    std::unordered_map<EntityId, CBullet>,
    std::unordered_map<EntityId, CCollision>,
    std::unordered_map<EntityId, CEnemy>,
    std::unordered_map<EntityId, CInput>,
    std::unordered_map<EntityId, CLifespan>,
    std::unordered_map<EntityId, CProp>,
    std::unordered_map<EntityId, CScore>,
    std::unordered_map<EntityId, CTransform>,
    std::unordered_map<EntityId, CShape>,
    std::unordered_map<EntityId, CFireCooldown>,
    std::unordered_map<EntityId, CPlayer>
>;

#endif  // COMPONENTS_H