// BVH Node header class
#ifndef BVH_NODE_H
#define BVH_NODE_H

#include <algorithm>

#include "AABB.h"
#include "Hitable.h"

class BVH_node : public Hitable
{
public:
    Hitable *left;
    Hitable *right;
    AABB box;

    BVH_node() {}
    BVH_node(Hitable **list, int list_size, float time0, float time1);

    BVH_node(Hitable **objects, int start, int end, float time0, float time1){
        int axis = int(3 * drand48());
        auto comparator = (axis == 0) ? box_x_compare
            : (axis == 1) ? box_y_compare
            : box_z_compare;
        int object_span = end - start;

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
            std::sort(objects + start, objects + end, comparator);
            int mid = start + object_span / 2;
            left = new BVH_node(objects, start, mid, time0, time1);
            right = new BVH_node(objects, mid, end, time0, time1);
        }

        AABB box_left, box_right;

        if (!left->bounding_box(time0, time1, box_left) || !right->bounding_box(time0, time1, box_right))
        {
            std::cerr << "No bounding box in BVH_node constructor.\n";
        }

        box = surrounding_box(box_left, box_right);
    }

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const;

    virtual bool bounding_box(float tm0, float tm1, AABB &box) const;
};

bool BVH_node::bounding_box(float tm0, float tm1, AABB &box) const
{
    box = this->box;
    return true;
};

bool BVH_node::intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const
{
    if (box.intersect(ray, t_min, t_max))
    {
        Hit_record left_hit_record, right_hit_record;
        bool hit_left = left->intersect(ray, t_min, t_max, left_hit_record);
        bool hit_right = right->intersect(ray, t_min, t_max, right_hit_record);

        if (hit_left && hit_right)
        {
            if (left_hit_record.t < right_hit_record.t)
            {
                hit_record = left_hit_record;
            }
            else
            {
                hit_record = right_hit_record;
            }
            return true;
        }
        else if (hit_left)
        {
            hit_record = left_hit_record;
            return true;
        }
        else if (hit_right)
        {
            hit_record = right_hit_record;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
};

bool box_compare(const Hitable *a, const Hitable *b, int axis)
{
    AABB box_a, box_b;

    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    {
        std::cerr << "No bounding box in BVH_node constructor.\n";
    }

    return box_a.min()[axis] < box_b.min()[axis];
};

bool box_x_compare(const Hitable *a, const Hitable *b)
{
    AABB box_left, box_right;

    if (!a->bounding_box(0, 0, box_left) || !b->bounding_box(0, 0, box_right))
    {
        std::cerr << "No bounding box in BVH_node constructor.\n";
    }

    return box_left.min().x() < box_right.min().x();
};

bool box_y_compare(const Hitable *a, const Hitable *b)
{
    AABB box_left, box_right;

    if (!a->bounding_box(0, 0, box_left) || !b->bounding_box(0, 0, box_right))
    {
        std::cerr << "No bounding box in BVH_node constructor.\n";
    }

    return box_left.min().y() < box_right.min().y();
};

bool box_z_compare(const Hitable *a, const Hitable *b)
{
    AABB box_left, box_right;

    if (!a->bounding_box(0, 0, box_left) || !b->bounding_box(0, 0, box_right))
    {
        std::cerr << "No bounding box in BVH_node constructor.\n";
    }

    return box_left.min().z() < box_right.min().z();
};

#endif