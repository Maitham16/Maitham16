// transform header class
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector3.h"
#include "Matrix4.h"

class Transform
{
private:
    Matrix4 transformMatrix;

public:
    Transform() {}

    void translate(float x, float y, float z)
    {
        transformMatrix = transformMatrix * Matrix4::translate(x, y, z);
    }

    void rotateX(float angle)
    {
        transformMatrix = transformMatrix * Matrix4::rotate(angle, 1.0f, 0.0f, 0.0f);
    }

    void rotateY(float angle)
    {
        transformMatrix = transformMatrix * Matrix4::rotate(angle, 0.0f, 1.0f, 0.0f);
    }

    void rotateZ(float angle)
    {
        transformMatrix = transformMatrix * Matrix4::rotate(angle, 0.0f, 0.0f, 1.0f);
    }

    void scale(float x, float y, float z)
    {
        transformMatrix = transformMatrix * Matrix4::scale(x, y, z);
    }

    Vector3 transformPoint(const Vector3& point) const
    {
        Vector4 transformed = transformMatrix * Vector4(point.x, point.y, point.z, 1.0f);
        return Vector3(transformed.x, transformed.y, transformed.z);
    }

    Vector3 transformDirection(const Vector3& direction) const
    {
        Vector4 transformed = transformMatrix * Vector4(direction.x, direction.y, direction.z, 0.0f);
        return Vector3(transformed.x, transformed.y, transformed.z).normalized();
    }
};

#endif
