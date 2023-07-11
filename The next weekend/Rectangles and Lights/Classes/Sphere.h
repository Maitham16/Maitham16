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

    static void get_sphere_uv(const Vec3 &p, double &u, double &v)
    {
        float phi = atan2(p.z(), p.x()); // p is a point on the sphere, centered at the origin, has coordinates (x, y, z) = (rsinθcosφ, rsinθsinφ, rcosθ)
        float theta = asin(p.y());
        u = 1 - (phi + M_PI) / (2 * M_PI); // value of u is between 0 and 1 of an angle around Y axis from x = -1
        v = (theta + M_PI / 2) / M_PI; // value of v is between 0 and 1 of an angle from Y = -1 to Y = 1
    }
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
            get_sphere_uv(hit_record.normal, hit_record.u, hit_record.v);
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