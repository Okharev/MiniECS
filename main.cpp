#include <fstream>
#include <iostream>

#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <SFML/Graphics.hpp>
#include <optional>
#include <functional>

#include "ECSManager.h"
#include "Game.h"
#include "Vec2.h"

constexpr uint8_t FONT_SIZE = 14;
constexpr float SCREEN_HEIGHT = 1080.0f;
constexpr float SCREEN_WIDTH = 1920.0f;

/*
template<typename ... T>
class Query {
    std::unordered_map<EntityId, std::tuple<T*...>> query;

    Query(ECSManager& ecs) {
        query = ecs.QueryComponents<T...>();
    }
    auto unwrap() {
        return query;
    }
};

auto player_controller(Query<CTransform, CScore> query) {

}

class Systems {


public:
    template<typename ...T>
    std::vector<std::function(Query<T...>)> functions;

    Systems() {

    }

};
*/


int main() {
    auto ecs = ECSManager { };

    auto playerOne = ecs.addEntity();
    auto playerTwo = ecs.addEntity();

    ecs.m_componentRegistry.Register<CScore>(playerOne->m_id, { 69 });
    ecs.m_componentRegistry.Register<CScore>(playerTwo->m_id, { 42 });

    auto testOne = ecs.m_componentRegistry.Get<CScore>(playerOne->m_id);
    auto testTwo = ecs.m_componentRegistry.Get<CScore>(playerOne->m_id);

    auto query = ecs.QueryComponents<CScore, CTransform>();

    // for (auto [entityId, components]: query) {
    //     auto score = std::get<0>(components);
    //     auto transform = std::get<1>(components);
    //     printf("%i %f\n", score->value, transform->direction.x);
    // }

    // auto game = Game();

    // game.init();
    // game.run();

    return EXIT_SUCCESS;
}