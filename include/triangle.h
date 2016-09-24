#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include "model.h"
#include "transform.h"
#include "material.h"
#include "mesh_data.h"
#include "shader.h"
#include "util.h"

using namespace rtti;
using namespace glm;

namespace graphics
{

    struct Triangle : public Model
    {
        vec3 p1;
        vec3 p2;
        vec3 p3;
    private:
        vec3 tp1;
        vec3 tp2;
        vec3 tp3;

    public:
        virtual bool Update (const float& dt)
        {
            tp1 = vec3 (transform.model () * vec4 (p1, 1.f));
            tp2 = vec3 (transform.model () * vec4 (p2, 1.f));
            tp3 = vec3 (transform.model () * vec4 (p3, 1.f));
        }

        virtual bool Intersect (Intersection& inter, const Ray& ray)
        {
            const vec3 e1 = tp2-tp1;
            const vec3 e2 = tp3-tp1;

            const vec3 n = glm::normalize (glm::cross (e1, e2));
            const vec3 q = glm::cross (ray.direction, e2);
            const float a = glm::dot (e1, q);

            if (glm::abs (a) <= EPSILON) {// || glm::dot (n, ray.direction) >= 0) {
                return false;
            }

            const vec3 s = (ray.origin - tp1) / a;
            const vec3 r = glm::cross (s, e1);
            float ba = glm::dot (s, q);
            float bb = glm::dot (r, ray.direction);
            float bg = 1.0f - ba - bb;

            if ((ba < 0.f) || (bb < 0.f) || (bg < 0.f)) {
                return false;
            }

            float t = glm::dot (e2, r);
            if (t < 0.f) return false;

            if (inter.distance == 0 || t < inter.distance) {
                inter.distance = t;
                inter.hitpoint = ray.origin + ray.direction*t;
                inter.normal   = n;
                inter.material = material;
            }
            return true;
        }

        /* RTTI */
        const char* GetType () { return "Triangle"; }
        const char* GetParentType (const ushort& i) { return nullptr; }
        ushort GetParentCount () { return 0; }
        size_t GetTypeID () { static size_t id = util::str_hashfn ("Triangle"); return id; }
        size_t GetParentTypeID (const ushort& i) { return 0; }
    };

}

#endif // TRIANGLE_H
