#ifndef RAYCASTRENDERER_H
#define RAYCASTRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "renderer.h"
#include "camera.h"
#include "texture.h"
#include "mesh_data.h"
#include "ray.h"

using namespace glm;

namespace graphics
{

    class RaycastRenderer : public Renderer
    {
    public:
        RaycastRenderer (Window* const window) : Renderer (window)
        {
            Setup ();
        }
        virtual ~RaycastRenderer () {}

    private:
        void Setup ();

    public:
        void render (const Scene& scene);

        void RayTrace (const Scene& scene, const uint& width, const uint& height, uint* buffer);

        void RayThruPixel (Ray& ray, const Camera& camera, const uint& x, const uint& y);

        bool Intersect (Intersection& inter, const Ray& ray, const Scene& scene);

        uint FindColor (const Intersection& inter, const Scene& scene);

        Shader shader;
        Texture texture;
        OpenGLMeshData screen;
    };

}

#endif // RAYCASTRENDERER_H
