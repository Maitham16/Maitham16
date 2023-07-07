// main function
#include <iostream>
#include <fstream>

#include "Classes/stdtemplate.h"
#include "Classes/Vec3.h"
#include "Classes/Ray.h"
#include "Classes/Camera.h"
#include "Classes/Sphere.h"

// return background color
Vec3 color(const Ray &ray, const Sphere &sphere)
{
    if (sphere.hit_sphere(sphere.center, sphere.radius, ray))
    {
        return Vec3(1.0, 1.0, 0.0);
    }
    Vec3 unit_direction = unit_vector(ray.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);

    return Vec3(1.0, 1.0, 1.0) * (1.0 - t) + Vec3(0.4, 0.5, 1.0) * t;
}

int main()
{
    int width = 800;
    int height = 400;
    std::ofstream ofs;
    ofs.open("./output.ppm");
    std::cout << "P3\n"
              << width << " " << height << "\n255\n";
    sdltemplate::sdl("Ray Trace tutorial", width, height);
    sdltemplate::loop();
    ofs << "P3\n"
        << width << " " << height << "\n255\n";
    
    // scene
    Camera camera;
    camera.left = Vec3(-2.0, -1.0, -1.0);
    camera.horizontal = Vec3(4.0, 0.0, 0.0);
    camera.vertical = Vec3(0.0, 2.0, 0.0);
    camera.position = Vec3(0.0, 0.0, 0.0);

    Sphere sphere;
    sphere.center = Vec3(0.0, 0.0, -1.0);
    sphere.radius = 0.5;
    
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            float u = float(i) / float(width);
            float v = float(j) / float(height);
            Ray ray = camera.get_ray(u, v);
            Vec3 bgColor = color(ray, sphere);
            int ir = int(255.99 * bgColor[0]);
            int ig = int(255.99 * bgColor[1]);
            int ib = int(255.99 * bgColor[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
            ofs << ir << " " << ig << " " << ib << "\n";
            sdltemplate::setDrawColor(sdltemplate::createColor(ir, ig, ib, 255));
            sdltemplate::drawPoint(i, height - j);
        }
    }
    ofs.close();
    while (sdltemplate::running)
    {
        sdltemplate::loop();
    }
    return 0;
}