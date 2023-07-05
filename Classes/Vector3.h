// header class for Vector3
#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <cmath>

class Vector3
{
public:
    float x, y, z;

    Vector3(float x = 0.0, float y = 0.0, float z = 0.0) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3 &v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
    Vector3 operator-(const Vector3 &v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
    Vector3 operator*(float f) const { return Vector3(x * f, y * f, z * f); }

    float dot(const Vector3 &v) const { return x * v.x + y * v.y + z * v.z; }
    Vector3 cross(const Vector3 &v) const
    {
        return Vector3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x);
    }

    Vector3 operator*(const Vector3 &v) const { return Vector3(x * v.x, y * v.y, z * v.z); }

    float length() const { return std::sqrt(x * x + y * y + z * z); }
    Vector3 normalized() const { return *this * (1.0 / length()); }
    Vector3 &normalize() { return *this = normalized(); }
    float norm() const { return x * x + y * y + z * z; }
    Vector3 &operator+=(const Vector3 &v) { return *this = *this + v; }
    Vector3 &operator-=(const Vector3 &v) { return *this = *this - v; }
    Vector3 &operator*=(float f) { return *this = *this * f; }
    Vector3 &operator*=(const Vector3 &v) { return *this = *this * v; }
    Vector3 operator-() const { return Vector3(-x, -y, -z); }

    // operand types are: double * Vector3
    friend Vector3 operator*(float f, const Vector3 &v) { return Vector3(f * v.x, f * v.y, f * v.z); }

   // friend function for reflection (friend because it needs access to private members)
    friend Vector3 reflect(const Vector3 &incident, const Vector3 &normal)
    {
        return incident - 2.0f * incident.dot(normal) * normal;
    }

    // color / num_samples
    friend Vector3 operator/(const Vector3 &v, float f) { return Vector3(v.x / f, v.y / f, v.z / f); }

    Vector3 reflect(const Vector3 &normal) const
    {
        return *this - 2.0f * this->dot(normal) * normal;
    }

    friend Vector3 refract(const Vector3 &incident, const Vector3 &normal, float ior)
    {
        float cosi = clamp(-1, 1, incident.dot(normal));
        float etai = 1, etat = ior;
        Vector3 n = normal;

        if (cosi < 0)
        {
            cosi = -cosi;
        }
        else
        {
            std::swap(etai, etat);
            n = -normal;
        }

        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);

        if (k < 0)
        {
            return Vector3(0.0, 0.0, 0.0);
        }
        else
        {
            return eta * incident + (eta * cosi - sqrtf(k)) * n;
        }
    }

    // clamp value between min and max
    static float clamp(float min, float max, float value)
    {
        return std::max(min, std::min(max, value));
    }

    // Calculate the normal at a point on the surface of the sphere
    friend Vector3 normal(const Vector3 &point)
    {
        return (point - Vector3(0.0, 0.0, 0.0)).normalized();
    }    
};

#endif