#include "raycast_renderer.h"
#include <iostream>
#include <iomanip>
#include "loader.h"
#include "glm/gtx/string_cast.hpp"
#include <SOIL.h>
#include <math.h>
#include <algorithm>

using namespace std;

namespace graphics
{

    static vec4 ComputeLight (const vec4& diffuse_color, const vec4& specular_color, const float& shininess,
                              const vec3& direction, const vec4& lightcolor,
                              const vec3& normal, const vec3& halfvec);

    void RaycastRenderer::Setup ()
    {
        string glslVersion = util::glsl_version ();

        shader.CreateProgram ();
        shader.AddShader (GL_VERTEX_SHADER, util::load_file ("../shaders/"+glslVersion+"/screen.vert.glsl"));
        shader.AddShader (GL_FRAGMENT_SHADER, util::load_file ("../shaders/"+glslVersion+"/screen.frag.glsl"));
        shader.Compile ();

        GLfloat vertices[] = {
             1.0f,-1.0f, 0.f,
             1.0f, 1.0f, 0.f,
            -1.0f,-1.0f, 0.f,
            -1.0f, 1.0f, 0.f
        };

        GLfloat texcoords[] = {
             1.f, 0.f,
             1.f, 1.f,
             0.f, 0.f,
             0.f, 1.f
        };

        GLuint indices[] = {
            0, 1, 2,
            2, 1, 3
        };

        screen.GenBuffers (1, 2, 1);
        screen.BindVertexArray (0);
        screen.ArrayBuffer (0, sizeof (vertices), vertices, GL_STATIC_DRAW, GL_FLOAT, 3);
        screen.ArrayBuffer (1, sizeof (texcoords), texcoords, GL_STATIC_DRAW, GL_FLOAT, 2);
        screen.ElementBuffer (0, sizeof (indices), indices, GL_STATIC_DRAW);
        screen.UnbindVertexArray ();
        screen.SetDrawCount (6);
        screen.SetUseIndices (true);

        texture.GenTexture ();
        texture.SetPixelAlignment (1);
        texture.SetFiltering (GL_NEAREST);
        texture.SetWrapping (GL_CLAMP_TO_BORDER);
    }

    void RaycastRenderer::render (const Scene& scene)
    {
        const uint width = GetWindow ().GetWidth ();
        const uint height = GetWindow ().GetHeight ();

        uint image[width][height];
        RayTrace (scene, width, height, (uint*)image);

        texture.Bind (0);
        unsigned char* p_image = reinterpret_cast<unsigned char*> ((uint*)image);
        texture.CreateImage (GL_RGBA, width, height, p_image);

        shader.Bind ();
        screen.Draw ();
    }

    void RaycastRenderer::RayTrace (const Scene& scene, const uint& width, const uint& height, uint* buffer)
    {
        for (uint i=0; i<height; ++i) {
            for (uint j=0; j<width; ++j) {
                uint* p_col = buffer + i*width + j;

                Ray ray;
                Intersection hit;

                RayThruPixel (ray, *scene.p_active_camera, j, i);
                if (Intersect (hit, ray, scene)) {
                    *p_col = FindColor (hit, scene);
                } else {
                    *p_col = 0xFF000000;
                }
            }
        }
    }

    void RaycastRenderer::RayThruPixel (Ray& ray, const Camera& camera, const uint& x, const uint& y)
    {
        vec3 i, j, k;
        k = camera.forward ();
        j = camera.up ();

        i = glm::cross(k,j);
        j = glm::cross(i,k);

        const float fovy = camera.fovy ();
        const float fovx = fovy * (camera.aspect ()-0.0026f);

        const float half_width = GetWindow ().GetWidth ()*0.5f;
        const float half_height = GetWindow ().GetHeight ()*0.5f;
        float a=0, b=0;
        a = tan (fovx*0.5f) * ((x - half_width) / half_width);
        b = tan (fovy*0.5f) * ((half_height - y) / half_height);

        ray.origin = camera.position;
        ray.direction = -glm::normalize (a*i + b*j - k);
    }

    bool RaycastRenderer::Intersect (Intersection& inter, const Ray& ray, const Scene& scene)
    {
        for (auto model : scene.models) {
            model->Intersect (inter, ray);
        }

        if (inter.distance != 0) {
            return true;
        }
        return false;
    }

    uint RaycastRenderer::FindColor (const Intersection& inter, const Scene& scene)
    {
        uint color;
        unsigned char* p_col = reinterpret_cast<unsigned char*> (&color);

        vec3 fragColor;

        vec3 viewDir = glm::normalize (inter.hitpoint - scene.p_active_camera->position);
        vec3 normal = inter.normal;

        vec3 diffuseColor = vec3 (inter.material.diffuse_color);
        vec3 specularColor = vec3 (inter.material.specular_color);

        vec3 ambient = vec3 (scene.ambient_color);
        vec3 diffuse = vec3 ();
        vec3 specular = vec3 ();

        for (int i=0; i<scene.num_lights; ++i)
        {
            vec3 lightColor = vec3 (scene.lights[i].color);

            vec3 lightPos = vec3 (scene.lights[i].position);
            vec3 lightDir;

            bool shadowed = false;
            Ray shadowRay; Intersection shadowInter;

            switch (scene.lights[i].type) {
            case DIRECTIONAL:
                lightDir = glm::normalize (lightPos);
                shadowRay.direction = lightDir;
                break;
            case POINT:
                lightDir = glm::normalize (lightPos - inter.hitpoint);
                shadowRay.direction = -glm::normalize (inter.hitpoint - lightPos);
                break;
            }

            shadowRay.origin = inter.hitpoint + shadowRay.direction * EPSILON;

            for (auto model : scene.models) {
                if (model->Intersect (shadowInter, shadowRay)) {
                    shadowed = true;
                    break;
                }
            }

            // Ambient
            const float ambientStrength = 0.1f;
            ambient += lightColor * ambientStrength;

            if (!shadowed) {
                // Diffuse
                float diff = glm::max (glm::dot (lightDir, normal), 0.f);
                diffuse += diffuseColor * diff * lightColor;

                // Specular
                const float specularStrength = 1.0f;//0.5f;
                vec3 reflectDir = glm::reflect (lightDir, normal);
                float spec = glm::pow (glm::max (glm::dot (viewDir, reflectDir), 0.f), inter.material.shininess);
                specular += specularColor * spec * (lightColor * specularStrength);
            }
        }

        switch (mode) {
        case LIGHTING:
            fragColor = ambient + diffuse + specular;
            break;

        case DIFFUSE:
            fragColor = diffuse;
            break;

        case SPECULAR:
            fragColor = specular;
            break;

        case NORMALS:
            vec3 normal2 = inter.normal;
            normal2 += 1; normal2 *= 0.5f;
            fragColor = normal2;
            break;
        }

        fragColor = glm::clamp (fragColor, vec3 (), vec3 (1,1,1));
        p_col[0] = (unsigned char)(fragColor.x * 0xFF);
        p_col[1] = (unsigned char)(fragColor.y * 0xFF);
        p_col[2] = (unsigned char)(fragColor.z * 0xFF);
        p_col[3] = (unsigned char)(0xFF);

        return color;
    }

    static vec4 ComputeLight (const vec4& diffuse_color, const vec4& specular_color, const float& shininess,
                              const vec3& direction, const vec4& lightcolor,
                              const vec3& normal, const vec3& halfvec)
    {
        float nDotL = glm::dot (normal, direction);
        vec4 lambert = diffuse_color * lightcolor * glm::max (nDotL, 0.f);

        float nDotH = glm::dot (normal, halfvec);
        vec4 phong = specular_color * lightcolor * glm::pow (glm::max (nDotH, 0.f), shininess);

        vec4 retval = lambert + phong;
        return retval;
    }

}
