// scene header class
#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Sphere.h"
#include "Light.h"
#include "Model.h"
#include "Camera.h"
#include "Spotlight.h"

class Scene
{
public:
    std::vector<Sphere> spheres;
    std::vector<Light> lights;
    std::vector<Model> models;
    std::vector<Spotlight> spotlights;
    Camera camera;

    void add(const Sphere &sphere)
    {
        spheres.push_back(sphere);
    }

    void add(const Light &light)
    {
        lights.push_back(light);
    }

    void add(const Model &model)
    {
        models.push_back(model);
    }

    void setCamera(const Camera &cam)
    {
        camera = cam;
    }

    // spotlight function to add a spotlight to the scene
    void addSpotlight(const Spotlight &spotlight)
    {
        spotlights.push_back(spotlight);
    }

    // scene intersect function
    bool intersect(const Ray &ray, float &t, Vector3 &point, Vector3 &normal, Material &material) const
    {
        for (const auto &sphere : spheres)
        {
            float t_sphere;
            if (sphere.intersect(ray, t_sphere))
            {
                if (t_sphere < t)
                {
                    t = t_sphere;
                    point = ray.origin + ray.direction * t;
                    normal = (point - sphere.center).normalized();
                    material = sphere.material;
                }
            }
        }
        for (const auto &model : models)
        {
            for (const auto &triangle : model.triangles)
            {
                float t_triangle, u, v;
                if (triangle.intersect(ray, 0.001f, std::numeric_limits<float>::max(), t_triangle, u, v))
                {
                    if (t_triangle < t)
                    {
                        t = t_triangle;
                        point = ray.origin + ray.direction * t;
                        normal = triangle.calculateNormal(u, v);
                        material = triangle.material;
                    }
                }
            }
        }
        return t < std::numeric_limits<float>::max();
    }

    // scene compute lighting function
    Vector3 computeLighting(const Vector3 &point, const Vector3 &normal, const Vector3 &viewDirection, double specular) const
    {
        Vector3 result = Vector3(0, 0, 0);
        for (const Spotlight &spotlight : spotlights)
        {
            Vector3 lightColor = spotlight.getLighting(point);
            Vector3 lightDirection = (spotlight.position - point).normalize();
            double cosTheta = normal.dot(lightDirection);
            if (cosTheta > 0)
            {
                result += lightColor * cosTheta;
                Vector3 reflectionDirection = (2 * cosTheta * normal - lightDirection).normalize();
                double cosAlpha = viewDirection.dot(reflectionDirection);
                if (cosAlpha > 0)
                {
                    result += lightColor * pow(cosAlpha, specular);
                }
            }
        }
        return result;
    }
};

#endif
