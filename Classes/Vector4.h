// vector4 header class
#ifndef _VECTOR4_H_
#define _VECTOR4_H_

#include <cmath>
#include <iostream>

#include "Vector3.h"

class Vector4
{
public:
    double x;
    double y;
    double z;
    double w;

    Vector4()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    Vector4(double x, double y, double z, double w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    Vector4 operator+(const Vector4 &v) const
    {
        return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    Vector4 operator-(const Vector4 &v) const
    {
        return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    Vector4 operator*(double s) const
    {
        return Vector4(x * s, y * s, z * s, w * s);
    }

    Vector4 operator/(double s) const
    {
        return Vector4(x / s, y / s, z / s, w / s);
    }

    Vector4 operator-() const
    {
        return Vector4(-x, -y, -z, -w);
    }

    Vector4 &operator+=(const Vector4 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    Vector4 &operator-=(const Vector4 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    Vector4 &operator*=(double s)
    {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    Vector4 &operator/=(double s)
    {
        x /= s;
        y /= s;
        z /= s;
        w /= s;
        return *this;
    }

    double dot(const Vector4 &v) const
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    double length() const
    {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Vector4 normalize() const
    {
        double l = length();
        return Vector4(x / l, y / l, z / l, w / l);
    }

    friend std::ostream &operator<<(std::ostream &os, const Vector4 &v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
        return os;
    }

    friend Vector4 operator*(double s, const Vector4 &v)
    {
        return Vector4(s * v.x, s * v.y, s * v.z, s * v.w);
    }

    friend Vector4 operator/(double s, const Vector4 &v)
    {
        return Vector4(s / v.x, s / v.y, s / v.z, s / v.w);
    }

    friend Vector4 operator+(const Vector4 &v, double s)
    {
        return Vector4(v.x + s, v.y + s, v.z + s, v.w + s);
    }

    friend Vector4 operator-(const Vector4 &v, double s)
    {
        return Vector4(v.x - s, v.y - s, v.z - s, v.w - s);
    }

    friend Vector4 operator+(double s, const Vector4 &v)
    {
        return Vector4(s + v.x, s + v.y, s + v.z, s + v.w);
    }

    friend Vector4 operator-(double s, const Vector4 &v)
    {
        return Vector4(s - v.x, s - v.y, s - v.z, s - v.w);
    }

    friend Vector4 operator+(const Vector4 &v1, const Vector4 &v2)
    {
        return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
    }

    friend Vector4 operator-(const Vector4 &v1, const Vector4 &v2)
    {
        return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
    }

    Vector4 operator*(const Vector4 &rhs) const
    {
        return Vector4(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

// this function from https://stackoverflow.com/questions/30198520/class-with-union-use-of-deleted-function-on-constructor
    Vector4 operator*(const double &rhs) const
    {
        return Vector4(x * rhs, y * rhs, z * rhs, w * rhs);
    }

    //  vector4(Vector3, double) function
    Vector4(Vector3 v, double w)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        this->w = w;
    }

    // get xyz
    Vector3 xyz() const
    {
        return Vector3(x, y, z);
    }
};

#endif