#include "raycast_renderer.h"
#include <iostream>
#include <iomanip>

namespace graphics
{

    void RaycastRenderer::render (const Scene& scene)
    {
        const uint width = 8;
        const uint height = 6;

        int image[width][height];
        RayTrace (scene, width, height, (int*)image);

        for (int i=0; i<width; ++i) {
            for (int j=0; j<height; ++j) {
                std::cout << hex << image[i][j] << " ";
                //glDrawPixels (1, 1, GL_RGBA, GL_INT, &image[i][j]);
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void RaycastRenderer::RayTrace (const Scene& scene, const uint& width, const uint& height, int* buffer)
    {
        for (int i=0; i<width; ++i) {
            for (int j=0; j<height; ++j) {
                Ray ray = RayThruPixel (*scene.p_active_camera, i, j);
                Intersection hit = Intersect (ray, scene);

                int* col = buffer + i*height + j;
                *col = FindColor (hit);
            }
        }
    }

    Ray RaycastRenderer::RayThruPixel (const Camera& camera, const uint& x, const uint& y)
    {
    }

    Intersection RaycastRenderer::Intersect (const Ray& ray, const Scene& scene)
    {
    }

    int RaycastRenderer::FindColor (const Intersection& intersection)
    {
        return 0xffffffff;
    }

}
