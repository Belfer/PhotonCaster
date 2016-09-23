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

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

using namespace std;
using namespace glm;
using namespace graphics;

static Window* p_window;
static Input* p_input;
static Renderer* p_renderer;
static RaycastRenderer* p_raycastRenderer;
static RasterRenderer* p_rasterRenderer;

static util::Controller* p_controller;
static util::CrystalBall* p_crystalball;
static util::FreeLook* p_freelook;

static SDL_Event e;

static vec2 mouse;
static vec2 trans;
static vec2 rot;

static bool isRunning;
static bool raytracing;
static bool raytraced;
static bool allowControl;

static Camera camera;
static Scene scene;

static Shader shader;

bool init ()
{
    p_window = new Window ();
    if (!p_window->Create ("PhotonCaster", WIN_WIDTH, WIN_HEIGHT))
        return false;
    p_input = new Input (*p_window);

    const string glslVersion = util::glsl_version ();
    std::cout << "GLSL version: " << glslVersion << "\n";

    p_raycastRenderer = new RaycastRenderer (p_window);
    p_rasterRenderer = new RasterRenderer (p_window);

    p_renderer = p_rasterRenderer;

    p_crystalball = new util::CrystalBall (30);
    p_freelook = new util::FreeLook (1);

    p_controller = p_freelook;

    camera.position = vec3 (4.538304f, 1.389559f, 7.531025f);
    camera.orientation = quat (-0.324587f, -0.0252605f, 0.942668f, -0.0733619f);
    camera.Perspective (70.f, (float) WIN_WIDTH/WIN_HEIGHT, 0.1f, 1000.f);
    camera.Update ();

    scene.ambient_color = vec4 (0.05f, 0.05f, 0.05f, 1.f);
    scene.p_active_camera = &camera;

    scene.lights[0].color = vec4 (0.9f, 0.9f, 0.9f, 1.f);
    scene.lights[0].position = vec4 (0.f, 1.f, 0.f, 0.f);
    scene.lights[0].type = LightType::DIRECTIONAL;
    scene.num_lights++;
    scene.lights[1].color = vec4 (0.1f, 0.3f, 0.9f, 1.f);
    scene.lights[1].position = vec4 (0.f, 0.f, 5.f, 1.f);
    scene.lights[1].type = LightType::POINT;
    scene.num_lights++;

    shader.CreateProgram ();
    shader.AddShader (GL_VERTEX_SHADER, util::load_file ("../shaders/light.vert"+glslVersion+".glsl"));
    shader.AddShader (GL_FRAGMENT_SHADER, util::load_file ("../shaders/light.frag"+glslVersion+".glsl"));
    shader.Compile ();

    Triangle* tri = new Triangle ();
    tri->p1 = vec3 (-1,-1, 0);
    tri->p2 = vec3 ( 1,-1, 0);
    tri->p3 = vec3 ( 0, 1, 0);

    util::gl::triangle (tri->gldata, tri->p1, tri->p2, tri->p3);
    tri->p_shader = &shader;
    tri->material.diffuse_color = vec4 (0.7f, 0.7f, 0.7f, 1.f);
    tri->material.specular_color = vec4 (1.f, 1.f, 1.f, 1.f);
    tri->material.emission_color = vec4 (0.f, 0.f, 0.f, 1.f);
    tri->material.shininess = 10000.f;

    scene.models.emplace_back (tri);

    return true;
}

void input ()
{
}

void update ()
{
    p_controller->Update (camera, 0.1f, trans, mouse);
    camera.Update ();
}

void render ()
{
    p_window->SetClearColor (0x00, 0x00, 0x00);

    if (raytracing) {
        if (!raytraced || allowControl) {
            p_window->Clear ();
            p_renderer->render (scene);
            p_window->SwapBuffers ();
            raytraced = true;
        }

    } else {

        p_window->Clear ();
        p_renderer->render (scene);
        p_window->SwapBuffers ();
    }
}

void loop ()
{
    double t = 0.0;
    double dt = 0.016;

    double currentTime = glfwGetTime ();
    double accumulator = 0.0;

    isRunning = true;
    raytracing = false;
    raytraced = false;
    allowControl = true;

    p_window->SetClearColor (0, 0, 0);

    do {
        double newTime = glfwGetTime();
        double frameTime = newTime - currentTime;
        if (frameTime > 0.25) {
            frameTime = 0.25;
        }
        currentTime = newTime;
        accumulator += frameTime;

        p_input->PollEvents ();
        input ();

        while (accumulator >= dt) {
            if (!raytracing || allowControl) {
                update ();
            }

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
    delete p_input;
    delete p_raycastRenderer;
    delete p_rasterRenderer;

    delete p_crystalball;
    delete p_freelook;

    p_window = nullptr;
    p_input = nullptr;
    p_raycastRenderer = nullptr;
    p_rasterRenderer = nullptr;

    p_crystalball = nullptr;
    p_freelook = nullptr;
}

int main (int argc, char ** args)
{
    if (init ())
        loop ();
    shutdown ();

	return 0;
}
