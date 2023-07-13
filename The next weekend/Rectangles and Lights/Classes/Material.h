// material header class
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"
#include "Ray.h"
#include "Hittable.h"
#include "Texture.h"

class Material
{
public:
    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const = 0;

    virtual Vec3 emitted(float u, float v, const Vec3 &p) const
    {
        return Vec3(0.0, 0.0, 0.0);
    }
};

class Lambertian : public Material
{
public:
    Texture *color;
    Vec3 albedo;

    Lambertian(const Vec3 &albedo) : albedo(albedo) {}

    Lambertian(Texture *color) : color(color) {}

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 scatter_direction = hit_record.normal + random_unit_vector();

        if (near_zero(scatter_direction))
            scatter_direction = hit_record.normal;

        scattered = Ray(hit_record.p, scatter_direction);
        attenuation = color->value(hit_record.u, hit_record.v, hit_record.p);

        return true;
    }
};

class Metal : public Material
{
public:
    Vec3 color;
    float fuzz;

    Metal(const Vec3 &color, float fuzz) : color(color)
    {
        if (fuzz < 1)
            this->fuzz = fuzz;
        else
            this->fuzz = 1;
    }

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 reflected = reflect(unit_vector(ray_in.direction()), hit_record.normal);
        scattered = Ray(hit_record.p, reflected + fuzz * random_in_unit_sphere(), ray_in.time());
        attenuation = color;
        return (dot(scattered.direction(), hit_record.normal) > 0);
    }
};

class dielectric : public Material
{
public:
    float refrection_index;

    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

    dielectric(float refrection_index) : refrection_index(refrection_index) {}

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        attenuation = Vec3(1.0, 1.0, 1.0);
        float refraction_ratio = hit_record.front_face ? (1.0 / refrection_index) : refrection_index;

        Vec3 unit_direction = unit_vector(ray_in.direction());
        float cos_theta = fmin(dot(-unit_direction, hit_record.normal), 1.0);
        float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > drand48())
            direction = reflect(unit_direction, hit_record.normal);
        else
            direction = refract(unit_direction, hit_record.normal, refraction_ratio);

        scattered = Ray(hit_record.p, direction, ray_in.time());
        return true;
    }
};

class Diffuse_light : public Material
{
public:
    Texture *emit;

    Diffuse_light(Texture *emit) : emit(emit) {}
    Diffuse_light(Vec3 color) : emit(new solid_color(color)) {}

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        return false;
    }

    virtual Vec3 emitted(float u, float v, const Vec3 &p) const
    {
        return emit->value(u, v, p);
    }
};

class Isotropic : public Material
{
public:
    Texture *color;

    Isotropic(Texture *color) : color(color) {}
    Isotropic(Vec3 albedo) : color(new solid_color(albedo)) {}

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        scattered = Ray(hit_record.p, random_in_unit_sphere(), ray_in.time());
        attenuation = color->value(hit_record.u, hit_record.v, hit_record.p);
        return true;
    }
};

#endif