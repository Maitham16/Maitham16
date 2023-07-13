// Hittable list header class
#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>

#include "Hittable.h"

class Hittable_list : public Hittable
{
public:
    std::vector<Hittable *> list;

    Hittable_list() {}

    Hittable_list(std::vector<Hittable *> hittable_list)
    {
        for (Hittable *hittable : hittable_list)
            add(hittable);
    }

    Hittable_list(std::vector<Hittable *> hittable_list, int size)
    {
        for (Hittable *hittable : hittable_list)
            add(hittable);
        this->list.resize(size);
    }

    Hittable_list(Hittable **hittable_list, int size)
    {
        for (int i = 0; i < size; i++)
        {
            add(hittable_list[i]);
        }
    }

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const;

    virtual bool bounding_box(float tm0, float tm1, AABB &box) const;

    void add(Hittable *hittable)
    {
        list.push_back(hittable);
    }

    int size() const
    {
        return list.size();
    }

    void clear()
    {
        list.clear();
    }
};

bool Hittable_list::intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const
{
    Hit_record temp_hit_record;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const Hittable *hittable : list)
    {
        if (hittable->intersect(ray, t_min, closest_so_far, temp_hit_record))
        {
            hit_anything = true;
            closest_so_far = temp_hit_record.t;
            hit_record = temp_hit_record;
        }
    }

    return hit_anything;
};

bool Hittable_list::bounding_box(float tm0, float tm1, AABB &box) const
{
    if (list.empty())
    {
        return false;
    }

    AABB temp_box;
    bool first_box = true;

    for (const Hittable *hittable : list)
    {
        if (!hittable->bounding_box(tm0, tm1, temp_box))
            return false;
        box = first_box ? temp_box : surrounding_box(box, temp_box);
        first_box = false;
    }

    return true;
};

#endif