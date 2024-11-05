//
// Created by Okharev on 27/07/2024.
//

#ifndef VEC2_H
#define VEC2_H
#include <cmath>
#include <complex>

struct Vec2 {
    float x, y;

    bool operator==(const Vec2 comp) const { return comp.x == x && comp.y == y; }

    bool operator!=(const Vec2 comp) const { return comp.x != x || comp.y != y; }

    Vec2 operator+(const Vec2 comp) const { return Vec2(comp.x + x, comp.y + y); }

    Vec2& add(const Vec2 comp) {
        x += comp.x;
        y += comp.y;
        return *this;
    }

    Vec2 operator-(const Vec2 comp) const { return Vec2(x - comp.x, y - comp.y); }

    Vec2& substract(const Vec2 comp) {
        x -= comp.x;
        y -= comp.y;
        return *this;
    }

    Vec2 operator*(const float factor) const { return Vec2(x * factor, y * factor); }

    Vec2& scale(const float factor) {
        x *= factor;
        y *= factor;
        return *this;
    }

    [[nodiscard]] float length() const { return std::sqrt(x * x + y * y); }

    Vec2& normalize() {
        if (const float length = this->length(); length != 0) {
            this->x /= length;
            this->y /= length;
            return *this;
        }
        this->x = 0;
        this->y = 0;
        return *this;
    }
};

#endif  // VEC2_H