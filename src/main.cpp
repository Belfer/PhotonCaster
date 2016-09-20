#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include "window.h"
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

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

using namespace std;
using namespace glm;
using namespace graphics;

static Window* p_window;
static Renderer* p_renderer;
static RaycastRenderer* p_raycastRenderer;
static RasterRenderer* p_rasterRenderer;

static SDL_Event e;

static vec2 trans;
static float xangle, yangle;
static float counter;
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

    const string glslVersion = util::glsl_version ();
    std::cout << "GLSL version: " << glslVersion << "\n";

    p_raycastRenderer = new RaycastRenderer (p_window);
    p_rasterRenderer = new RasterRenderer (p_window);

    p_renderer = p_rasterRenderer;

    camera.position = UP * 6.f;
    camera.orientation = glm::angleAxis (90.f, RIGHT);
    camera.Perspective (70.f, (float) WIN_WIDTH/WIN_HEIGHT, 0.1f, 1000.f);
    camera.LookForward ();

    scene.ambient_color = vec4 (0.1f, 0.1f, 0.1f, 1.f);
    scene.p_active_camera = &camera;

    scene.lights[0].color = vec4 (0.9f, 0.9f, 0.9f, 1.f);
    scene.lights[0].position = vec4 (0.f, 0.f, 1.f, 0.f);
    scene.lights[0].type = LightType::DIRECTIONAL;
    scene.num_lights++;

    shader.CreateProgram ();
    shader.AddShader (GL_VERTEX_SHADER, util::load_file ("../shaders/light.vert"+glslVersion+".glsl"));
    shader.AddShader (GL_FRAGMENT_SHADER, util::load_file ("../shaders/light.frag"+glslVersion+".glsl"));
    shader.Compile ();

    Sphere* shpere = new Sphere ();
    shpere->radius = 0.5f;

    util::gl::icoshpere (shpere->gldata, shpere->radius, 0);
    shpere->p_shader = &shader;
    shpere->material.diffuse_color = vec4 (0.7f, 0.1f, 0.1f, 1.f);
    shpere->material.specular_color = vec4 (1.f, 1.f, 1.f, 1.f);
    shpere->material.emission_color = vec4 (0.f, 0.f, 0.f, 1.f);
    shpere->material.shininess = 100.f;

    scene.models.emplace_back (shpere);

    /*for (uint i=0; i<8; ++i) {
        Triangle* tri = new Triangle ();
        tri->p1 = vec3 (-1,-1, 0);
        tri->p2 = vec3 ( 1,-1, 0);
        tri->p3 = vec3 ( 0, 1, 0);
        tri->transform.rotation = glm::angleAxis (i*45.f, UP);
        tri->transform.position = tri->transform.rotation * FORWARD * 3.f;

        util::gl::triangle (tri->gldata, tri->p1, tri->p2, tri->p3);
        tri->p_shader = &shader;
        tri->material.diffuse_color = vec4 (0.7f, 0.7f, 0.7f, 1.f);
        tri->material.specular_color = vec4 (1.f, 1.f, 1.f, 1.f);
        tri->material.emission_color = vec4 (0.f, 0.f, 0.f, 1.f);
        tri->material.shininess = 10000.f;

        scene.models.emplace_back (tri);
    }*/

    return true;
}

void input ()
{
    while (SDL_PollEvent (&e) != 0) {
        if (e.type == SDL_QUIT || e.type == SDL_APP_TERMINATING) {
            isRunning = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.scancode == SDL_SCANCODE_W) {
                trans.x = 1;
            } else if (e.key.keysym.scancode == SDL_SCANCODE_S) {
                trans.x =-1;
            }

            if (e.key.keysym.scancode == SDL_SCANCODE_A) {
                trans.y = 1;
            } else if (e.key.keysym.scancode == SDL_SCANCODE_D) {
                trans.y =-1;
            }

            if (e.key.keysym.scancode == SDL_SCANCODE_E) {
                p_renderer = p_rasterRenderer;
                raytracing = false;
                raytraced = false;
            }

            if (e.key.keysym.scancode == SDL_SCANCODE_R) {
                p_renderer = p_raycastRenderer;
                raytracing = true;
            }

            if (e.key.keysym.scancode == SDL_SCANCODE_F) {
                glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
            }

            if (e.key.keysym.scancode == SDL_SCANCODE_G) {
                glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
            }
        }
        else if (e.type == SDL_KEYUP) {
            if (e.key.keysym.scancode == SDL_SCANCODE_W) {
                trans.x = 0;
            } else if (e.key.keysym.scancode == SDL_SCANCODE_S) {
                trans.x = 0;
            }

            if (e.key.keysym.scancode == SDL_SCANCODE_A) {
                trans.y = 0;
            } else if (e.key.keysym.scancode == SDL_SCANCODE_D) {
                trans.y = 0;
            }
        }
        else if (e.type == SDL_MOUSEMOTION) {
            xangle -= e.motion.xrel * 1.f;
            yangle += e.motion.yrel * 1.f;

            yangle = yangle > 89.9f ? 89.9f : yangle <-89.9f ?-89.9f : yangle;

            quat xrot = glm::angleAxis (yangle, RIGHT);
            quat yrot = glm::angleAxis (xangle, UP);
            camera.orientation = yrot * xrot;
        }
    }
}

void update ()
{
    if (trans.x != 0 || trans.y != 0) {
        camera.position += camera.orientation * FORWARD * trans.x * 0.1f;
        camera.position += camera.orientation * RIGHT * trans.y * 0.1f;
    }

    counter += 0.16f;
    vec3 tmp = glm::angleAxis (counter, UP) * FORWARD;
    scene.lights[0].position = vec4 (tmp.x, tmp.y, tmp.z, 0.f);

    camera.Perspective (70.f, (float) WIN_WIDTH/WIN_HEIGHT, 0.1f, 1000.f);
    camera.LookForward ();
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
    xangle = 0;
    yangle = 0;
    counter = 0;
    isRunning = true;
    raytracing = false;
    raytraced = false;
    allowControl = true;

    while (isRunning) {
        input ();
        if (!raytracing || allowControl)
            update ();
        render ();
    }
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
