// hitable abstract class
#ifndef HITABLEH
#define HITABLEH

#include "Ray.h"

class Material; // forward declaration

struct Hit_record
{
    float t;
    Vec3 p;
    Vec3 normal;
    Material *material;
};

class Hitable
{
public:
    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const = 0;
};

#endif