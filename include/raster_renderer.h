#ifndef RASTERRENDERER_H
#define RASTERRENDERER_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "renderer.h"
#include "camera.h"

namespace graphics
{

    class RasterRenderer : public Renderer
    {
    public:
        void render (const Scene& scene);
    };

}

#endif // RASTERRENDERER_H
