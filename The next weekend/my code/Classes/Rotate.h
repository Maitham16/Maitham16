// rotate header class
#ifndef ROTATE_H
#define ROTATE_H

#include "Hittable.h"
#include "Vec3.h"
#include "AABB.h"
#include "Ray.h"

class Rotate_y : public Hittable
{
public:
    Hittable *pointer;
    float sin_theta;
    float cos_theta;
    bool hasbox;
    AABB bbox;

    Rotate_y(Hittable *p, float angle);

    virtual bool intersect(const Ray &r, float t_min, float t_max, Hit_record &rec) const override;

    virtual bool bounding_box(float t0, float t1, AABB &output_box) const override;
};

Rotate_y::Rotate_y(Hittable *p, float angle) : pointer(p)
{
    float radians = (M_PI / 180.) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = pointer->bounding_box(0, 1, bbox);

    Vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    Vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; ++j)
            for (int k = 0; k < 2; ++k)
            {
                float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
                float y = j * bbox.max().y() + (1 - j) * bbox.min().y();
                float z = k * bbox.max().z() + (1 - k) * bbox.min().z();

                float newx = cos_theta * x + sin_theta * z;
                float newz = -sin_theta * x + cos_theta * z;

                Vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; ++c)
                {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }

    bbox = AABB(min, max);
}

bool Rotate_y::intersect(const Ray &ray, float t_min, float t_max, Hit_record &rec) const
{
    Vec3 origin = ray.origin();
    Vec3 direction = ray.direction();

    origin[0] = cos_theta * ray.origin()[0] - sin_theta * ray.origin()[2];
    origin[2] = sin_theta * ray.origin()[0] + cos_theta * ray.origin()[2];

    direction[0] = cos_theta * ray.direction()[0] - sin_theta * ray.direction()[2];
    direction[2] = sin_theta * ray.direction()[0] + cos_theta * ray.direction()[2];

    Ray rotated_r(origin, direction, ray.time());

    if (!pointer->intersect(rotated_r, t_min, t_max, rec))
        return false;

    Vec3 p = rec.p;
    Vec3 normal = rec.normal;

    p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
    p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

    normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
    normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

bool Rotate_y::bounding_box(float t0, float t1, AABB &output_box) const
{
    output_box = bbox;
    return hasbox;
}

#endif