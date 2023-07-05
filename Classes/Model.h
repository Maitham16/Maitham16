// header class model
#ifndef MODEL_H
#define MODEL_H

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "Triangle.h"
#include "Material.h"
#include "Vector2.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"

class Model
{
public:
    std::vector<Vector3> vertices;
    std::vector<Vector2> textures;
    std::vector<Vector3> normals;
    std::vector<Triangle> triangles;
    Transform transform;

    Model(const std::string &filename, const Material &material)
    {
        std::ifstream in(filename, std::ios::in);
        if (!in)
        {
            std::cerr << "Cannot open " << filename << std::endl;
            exit(1);
        }

        std::string line;
        while (std::getline(in, line))
        {
            if (line.substr(0, 2) == "v ")
            {
                std::istringstream s(line.substr(2));
                Vector3 v;
                s >> v.x;
                s >> v.y;
                s >> v.z;
                vertices.push_back(v);
            }
            else if (line.substr(0, 3) == "vt ")
            {
                std::istringstream s(line.substr(3));
                Vector2 t;
                s >> t.x;
                s >> t.y;
                textures.push_back(t);
            }
            else if (line.substr(0, 3) == "vn ")
            {
                std::istringstream s(line.substr(3));
                Vector3 n;
                s >> n.x;
                s >> n.y;
                s >> n.z;
                normals.push_back(n);
            }
            else if (line.substr(0, 2) == "f ")
            {
                std::istringstream s(line.substr(2));
                int a, b, c;
                int aT, bT, cT;
                int aN, bN, cN;

                char ch;

                // Parsing faces with vertex, texture, and normal indices
                s >> a >> ch >> aT >> ch >> aN;
                s >> b >> ch >> bT >> ch >> bN;
                s >> c >> ch >> cT >> ch >> cN;

                // console checking
                std::cout << "Face: " << a << " " << b << " " << c << std::endl;
                // create triangle with texture and normal if the material has texture and normal map
                if (material.texture)
                {
                    triangles.push_back(Triangle(vertices[a - 1], vertices[b - 1], vertices[c - 1],
                                                 textures[aT - 1], textures[bT - 1], textures[cT - 1],
                                                 normals[aN - 1], normals[bN - 1], normals[cN - 1],
                                                 material, material.texture));
                }
                else
                {
                    triangles.push_back(Triangle(vertices[a - 1], vertices[b - 1], vertices[c - 1],
                                                 normals[aN - 1], normals[bN - 1], normals[cN - 1],
                                                 material));
                }
            }
        }
    }

    // function to get the transform
    const Transform &getTransform() const
    {
        return transform;
    }

    // function to set the transform
    void setTransform(const Transform &transform)
    {
        this->transform = transform;
    }
};

#endif
