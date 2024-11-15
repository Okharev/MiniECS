//
// Created by Okharev on 27/07/2024.
//

#include "Game.h"

#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>
#include <ranges>

#include "Helpers.h"
#include "SFML/Window/Event.hpp"

void Game::init() {
    m_window.create(sf::VideoMode(2440, 1560), "UltraAdvancedGeometryWarz");

    if (!ImGui::SFML::Init(m_window)) {
        std::cerr << "Failed to load imgui" << std::endl;
    }
}

// void Game::run() {
    sSetupPlayer();

    m_font.loadFromFile(R"(C:\Users\Okharev\CLionProjects\ECSEngine\font\Ubuntu-Medium.ttf)");
    m_text.setFont(m_font);
    m_text.setCharacterSize(24);
    m_text.setString(std::to_string(m_score));
    m_text.setPosition(10.0f, 0.0f);

    while (m_running) {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(m_window, event);

            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        m_frameTime = m_clock.restart();

        ImGui::SFML::Update(m_window, m_frameTime);

        sTickTimers();
        sEnemySpawner();

        sUserInput();
        sFireWeapon();

        sCameraShake();
        if (!m_configs.m_movementDisabled) {
            sMovement();
        }
        if (!m_configs.m_collisionDisabled) {
            sCollision();
        }
        sPlayerBoundChecking();
        sEnemyBounce();
        sSplitEnemy();
        sFadeProps();
        sGUI();

        if (!m_configs.m_renderDisabled) {
            sRender();
        }

        ImGui::SFML::Render(m_window);
        m_window.display();
    }
}

void Game::sCameraShake() {
    std::cout << camera_shaking << " " << m_cameraShake.getElapsedTime().asSeconds() << " " << m_cameraShakeInterval << std::endl;

    if (!camera_shaking) {
        m_cameraShake.restart();
        return;
    }

    if (m_cameraShake.getElapsedTime().asSeconds() >= m_cameraShakeInterval) {
        // camera_shaking = false;
        m_cameraShake.restart();
        camera_shaking = false;
        m_window.setView(m_window.getDefaultView());
        return;
    }

    auto view = m_window.getView();

    const int randomX = rand() % (1 - -1 + 1) + -1;
    const int randomY = rand() % (1 - -1 + 1) + -1;

    view.setCenter(view.getCenter() + sf::Vector2f(randomX, randomY));
    m_window.setView(view);
}

void Game::sSetupPlayer() {
    const auto player = m_entityManager.addEntity(Player);
    player->cShape.emplace(CShape(20.0f, 8, sf::Color(0, 0, 0, 0), 2.0f, sf::Color::Blue));

    const auto centerScreen = m_window.getSize();

    player->cTransform.emplace(CTransform(
        Vec2{static_cast<float>(centerScreen.x / 2), static_cast<float>(centerScreen.y) / 2},
        Vec2{0, 0},
        385.0f,
        100.0f
        ));
    player->cCollison.emplace(CCollision(player->cShape->circle.getRadius()));
    player->cFireCooldown.emplace(CFireCooldown(0.2f));
    player->cInput.emplace(CInput());
}

void Game::sFireWeapon() {
    const auto player = m_entityManager.getEntities(Player).front();

    if (player == nullptr || !player->cFireCooldown.has_value() || !player->cTransform.has_value())
        return;
    if (player->cFireCooldown->remaining > 0.0f)
        return;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        const auto mousePos = sf::Mouse::getPosition(m_window);

        auto dir = Vec2(mousePos.x, mousePos.y) - player->cTransform->position;
        dir.normalize();

        player->cFireCooldown->remaining = player->cFireCooldown->totalLifespan;

        const auto bullet = m_entityManager.addEntity(Bullet);

        bullet->cShape.emplace(CShape(10.0f, 25, sf::Color::White, 0.0f, sf::Color::White));
        bullet->cTransform.emplace(CTransform(player->cTransform->position, dir, 1200.0f, 0.0f));
        bullet->cCollison.emplace(CCollision(bullet->cShape->circle.getRadius()));
        bullet->cLifespan.emplace(CLifespan(1.75f));

        camera_shaking = true;
    }
}

void Game::sMovement(EntityId entityId, CMovement& movement) {
    for (const auto& entity : m_entityManager.getEntities()) {
        if (!entity->cTransform.has_value() || !entity->cShape.has_value())
            continue;

        entity->cTransform->position.add(
            entity->cTransform->direction.normalize() * entity->cTransform->speed * m_frameTime.asSeconds()
            );
        entity->cShape->circle.rotate(entity->cTransform->angle * m_frameTime.asSeconds());
        entity->cShape->circle.setPosition(entity->cTransform->position.x, entity->cTransform->position.y);
    }
}

void Game::sUserInput() {
    const auto player = m_entityManager.getEntities(Player).front();

    if (player == nullptr || !player->cTransform.has_value())
        return;

    Vec2 dir{0.0, 0.0};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        dir.y = -1.0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        dir.y = 1.0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        dir.x = 1.0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        dir.x = -1.0;

    player->cTransform->direction = dir.normalize();
}

void Game::sRender() {
    m_window.clear();

    for (const auto& entity : m_entityManager.getEntities()) {
        if (!entity->cShape.has_value())
            continue;

        m_window.draw(entity->cShape->circle);
    }

    m_text.setString(std::string("Score: " + std::to_string(m_score)));
    m_window.draw(m_text);
}

void Game::sGUI() {
    ImGui::Begin("Game");
    ImGui::Checkbox("Movementsytem", &m_configs.m_movementDisabled);
    ImGui::Checkbox("CollisionSystem", &m_configs.m_collisionDisabled);
    ImGui::Checkbox("RenderSystem", &m_configs.m_renderDisabled);
    const auto player = m_entityManager.getEntities(Player).front();
    ImGui::InputFloat("firerate", &player->cFireCooldown->totalLifespan);
    ImGui::SliderFloat("IntervalSpawning", &m_spawnInterval, 0.1f, 100.0f);

    for (const auto& entity : m_entityManager.getEntities()) {
        ImGui::PushID(&entity);
        ImGui::BeginGroup();
        ImGui::InputInt("##id", reinterpret_cast<int*>(&entity->getId()), ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat2("##dir", &entity->cTransform->direction.x);
        ImGui::InputFloat2("##pos", &entity->cTransform->position.x);
        ImGui::EndGroup();
        ImGui::Separator();
        ImGui::PopID();
    }
    ImGui::End();
}

void Game::sEnemyBounce() {
    for (const auto& entity : m_entityManager.getEntities(Enemy)) {
        if (!entity->cTransform.has_value() || !entity->cCollison.has_value())
            continue;

        const auto screenSize = m_window.getSize();

        if (entity->cTransform->position.x + entity->cCollison->radius > screenSize.x || entity->cTransform->position.x - entity->cCollison->radius < 0.0) {
            entity->cTransform->direction.x = -entity->cTransform->direction.x;
        }
        if (entity->cTransform->position.y + entity->cCollison->radius > screenSize.y || entity->cTransform->position.y - entity->cCollison->radius < 0.0) {
            entity->cTransform->direction.y = -entity->cTransform->direction.y;
        }
    }

    for (const auto& entity : m_entityManager.getEntities(Prop)) {
        if (!entity->cTransform.has_value() || !entity->cCollison.has_value())
            continue;

        const auto screenSize = m_window.getSize();

        if (entity->cTransform->position.x + entity->cCollison->radius > screenSize.x || entity->cTransform->position.x - entity->cCollison->radius < 0.0) {
            entity->cTransform->direction.x = -entity->cTransform->direction.x;
        }
        if (entity->cTransform->position.y + entity->cCollison->radius > screenSize.y || entity->cTransform->position.y - entity->cCollison->radius < 0.0) {
            entity->cTransform->direction.y = -entity->cTransform->direction.y;
        }
    }
}

void Game::sPlayerBoundChecking() {
    const auto player = m_entityManager.getEntities(Player).front();
    if (!player->cTransform.has_value() || !player->cCollison.has_value())
        return;

    const auto screenSize = m_window.getSize();

    if (player->cTransform->position.y + player->cCollison->radius > screenSize.y) {
        player->cTransform->position.y = screenSize.y - player->cCollison->radius;
    }
    if (player->cTransform->position.y - player->cCollison->radius < 0.0) {
        player->cTransform->position.y = player->cCollison->radius;
    }
    if (player->cTransform->position.x + player->cCollison->radius > screenSize.x) {
        player->cTransform->position.x = screenSize.x - player->cCollison->radius;
    }
    if (player->cTransform->position.x - player->cCollison->radius < 0.0) {
        player->cTransform->position.x = player->cCollison->radius;
    }
}

void Game::sEnemySpawner() {
    if (m_enemySpawnClock.getElapsedTime().asSeconds() <= m_spawnInterval)
        return;

    const auto screenSize = m_window.getSize();

    const int randomPoints = rand() % (10 - 3 + 1) + 3;
    const int randomSpeed = rand() % (90 - 10 + 1) + 10;
    const int randomAngle = rand() % (-100 - 100 + 1) - 100;
    const int randomDirX = rand() % (1 - -1 + 1) + 1;
    const int randomDirY = rand() % (1 - -1 + 1) + 1;
    const int randomPosX = rand() % (screenSize.x - 80 + 1) + 80;
    const int randomPosY = rand() % (screenSize.y - 80 + 1) + 80;
    const int randomRed = rand() % (255 - 0 + 1) + 0;
    const int randomGreen = rand() % (255 - 0 + 1) + 0;
    const int randomBlue = rand() % (255 - 0 + 1) + 0;

    const auto enemy = m_entityManager.addEntity(Enemy);
    enemy->cShape.emplace(CShape(30, randomPoints, sf::Color(randomRed, randomGreen, randomBlue, 255), 2.0f, sf::Color::White));
    enemy->cTransform.emplace(CTransform(Vec2{static_cast<float>(randomPosX), static_cast<float>(randomPosY)},
                                         Vec2{static_cast<float>(randomDirX), static_cast<float>(randomDirY)},
                                         static_cast<float>(randomSpeed), static_cast<float>(randomAngle)));
    enemy->cCollison.emplace(CCollision(enemy->cShape->circle.getRadius()));
    enemy->cScore.emplace(CScore(randomPoints));

    m_enemySpawnClock.restart();
}

void Game::sSplitEnemy() {
    for (const auto& entity : m_entityManager.getEntities(Enemy)) {
        if (!entity->cShape.has_value() && !entity->cTransform.has_value() && !entity->cScore.has_value())
            continue;

        if (!entity->isActive()) {
            const auto anglePart = 360.0f / entity->cShape->circle.getPointCount();
            for (int pointCount = entity->cShape->circle.getPointCount(); const int i : std::views::iota(0, pointCount)) {
                const auto angle = (i + 1) * anglePart;
                const auto debrisDir = Vec2(cos(degToRad(angle)), sin(degToRad(angle)));
                const auto debris = m_entityManager.addEntity(Prop);

                debris->cShape.emplace(CShape(
                    entity->cShape->circle.getRadius() * 0.5f,
                    entity->cShape->circle.getPointCount(),
                    entity->cShape->circle.getFillColor(),
                    2.0f,
                    sf::Color(100, 100, 100, 255)
                    ));
                debris->cCollison.emplace(CCollision(debris->cShape->circle.getRadius()));
                debris->cScore.emplace(CScore(entity->cScore.value().value * 10));
                debris->cTransform.emplace(CTransform(
                    Vec2{entity->cTransform->position.x,
                         entity->cTransform->position.y},
                    debrisDir,
                    entity->cTransform->speed * 2,
                    entity->cTransform->angle
                    ));
                debris->cLifespan.emplace(CLifespan(1.0f));
            }
        }
    }
}

void Game::sTickTimers() {
    for (const auto& entity : m_entityManager.getEntities()) {
        if (entity->cLifespan.has_value()) {
            entity->cLifespan->remaining -= m_frameTime.asSeconds();
        }
        if (entity->cFireCooldown.has_value()) {
            entity->cFireCooldown->remaining -= m_frameTime.asSeconds();
        }
    }
}

void Game::sFadeProps() {
    for (const auto& entity : m_entityManager.getEntities(Prop)) {
        if (!entity->cLifespan.has_value() && !entity->cShape.has_value())
            continue;

        if (entity->cLifespan->remaining <= 0.0f) {
            entity->destroy();
        }

        const float ratio = entity->cLifespan->remaining / entity->cLifespan->totalLifespan * 255;

        auto color = entity->cShape->circle.getFillColor();
        color.a = static_cast<sf::Uint8>(ratio);
        auto lineColor = entity->cShape->circle.getOutlineColor();
        lineColor.a = static_cast<sf::Uint8>(ratio);

        entity->cShape->circle.setFillColor(color);
        entity->cShape->circle.setOutlineColor(lineColor);
    }
    for (const auto& entity : m_entityManager.getEntities(Bullet)) {
        if (!entity->cLifespan.has_value() && !entity->cShape.has_value())
            continue;

        if (entity->cLifespan->remaining <= 0.0f)
            entity->destroy();

        const float ratio = entity->cLifespan->remaining / entity->cLifespan->totalLifespan * 255;

        auto color = entity->cShape->circle.getFillColor();
        color.a = static_cast<sf::Uint8>(ratio);
        auto lineColor = entity->cShape->circle.getOutlineColor();
        lineColor.a = static_cast<sf::Uint8>(ratio);

        entity->cShape->circle.setFillColor(color);
        entity->cShape->circle.setOutlineColor(lineColor);
    }
}

void Game::sCollision() {
    const auto player = m_entityManager.getEntities(Player).front();
    if (!player->cTransform.has_value() || !player->cCollison.has_value())
        return;

    for (const auto& enemy : m_entityManager.getEntities(Enemy)) {
        if (!enemy->cTransform.has_value() || !enemy->cCollison.has_value())
            continue;

        if (const auto distanceCircles = player->cTransform->position - enemy->cTransform->position;
            distanceCircles.length() < player->cCollison->radius + enemy->cCollison->radius) {

            enemy->destroy();
        }
    }

    for (const auto& bullet : m_entityManager.getEntities(Bullet)) {
        if (!bullet->cTransform.has_value() || !bullet->cCollison.has_value())
            continue;

        for (const auto& enemy : m_entityManager.getEntities(Enemy)) {
            if (!enemy->cTransform.has_value() || !enemy->cCollison.has_value())
                continue;

            if (const auto distanceCircles = bullet->cTransform->position - enemy->cTransform->position;
                distanceCircles.length() < bullet->cCollison->radius + enemy->cCollison->radius) {
                if (enemy->cScore.has_value()) {
                    m_score += enemy->cScore.value().value;
                }
                enemy->destroy();
                bullet->destroy();
            }
        }

        for (const auto& prop : m_entityManager.getEntities(Prop)) {
            if (!prop->cTransform.has_value() || !prop->cCollison.has_value())
                continue;
            if (const auto distanceCircles = bullet->cTransform->position - prop->cTransform->position;
                distanceCircles.length() < bullet->cCollison->radius + prop->cCollison->radius) {
                if (prop->cScore.has_value()) {
                    m_score += prop->cScore.value().value;
                }
                prop->destroy();
                bullet->destroy();
            }
        }
    }

    // for (const auto& enemy : m_entityManager.getEntities(Enemy)) {
    //     if (!enemy->cTransform.has_value() || !enemy->cCollison.has_value())
    //         continue;
    //
    //     for (const auto& bullet : m_entityManager.getEntities(Bullet)) {
    //         if (!bullet->cTransform.has_value() || !bullet->cCollison.has_value())
    //             continue;
    //
    //         if (const auto distanceCircles = bullet->cTransform->position - enemy->cTransform->position;
    //             distanceCircles.length() < bullet->cCollison->radius + enemy->cCollison->radius) {
    //
    //             if (enemy->cScore.has_value()) {
    //                 m_score += enemy->cScore.value().value;
    //             }
    //
    //             enemy->destroy();
    //             bullet->destroy();
    //         }
    //     }
    //
    //     if (const auto distanceCircles = player->cTransform->position - enemy->cTransform->position;
    //         distanceCircles.length() < player->cCollison->radius + enemy->cCollison->radius) {
    //         enemy->destroy();
    //     }
    // }
}