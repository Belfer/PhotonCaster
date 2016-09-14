#include <iostream>
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

using namespace graphics;

Window* p_window;
Renderer* p_renderer;
RaycastRenderer* p_raycastRenderer;
RasterRenderer* p_rasterRenderer;

Camera camera;
Scene scene;

Shader* shader;
Mesh* mesh;
Triangle* tri;
Sphere* sphere;

static const glm::vec3 UP = glm::vec3 (0,1,0);
static const glm::vec3 FORWARD = glm::vec3 (0,0,1);
static const glm::vec3 RIGHT = glm::vec3 (1,0,0);

bool init ()
{
    p_window = new Window ();
    if (!p_window->Create ("PhotonCaster", WIN_WIDTH, WIN_HEIGHT))
        return false;

    p_raycastRenderer = new RaycastRenderer ();
    p_rasterRenderer = new RasterRenderer ();

    //p_renderer = p_rasterRenderer;
    p_renderer = p_raycastRenderer;

    camera.position = glm::vec3 (0, 0,-1);
    camera.orientation = glm::quat ();
    camera.projection = glm::perspective (70.f, (float) WIN_WIDTH/WIN_HEIGHT, 0.1f, 1000.f);
    camera.view = glm::lookAt (camera.position, camera.position + camera.orientation * FORWARD, UP);

    scene.ambient_color = glm::vec4 (0.1f, 0.1f, 0.1f, 1.f);
    scene.p_active_camera = &camera;

    scene.lights[scene.num_lights].color = glm::vec4 (0.9f, 0.9f, 0.9f, 1.f);
    scene.lights[scene.num_lights].position = glm::vec4 (9.f, 0.f, 1.f, 0.f);
    scene.lights[scene.num_lights].type = LightType::DIRECTIONAL;
    scene.num_lights++;

    GLfloat vertices[] = {
       -0.5f,-0.5f, 0.0f,
        0.5f,-0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    GLfloat normals[] = {
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    shader = new Shader ();
    shader->AddShader (GL_VERTEX_SHADER, LoadFile ("../shaders/light.vert.glsl"));
    shader->AddShader (GL_FRAGMENT_SHADER, LoadFile ("../shaders/light.frag.glsl"));
    shader->Compile ();

    tri = new Triangle ();
    tri->p1 = glm::vec3 (vertices[0], vertices[1], vertices[2]);
    tri->p2 = glm::vec3 (vertices[3], vertices[4], vertices[5]);
    tri->p3 = glm::vec3 (vertices[6], vertices[7], vertices[8]);

    tri->gldata.GenBuffers (1, 1, 0);
    tri->gldata.BindVertexArray (0);
    tri->gldata.AddArrayBuffer (0, sizeof (vertices), vertices, GL_STATIC_DRAW, GL_FLOAT, 3);
    tri->gldata.AddArrayBuffer (1, sizeof (normals), normals, GL_STATIC_DRAW, GL_FLOAT, 3);
    //tri->gldata.AddElementBuffer (0, 6, indices, GL_STATIC_DRAW);
    tri->gldata.UnbindVertexArray ();
    tri->gldata.SetDrawCount (3);
    tri->gldata.SetUseIndices (false);

    tri->p_shader = shader;

    tri->material.diffuse_color = glm::vec4 (0.7f, 0.7f, 0.7f, 1.f);
    tri->material.specular_color = glm::vec4 (1.f, 1.f, 1.f, 1.f);
    tri->material.emission_color = glm::vec4 (0.f, 0.f, 0.f, 1.f);
    tri->material.shininess = 10000.f;

    scene.models.emplace_back (tri);

    return true;
}

void render ()
{
    SDL_Event e;

    glm::vec2 move;
    float xangle = 0, yangle = 0;

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

                glm::quat xrot = glm::angleAxis (yangle, RIGHT);
                glm::quat yrot = glm::angleAxis (xangle, UP);
                camera.orientation = yrot * xrot;
            }
        }

        if (move.x != 0 || move.y != 0) {
            camera.position += camera.orientation * FORWARD * move.x * 0.1f;
            camera.position += camera.orientation * RIGHT * move.y * 0.1f;
        }

        camera.projection = glm::perspective (70.f, (float) WIN_WIDTH/WIN_HEIGHT, 0.1f, 1000.f);
        camera.view = glm::lookAt (camera.position, camera.position + camera.orientation * FORWARD, UP);

        p_window->SetClearColor (0x00, 0x00, 0x00);
        p_window->Clear ();

        p_renderer->render (scene);

        p_window->SwapBuffers ();
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
        render ();
    shutdown ();

	return 0;
}
