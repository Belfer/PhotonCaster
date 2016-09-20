#ifndef SPHERE_H
#define SPHERE_H

#include "model.h"
#include "transform.h"
#include "material.h"
#include "mesh_data.h"
#include "shader.h"
#include "util.h"

namespace graphics
{

    struct Sphere : public Model
    {
        float radius;

        virtual bool Intersect (Intersection& inter, const Ray& ray)
        {
            vec3 p, w;
            float h;

            w = transform.position - ray.position;
            p = ray.position + ((glm::dot (ray.direction, w) / glm::length (ray.direction)) * ray.direction);
            h = glm::length (transform.position - p);

            if (h > radius) { // No intersection
                return false;

            } else if (h < radius) { // 2 intersections
                vec3 p1;
                if (glm::length (w) > radius) {
                    p1 = glm::length (p-ray.position) - transform.position;
                } else {
                    p1 = glm::length (p-ray.position) + transform.position;
                }

                inter.distance = glm::length (p1-ray.position);
                inter.normal = glm::normalize (p1-transform.position);

            } else { // 1 intersection
                inter.distance = glm::length (p-ray.position);
                inter.normal = glm::normalize (p-transform.position);
            }

            inter.material = material;
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
