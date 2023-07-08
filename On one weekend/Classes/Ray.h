// Ray header class
#ifndef RAY_H
#define RAY_H

#include "Vec3.h"

class Ray
{
public:
    Ray() {}
    Ray(const Vec3 &a, const Vec3 &b)
    {
        A = a;
        B = b;
    }

    Vec3 origin() const { return A; }
    Vec3 direction() const { return B; }
    Vec3 point_at(float t) const { return A + B * t; }

    Vec3 A;
    Vec3 B;
};

#endif // RAY_H