#include "window.h"

void Window::Setup ()
{
    SDL_Init (SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute (SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute (SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute (SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute (SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute (SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute (SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_GL_SetAttribute (SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute (SDL_GL_MULTISAMPLESAMPLES, 4);
}

Window::~Window ()
{
}

bool Window::Create (const char* title, int width, int height)
{
    p_window = SDL_CreateWindow ("PhotonCaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                      800, 600, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

    if (p_window == NULL) {
        std::cerr << "Could not create SDL window!\n";
        Destroy ();
        return false;
    }

    glContext = SDL_GL_CreateContext (p_window);

    if (!glContext) {
        std::cerr << "Could not create GL context!\n";
        Destroy ();
        return false;
    }

    GLenum status = glewInit ();

    if (status != GLEW_OK) {
        std::cerr << "Could not initialize GLEW!\n";
        Destroy ();
        return false;
    }

    glEnable (GL_DEPTH_TEST);
    glEnable (GL_DEPTH_CLAMP);
    glEnable (GL_MULTISAMPLE);

    glEnable (GL_CULL_FACE);
    glCullFace (GL_BACK);
    glFrontFace (GL_CCW);

    this->width = width;
    this->height = height;

    return true;
}

void Window::Destroy ()
{
    // Clean up
    SDL_GL_DeleteContext (glContext);

    if (p_window != NULL)
        SDL_DestroyWindow (p_window);
    p_window = NULL;
}
