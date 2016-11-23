#ifndef RAYCASTRENDERER_H
#define RAYCASTRENDERER_H

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <SOIL.h>
#include "renderer.h"
#include "camera.h"
#include "texture.h"
#include "mesh_data.h"
#include "ray.h"
#include "loader.h"
#include "math_util.h"

#define MAX_RECURSION_DEPTH 3

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
