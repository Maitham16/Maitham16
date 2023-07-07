// main function
#include <iostream>
#include <fstream>

#include "Classes/stdtemplate.h"
#include "Classes/Vec3.h"
#include "Classes/Hitable_list.h"
#include "Classes/Camera.h"
#include "Classes/Sphere.h"
#include "Classes/Material.h"

// return background color
Vec3 color(const Ray &ray, Hitable *World, int depth)
{
    Hit_record hit_record;
    if (World->intersect(ray, 0.001, MAXFLOAT, hit_record))
    {
        Ray scattered;
        Vec3 attenuation;
        if (depth < 50 && hit_record.material->scatter(ray, hit_record, attenuation, scattered))
        {
            return attenuation * color(scattered, World, depth + 1);
        }
        else
        {
            return Vec3(0.0, 0.0, 0.0);
        }
    }
    else
    {
        Vec3 unit_direction = unit_vector(ray.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.4, 0.5, 1.0);
    }
}

int main()
{
    int width = 2048;
    int height = 1024;
    int ns = 100;
    std::ofstream ofs;
    ofs.open("./output.ppm");
    std::cout << "P3\n"
              << width << " " << height << "\n255\n";
    sdltemplate::sdl("Ray Trace tutorial", width, height);
    sdltemplate::loop();
    ofs << "P3\n"
        << width << " " << height << "\n255\n";

    // scene
    Vec3 left = Vec3(-2.0, -1.0, -1.0);
    Vec3 horizontal = Vec3(4.0, 0.0, 0.0);
    Vec3 vertical = Vec3(0.0, 2.0, 0.0);
    Vec3 position = Vec3(0.0, 0.0, 0.0);
    float fov = 20.0;
    float aspect = float(width) / float(height);
    Vec3 look_from = Vec3(3.0, 3.0, 2.0);
    Vec3 look_at = Vec3(0.0, 0.0, -1.0);
    Vec3 up = Vec3(0.0, 1.0, 0.0);
    float focus = (look_from - look_at).length();
    float aperture = 2.0;
    Camera camera(look_from, look_at, up, fov, aspect, aperture, focus);

    Hitable *list[5];
    list[0] = new Sphere(Vec3(0.0, 0.0, -1.0), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5)));      // sphere 1
    list[1] = new Sphere(Vec3(0.0, -100.5, -1.0), 100.0, new Lambertian(Vec3(0.8, 0.8, 0.0))); // ground
    list[2] = new Sphere(Vec3(1.0, 0.0, -1.0), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.3));      // sphere 2
    list[3] = new Sphere(Vec3(-1.0, 0.0, -1.0), 0.5, new dialectric(1.5));                     // sphere 3
    list[4] = new Sphere(Vec3(-1.0, 0.0, -1.0), -0.45, new dialectric(1.5));                   // sphere 3

    // Hitable *list[2];
    // float R = cos(M_PI / 4);
    // list[0] = new Sphere(Vec3(-R, 0.0, -1.0), R, new Lambertian(Vec3(0.0, 0.0, 1.0))); // sphere 1
    // list[1] = new Sphere(Vec3(R, 0.0, -1.0), R, new Lambertian(Vec3(1.0, 0.0, 0.0))); // sphere 2

    Hitable *world = new Hitable_list(list, 5);

    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            Vec3 sceneColor(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; s++)
            {
                float u = float(i + drand48()) / float(width);
                float v = float(j + drand48()) / float(height);
                Ray ray = camera.get_ray(u, v);
                sceneColor += color(ray, world, 0);
            }
            sceneColor /= float(ns);
            sceneColor = Vec3(sqrt(sceneColor.r()), sqrt(sceneColor.g()), sqrt(sceneColor.b()));
            int ir = int(255.99 * sceneColor[0]);
            int ig = int(255.99 * sceneColor[1]);
            int ib = int(255.99 * sceneColor[2]);
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