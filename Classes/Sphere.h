// Sphere header class
#ifndef SPHERE_H
#define SPHERE_H

#include "Vec3.h"
#include "Ray.h"

class Sphere
{
public:
    Sphere() {}
    Sphere(Vec3 center, float redius) : center(center), radius(redius) {}

    Vec3 center;
    float radius;

    bool hit_sphere(const Vec3 center, float radius, const Ray &ray) const
    {
        Vec3 oc = ray.origin() - center;
        float a = dot(ray.direction(), ray.direction());
        float b = 2.0 * dot(oc, ray.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;
        return (discriminant > 0);
    }
};

#endif