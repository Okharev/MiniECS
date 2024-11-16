#include <fstream>
#include <iostream>

// #include <imgui-SFML.h>
// #include <imgui.h>
// #include <imgui_internal.h>
#include <SFML/Graphics.hpp>
#include <functional>
#include <optional>

#include "Components.h"
#include "ECSManager.h"
#include "Entity.h"
#include "Game.h"
#include "Vec2.h"

constexpr uint8_t FONT_SIZE = 14;
constexpr float SCREEN_HEIGHT = 1080.0f;
constexpr float SCREEN_WIDTH = 1920.0f;

void sSpawnPlayer(ECSQuery<CPlayer>& ecsQuery, int argument1, int argument2) {
    printf("Spawning player %i %i\n", argument1, argument2);
}

void sMovePlayer(ECSQuery<CPlayer>& ecsQuery, int argument1, int argument2) {
    printf("Moving player %i %i\n", argument1, argument2);
}

void sMoveEnemy(ECSQuery<CEnemy> &ecsQuery, int argument1, int argument2) {
    printf("Moving enemy %i %i\n", argument1, argument2);
}

void sCleanResources(ECSQuery<CPlayer> &ecsQuery, int argument1, int argument2) {
    printf("saving game %i %i\n", argument1, argument2);
}

int main() {

    auto game = Game::New()
                    .Register<GameState::Setup>(sSpawnPlayer, 0, 1337, 1338)
                    .Register<GameState::Closing>(sCleanResources, 0, 1337, 1338)
                    .RegisterRange<GameState::Running>(std::make_tuple(sMovePlayer, 1, 1337, 1338), std::make_tuple(sMoveEnemy, 0, 1337, 1338))
                    .Register<GameState::Setup>([&](ECSQuery<CPlayer>& ecsQuery) {
                        printf("lol\n");
                    }, 1);
    game.Run();

    return EXIT_SUCCESS;
}