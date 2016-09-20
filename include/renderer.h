#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "scene.h"
#include "window.h"

namespace graphics
{

    class Renderer
    {
    public:
        Renderer (Window* const window) : p_window (window) {}

        virtual void render (const Scene& scene) = 0;

        inline Window& GetWindow () const { return *p_window; }

    private:
        Window* p_window;
    };

}

#endif // RENDERER_H
