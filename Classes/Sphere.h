// header class for Sphere
#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include "Vector3.h"
#include "Ray.h"
#include "Material.h"
#include "Transform.h"

class Sphere
{
public:
    Vector3 center;
    float radius;
    Material material;
    Transform transform;

    Sphere(const Vector3 &center, float radius, const Material &material)
        : center(center), radius(radius), material(material) {}

// sphere intersect function
    bool intersect(const Ray &ray, float &t) const
    {
        Vector3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0 * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0)
        {
            return false;
        }
        else
        {
            float temp = (-b - sqrt(discriminant)) / (2.0 * a);
            if (temp > 0)
            {
                t = temp;
                return true;
            }
            temp = (-b + sqrt(discriminant)) / (2.0 * a);
            if (temp > 0)
            {
                t = temp;
                return true;
            }
            return false;
        }
    }

    // default constructor
    Sphere()
    {
        center = Vector3(0.0, 0.0, 0.0);
        radius = 1.0;
        material = Material();
    }

    Vector3 normal(const Vector3 &point) const
    {
        return (point - center).normalized();
    }

    Material get_material() const
    {
        return material;
    }

    const Transform &getTransform() const
    {
        return transform;
    }

    void setTransform(const Transform &transform)
    {
        this->transform = transform;
    }
};

#endif