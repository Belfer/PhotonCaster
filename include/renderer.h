#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "scene.h"

namespace graphics
{

    class Renderer
    {
    public:
        virtual void render (const Scene& scene) = 0;
    };

}

#endif // RENDERER_H
