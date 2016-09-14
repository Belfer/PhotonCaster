#include <iostream>
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

#define WIN_WIDTH  800
#define WIN_HEIGHT 600

using namespace std;
using namespace glm;
using namespace graphics;

Window* p_window;
Renderer* p_renderer;
RaycastRenderer* p_raycastRenderer;
RasterRenderer* p_rasterRenderer;

Camera camera;
Scene scene;

Shader* shader;

static const vec3 UP = vec3 (0,1,0);
static const vec3 FORWARD = vec3 (0,0,1);
static const vec3 RIGHT = vec3 (1,0,0);

bool init ()
{
    p_window = new Window ();
    if (!p_window->Create ("PhotonCaster", WIN_WIDTH, WIN_HEIGHT))
        return false;

    string glslVersion = string ((char*)glGetString (GL_SHADING_LANGUAGE_VERSION));
    glslVersion.erase (std::find (glslVersion.begin(), glslVersion.end (), '.'));
    if (glslVersion != "120" && glslVersion != "130") {
        glslVersion = "120";
    }

    std::cout << "GLSL version: " << glslVersion << "\n";

    p_raycastRenderer = new RaycastRenderer ();
    p_rasterRenderer = new RasterRenderer ();

    //p_renderer = p_rasterRenderer;
    p_renderer = p_raycastRenderer;

    //camera.position = -FORWARD * 2.f;
    camera.orientation = quat ();
    camera.projection = glm::perspective (70.f, (float) WIN_WIDTH/WIN_HEIGHT, 0.1f, 1000.f);
    camera.view = glm::lookAt (camera.position, camera.position + camera.orientation * FORWARD, UP);

    scene.ambient_color = vec4 (0.1f, 0.1f, 0.1f, 1.f);
    scene.p_active_camera = &camera;

    scene.lights[0].color = vec4 (0.9f, 0.9f, 0.9f, 1.f);
    scene.lights[0].position = vec4 (0.f, 0.f, 1.f, 0.f);
    scene.lights[0].type = LightType::DIRECTIONAL;
    scene.num_lights++;

    shader = new Shader ();
    shader->AddShader (GL_VERTEX_SHADER, util::load_file ("../shaders/light.vert"+glslVersion+".glsl"));
    shader->AddShader (GL_FRAGMENT_SHADER, util::load_file ("../shaders/light.frag"+glslVersion+".glsl"));
    shader->Compile ();

    for (uint i=0; i<8; ++i) {
        Triangle* tri = new Triangle ();
        tri->p1 = vec3 (-1,-1, 0);
        tri->p2 = vec3 ( 1,-1, 0);
        tri->p3 = vec3 ( 0, 1, 0);
        tri->transform.rotation = glm::angleAxis (i*45.f, UP);
        tri->transform.position = tri->transform.rotation * FORWARD * 2.f;

        util::gl_triangle (tri->gldata, tri->p1, tri->p2, tri->p3);
        tri->p_shader = shader;
        tri->material.diffuse_color = vec4 (0.7f, 0.7f, 0.7f, 1.f);
        tri->material.specular_color = vec4 (1.f, 1.f, 1.f, 1.f);
        tri->material.emission_color = vec4 (0.f, 0.f, 0.f, 1.f);
        tri->material.shininess = 10000.f;

        scene.models.emplace_back (tri);
    }

    return true;
}

void render ()
{
    SDL_Event e;

    vec2 move;
    float xangle = 0, yangle = 0;
    float counter = 0;

    bool isRunning = true;
    while (isRunning) {
        while (SDL_PollEvent (&e) != 0) {
            if (e.type == SDL_QUIT || e.type == SDL_APP_TERMINATING) {
                isRunning = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.scancode == SDL_SCANCODE_W) {
                    move.x = 1;
                } else if (e.key.keysym.scancode == SDL_SCANCODE_S) {
                    move.x =-1;
                }

                if (e.key.keysym.scancode == SDL_SCANCODE_A) {
                    move.y = 1;
                } else if (e.key.keysym.scancode == SDL_SCANCODE_D) {
                    move.y =-1;
                }
            }
            else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.scancode == SDL_SCANCODE_W) {
                    move.x = 0;
                } else if (e.key.keysym.scancode == SDL_SCANCODE_S) {
                    move.x = 0;
                }

                if (e.key.keysym.scancode == SDL_SCANCODE_A) {
                    move.y = 0;
                } else if (e.key.keysym.scancode == SDL_SCANCODE_D) {
                    move.y = 0;
                }
            }
            else if (e.type == SDL_MOUSEMOTION) {
                xangle -= e.motion.xrel * 1.f;
                yangle += e.motion.yrel * 1.f;

                yangle = yangle > 89 ? 89 : yangle;
                yangle = yangle <-89 ?-89 : yangle;

                quat xrot = glm::angleAxis (yangle, RIGHT);
                quat yrot = glm::angleAxis (xangle, UP);
                camera.orientation = yrot * xrot;
            }
        }

        if (move.x != 0 || move.y != 0) {
            camera.position += camera.orientation * FORWARD * move.x * 0.1f;
            camera.position += camera.orientation * RIGHT * move.y * 0.1f;
        }

        counter += 0.16f;
        vec3 tmp = glm::angleAxis (counter, UP) * FORWARD;
        scene.lights[0].position = vec4 (tmp.x, tmp.y, tmp.z, 0.f);

        camera.projection = glm::perspective (70.f, (float) WIN_WIDTH/WIN_HEIGHT, 0.1f, 1000.f);
        camera.view = glm::lookAt (camera.position, camera.position + camera.orientation * FORWARD, UP);

        p_window->SetClearColor (0x00, 0x00, 0x00);
        p_window->Clear ();

        //p_renderer->render (scene);
        //p_raycastRenderer->render (scene);
        p_rasterRenderer->render (scene);

        p_window->SwapBuffers ();
    }
}

void shutdown ()
{
    p_window->Destroy ();

    delete p_window;
    delete p_raycastRenderer;
    delete p_rasterRenderer;
    delete shader;

    p_window = nullptr;
    p_raycastRenderer = nullptr;
    p_rasterRenderer = nullptr;
    shader = nullptr;
}

int main (int argc, char ** args)
{
    if (init ())
        render ();
    shutdown ();

	return 0;
}
