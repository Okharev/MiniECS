//
// Created by Okharev on 27/07/2024.
//

#ifndef GAME_H
#define GAME_H
#include "EntityManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

struct SystemsConfigs final {
    bool m_movementDisabled = false;
    bool m_renderDisabled = false;
    bool m_collisionDisabled = false;
};

class Game final {
    sf::RenderWindow m_window;
    EntityManager m_entityManager;
    sf::Font m_font;
    sf::Text m_text;
    sf::Time m_frameTime;
    sf::Clock m_clock;
    sf::Clock m_enemySpawnClock;
    sf::Clock m_cameraShake;
    int m_score = 0;
    bool m_running = true;

public:
    bool camera_shaking = false;
    SystemsConfigs m_configs;
    float m_spawnInterval = 5.0f;
    float m_cameraShakeInterval = 0.08f;

    Game() : m_entityManager(EntityManager()) {};

    void init();
    void run();

    void sCameraShake();
    void sSetupPlayer();
    void sFireWeapon();
    void sMovement();
    void sUserInput();
    void sRender();
    void sGUI();
    void sEnemyBounce();
    void sPlayerBoundChecking();
    void sEnemySpawner();
    void sSplitEnemy();
    void sTickTimers();
    void sFadeProps();
    void sCollision();
};

#endif  // GAME_H