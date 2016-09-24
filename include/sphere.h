#ifndef SPHERE_H
#define SPHERE_H

#include "model.h"
#include "transform.h"
#include "material.h"
#include "mesh_data.h"
#include "shader.h"
#include "util.h"
#include <glm/gtx/string_cast.hpp>

namespace graphics
{

    struct Sphere : public Model
    {
        float radius;

        virtual bool Update (const float& dt)
        {
        }

        virtual bool Intersect (Intersection& inter, const Ray& ray)
        {
            float t0, t1;

            vec3 o_c = ray.origin - transform.position;
            float b = 2.f * glm::dot (ray.direction, o_c);
            float c = glm::dot (o_c, o_c) - radius*radius;

            if (!util::solve_quadratic (1, b, c, t0, t1)) return false;

            if (t0 < 0) {
                t0 = t1;
                if (t0 < 0) return false;
            }

            if (inter.distance == 0 || t0 < inter.distance) {
                inter.distance = t0;
                inter.hitpoint = ray.origin + ray.direction * t0;
                inter.normal   = glm::normalize (inter.hitpoint - transform.position);
                inter.material = material;
            }

            return true;
        }

        /* RTTI */
        const char* GetType () { return "Sphere"; }
        const char* GetParentType (const ushort& i) { return nullptr; }
        ushort GetParentCount () { return 0; }
        size_t GetTypeID () { static size_t id = util::str_hashfn ("Sphere"); return id; }
        size_t GetParentTypeID (const ushort& i) { return 0; }
    };

}

#endif // SPHERE_H
