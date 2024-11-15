//
// Created by Okharev on 27/07/2024.
//

#ifndef GAME_H
#define GAME_H
#include <bits/stdc++.h>
#include "ECSManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

enum GameState {
    Setup,
    Running,
    Closing,
};

using PrioritizedFunc = std::pair<std::function<void(ECSManager&)>, std::uint8_t>;

class Game final {
    ECSManager _ecs;

    std::vector<PrioritizedFunc> _setupSystems;
    std::vector<PrioritizedFunc> _runningSystems;
    std::vector<PrioritizedFunc> _closingSystems;

    Game();

   public:
    static Game New() { return Game{}; }

    template <GameState TGameState>
    Game& Register(std::function<void(ECSManager&)> function, uint8_t priority) {
        if constexpr (TGameState == GameState::Running) {
            _setupSystems.emplace_back(std::make_pair(function, priority));
            std::sort(_setupSystems.begin(), _setupSystems.end(), [](PrioritizedFunc& one, PrioritizedFunc& two) { return one.second > two.second; });
        } else if constexpr (TGameState == GameState::Running) {
            _runningSystems.emplace_back(std::make_pair(function, priority));
            std::sort(_setupSystems.begin(), _setupSystems.end(), [](PrioritizedFunc& one, PrioritizedFunc& two) { return one.second > two.second; });
        } else if constexpr (TGameState == GameState::Closing) {
            _closingSystems.emplace_back(std::make_pair(function, priority));
            std::sort(_setupSystems.begin(), _setupSystems.end(), [](PrioritizedFunc& one, PrioritizedFunc& two) { return one.second > two.second; });
        }

        return *this;
    }

    template <GameState TGameState, std::ranges::forward_range Iterable>
    requires std::same_as<std::ranges::range_value_t<Iterable>, PrioritizedFunc> Game& RegisterRange(Iterable& iterFuncs) {
        if constexpr (TGameState == GameState::Running) {
            _setupSystems.insert(_setupSystems.end(), iterFuncs.begin(), iterFuncs.end());
            std::sort(_setupSystems.begin(), _setupSystems.end(), [](PrioritizedFunc& one, PrioritizedFunc& two) { return one.second > two.second; });
        } else if constexpr (TGameState == GameState::Running) {
            _runningSystems.insert(_runningSystems.end(), iterFuncs.begin(), iterFuncs.end());
            std::sort(_runningSystems.begin(), _runningSystems.end(), [](PrioritizedFunc& one, PrioritizedFunc& two) { return one.second > two.second; });
        } else if constexpr (TGameState == GameState::Closing) {
            _closingSystems.insert(_closingSystems.end(), iterFuncs.begin(), iterFuncs.end());
            std::sort(_closingSystems.begin(), _closingSystems.end(), [](PrioritizedFunc& one, PrioritizedFunc& two) { return one.second > two.second; });
        }

        return *this;
    }

    void Run() {
        for (auto&& func : _setupSystems) {
            func.first(_ecs);
        }

        while (true) {
            for (auto&& func : _setupSystems) {
                func.first(_ecs);
            }
        }

        for (auto&& func : _runningSystems) {
            func.first(_ecs);
        }
    }
};

#endif  // GAME_H