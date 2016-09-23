#ifndef INPUT_H
#define INPUT_H

#include <window.h>
#include <glfw3.h>

class Input
{
public:
    Input (const Window& window);

    inline void PollEvents () const { glfwPollEvents (); }
};

#endif // INPUT_H
