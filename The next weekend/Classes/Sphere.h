// Sphere header class
#ifndef SPHERE_H
#define SPHERE_H

#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
#include "Material.h"

class Sphere : public Hitable
{
public:
    Vec3 center;
    float radius;
    Material *material;

    Sphere() {}
    Sphere(Vec3 center, float radius, Material *material) : center(center), radius(radius), material(material) {}

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const;
    virtual bool bounding_box(float tm0, float tm1, AABB &box) const;
};

bool Sphere::intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const
{
    Vec3 oc = ray.origin() - center;
    float a = dot(ray.direction(), ray.direction());
    float b = dot(oc, ray.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - a * c;

    if (discriminant > 0.0)
    {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min)
        {
            hit_record.t = temp;
            hit_record.p = ray.point_at(hit_record.t);
            hit_record.normal = (hit_record.p - center) / radius;
            hit_record.material = material;
            return true;
        }

        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp < t_max && temp > t_min)
        {
            hit_record.t = temp;
            hit_record.p = ray.point_at(hit_record.t);
            hit_record.normal = (hit_record.p - center) / radius;
            hit_record.material = material;
            return true;
        }
    }

    return false;
};

bool Sphere::bounding_box(float tm0, float tm1, AABB &box) const
{
    box = AABB(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
    return true;
};

#endif