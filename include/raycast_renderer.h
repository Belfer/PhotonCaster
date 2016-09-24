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

        virtual void render (const Scene& scene);

    private:
        void Setup ();

        Shader shader;
        Texture texture;
        OpenGLMeshData screen;
    };

}

#endif // RAYCASTRENDERER_H
