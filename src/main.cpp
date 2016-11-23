#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include "window.h"
#include "input.h"
#include "raycast_renderer.h"
#include "raster_renderer.h"
#include "loader.h"

#include "scene.h"
#include "camera.h"
#include "transform.h"
#include "triangle.h"
#include "sphere.h"
#include "mesh.h"
#include "primitives.h"
#include "controller.h"

#define WIN_WIDTH  640
#define WIN_HEIGHT 480

using namespace std;
using namespace glm;
using namespace graphics;

static Window* p_window;
static Renderer* p_renderer;
static RaycastRenderer* p_raycastRenderer;
static RasterRenderer* p_rasterRenderer;

static bool isRunning;
static Scene scene;

static bool allowControl;
static util::FreeLook* p_controller;
static vec2 mouse;
static vec2 trans;

bool init ()
{
    p_window = new Window ();
    if (!p_window->Create ("PhotonCaster", WIN_WIDTH, WIN_HEIGHT))
        return false;
    Input::Instance ().Initialize (*p_window);

    const string glslVersion = util::glsl_version ();
    std::cout << "GLSL version: " << glslVersion << "\n";

    p_raycastRenderer = new RaycastRenderer (p_window);
    p_rasterRenderer = new RasterRenderer (p_window);

    p_renderer = p_rasterRenderer;

    p_controller = new util::FreeLook (1);

    //scene.bgcolor = util::rgb (142, 200, 235);
    util::load_scene ("../scenes/scene5.test", scene);
    //scene.print ();

    return true;
}

void input ()
{
    trans.x = trans.y = 0;
    if (Input::get_key (GLFW_KEY_W)) {
        trans.x = 1;
    }
    if (Input::get_key (GLFW_KEY_S)) {
        trans.x =-1;
    }
    if (Input::get_key (GLFW_KEY_A)) {
        trans.y =-1;
    }
    if (Input::get_key (GLFW_KEY_D)) {
        trans.y = 1;
    }

    if (Input::get_key_down (GLFW_KEY_Q)) {
        allowControl = !allowControl;
    }
    if (Input::get_key_down (GLFW_KEY_P)) {
        scene.p_active_camera->print ();
    }
    if (Input::get_key_down (GLFW_KEY_E)) {
        p_renderer = p_rasterRenderer;
    }
    if (Input::get_key_down (GLFW_KEY_R)) {
        p_renderer = p_raycastRenderer;
    }

    vec2 mouse_dt = Input::get_mouse_dt ();
    mouse.x -= mouse_dt.x * 1.f;
    mouse.y -= mouse_dt.y * 1.f;

    mouse.y = mouse.y > 89.9f ? 89.9f : mouse.y <-89.9f ?-89.9f : mouse.y;
}

void update (const float dt)
{
    if (allowControl) {
        p_controller->Update (*scene.p_active_camera, 0.1f, trans, mouse);
    }

    scene.p_active_camera->Update ();

    for (auto model : scene.models) {
        model->Update (dt);
    }
}

void render ()
{
    p_window->SetClearColor (scene.bgcolor.x, scene.bgcolor.y, scene.bgcolor.z);
    p_window->Clear ();
    p_renderer->render (scene);
    p_window->SwapBuffers ();
}

void loop ()
{
    double t = 0.0;
    double dt = 0.016;

    double currentTime = glfwGetTime ();
    double accumulator = 0.0;

    isRunning = true;
    allowControl = false;

    p_window->SetClearColor (0, 0, 0);

    do {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        if (frameTime > 0.25) {
            frameTime = 0.25;
        }
        currentTime = newTime;
        accumulator += frameTime;

        while (accumulator >= dt) {
            Input::Instance ().PollEvents ();
            input ();
            update ((float)dt);

            t += dt;
            accumulator -= dt;
        }

        float ratio;
        int width, height;

        p_window->GetFramebufferSize (width, height);
        ratio = width / (float) height;
        glViewport (0, 0, width, height);

        p_window->Clear ();
        render ();
        p_window->SwapBuffers ();

    } while (!p_window->ShouldClose () && isRunning);
}

void shutdown ()
{
    p_window->Destroy ();

    delete p_window;
    delete p_raycastRenderer;
    delete p_rasterRenderer;

    p_window = nullptr;
    p_raycastRenderer = nullptr;
    p_rasterRenderer = nullptr;
}

int main (int argc, char ** args)
{
    if (init ())
        loop ();
    shutdown ();

    return 0;
}
