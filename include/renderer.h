#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "scene.h"
#include "window.h"

namespace graphics
{

    enum RenderMode
    {
        POSITION,
        NORMALS,
        DIFFUSE,
        SPECULAR,
        EMISSION,
        LIGHTING
    };

    class Renderer
    {
    public:
        Renderer (Window* const window) : p_window (window), mode (LIGHTING) {}

        virtual void render (const Scene& scene) = 0;

        inline Window& GetWindow () const { return *p_window; }

        inline void SetRenderMode (const RenderMode& mode) { this->mode = mode; }

    protected:
        RenderMode mode;

    private:
        Window* p_window;
    };

}

#endif // RENDERER_H
