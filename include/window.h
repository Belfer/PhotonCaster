#ifndef WINDOW_H
#define WINDOW_H

#include <assert.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>

class Window
{
public:
    Window () : width (0), height (0)
    {
        Setup ();
    }
    virtual ~Window ();

private:
    void Setup ();

public:

    bool Create (const char* title, int width, int height);

    void Destroy ();

    inline void Clear () { glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    inline void SwapBuffers () { SDL_GL_SwapWindow (p_window); }

    inline void SetClearColor (GLclampf r, GLclampf g, GLclampf b) { glClearColor (r, g, b, 1.f); }

    inline size_t GetWidth () const { return width; }
    inline size_t GetHeight () const { return height; }

private:
    SDL_Window* p_window;
    SDL_GLContext glContext;
    size_t width, height;
};

#endif // WINDOW_H
