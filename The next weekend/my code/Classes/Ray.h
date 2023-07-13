// Ray header class
#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray
{
public:
    Vec3 origin_co;
    Vec3 direction_co;
    double time_du;

    Ray() {}
    Ray(const Vec3 &a, const Vec3 &b, double ti = 0.0) : origin_co(a), direction_co(b), time_du(ti) {}

    Vec3 origin() const { return origin_co; }
    Vec3 direction() const { return direction_co; }
    Vec3 point_at(float t) const { return origin_co + direction_co * t; }
    double time() const { return time_du; }
};

#endif