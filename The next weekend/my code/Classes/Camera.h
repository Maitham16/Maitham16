// camera header class
#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"

class Camera
{
public:
    Vec3 position;
    Vec3 left;
    Vec3 up;

    Vec3 look_at;
    Vec3 look_from;

    Vec3 horizontal;
    Vec3 vertical;

    float aspect;
    float fov;

    float focus;
    float aperture;
    float lens_radius;

    Vec3 u, v, w;

    double time0, time1;

    // constructor
    Camera(Vec3 look_from, Vec3 look_at, Vec3 up, float fov, float aspect, float aperture, float focus, double time0 = 0, double time1 = 0)
    {
        float theta = fov * M_PI / 180.0;
        float height = 2.0 * tan(theta / 2.0);
        float width = aspect * height;
        position = look_from;
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(up, w));
        v = cross(w, u);
        horizontal = width * focus * u;
        vertical = height * focus * v;
        left = position - horizontal / 2.0 - vertical / 2.0 - focus * w;
        lens_radius = aperture / 2.0;
        time0 = time0;
        time1 = time1;
    }

    // rf. youtube channel Avery Makes Games
    Ray get_ray(float s, float t)
    {
        Vec3 rd = lens_radius * random_in_unit_disk();
        Vec3 offset = u * rd.x() + v * rd.y();
        return Ray(position + offset, left + s * horizontal + t * vertical - position - offset, random_double(time0, time1));
    }
};

#endif