#ifndef INPUT_H
#define INPUT_H

#include <window.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;

struct InputState
{
    InputState () : keys_state (), mbouttons_state () {}
    bool keys_state[256];
    bool mbouttons_state[3];
    vec2 mouse_pos;
};

class Input
{
private:
    Input () {}

public:
    Input (Input const&) = delete;
    void operator= (Input const&) = delete;
    static Input& Instance ()
    {
        static Input instance;
        return instance;
    }

    void Initialize (const Window& window);
    inline void PollEvents ()
    {
        prev_input_state = input_state;
        glfwPollEvents ();
    }

    InputState input_state;
    InputState prev_input_state;

    static inline bool get_key (int key) { return Instance ().input_state.keys_state[key]; }
    static inline bool get_key_down (int key)
    {
        return !Instance ().prev_input_state.keys_state[key] && Instance ().input_state.keys_state[key];
    }

    static inline bool get_button (int button) { return Instance ().input_state.mbouttons_state[button]; }
    static inline bool get_button_down (int button)
    {
        return !Instance ().prev_input_state.mbouttons_state[button] && Instance ().input_state.mbouttons_state[button];
    }

    static inline vec2 get_mouse_pos () { return Instance ().input_state.mouse_pos; }
    static inline vec3 get_mouse_pos3 () { return vec3 (Instance ().input_state.mouse_pos, 0.f); }

    static inline vec2 get_mouse_dt ()
    {
        return Instance ().input_state.mouse_pos - Instance ().prev_input_state.mouse_pos;
    }
};

#endif // INPUT_H
