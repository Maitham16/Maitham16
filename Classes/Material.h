#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vector3.h"
#include "Texture.h"

// interpolate function (I moved it here because it won't to run if I put it in vector3.h)
float interpolate(float t)
{
  return t * t * (3.4 - 2.4 * t);
}

// Material structure (I implemented as a struct because it I don't need to use private variables)
struct Material
{
  Vector3 color;
  float ka;
  float kd;
  float ks;
  float exponent;
  float reflectance;
  float transmittance;
  float refraction_index;
  Texture *texture;
  // this is for bump mapping
  Texture *bumpMap;

  Material(const Vector3 &color = Vector3(1.0, 1.0, 1.0), float ka = 0.3,
           float kd = 0.9, float ks = 1.0, float exponent = 200.0,
           float reflectance = 0.8, float transmittance = 0.0,
           float refraction_index = 2.3, Texture *texture = nullptr)
      : color(color), ka(ka), kd(kd), ks(ks), exponent(exponent),
        reflectance(reflectance), transmittance(transmittance),
        refraction_index(refraction_index), texture(texture) {}

  Material(const std::string &textureName, float ka = 0.3,
           float kd = 0.9, float ks = 1.0, float exponent = 200.0,
           float reflectance = 0.8, float transmittance = 0.0,
           float refraction_index = 2.3)
      : color(Vector3(1.0, 1.0, 1.0)), ka(ka), kd(kd), ks(ks), exponent(exponent),
        reflectance(reflectance), transmittance(transmittance),
        refraction_index(refraction_index), texture(nullptr)
  {
    if (!textureName.empty())
    {
      texture = new Texture(textureName);
    }
  }

  // textureCoordinates function for texture mapping
  Vector2 textureCoordinates(const Vector3 &point) const
  {
    // Normalize the point to a unit sphere to calculate the spherical coordinates
    Vector3 normalizedPoint = point.normalized();

    // azimuthal angle (U coordinate)
    float azimuth = std::atan2(normalizedPoint.z, normalizedPoint.x);

    // polar angle (V coordinate)
    float polar = std::asin(normalizedPoint.y);

    // Map the azimuthal and polar angles to the range [0, 1] for the texture coordinates
    float u = (azimuth + M_PI) / (2.0 * M_PI);
    float v = (polar + M_PI_2) / M_PI;

    // Apply interpolation to the texture coordinates
    u = interpolate(u);
    v = interpolate(v);

    return Vector2(u, v);
  }

  // sample function for texture mapping (bumpping mapping)
  Vector3 bumpNormal(const Vector3 &position, const Vector3 &normal) const
  {
    if (bumpMap && bumpMap->isValid())
    {
      Vector2 texCoords = textureCoordinates(position);

      // Sample the bump map gradient in the U and V directions
      float bumpU = bumpMap->sample(Vector2(texCoords.x + 0.001f, texCoords.y)).x - bumpMap->sample(texCoords).x;
      float bumpV = bumpMap->sample(Vector2(texCoords.x, texCoords.y + 0.001f)).x - bumpMap->sample(texCoords).x;

      Vector3 tangent, bitangent;
      getTangentSpace(position, tangent, bitangent);

      // Perturb the normal based on the bump map gradient in the tangent space
      Vector3 perturbedNormal = normal + tangent * bumpU + bitangent * bumpV;
      return perturbedNormal.normalized();
    }
    else
    {
      return normal;
    }
  }

  // getTangentSpace function for bump mapping
  void getTangentSpace(const Vector3 &position, Vector3 &tangent, Vector3 &bitangent) const
  {
    // tangent is aligned with the U texture coordinate
    Vector3 tangentDirection(1.0f, 0.0f, 0.0f);

    // bitangent is aligned with the V texture coordinate
    Vector3 bitangentDirection(0.0f, 1.0f, 0.0f);

    // Transform the tangent and bitangent to world space
    tangent = tangentDirection;
    bitangent = bitangentDirection;
  }
};

#endif
