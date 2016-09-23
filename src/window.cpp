#include "window.h"

void error_callback (int error, const char* description)
{
    std::cerr << "GLFW Error: " << description << "\n";
    throw 0;
}

void Window::Setup ()
{
    glfwSetErrorCallback (error_callback);

    if (!glfwInit ()) throw 1;

    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);
}

Window::~Window ()
{
    Destroy ();
}

bool Window::Create (const string& title, int width, int height)
{
    p_window = glfwCreateWindow (width, height, title.c_str (), NULL, NULL);
    if (!p_window)
    {
        std::cerr << "Could not create window.\n";
        Destroy ();
        return false;
    }
    glfwMakeContextCurrent (p_window);

    glewExperimental = GL_TRUE;
    GLenum status = glewInit ();
    if (status != GLEW_OK) {
        std::cerr << "Could not initialize GLEW.\n";
        Destroy ();
        return false;
    }

    glfwSwapInterval (1);
    glfwGetFramebufferSize (p_window, &width, &height);
    glViewport (0, 0, width, height);
    this->width = width;
    this->height = height;

    return true;
}

void Window::Destroy ()
{
    if (p_window) glfwDestroyWindow (p_window);
    p_window = nullptr;

    glfwTerminate ();
}
