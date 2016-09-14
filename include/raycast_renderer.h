#ifndef RAYCASTRENDERER_H
#define RAYCASTRENDERER_H

#include "renderer.h"
#include "camera.h"

namespace graphics
{

    struct Ray
    {
    };

    struct Intersection
    {
    };

    class RaycastRenderer : public Renderer
    {
    public:
        void render (const Scene& scene);

    private:
        void RayTrace (const Scene& scene, const uint& width, const uint& height, int* buffer);

        Ray RayThruPixel (const Camera& camera, const uint& x, const uint& y);

        Intersection Intersect (const Ray& ray, const Scene& scene);

        int FindColor (const Intersection& intersection);
    };

}

#endif // RAYCASTRENDERER_H
