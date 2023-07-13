// BVH Node header class
#ifndef BVHNODE_H
#define BVHNODE_H

#include <algorithm>

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

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &rec) const override;

    virtual bool bounding_box(float time0, float time1, AABB &output_box) const override;

public:
    Hittable *left;
    Hittable *right;
    AABB box;
};

    bool BVH_node::intersect(const Ray &ray, float t_min, float t_max, Hit_record &rec) const
    {
        if (!box.intersect(ray, t_min, t_max))
            return false;

        bool hit_left = left->intersect(ray, t_min, t_max, rec);
        bool hit_right = right->intersect(ray, t_min, hit_left ? rec.t : t_max, rec);

        return hit_left || hit_right;
    }

    bool BVH_node::bounding_box(float time0, float time1, AABB &output_box) const
    {
        output_box = box;
        return true;
    }

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
    int axis = random_int(0, 2);
    auto comparator = (axis == 0)   ? box_x_compare
                      : (axis == 1) ? box_y_compare
                                    : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1)
    {
        left = right = src_objects[start];
    }
    else if (object_span == 2)
    {
        if (comparator(src_objects[start], src_objects[start + 1]))
        {
            left = src_objects[start];
            right = src_objects[start + 1];
        }
        else
        {
            left = src_objects[start + 1];
            right = src_objects[start];
        }
    }
    else
    {
        std::vector<Hittable *> objects = src_objects;                         // copy the entire src_objects
        std::sort(objects.begin() + start, objects.begin() + end, comparator); // only sort the relevant part

        auto mid = start + object_span / 2;
        left = new BVH_node(objects, start, mid, time0, time1);
        right = new BVH_node(objects, mid, end, time0, time1);
    }

    AABB box_left, box_right;

    if (!left->bounding_box(time0, time1, box_left) ||
        !right->bounding_box(time0, time1, box_right))
        std::cerr << "No bounding box in BVH_node constructor.\n";

    box = surrounding_box(box_left, box_right);
}

#endif