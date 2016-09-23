#include "input.h"

void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);

Input::Input (const Window& window)
{
    glfwSetKeyCallback (window.GetWindow (), key_callback);
}

void key_callback (GLFWwindow* p_window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose (p_window, 1);
    }
}
