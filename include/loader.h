#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <iostream>
#include <fstream>
#include <stack>
#include <SOIL.h>
#include <GL/glew.h>
#include "texture.h"
#include "scene.h"
#include "util.h"
#include "sphere.h"
#include "triangle.h"
#include "shader.h"

using namespace graphics;

namespace cmd
{
    // General
    static size_t SIZE = util::str_hashfn ("size");
    static size_t MAX_DEPTH = util::str_hashfn ("maxdepth");
    static size_t OUTPUT = util::str_hashfn ("output");

    // Camera
    static size_t CAMERA = util::str_hashfn ("camera");

    // Geometry
    static size_t SPHERE = util::str_hashfn ("sphere");
    static size_t MAX_VERTS = util::str_hashfn ("maxverts");
    static size_t MAX_VERTNORMS = util::str_hashfn ("maxvertnorms");
    static size_t VERTEX = util::str_hashfn ("vertex");
    static size_t VERTEX_NORMAL = util::str_hashfn ("vertexnormal");
    static size_t TRI = util::str_hashfn ("tri");
    static size_t TRI_NORMAL = util::str_hashfn ("trinormal");

    // Transforms
    static size_t TRANSLATE = util::str_hashfn ("translate");
    static size_t ROTATE = util::str_hashfn ("rotate");
    static size_t SCALE = util::str_hashfn ("scale");
    static size_t PUSH_TRANS = util::str_hashfn ("pushTransform");
    static size_t POP_TRANS = util::str_hashfn ("popTransform");

    // Lights
    static size_t DIRECTIONAL = util::str_hashfn ("directional");
    static size_t POINT = util::str_hashfn ("point");
    static size_t ATTENUATION = util::str_hashfn ("attenuation");
    static size_t AMBIENT = util::str_hashfn ("ambient");

    // Materials
    static size_t DIFFUSE = util::str_hashfn ("diffuse");
    static size_t SPECULAR = util::str_hashfn ("specular");
    static size_t SHININESS = util::str_hashfn ("shininess");
    static size_t EMISSION = util::str_hashfn ("emission");
}

namespace util
{

    static string load_file (const std::string& filename)
    {
        ifstream file;
        file.open (filename.c_str ());

        string out;
        string line;

        if (file.is_open ()) {
            while (file.good ()) {
                getline (file, line);
                out.append (line+"\n");
            }
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }

        return out;
    }

    static void load_image (Texture& texture, const std::string& filename,
                            const GLuint& format, const GLuint& wrap,
                            const GLuint& filter, const GLuint& mipmap)
    {
        int width = 0, height = 0;
        unsigned char* image = SOIL_load_image (filename.c_str (), &width, &height, 0,
                                                format == GL_RGB ? SOIL_LOAD_RGB : SOIL_LOAD_RGBA);

        texture.GenTexture ();
        texture.Bind (0);
        texture.SetWrapping (wrap);
        texture.SetFiltering (filter);
        texture.SetMipMapping (mipmap);
        texture.CreateImage (format, width, height, image, true);
        texture.Unbind ();

        SOIL_free_image_data (image);
    }

    static void save_image ()
    {
        //SOIL_save_image
    }

    inline static Shader* const DefaultShader ()
    {
        static Shader* instance = nullptr;
        if (instance == nullptr) {
            instance = new Shader ();
            instance->CreateProgram ();
            string glslVersion = util::glsl_version ();
            instance->AddShader (GL_VERTEX_SHADER, util::load_file ("../shaders/"+glslVersion+"/light.vert.glsl"));
            instance->AddShader (GL_FRAGMENT_SHADER, util::load_file ("../shaders/"+glslVersion+"/light.frag.glsl"));
            instance->Compile ();
        }
        return instance;
    }

    static void load_scene (const string& filename, Scene& scene)
    {
        ifstream file;
        file.open (filename.c_str ());

        string line;

        Material material;

        size_t vertex_counter = 0;
        size_t normal_counter = 0;
        vector<Vertex> vertices;

        stack<Transform> trans_stack;
        trans_stack.push (Transform ());

        if (file.is_open ()) {
            while (file.good ()) {
                getline (file, line);
                vector<string> elems;
                util::split (line, " ", elems);

                if (elems.size() == 0 || elems[0][0] == '#') continue;
                size_t command = util::str_hashfn (elems[0]);

                if (command == cmd::SIZE) {
                    scene.width = std::atoi (elems[1].c_str ());
                    scene.height = std::atoi (elems[2].c_str ());

                } else if (command ==  cmd::MAX_DEPTH) {
                    scene.maxdepth = std::atoi (elems[1].c_str ());

                } else if (command == cmd::OUTPUT) {
                    scene.output = elems[1];
                }

                // Camera
                else if (command == cmd::CAMERA) {
                    vec3 pos = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                    vec3 target = vec3 (std::atof(elems[4].c_str ()), std::atof(elems[5].c_str ()), std::atof(elems[6].c_str ()));
                    vec3 up = vec3 (std::atof(elems[7].c_str ()), std::atof(elems[8].c_str ()), std::atof(elems[9].c_str ()));
                    vec3 z = glm::normalize (target - pos);
                    vec3 x = glm::cross (up,z);
                    up = glm::normalize(glm::cross(z,x));

                    float fov = std::atof (elems[10].c_str ());
                    scene.p_active_camera = new Camera (pos, target, up, fov);
                    scene.p_active_camera->Perspective (fov, (float) scene.width/scene.height, 0.1f, 1000.f);
                    scene.cameras.emplace_back (scene.p_active_camera);
                }

                // Geometry
                else if (command == cmd::SPHERE) {
                    vec3 pos = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                    float radius = std::atof (elems[4].c_str ());
                    Model* model = new Sphere (pos, radius);//*0.25f);
                    util::gl::icosphere (model->gldata, radius, 0);
                    model->transform = trans_stack.top ();
                    model->material = material;
                    model->p_shader = DefaultShader ();
                    scene.models.emplace_back (model);

                } else if (command == cmd::MAX_VERTS) {
                    vertices.resize (std::atoi (elems[1].c_str ()));

                } else if (command == cmd::MAX_VERTNORMS) {
                    vertices.resize (std::atoi (elems[1].c_str ()));

                } else if (command == cmd::VERTEX) {
                    vec3 pos = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                    vertices[vertex_counter++].position = pos;

                } else if (command == cmd::VERTEX_NORMAL) {
                    vec3 normal = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                    vertices[normal_counter++].normal = normal;

                } else if (command == cmd::TRI) {
                    vec3 p1 = vertices[std::atoi(elems[1].c_str ())].position;
                    vec3 p2 = vertices[std::atoi(elems[2].c_str ())].position;
                    vec3 p3 = vertices[std::atoi(elems[3].c_str ())].position;
                    Model* model = new Triangle (p1, p2, p3);
                    util::gl::triangle (model->gldata, p1, p2, p3);
                    model->transform = trans_stack.top ();
                    model->material = material;
                    model->p_shader = DefaultShader ();
                    scene.models.emplace_back (model);

                } else if (command == cmd::TRI_NORMAL) {
                    // TODO
                }

                // Transform
                else if (command == cmd::TRANSLATE) {
                    Transform& top = trans_stack.top ();
                    vec3 translation = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                    top.position += translation;
                    //top.position = vec3 (top.model () * vec4 (translation, 1.f));
                    top.transform = top.transform * glm::translate (translation);

                } else if (command == cmd::ROTATE) {
                    Transform& top = trans_stack.top ();
                    vec3 rotate_vec = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                    //float angle = glm::radians (std::atof(elems[4].c_str ()));
                    float angle = std::atof(elems[4].c_str ());
                    top.rotation = glm::angleAxis (angle, rotate_vec) * top.rotation;
                    //top.rotation = glm::conjugate (glm::toQuat (top.model () * glm::toMat4 (rotation)));
                    top.transform = top.transform * glm::toMat4 (glm::angleAxis (angle, rotate_vec));

                } else if (command == cmd::SCALE) {
                    Transform& top = trans_stack.top ();
                    vec3 scale = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                    top.scale = scale;
                    //top.scale = vec3 (top.model () * vec4 (translation, 1.f));
                    top.transform = top.transform * glm::scale (scale);

                } else if (command == cmd::PUSH_TRANS) {
                    trans_stack.push (trans_stack.top ());

                } else if (command == cmd::POP_TRANS) {
                    if (trans_stack.size () > 0)
                        trans_stack.pop ();
                }

                // Lights
                else if (command == cmd::DIRECTIONAL) {
                    scene.lights[scene.num_lights].position = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                    scene.lights[scene.num_lights].color = vec3 (std::atof(elems[4].c_str ()), std::atof(elems[5].c_str ()), std::atof(elems[6].c_str ()));
                    scene.lights[scene.num_lights].type = LightType::DIRECTIONAL;
                    scene.num_lights++;

                } else if (command == cmd::POINT) {
                    scene.lights[scene.num_lights].position = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                    scene.lights[scene.num_lights].color = vec3 (std::atof(elems[4].c_str ()), std::atof(elems[5].c_str ()), std::atof(elems[6].c_str ()));
                    scene.lights[scene.num_lights].type = LightType::POINT;
                    scene.num_lights++;

                } else if (command == cmd::ATTENUATION) {
                    scene.lights[scene.num_lights].constant = std::atof(elems[1].c_str ());
                    scene.lights[scene.num_lights].linear = std::atof(elems[2].c_str ());
                    scene.lights[scene.num_lights].quadratic = std::atof(elems[3].c_str ());

                } else if (command == cmd::AMBIENT) {
                    material.ambient = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                }

                // Materials
                else if (command == cmd::DIFFUSE) {
                    material.diffuse = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));

                } else if (command == cmd::SPECULAR) {
                    material.specular = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));

                } else if (command == cmd::SHININESS) {
                    material.shininess = std::atof(elems[1].c_str ());

                } else if (command == cmd::EMISSION) {
                    material.emission = vec3 (std::atof(elems[1].c_str ()), std::atof(elems[2].c_str ()), std::atof(elems[3].c_str ()));
                }
            }
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
        }
    }

}

#endif // LOADER_H
