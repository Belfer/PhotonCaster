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

        virtual bool Intersect (Intersection& inter, const Ray& ray)
        {
            vec3 v0 = p2-p1, v1 = p3-p1;

            vec3 normal = glm::cross (v0, v1);
            float nDotd = glm::dot (normal, ray.direction);

            if (nDotd == 0) return false;

            float t = (glm::dot (ray.direction, p1) - glm::dot (normal, ray.origin)) / nDotd;

            vec3 p = ray.origin + ray.direction * t;

            vec3 v2 = p-p1;
            float d00 = glm::dot (v0, v0);
            float d01 = glm::dot (v0, v1);
            float d11 = glm::dot (v1, v1);
            float d20 = glm::dot (v2, v0);
            float d21 = glm::dot (v2, v1);
            float denom = d00 * d11 - d01 * d01;

            float b = (d11 * d20 - d01 * d21) / denom;
            float g = (d00 * d21 - d01 * d20) / denom;
            float a = 1.f - b - g;

            if (a+b+g > 1.f) return false;

            /*if (inter.distance == 0 || t < inter.distance) {
                inter.distance = t;
                inter.hitpoint = p;
                inter.normal   = normal;
                inter.material = material;
            }*/
            inter.distance = 1;
            inter.material = material;

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
