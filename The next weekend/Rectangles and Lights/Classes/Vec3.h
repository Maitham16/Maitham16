// Vector3 header class (according to the book Ray Tracing in One Weekend)
#ifndef Vec3_H
#define Vec3_H

#include <cmath>
#include <iostream>

class Vec3
{
public:
    Vec3() {}
    Vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    inline const Vec3 &operator+() const { return *this; }
    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float &operator[](int i) { return e[i]; }

    inline Vec3 &operator+=(const Vec3 &v2);
    inline Vec3 &operator-=(const Vec3 &v2);
    inline Vec3 &operator*=(const Vec3 &v2);
    inline Vec3 &operator/=(const Vec3 &v2);
    inline Vec3 &operator*=(const float t);
    inline Vec3 &operator/=(const float t);

    inline float length() const { return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
    inline float squared_length() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }
    inline void make_unit_vector();

    float e[3];
};

inline std::istream &operator>>(std::istream &is, Vec3 &t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream &operator>>(std::ostream &os, const Vec3 &t)
{
    os << t.e[0] << t.e[1] << t.e[2];
    return os;
}

inline void Vec3::make_unit_vector()
{
    float k = 1.0 / sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] + v2.e[0],
                v1.e[1] + v2.e[1],
                v1.e[2] + v2.e[2]);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] - v2.e[0],
                v1.e[1] - v2.e[1],
                v1.e[2] - v2.e[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] * v2.e[0],
                v1.e[1] * v2.e[1],
                v1.e[2] * v2.e[2]);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3(v1.e[0] / v2.e[0],
                v1.e[1] / v2.e[1],
                v1.e[2] / v2.e[2]);
}

inline Vec3 operator*(const Vec3 &v, float t)
{
    return Vec3(v.e[0] * t,
                v.e[1] * t,
                v.e[2] * t);
}

inline Vec3 operator*(double t, const Vec3 &v)
{
    return Vec3(t * v.e[0],
                t * v.e[1],
                t * v.e[2]);
}

inline Vec3 operator/(Vec3 v, float t)
{
    return Vec3(v.e[0] / t,
                v.e[1] / t,
                v.e[2] / t);
}

inline float dot(const Vec3 &v1, const Vec3 &v2)
{
    return v1.e[0] * v2.e[0] +
           v1.e[1] * v2.e[1] +
           v1.e[2] * v2.e[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2)
{
    return Vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
                (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
                (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline Vec3 &Vec3::operator+=(const Vec3 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];

    return *this;
}

inline Vec3 &Vec3::operator-=(const Vec3 &v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];

    return *this;
}

inline Vec3 &Vec3::operator*=(const Vec3 &v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];

    return *this;
}

inline Vec3 &Vec3::operator/=(const Vec3 &v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];

    return *this;
}

inline Vec3 &Vec3::operator*=(const float t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;

    return *this;
}

inline Vec3 &Vec3::operator/=(const float t)
{
    float k = 1.0 / t;

    e[0] *= k;
    e[1] *= k;
    e[2] *= k;

    return *this;
}

inline Vec3 unit_vector(Vec3 v)
{
    return v / v.length();
}

inline Vec3 random_in_unit_sphere()
{
    Vec3 p;
    do
    {
        p = 2.0 * Vec3(drand48(), drand48(), drand48()) - Vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);

    return p;
}

inline Vec3 reflect(const Vec3 &v, const Vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

inline bool refract(const Vec3 &v, const Vec3 &n, float ni_over_nt, Vec3 &refracted)
{
    Vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);

    if (discriminant > 0)
    {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);

        return true;
    }
    else
    {
        return false;
    }
}

inline Vec3 refract(const Vec3 &uv, const Vec3 &n, double etai_over_etat)
{
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.squared_length())) * n;
    return r_out_perp + r_out_parallel;
}

inline Vec3 random_in_unit_disk()
{
    Vec3 p;
    do
    {
        p = 2.0 * Vec3(drand48(), drand48(), 0.0) - Vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);

    return p;
}

inline double random_double(double time0, double time1)
{
    return time0 + (time1 - time0) * drand48();
}

// random_int
inline int random_int(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// unit_vector(double x)
inline Vec3 unit_vector(double x)
{
    return Vec3(x, x, x);
}

// clamp
inline double clamp(double x, double min, double max)
{
    if (x < min)
        return min;
    if (x > max)
        return max;

    return x;
}

// double += Vec3
inline void operator+=(double &x, const Vec3 &v)
{
    x += v.x();
}

// near_zero
inline bool near_zero(const Vec3 &v)
{
    // Return true if the vector is close to zero in all dimensions.
    const auto s = 1e-8;
    return (fabs(v.x()) < s) && (fabs(v.y()) < s) && (fabs(v.z()) < s);
}

// random_unit_vector
inline Vec3 random_unit_vector()
{
    return unit_vector(random_in_unit_sphere());
}

// ostream << Vec3
inline std::ostream &operator<<(std::ostream &out, const Vec3 &v)
{
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

// random_vec3(0, 165), 10, white)
inline Vec3 random_vec3(double min, double max)
{
    return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

#endif