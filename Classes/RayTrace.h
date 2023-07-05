// header class for Ray Tracing
#ifndef RAYTRACE_H
#define RAYTRACE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>
#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "Material.h"
#include "Light.h"

Vector3 ray_trace(const Ray &ray, const Scene &scene, const Camera &camera, int depth = 0)
{
    // check if the ray has reached its maximum depth
    if (depth > camera.maxBounce)
        return Vector3(0.0, 0.0, 0.0);

    float t = std::numeric_limits<float>::max();
    Vector3 point, normal;
    Material hit_material;

    // check if the ray intersects with any object in the scene
    if (scene.intersect(ray, t, point, normal, hit_material))
    {
        Vector3 color(0.0, 0.0, 0.0);

        // Ambient light
        Vector3 ambient_color = hit_material.color * hit_material.ka;
        color = color + ambient_color;

        for (const Light &light : scene.lights)
        {
            Vector3 light_dir = (light.position - point).normalized();

            // Shadow ray direction
            Vector3 shadow_dir = light_dir;

            // Shadow ray origin
            Vector3 shadow_origin = point + shadow_dir * 0.001f;

            // Shadow ray
            Ray shadow_ray(shadow_origin, shadow_dir);

            float shadow_t = std::numeric_limits<float>::max();
            Vector3 shadow_point, shadow_normal;
            Material shadow_material;

            // check if the shadow ray intersects with any object in the scene
            if (scene.intersect(shadow_ray, shadow_t, shadow_point, shadow_normal, shadow_material))
            {
                // if the shadow ray intersects with an object, skip the current light source
                continue;
            }

            // Diffuse shading
            Vector3 bumpedNormal = hit_material.bumpNormal(point, normal);
            float diffuse_factor = std::max(0.0f, bumpedNormal.dot(light_dir));
            Vector3 reflect_dir = reflect(-light_dir, bumpedNormal);

            Vector3 diffuse_color = hit_material.color * hit_material.kd * diffuse_factor;

            // Specular shading

            Vector3 view_dir = (camera.position - point).normalized();
            float spec_factor = std::pow(std::max(0.0f, reflect_dir.dot(view_dir)), hit_material.exponent);
            Vector3 specular_color = hit_material.ks * light.intensity * spec_factor;

            // Attenuation
            float distance = (light.position - point).length();
            float attenuation = 1.0 / (1.0 + hit_material.ka * distance);

            // color (diffuse and specular parts with attenuation)
            color = color + (diffuse_color + specular_color) * light.intensity * attenuation;
        }

        // Reflection calculation
        Vector3 reflect_dir = reflect(ray.direction, normal).normalized();
        Vector3 reflect_origin = point + reflect_dir * 0.001f;
        Ray reflected_ray(reflect_origin, reflect_dir);

        // Recursive reflected ray
        Vector3 reflected_color = ray_trace(reflected_ray, scene, camera, depth + 1);

        // color depending on the reflectance of the material
        color = (color * (1.0f - hit_material.reflectance)) + (reflected_color * hit_material.reflectance);

        Vector3 refraction_direction = refract(ray.direction, normal, hit_material.refraction_index).normalized();
        Vector3 refraction_origin = point + refraction_direction * 0.001f;
        Ray refracted_ray(refraction_origin, refraction_direction);

        // Recursive refracted ray
        Vector3 refracted_color = ray_trace(refracted_ray, scene, camera, depth + 1);

        // color depending on the transmittance of the material
        color = color * (1.0f - hit_material.transmittance) + refracted_color * hit_material.transmittance;

        // Calculate the final color with the reflected and refracted colors
        color = (color * (1.0f - hit_material.reflectance - hit_material.transmittance)) + (reflected_color * hit_material.reflectance) + (refracted_color * hit_material.transmittance);

        for (const Spotlight &spotlight : scene.spotlights)
        {
            Vector3 light_dir = (spotlight.position - point).normalized();

            // Check if the point is within the cone of the spotlight
            float angle = acos(light_dir.dot(spotlight.direction.normalized()));
            if (angle > spotlight.angle)
            {
                // if the point is not within the cone of the spotlight, skip the current light source
                continue;
            }

            // falloff
            float falloff = pow(clamp((1.0 - angle / spotlight.angle), 0.0, 1.0), spotlight.falloffExponent);

            // Shadow ray direction
            Vector3 shadow_dir = light_dir;

            // Shadow ray origin
            Vector3 shadow_origin = point + shadow_dir * 0.001f;

            // Shadow ray
            Ray shadow_ray(shadow_origin, shadow_dir);

            float shadow_t = std::numeric_limits<float>::max();
            Vector3 shadow_point, shadow_normal;
            Material shadow_material;

            // check if the shadow ray intersects with any object in the scene
            if (scene.intersect(shadow_ray, shadow_t, shadow_point, shadow_normal, shadow_material))
            {
                continue;
            }

            // Compute lighting from spotlight
            Vector3 view_dir = (camera.position - point).normalized();
            Vector3 spotlight_color = scene.computeLighting(point, normal, view_dir, hit_material.exponent);

            // Attenuation
            float distance = (spotlight.position - point).length();
            float attenuation = 1.0 / (1.0 + hit_material.ka * distance);

            // color with falloff and attenuation
            color = color + spotlight_color * spotlight.intensity * falloff * attenuation;
        }

        // Texture mapping
        if (hit_material.texture && hit_material.texture->isValid())
        {
            // Get the texture coordinates at the intersection point
            Vector2 texCoords = hit_material.textureCoordinates(point);

            // Sample the texture color at the texture coordinates
            Vector3 texture_color = hit_material.texture->sampleSuper(texCoords, 4);

            // Apply the texture color to the material color
            color = color * texture_color;
        }

        return color;
    }
    else
    {
        return Vector3(0.0, 0.0, 0.0);
    }
}

#endif
