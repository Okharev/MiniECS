#include <fstream>
#include <iostream>

#include <imgui-SFML.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <SFML/Graphics.hpp>
#include <optional>

#include "EntityManager.h"
#include "Game.h"
#include "Vec2.h"

constexpr uint8_t FONT_SIZE = 14;
constexpr float SCREEN_HEIGHT = 1080.0f;
constexpr float SCREEN_WIDTH = 1920.0f;

int main() {
    auto game = Game();

    game.init();
    game.run();

    return EXIT_SUCCESS;
}