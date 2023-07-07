// camera header class
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class Camera
{
public:
    Vec3 position;
    Vec3 left;
    Vec3 horizontal;
    Vec3 vertical;

    Camera()
    {
        position = this->position;
        left = this->left;
        horizontal = this->horizontal;
        vertical = this->vertical;
    }

    Ray get_ray(float u, float v)
    {
        return Ray(position, left + horizontal * u + vertical * v);
    }
};

#endif