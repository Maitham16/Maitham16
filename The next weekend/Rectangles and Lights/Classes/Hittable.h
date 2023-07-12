// Hittable abstract class
#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "AABB.h"
#include <vector>

class Material; // forward declaration

struct Hit_record
{
    float t;
    Vec3 p;
    Vec3 normal;
    Material *material;
    double u;
    double v;
    bool front_face;
    // set_face_normal function
    inline void set_face_normal(const Ray &r, const Vec3 &outward_normal)
    {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const = 0;

    virtual bool bounding_box(float tm0, float tm1, AABB &box) const = 0;
};

#endif