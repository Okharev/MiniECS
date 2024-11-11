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
};

struct CShape final {
    sf::CircleShape circle;
};

struct CCollision final {
    float radius;
};

struct CInput final {
    bool left{false};
    bool right{false};
    bool up{false};
    bool down{false};
};

struct CScore final {
    unsigned int value;
};

struct CFireCooldown final {
    float remaining{};
    float totalLifespan{};
};

struct CLifespan final {
    float remaining{};
    float totalLifespan{};
};

template <typename TComponent>
using Component = std::unordered_map<EntityId, TComponent>;

template<typename... TComponents>
using Components = std::tuple<Component<TComponents>...>;

#endif  // COMPONENTS_H