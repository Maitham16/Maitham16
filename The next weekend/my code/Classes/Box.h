// box header class
#ifndef BOXH
#define BOXH

#include "Hittable.h"
#include "Hittable_list.h"
#include "aarect.h"

class box : public Hittable
{
public:
    Vec3 box_min, box_max;
    Hittable_list *sides;

    box() {}
    box(const Vec3 &p0, const Vec3 &p1, Material *material);

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const;

    virtual bool bounding_box(float t0, float t1, AABB &opbox) const
    {
        opbox = AABB(box_min, box_max);
        return true;
    }
};

box::box(const Vec3 &p0, const Vec3 &p1, Material *material)
{
    box_min = p0;
    box_max = p1;

    std::vector<Hittable *> list;

    list.push_back(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material));
    list.push_back(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material));

    list.push_back(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material));
    list.push_back(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material));

    list.push_back(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material));
    list.push_back(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material));

    sides = new Hittable_list(list);
}

bool box::intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const
{
    return sides->intersect(ray, t_min, t_max, hit_record);
}

#endif