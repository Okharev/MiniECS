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
#include "Game.h"
#include "Vec2.h"

constexpr uint8_t FONT_SIZE = 14;
constexpr float SCREEN_HEIGHT = 1080.0f;
constexpr float SCREEN_WIDTH = 1920.0f;

void sSpawnPlayer(ECSManager& query) {
    printf("Spawning player");
}

void sMovePlayer(ECSManager& query) {
    printf("Spawning player");
}

void sMoveEnemy(ECSManager& query) {
    printf("Spawning player");
}

void sCleanResources(ECSManager& query) {
    printf("saving game");
}

int main() {

    auto game = Game::New()
        .Register<GameState::Setup>(sSpawnPlayer, 0)
        .RegisterRange<GameState::Running>((PrioritizedFunc []){ std::make_pair(sMoveEnemy, 0), std::make_pair(sMovePlayer, 0) });

    return EXIT_SUCCESS;
}