// header class vector2
#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
#include <iostream>

class Vector2 {
public:
    float x, y;

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2 operator-(const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }

    Vector2 operator*(float f) const {
        return Vector2(x * f, y * f);
    }

    Vector2 operator/(float f) const {
        return Vector2(x / f, y / f);
    }

    float dot(const Vector2& v) const {
        return x * v.x + y * v.y;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalize() const {
        float inv_length = 1 / length();
        return Vector2(x * inv_length, y * inv_length);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};

#endif