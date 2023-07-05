// triangle header class
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Vector3.h"
#include "Vector2.h"
#include "Material.h"
#include "Ray.h"
#include "Texture.h"

class Triangle
{
public:
    // v = vertex, t = texture, n = normal
    Vector3 v0, v1, v2;
    Vector2 t0, t1, t2;
    Vector3 n0, n1, n2;
    Material material;
    Texture *texture;

    Triangle(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
             const Vector3 &n0, const Vector3 &n1, const Vector3 &n2,
             const Material &material)
        : v0(v0), v1(v1), v2(v2), n0(n0), n1(n1), n2(n2), material(material), texture(nullptr) {}

    Triangle(const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
             const Vector2 &t0, const Vector2 &t1, const Vector2 &t2,
             const Vector3 &n0, const Vector3 &n1, const Vector3 &n2,
             const Material &material, Texture *texture)
        : v0(v0), v1(v1), v2(v2), t0(t0), t1(t1), t2(t2), n0(n0), n1(n1), n2(n2), material(material), texture(texture) {}

    // triangle intersect function
    bool intersect(const Ray &ray, float tMin, float tMax, float &t, float &u, float &v) const
    {
        Vector3 edge1 = v1 - v0;
        Vector3 edge2 = v2 - v0;

        Vector3 pVec = ray.direction.cross(edge2);
        float det = edge1.dot(pVec);

        if (det == 0.0f)
            return false;

        float invDet = 1.0f / det;

        Vector3 tVec = ray.origin - v0;
        u = tVec.dot(pVec) * invDet;
        if (u < 0.0f || u > 1.0f)
            return false;

        Vector3 qVec = tVec.cross(edge1);
        v = ray.direction.dot(qVec) * invDet;
        if (v < 0.0f || u + v > 1.0f)
            return false;

        t = edge2.dot(qVec) * invDet;

        if (t < tMin || t > tMax)
            return false;

        return true;
    }

    // calculateNormal function
    Vector3 calculateNormal(float u, float v) const
    {
        return (1 - u - v) * n0 + u * n1 + v * n2;
    }

    // textureCoordinates function
    Vector2 textureCoordinates(float a, float b) const
    {
        // Use barycentric interpolation for triangle texture mapping
        float u = (1 - a - b) * t0.x + a * t1.x + b * t2.x;
        float v = (1 - a - b) * t0.y + a * t1.y + b * t2.y;

        // Apply interpolation to the texture coordinates
        u = interpolate(u);
        v = interpolate(v);

        return Vector2(u, v);
    }
};

#endif
