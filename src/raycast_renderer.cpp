#include "raycast_renderer.h"

using namespace std;

namespace graphics
{

    void render_chunk (const Scene& scene, RenderMode mode, const uint& width, const uint& height, uint divisions, uint* buffer);

    void RayTrace (const Scene& scene, RenderMode mode, uint buffwidth, uint buffheight, uint* buffer, const uint& x, const uint& y, const uint& w, const uint& h);

    void RayThruPixel (Ray& ray, const Camera& camera, const uint& x, const uint& y, const uint& w, const uint& h);

    bool Intersect (Intersection& inter, const Ray& ray, const Scene& scene, bool earlyout = false, size_t ignoreOUID = 0);

    vec3 FindColor (const Intersection& inter, const Scene& scene, RenderMode mode, uint depth);

    void ComputeLight (vec3& diffuse, vec3& specular, const Material& material, const Light& light,
                       const vec3& viewdir, const Intersection& inter);

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
        uint num_threads = 2;
        uint num_threadsSq = num_threads*num_threads;
        thread* p_threads[num_threadsSq];

        uint cw = width / num_threads;
        uint ch = height / num_threads;
        for (int i=0; i<num_threads; ++i) {
            for (int j=0; j<num_threads; ++j) {
                p_threads[i*num_threads+j] = new thread (RayTrace, scene, mode, width, height, (uint*)image, j*cw, i*ch, cw, ch);
            }
        }

        for (int i=0; i<num_threadsSq; ++i) {
            p_threads[i]->join ();
        }

        for (int i=0; i<num_threadsSq; ++i) {
            delete p_threads[i];
        }

        texture.Bind (0);
        unsigned char* p_image = reinterpret_cast<unsigned char*> ((uint*)image);
        texture.CreateImage (GL_RGBA, width, height, p_image);

        shader.Bind ();
        screen.Draw ();
    }

    void RayTrace (const Scene& scene, RenderMode mode, uint buffwidth, uint buffheight, uint* buffer, const uint& x, const uint& y, const uint& w, const uint& h)
    {
        for (uint i=y; i<y+h; ++i) {
            for (uint j=x; j<x+w; ++j) {
                uint* p_col = buffer + i*buffwidth + j;
                vec3 fragColor;

                Ray ray;
                Intersection hit;

                RayThruPixel (ray, *scene.p_active_camera, j, i, buffwidth, buffheight);
                if (Intersect (hit, ray, scene)) {
                    fragColor = FindColor (hit, scene, mode, 0);
                } else {
                    fragColor = scene.bgcolor;
                }

                uint color;
                unsigned char* tmp = reinterpret_cast<unsigned char*> (&color);
                tmp[0] = (unsigned char)(fragColor.x * 0xFF);
                tmp[1] = (unsigned char)(fragColor.y * 0xFF);
                tmp[2] = (unsigned char)(fragColor.z * 0xFF);
                tmp[3] = (unsigned char)(0xFF);
                *p_col = color;
            }
        }
    }

    void RayThruPixel (Ray& ray, const Camera& camera, const uint& x, const uint& y, const uint& w, const uint& h)
    {
        vec3 i, j, k;
        k = camera.forward ();
        j = camera.up ();

        i = glm::cross(k,j);
        j = glm::cross(i,k);

        const float iw = 1.f / w;
        const float ih = 1.f / h;
        const float fov = camera.fov ();
        const float aspect = camera.aspect ();
        const float angle = glm::tan (PI * 0.5f * fov / 180);
        float a=0, b=0;

        a = -(      2.f * ((x+0.5f) * iw) - 1.f) * angle * aspect;
        b =  (1.f - 2.f * ((y+0.5f) * ih)      ) * angle;

        ray.origin = camera.position;
        ray.direction = -glm::normalize (a*i + b*j - k);
    }

    bool Intersect (Intersection& inter, const Ray& ray, const Scene& scene, bool earlyout, size_t ignoreOUID)
    {
        bool intersected = false;
        for (auto model : scene.models) {
            if (model->GetUID () == ignoreOUID) continue;
            if (model->Intersect (inter, ray)) {
                intersected = true;
                if (earlyout) break;
            }
        }

        return intersected;
    }

    vec3 FindColor (const Intersection& inter, const Scene& scene, RenderMode mode, uint depth)
    {
        if (depth > 5) return vec3 ();
        vec3 fragColor;

        vec3 ambient = inter.material.ambient;
        vec3 emission = inter.material.emission;
        vec3 diffuse = vec3 ();
        vec3 specular = vec3 ();

        vec3 reflColor = vec3 ();
        vec3 refrColor = vec3 ();

        const vec3 viewdir = glm::normalize (inter.hitpoint - inter.origin);
        for (int i=0; i<scene.num_lights; ++i) {

            Ray shadowRay; Intersection shadowInter;
            if (scene.lights[i].type == POINT) {
                shadowInter.distance = glm::length (scene.lights[i].position - inter.hitpoint);
            }

            shadowRay.direction = -scene.lights[i].dir (inter.hitpoint);
            shadowRay.origin = inter.hitpoint + shadowRay.direction * EPSILON;

            if (!Intersect (shadowInter, shadowRay, scene, true, 0)) {
                ComputeLight (diffuse, specular, inter.material, scene.lights[i], viewdir, inter);
            }
        }

        if (inter.material.shininess >= 1) {
            Ray reflRay; Intersection reflInter;
            reflRay.direction = util::reflect (inter.incident, inter.normal);
            reflRay.origin = inter.hitpoint + reflRay.direction * EPSILON;
            if (Intersect (reflInter, reflRay, scene)) {
                reflColor = inter.material.specular * FindColor (reflInter, scene, mode, depth++);
            }
        }

        switch (mode) {
        case LIGHTING:
            fragColor = ambient + emission + diffuse + specular + reflColor;
            break;
        }

        return glm::clamp (fragColor, vec3 (), vec3 (1,1,1));
    }

    void ComputeLight (vec3& diffuse, vec3& specular, const Material& material, const Light& light,
                       const vec3& viewdir, const Intersection& inter)
    {
        const vec3 lightdir = light.dir (inter.hitpoint);
        const vec3 halfvec = glm::normalize (viewdir + lightdir);
        const float dist = glm::length (light.position - inter.hitpoint);
        const float att = light.constant + light.linear * dist + light.quadratic * dist*dist;
        const float lum = 1.f / att * light.intensity;

        float nDotL = glm::dot (inter.normal, -lightdir);
        diffuse += lum * material.diffuse * light.color * glm::max (nDotL, 0.f);

        float nDotH = glm::dot (inter.normal, -halfvec);
        specular += lum * material.specular * light.color * glm::pow (glm::max (nDotH, 0.f), material.shininess);
    }

}
