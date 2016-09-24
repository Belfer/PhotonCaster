#include "input.h"

void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_button_callback (GLFWwindow* window, int button, int action, int mods);

void cursor_pos_callback (GLFWwindow* window, double xpos, double ypos);

void scroll_callback (GLFWwindow* window, double xoffset, double yoffset);

void Input::Initialize (const Window& window)
{
    //glfwSetInputMode (window.GetWindow (), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //glfwSetInputMode (window.GetWindow (), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    //glfwSetInputMode (window.GetWindow (), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetInputMode (window.GetWindow (), GLFW_STICKY_KEYS, 1);
    //glfwSetInputMode (window.GetWindow (), GLFW_STICKY_MOUSE_BUTTONS, 1);

    glfwSetKeyCallback (window.GetWindow (), key_callback);
    glfwSetMouseButtonCallback (window.GetWindow (), mouse_button_callback);
    glfwSetCursorPosCallback (window.GetWindow (), cursor_pos_callback);
}

void key_callback (GLFWwindow* p_window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose (p_window, 1);
    } else {
        if (action == GLFW_PRESS) {
            Input::Instance ().input_state.keys_state[key] = true;
        } else if (action == GLFW_RELEASE) {
            Input::Instance ().input_state.keys_state[key] = false;
        }
    }
}

void mouse_button_callback (GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        Input::Instance ().input_state.mbouttons_state[button] = true;
    } else if (action == GLFW_RELEASE) {
        Input::Instance ().input_state.mbouttons_state[button] = false;
    }
}

void cursor_pos_callback (GLFWwindow* window, double xpos, double ypos)
{
    Input::Instance ().input_state.mouse_pos.x = (float)xpos;
    Input::Instance ().input_state.mouse_pos.y = (float)ypos;
}

void scroll_callback (GLFWwindow* window, double xoffset, double yoffset)
{
}
