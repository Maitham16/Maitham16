// header class for Ray
#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "Vector3.h"

class Ray {
public:
    Vector3 origin, direction;

    Ray(const Vector3& origin, const Vector3& direction)
        : origin(origin), direction(direction.normalized()) {}

    Vector3 point_at(float t) const { return origin + direction * t; }
};

#endif