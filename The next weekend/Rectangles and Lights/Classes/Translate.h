// Translate header class
#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "Hittable.h"
#include "Vec3.h"
#include "AABB.h"
#include "Ray.h"

class Translate : public Hittable
{
    public:
    Hittable *pointer;
    Vec3 offset;

    Translate(Hittable *p, const Vec3 &displacement) : pointer(p), offset(displacement) {}

    virtual bool intersect(const Ray &r, float t_min, float t_max, Hit_record &rec) const override
    {
        Ray moved_r(r.origin() - offset, r.direction(), r.time());
        if (!pointer->intersect(moved_r, t_min, t_max, rec))
            return false;

        rec.p += offset;
        rec.set_face_normal(moved_r, rec.normal);

        return true;
    }

    virtual bool bounding_box(float t0, float t1, AABB &output_box) const override
    {
        if (!pointer->bounding_box(t0, t1, output_box))
            return false;

        output_box = AABB(output_box.min() + offset, output_box.max() + offset);

        return true;
    }

};

#endif