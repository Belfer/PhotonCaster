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

        virtual bool Intersect (Intersection& inter, const Ray& ray)
        {
            vec3 p = ray.position - transform.position;
            float rSq = radius*radius;
            float pDotd = glm::dot (p, ray.direction);

            if (pDotd < 0 || glm::dot (p,p) < rSq) {
                return false;
            }

            vec3 a = p - pDotd * ray.direction;
            float aSq = glm::dot (a,a);

            if (aSq > rSq) {
                return false;
            }

            float h = glm::sqrt (rSq - aSq);

            vec3 i = a - h * ray.direction;
            vec3 d = (transform.position + i) - ray.position;
            float dSq = glm::dot (d,d);

            if (inter.distance == 0 || dSq < inter.distance) {
                inter.distance = dSq;
                inter.hitpoint = transform.position + i;
                inter.normal = i / radius;
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
