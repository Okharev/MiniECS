//
// Created by Okharev on 31/07/2024.
//

#ifndef HELPERS_H
#define HELPERS_H
#include <array>
#include <numbers>
#include <utility>

inline float degToRad(const float degrees) {
    return degrees * (std::numbers::pi / 180.0f);
};

#endif  // HELPERS_H