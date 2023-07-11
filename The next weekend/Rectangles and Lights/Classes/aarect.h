// aa rect header class
#ifndef AARECT_H
#define AARECT_H

#include "Hitable.h"

class xy_rect : public Hitable
{
public:
    float x0, x1, y0, y1, k;
    Material *material;

    xy_rect() {}
    xy_rect(float x0, float x1, float y0, float y1, float k, Material *material) : x0(x0), x1(x1), y0(y0), y1(y1), k(k), material(material) {}

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = AABB(Vec3(x0, y0, k - 0.0001), Vec3(x1, y1, k + 0.0001));
        return true;
    }
};

bool xy_rect::intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const
{
    float t = (k - ray.origin().z()) / ray.direction().z();
    if (t < t_min || t > t_max)
        return false;
    float x = ray.origin().x() + t * ray.direction().x();
    float y = ray.origin().y() + t * ray.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    hit_record.u = (x - x0) / (x1 - x0);
    hit_record.v = (y - y0) / (y1 - y0);
    hit_record.t = t;
    hit_record.material = material;
    hit_record.p = ray.point_at(t);
    hit_record.normal = Vec3(0.0, 0.0, 1.0);
    return true;
};

class xz_rect : public Hitable
{
public:
    float x0, x1, z0, z1, k;
    Material *material;

    xz_rect() {}
    xz_rect(float x0, float x1, float z0, float z1, float k, Material *material) : x0(x0), x1(x1), z0(z0), z1(z1), k(k), material(material) {}

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = AABB(Vec3(x0, k - 0.0001, z0), Vec3(x1, k + 0.0001, z1));
        return true;
    }
};

bool xz_rect::intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const
{
    float t = (k - ray.origin().y()) / ray.direction().y();
    if (t < t_min || t > t_max)
        return false;
    float x = ray.origin().x() + t * ray.direction().x();
    float z = ray.origin().z() + t * ray.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    hit_record.u = (x - x0) / (x1 - x0);
    hit_record.v = (z - z0) / (z1 - z0);
    hit_record.t = t;
    hit_record.material = material;
    hit_record.p = ray.point_at(t);
    hit_record.normal = Vec3(0.0, 1.0, 0.0);
    return true;
};

class yz_rect : public Hitable
{
public:
    float y0, y1, z0, z1, k;
    Material *material;

    yz_rect() {}
    yz_rect(float y0, float y1, float z0, float z1, float k, Material *material) : y0(y0), y1(y1), z0(z0), z1(z1), k(k), material(material) {}

    virtual bool intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = AABB(Vec3(k - 0.0001, y0, z0), Vec3(k + 0.0001, y1, z1));
        return true;
    }
};

bool yz_rect::intersect(const Ray &ray, float t_min, float t_max, Hit_record &hit_record) const
{
    float t = (k - ray.origin().x()) / ray.direction().x();
    if (t < t_min || t > t_max)
        return false;
    float y = ray.origin().y() + t * ray.direction().y();
    float z = ray.origin().z() + t * ray.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    hit_record.u = (y - y0) / (y1 - y0);
    hit_record.v = (z - z0) / (z1 - z0);
    hit_record.t = t;
    hit_record.material = material;
    hit_record.p = ray.point_at(t);
    hit_record.normal = Vec3(1.0, 0.0, 0.0);
    return true;
};
#endif