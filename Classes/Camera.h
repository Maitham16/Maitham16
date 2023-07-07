// camera header class
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class Camera
{
public:
    Camera()
    {
        left = this->left;
        horizontal = this->horizontal;
        vertical = this->vertical;
        position = this->position;
    }

    Ray get_ray(float u, float v)
    {
        return Ray(position, left + horizontal * u + vertical * v);
    }

    Vec3 position;
    Vec3 left;
    Vec3 horizontal;
    Vec3 vertical;
};

#endif