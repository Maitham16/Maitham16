// hitable abstract class
#ifndef HITABLEH
#define HITABLEH

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
};

class Hitable
{
public:
    std::vector<Hitable *> objects;
    Vec3 center;

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const = 0;
    virtual bool bounding_box(float tm0, float tm1, AABB &box) const = 0;

    // add a Hitable object to the list
    void addObject(Hitable *obj)
    {
        objects.push_back(obj);
    }

    // iterate over the list of Hitable objects
    const std::vector<Hitable *> &getObjects() const
    {
        return objects;
    }

    // is light source
    bool is_light_source() const
    {
        // check whether the object is a light source
        if (objects.size() > 0)
        {
            return objects[0]->is_light_source();
        }
        else
        {
            return false;
        }
    }
};

#endif