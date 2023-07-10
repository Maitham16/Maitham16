// AABB header class
#ifndef AABB_H
#define AABB_H

#include "Vec3.h"
#include "Ray.h"

class AABB
{
public:
    Vec3 minH;
    Vec3 maxH;

    AABB() {}
    AABB(const Vec3 &a, const Vec3 &b)
    {
        minH = a;
        maxH = b;
    }

    Vec3 min() const { return minH; }
    Vec3 max() const { return maxH; }

    bool intersect(const Ray &ray, float tmin, float tmax) const
    {
        for (int x = 0; x < 3; x++)
        {
            float t0 = fmin((minH[x] - ray.origin()[x]) / ray.direction()[x],
                            (maxH[x] - ray.origin()[x]) / ray.direction()[x]);
            float t1 = fmax((minH[x] - ray.origin()[x]) / ray.direction()[x],
                            (maxH[x] - ray.origin()[x]) / ray.direction()[x]);
            tmin = fmax(t0, tmin);
            tmax = fmin(t1, tmax);
            if (tmax <= tmin)
            {
                return false;
            }
        }
        return true;
    }
};

static AABB surrounding_box(AABB box0, AABB box1)
{
    Vec3 small(fmin(box0.min().x(), box1.min().x()),
               fmin(box0.min().y(), box1.min().y()),
               fmin(box0.min().z(), box1.min().z()));
    Vec3 big(fmax(box0.max().x(), box1.max().x()),
             fmax(box0.max().y(), box1.max().y()),
             fmax(box0.max().z(), box1.max().z()));
    return AABB(small, big);
}

#endif