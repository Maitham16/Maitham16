// hitable list header class
#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "Hitable.h"

class Hitable_list : public Hitable
{
public:
    Hitable **list;
    int list_size;

    Hitable_list() {}
    Hitable_list(Hitable **list, int list_size) : list(list), list_size(list_size) {}

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const;
};

bool Hitable_list::intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const
{
    Hit_record temp_hit_record;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (int i = 0; i < list_size; i++)
    {
        if (list[i]->intersect(ray, t_min, closest_so_far, temp_hit_record))
        {
            hit_anything = true;
            closest_so_far = temp_hit_record.t;
            hit_record = temp_hit_record;
        }
    }

    return hit_anything;
};

#endif