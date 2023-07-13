// constant_medium header class
#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"
#include "Vec3.h"

class constant_medium : public Hittable {
    public:
    Hittable *boundary;
    Material *phase_function;
    double neg_inv_density;

    constant_medium(Hittable *boundary, double density, Texture *a) : boundary(boundary), neg_inv_density(-1/density) {
        phase_function = new Isotropic(a);
    }

    constant_medium(Hittable *boundary, double density, Vec3 c) : boundary(boundary), neg_inv_density(-1/density) {
        phase_function = new Isotropic(c);
    }

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const override;

    virtual bool bounding_box(float t0, float t1, AABB &opbox) const override {
        return boundary->bounding_box(t0, t1, opbox);
    }

};

// almost the same as the one in the book
bool constant_medium::intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const 
{
    // Print occasional samples when debugging. To enable, set enableDebug true.
    const bool enableDebug = false;
    const bool debugging = enableDebug && drand48() < 0.00001;

    Hit_record rec1, rec2;

    if (!boundary->intersect(ray, -INFINITY, INFINITY, rec1))
        return false;

    if (!boundary->intersect(ray, rec1.t + 0.0001, INFINITY, rec2))
        return false;

    if (debugging) std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << '\n';

    if (rec1.t < t_min)
        rec1.t = t_min;

    if (rec2.t > t_max)
        rec2.t = t_max;

    if (rec1.t >= rec2.t)
        return false;

    if (rec1.t < 0)
        rec1.t = 0;

    const auto ray_length = ray.direction().length();
    const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const auto hit_distance = neg_inv_density * log(drand48());

    if (hit_distance > distance_inside_boundary)
        return false;

    hit_record.t = rec1.t + hit_distance / ray_length;
    hit_record.p = ray.point_at(hit_record.t);

    if (debugging) {
        std::cerr << "hit_distance = " <<  hit_distance << '\n'
                  << "rec.t = " <<  hit_record.t << '\n'
                  << "rec.p = " <<  hit_record.p << '\n';
    }

    hit_record.normal = Vec3(1, 0, 0);  // arbitrary
    hit_record.front_face = true;     // also arbitrary
    hit_record.material = phase_function;

    return true;  
}

#endif