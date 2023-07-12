// main funnction for the book ray tracing next weekend
#include <iostream>
#include <fstream>
#include <float.h>
#include <random>
#include <time.h>
#include <omp.h>
#include <thread>

#include "Classes/Vec3.h"
#include "Classes/Ray.h"
#include "Classes/Hittable.h"
#include "Classes/Hittable_list.h"
#include "Classes/Sphere.h"
#include "Classes/Camera.h"
#include "Classes/Material.h"
#include "Classes/Moving_sphere.h"
#include "Classes/Texture.h"
#include "Classes/aarect.h"
#include "Classes/Box.h"

using namespace std;

// return tracing color
Vec3 color(const Ray &ray, const Vec3 &background, Hittable *world, int depth)
{
    Hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return Vec3(0, 0, 0);

    // If the ray hits nothing, return the background color.
    if (!world->intersect(ray, 0.001, INFINITY, rec))
        return background;

    Ray scattered;
    Vec3 attenuation;
    Vec3 emitted = rec.material->emitted(rec.u, rec.v, rec.p);

    if (!rec.material->scatter(ray, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * color(scattered, background, world, depth - 1);
}

Hittable *random_scene()
{
    int n = 500;
    Hittable **list = new Hittable *[n + 1];
    // texture black and white checker board
    Texture *checker = new checker_texture(new solid_color(Vec3(0.0, 0.0, 0.0)), new solid_color(Vec3(1.0, 1.0, 1.0)));
    list[0] = new Sphere(Vec3(0.0, -1000.0, 0.0), 1000.0, new Lambertian(checker));
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_material = drand48();
            Vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - Vec3(4.0, 0.2, 0.0)).length() > 0.9)
            {
                if (choose_material < 0.8)
                { // diffuse
                    // moving sphere
                    Vec3 center0 = center;
                    Vec3 center1 = center + Vec3(0.0, 0.5 * drand48(), 0.0);

                    list[i++] = new Moving_sphere(center0, center1, 0.0, 1.0, 0.2, new Lambertian(Vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
                }
                else if (choose_material < 0.95)
                { // metal
                    list[i++] = new Sphere(center, 0.2, new Metal(Vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
                }
                else
                { // glass
                    list[i++] = new Sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new Sphere(Vec3(0.0, 1.0, 0.0), 1.0, new dielectric(1.5));
    list[i++] = new Sphere(Vec3(-4.0, 1.0, 0.0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(Vec3(4.0, 1.0, 0.0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
    return new Hittable_list(list, i);
}

Hittable *two_spheres()
{
    Texture *checker = new checker_texture(new solid_color(Vec3(0.2, 0.3, 0.1)), new solid_color(Vec3(0.9, 0.9, 0.9)));
    int n = 50;
    Hittable **list = new Hittable *[n + 1];
    list[0] = new Sphere(Vec3(0.0, -10.0, 0.0), 10.0, new Lambertian(checker));
    list[1] = new Sphere(Vec3(0.0, 10.0, 0.0), 10.0, new Lambertian(checker));

    return new Hittable_list(list, 2);
};

Hittable *two_perlin_spheres()
{
    Texture *pertext = new noise_texture(4);
    Hittable **list = new Hittable *[2];
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(pertext));
    list[1] = new Sphere(Vec3(0, 2, 0), 2, new Lambertian(pertext));
    return new Hittable_list(list, 2);
}

Hittable *earth()
{
    const char *filename = "./earthmap.jpg"; // assuming earthmap.jpg is in the same directory as the executable
    Material *mat = new Lambertian(new image_texture(filename));
    return new Sphere(Vec3(0, 0, 0), 2, mat);
}

Hittable *simple_light()
{
    Texture *pertext = new noise_texture(4);
    Hittable **list = new Hittable *[4];
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(pertext));
    list[1] = new Sphere(Vec3(0, 2, 0), 2, new Lambertian(pertext));
    list[2] = new Sphere(Vec3(0, 7, 0), 2, new Diffuse_light(new solid_color(Vec3(4, 4, 4))));
    list[3] = new xy_rect(3, 5, 1, 3, -2, new Diffuse_light(new solid_color(Vec3(4, 4, 4))));
    return new Hittable_list(list, 4);
}

// Cornell Box
Hittable *cornell_box()
{
    Hittable **list = new Hittable *[8];
    Material *red = new Lambertian(new solid_color(Vec3(0.65, 0.05, 0.05)));
    Material *white = new Lambertian(new solid_color(Vec3(0.73, 0.73, 0.73)));
    Material *green = new Lambertian(new solid_color(Vec3(0.12, 0.45, 0.15)));
    Material *light = new Diffuse_light(new solid_color(Vec3(15, 15, 15)));

    // walls xy, xz, yz
    list[0] = new yz_rect(0, 555, 0, 555, 555, green);
    list[1] = new yz_rect(0, 555, 0, 555, 0, red);
    list[2] = new xz_rect(213, 343, 227, 332, 554, light);
    list[3] = new xz_rect(0, 555, 0, 555, 0, white);
    list[4] = new xz_rect(0, 555, 0, 555, 555, white);
    list[5] = new xy_rect(0, 555, 0, 555, 555, white);

    // boxes non moving
    list[6] = new box(Vec3(130, 0, 65), Vec3(295, 165, 230), white);
    list[7] = new box(Vec3(265, 0, 295), Vec3(430, 330, 460), white);

    return new Hittable_list(list, 8);
}

// random scene
int main()
{
    int width = 1200;
    int height = 800;
    int ns = 100;
    int max_depth = 50;
    Vec3 background(0.0, 0.0, 0.0);

    std::ofstream ofs;
    ofs.open("./output.ppm");
    std::cout << "P3\n"
              << width << " " << height << "\n255\n";
    ofs << "P3\n"
        << width << " " << height << "\n255\n";

    // scene
    Hittable *scene;

    Vec3 left = Vec3(-2.0, -1.0, -1.0);
    Vec3 horizontal = Vec3(4.0, 0.0, 0.0);
    Vec3 vertical = Vec3(0.0, 2.0, 0.0);
    Vec3 position = Vec3(0.0, 0.0, 0.0);
    float fov = 20.0;
    float aspect = float(width) / float(height);
    Vec3 look_from = Vec3(13.0, 2.0, 3.0);
    Vec3 look_at = Vec3(0.0, 0.0, 0.0);
    Vec3 up = Vec3(0.0, 1.0, 0.0);
    float focus = 10.0;
    float aperture = 0.1;

    switch (6)
    {
    case 1:
        scene = random_scene();
        background = Vec3(0.70, 0.80, 1.00);
        look_from = Vec3(13.0, 2.0, 3.0);
        look_at = Vec3(0.0, 0.0, 0.0);
        fov = 20.0;
        aperture = 0.1;
        break;

    case 2:
        scene = two_spheres();
        background = Vec3(0.70, 0.80, 1.00);
        look_from = Vec3(13.0, 2.0, 3.0);
        look_at = Vec3(0.0, 0.0, 0.0);
        fov = 20.0;
        break;

    case 3:
        scene = two_perlin_spheres();
        background = Vec3(0.70, 0.80, 1.00);
        look_from = Vec3(13.0, 2.0, 3.0);
        look_at = Vec3(0.0, 0.0, 0.0);
        fov = 20.0;
        break;

    case 4:
        scene = earth();
        background = Vec3(0.70, 0.80, 1.00);
        look_from = Vec3(13.0, 2.0, 3.0);
        look_at = Vec3(0.0, 0.0, 0.0);
        fov = 20.0;
        break;

    case 5:
        scene = simple_light();
        ns = 400;
        look_from = Vec3(26.0, 3.0, 6.0);
        look_at = Vec3(0.0, 2.0, 0.0);
        fov = 20.0;
        break;

    case 6:
        scene = cornell_box();
        ns = 200;
        look_from = Vec3(278.0, 278.0, -800.0);
        look_at = Vec3(278.0, 278.0, 0.0);
        fov = 40.0;
        break;
    }

    Camera camera(look_from, look_at, up, fov, aspect, aperture, focus, 0.0, 1.0);

    // Number of threads
    const int num_threads = std::thread::hardware_concurrency();

    // Calculate the height of each chunk
    int chunk_height = height / num_threads;

    // Create a vector to hold the threads
    std::vector<std::thread> threads;

    // Create a buffer to store the color data
    std::vector<std::vector<Vec3>> buffer(height, std::vector<Vec3>(width));

    // Create and launch the threads
    for (int t = 0; t < num_threads; ++t)
    {
        threads.push_back(std::thread([&, t]()
                                      {
        // Calculate the start and end y values for this chunk
        int y_start = t * chunk_height;
        int y_end = (t == num_threads - 1) ? height : y_start + chunk_height;

        // Render this chunk
        for (int j = y_start; j < y_end; ++j) {
            for (int i = 0; i < width; ++i) {
                Vec3 sceneColor(0.0, 0.0, 0.0);
                for (int s = 0; s < ns; s++) {
                    float u = float(i + drand48()) / float(width);
                    float v = float(j + drand48()) / float(height);
                    Ray ray = camera.get_ray(u, v);
                    sceneColor += color(ray, background, scene, max_depth);
                }
                sceneColor /= float(ns);
                sceneColor = Vec3(sqrt(sceneColor.r()), sqrt(sceneColor.g()), sqrt(sceneColor.b()));

                // console output progress bar (for debugging) % done
                std::cout << "\r"
                          << int((float(j - y_start) / float(y_end - y_start)) * 100.0) << "%"
                          << std::flush;

                // store the color data in the buffers
                buffer[j][i] = sceneColor;
            }
        } }));
    }

    // Wait for all threads to finish
    for (auto &thread : threads)
    {
        thread.join();
    }

    // Output the color data from the buffer
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            Vec3 &sceneColor = buffer[j][i];
            int ir = int(255.99 * sceneColor[0]);
            int ig = int(255.99 * sceneColor[1]);
            int ib = int(255.99 * sceneColor[2]);
            std::cout << ir << " " << ig << " " << ib << "\n";
            ofs << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}