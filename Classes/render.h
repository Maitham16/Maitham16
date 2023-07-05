// header for render function
#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>

#include "Vector3.h"
#include "Ray.h"
#include "Sphere.h"
#include "Scene.h"
#include "RayTrace.h"

// Define the number of threads to use
const int numThreads = 16;

void renderRegion(const Scene &scene, const Camera &camera, Vector3 *image, int startX, int startY, int endX, int endY, int width, std::atomic<int> &progress)
{
    int totalLines = endY - startY;
    int renderedLines = 0;
    int updateThreshold = totalLines / 10;

    for (int j = startY; j < endY; ++j)
    {
        for (int i = startX; i < endX; ++i)
        {
            Vector3 colorSum(0.0f, 0.0f, 0.0f);

            // Supersampling over 2x2 grid
            for (int dy = 0; dy < 2; ++dy)
            {
                for (int dx = 0; dx < 2; ++dx)
                {
                    // Compute primary ray direction with offset for supersampling
                    float u = (i + (dx - 0.5f) / 2.0f) / width;
                    float v = (j + (dy - 0.5f) / 2.0f) / camera.imgHeight;
                    Ray ray = camera.generateRay(u, v);

                    // Cast ray and accumulate color
                    colorSum = colorSum + ray_trace(ray, scene, camera);
                }
            }

            // Average color and store in image
            Vector3 colorAvg = colorSum / 4.0f;
            image[j * width + i] = colorAvg;
        }

        renderedLines++;

        if (renderedLines % updateThreshold == 0)
        {
            // Calculate and print the progress percentage of the region
            float progressPercentage = static_cast<float>(renderedLines) / totalLines * 100;
            std::cout << "Thread " << std::this_thread::get_id() << " progress: " << progressPercentage << "%" << std::endl;
        }
    }

    // Update the overall progress
    progress += totalLines;
}

void render(const Scene &scene, const Camera &camera)
{
    const int width = camera.imgWidth;
    const int height = camera.imgHeight;
    Vector3 *image = new Vector3[width * height];

    // Calculate region dimensions
    int regionWidth = width / numThreads;
    int regionHeight = height / numThreads;

    std::vector<std::thread> threads;

    std::atomic<int> progress(0);

    for (int t = 0; t < numThreads; ++t)
    {
        int startX = t * regionWidth;
        int endX = startX + regionWidth;
        int startY = 0;
        int endY = height;

        threads.emplace_back([startX, endX, startY, endY, &scene, &camera, &image, &progress, width]()
                             { renderRegion(scene, camera, image, startX, startY, endX, endY, width, progress); });
    }

    // Wait for all threads to finish before writing to file
    for (auto &thread : threads)
    {
        thread.join();
    }

    // Write to file
    std::ofstream ofs;
    ofs.open("./output.ppm");
    ofs << "P3\n"
        << width << " " << height << "\n255\n";
    for (int j = height - 1; j >= 0; --j)
    {
        for (int i = 0; i < width; ++i)
        {
            int index = j * width + i;
            int ir = int(255.99 * image[index].x);
            int ig = int(255.99 * image[index].y);
            int ib = int(255.99 * image[index].z);
            ofs << ir << " " << ig << " " << ib << "\n";
        }
    }
    ofs.close();

    delete[] image;

    // console output
    std::cout << "Rendering completed!" << std::endl;
}

#endif
