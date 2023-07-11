// material header class
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"
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

    Lambertian(const Vec3 &color) : color(new solid_color(color)) {}
    Lambertian(Texture *color) : color(color) {}

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 target = hit_record.p + hit_record.normal + random_in_unit_sphere();
        scattered = Ray(hit_record.p, scattered.direction(), ray_in.time());
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

// schlick approximation for reflectance of dielectric material (glass) at different angles of incidence and refraction indices (air = 1.0)
float schlick(float cosine, float reference_index)
{
    float r0 = (1 - reference_index) / (1 + reference_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}
class dielectric : public Material
{
public:
    float reference_index;

    dielectric(float reference_index) : reference_index(reference_index) {}

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 outward_normal;
        Vec3 reflected = reflect(ray_in.direction(), hit_record.normal);
        float ni_over_nt;
        attenuation = Vec3(1.0, 1.0, 1.0);
        Vec3 refracted;
        float reflect_probability;
        float cosine;
        if (dot(ray_in.direction(), hit_record.normal) > 0)
        {
            outward_normal = -hit_record.normal;
            ni_over_nt = reference_index;
            cosine = reference_index * dot(ray_in.direction(), hit_record.normal) / ray_in.direction().length();
        }
        else
        {
            outward_normal = hit_record.normal;
            ni_over_nt = 1.0 / reference_index;
            cosine = -dot(ray_in.direction(), hit_record.normal) / ray_in.direction().length();
        }
        if (refract(ray_in.direction(), outward_normal, ni_over_nt, refracted))
        {
            reflect_probability = schlick(cosine, reference_index);
        }
        else
        {
            reflect_probability = 1.0;
        }
        if (drand48() < reflect_probability)
        {
            scattered = Ray(hit_record.p, reflected, ray_in.time());
        }
        else
        {
            scattered = Ray(hit_record.p, refracted, ray_in.time());
        }
        return true;
    }
};

class Diffuse_light : public Material
{
public:
    Texture *emit;

    Diffuse_light(Texture *emit) : emit(emit) {}

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        return false;
    }

    virtual Vec3 emitted(float u, float v, const Vec3 &p) const
    {
        return emit->value(u, v, p);
    }
};

#endif