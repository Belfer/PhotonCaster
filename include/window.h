#ifndef WINDOW_H
#define WINDOW_H

#include <assert.h>
#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glfw3.h>

using namespace std;

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
    bool Create (const string& title, int width, int height);

    void Destroy ();

    inline bool ShouldClose () const { return glfwWindowShouldClose (p_window); }

    inline void Clear () { glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT); }

    inline void GetFramebufferSize (int& width, int& height) const { glfwGetFramebufferSize (p_window, &width, &height); }

    inline void SwapBuffers () { glfwSwapBuffers (p_window); }

    inline void SetClearColor (GLclampf r, GLclampf g, GLclampf b) { glClearColor (r, g, b, 1.f); }

    inline uint GetWidth () const { return width; }
    inline uint GetHeight () const { return height; }

    inline GLFWwindow* const GetWindow () const { return p_window; }

private:
    GLFWwindow* p_window;
    uint width, height;
};

#endif // WINDOW_H
