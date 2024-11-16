//
// Created by Okharev on 27/07/2024.
//

#ifndef GAME_H
#define GAME_H
#include <bits/stdc++.h>
#include <initializer_list>
#include <type_traits>
#include "ECSManager.h"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"

enum GameState {
    Setup,
    Running,
    Closing,
};

struct SystemFunction {
    std::function<void(ECSManager& ecsManager)> callback;
    int priority;

    auto operator()(ECSManager& ecsManager) const {
        callback(ecsManager);    
    }
};

template <typename... TComponents>
struct SystemFunctionSignature;

// Spécifie les signatures de tes fonctions qui sont possible d'écrire ici
// Genre si tu veux ajouter un entier après ECSQuery
template <typename... TComponents>
struct SystemFunctionSignature<std::function<void(ECSQuery<TComponents...>&)>> {
    using TECSQuery = ECSQuery<TComponents...>;
};

class Game final {
    ECSManager _ecs;

    std::vector<SystemFunction> _setupSystems;
    std::vector<SystemFunction> _runningSystems;
    std::vector<SystemFunction> _closingSystems;

    Game() = default;

   public:
    static Game New() { return Game{}; }

    template <GameState TGameState, typename Function>
    Game& Register(Function&& function, int priority) {
        std::function stdfunction = function;

        using TECSQuery = SystemFunctionSignature<decltype(stdfunction)>::TECSQuery;

        auto callback = [=](ECSManager& ecsManager) {
            TECSQuery ecsQuery;
            ecsManager.QueryComponents(ecsQuery);
            stdfunction(ecsQuery);
        };

        if constexpr (TGameState == GameState::Setup) {
            _setupSystems.push_back({callback, priority});
            std::sort(_setupSystems.begin(), _setupSystems.end(), [](const SystemFunction& systemFunction1, const SystemFunction& systemFunction2) {
                return systemFunction1.priority > systemFunction2.priority;
            });
        } else if constexpr (TGameState == GameState::Running) {
            _runningSystems.push_back({callback, priority});
            std::sort(_runningSystems.begin(), _runningSystems.end(), [](const SystemFunction& systemFunction1, const SystemFunction& systemFunction2) {
                return systemFunction1.priority > systemFunction2.priority;
            });
        } else if constexpr (TGameState == GameState::Closing) {
            _closingSystems.push_back({callback, priority});
            std::sort(_closingSystems.begin(), _closingSystems.end(), [](const SystemFunction& systemFunction1, const SystemFunction& systemFunction2) {
                return systemFunction1.priority > systemFunction2.priority;
            });
        }

        return *this;
    }

    template <GameState TGameState, typename... TFunctions>
    Game& RegisterRange(std::pair<TFunctions, int>&&... functions) {
        (Register<TGameState>(std::get<TFunctions>(functions), std::get<int>(functions)), ...);
        return *this;
    }

    void Run() {
        for (auto&& func : _setupSystems) {
            func(_ecs);
        }

        while (true) {
            for (auto&& func : _runningSystems) {
                func(_ecs);
            }
        }

        for (auto&& func : _closingSystems) {
            func(_ecs);
        }
    }
};

#endif  // GAME_H