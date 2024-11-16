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

void sSpawnPlayer(ECSQuery<CPlayer>& ecsQuery) {
    printf("Spawning player\n");
}

void sMovePlayer(ECSQuery<CPlayer>& ecsQuery) {
    printf("Moving player\n");
}

void sMoveEnemy(ECSQuery<CEnemy> &ecsQuery) {
    printf("Moving enemy\n");
}

void sCleanResources(ECSQuery<CPlayer> &ecsQuery) {
    printf("saving game\n");
}

int main() {

    auto game = Game::New()
                    .Register<GameState::Setup>(sSpawnPlayer, 0)
                    .Register<GameState::Closing>(sCleanResources, 0)
                    .RegisterRange<GameState::Running>(std::make_pair(sMovePlayer, 1), std::make_pair(sMoveEnemy, 0))
                    .Register<GameState::Setup>([&](ECSQuery<CPlayer>& ecsQuery) {
                        printf("lol\n");
                    }, 1);
    game.Run();

    return EXIT_SUCCESS;
}