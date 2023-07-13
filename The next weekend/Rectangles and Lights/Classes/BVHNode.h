// BVH Node header class
#ifndef BVHNODE_H
#define BVHNODE_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <vector>

#include "AABB.h"
#include "Hittable.h"
#include "Hittable_list.h"

class BVH_node : public Hittable
{
public:
    BVH_node();

    BVH_node(const Hittable_list &list, double time0, double time1)
        : BVH_node(list.list, 0, list.list.size(), time0, time1)
    {
    }

    BVH_node(const std::vector<Hittable *> &src_objects, size_t start, size_t end, double time0, double time1);

    bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &rec) const override
    {
        if (!box.intersect(ray, t_min, t_max))
            return false;

        bool hit_left = left->intersect(ray, t_min, t_max, rec);
        bool hit_right = right->intersect(ray, t_min, hit_left ? rec.t : t_max, rec);

        return hit_left || hit_right;
    }

    bool bounding_box(float time0, float time1, AABB &output_box) const override
    {
        output_box = box;
        return true;
    }

public:
    Hittable *left;
    Hittable *right;
    AABB box;
};

inline bool box_compare(const Hittable *a, const Hittable *b, int axis)
{
    AABB box_a;
    AABB box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
        std::cerr << "No bounding box in BVH_node constructor.\n";

    return box_a.min().e[axis] < box_b.min().e[axis];
}

bool box_x_compare(const Hittable *a, const Hittable *b)
{
    return box_compare(a, b, 0);
}

bool box_y_compare(const Hittable *a, const Hittable *b)
{
    return box_compare(a, b, 1);
}

bool box_z_compare(const Hittable *a, const Hittable *b)
{
    return box_compare(a, b, 2);
}

BVH_node::BVH_node(const std::vector<Hittable *> &src_objects, size_t start, size_t end, double time0, double time1)
{
    std::vector<Hittable *> objects(src_objects.begin() + start, src_objects.begin() + end);

    int axis = random_int(0, 2);
    auto comparator = (axis == 0)   ? box_x_compare
                      : (axis == 1) ? box_y_compare
                                    : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1)
    {
        left = right = objects[start];
    }
    else if (object_span == 2)
    {
        if (comparator(objects[start], objects[start + 1]))
        {
            left = objects[start];
            right = objects[start + 1];
        }
        else
        {
            left = objects[start + 1];
            right = objects[start];
        }
    }
    else
    {
        std::sort(objects.begin(), objects.end(), comparator);

        auto mid = start + object_span / 2;
        left = new BVH_node(objects, 0, mid - start, time0, time1);
        right = new BVH_node(objects, mid - start, end - start, time0, time1);
    }

    AABB box_left, box_right;

    if (!left->bounding_box(time0, time1, box_left) ||
        !right->bounding_box(time0, time1, box_right))
        std::cerr << "No bounding box in BVH_node constructor.\n";

    box = surrounding_box(box_left, box_right);
};

#endif