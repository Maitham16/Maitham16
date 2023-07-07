// main function
#include <iostream>
#include <fstream>

#include "Classes/stdtemplate.h"
#include "Classes/Vec3.h"

int main()
{
    int width = 800;
    int height = 600;
    std::ofstream ofs;
    ofs.open("./output.ppm");
    std::cout << "P3\n"
              << width << " " << height << "\n255\n";
    sdltemplate::sdl("Ray Trace tutorial", width, height);
    sdltemplate::loop();
    ofs << "P3\n" << width << " " << height << "\n255\n";
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            Vec3 color(float(i) / float(width), float(j) / float(height), 0.2);
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
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