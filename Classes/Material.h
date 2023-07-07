// material header class
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3.h"
#include "Ray.h"
#include "Hitable.h"

class Material
{
public:
    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Vec3 color;

    Lambertian(const Vec3 &color) : color(color) {}

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 target = hit_record.p + hit_record.normal + random_in_unit_sphere();
        scattered = Ray(hit_record.p, target - hit_record.p);
        attenuation = color;
        return true;
    }
};

class Metal : public Material
{
public:
    Vec3 color;
    float fuzz;

    Metal(const Vec3 &color, float fuzz) : color(color) {if (fuzz < 1) this->fuzz = fuzz; else this->fuzz = 1;}

    virtual bool scatter(const Ray &ray_in, const Hit_record &hit_record, Vec3 &attenuation, Ray &scattered) const
    {
        Vec3 reflected = reflect(unit_vector(ray_in.direction()), hit_record.normal);
        scattered = Ray(hit_record.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = color;
        return (dot(scattered.direction(), hit_record.normal) > 0);
    }
};

#endif