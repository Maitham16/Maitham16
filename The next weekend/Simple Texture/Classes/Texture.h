// Texture header class
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "Vec3.h"

class Texture
{
public:
    virtual Vec3 value(float u, float v, const Vec3 &p) const = 0;
};

class solid_color : public Texture
{
public:
    Vec3 color_value;

    solid_color() {}
    solid_color(Vec3 c) : color_value(c) {}

    solid_color(float red, float green, float blue)
        : solid_color(Vec3(red, green, blue)) {}

    virtual Vec3 value(float u, float v, const Vec3 &p) const override
    {
        return color_value;
    }
};

class checker_texture : public Texture
{
public:
    Texture *odd;
    Texture *even;

    checker_texture() {}
    checker_texture(Texture *t0, Texture *t1) : even(t0), odd(t1) {}

    virtual Vec3 value(float u, float v, const Vec3 &p) const override
    {
        float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }
};

#endif