// header class Light
#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include "Vector3.h"

enum class LightType {
    AMBIENT,
    POINT
};

class Light {
public:
    Vector3 position;
    Vector3 intensity;
    LightType type;

    Light(const Vector3& position, const Vector3& intensity, LightType type)
        : position(position), intensity(intensity), type(type) {}
};

#endif
