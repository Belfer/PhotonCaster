#ifndef SPHERE_H
#define SPHERE_H

#include "model.h"
#include "transform.h"
#include "material.h"
#include "mesh_data.h"
#include "shader.h"
#include "util.h"
#include "math_util.h"
#include "ray.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace graphics
{

    struct Sphere : public Model
    {
        Sphere () : radius (0) {}
        Sphere (const vec3& pos, float radius) : radius (radius)
        {
            transform.position = pos;
        }

        float radius;

        virtual bool Update (const float& dt) {}

        virtual bool Intersect (Intersection& inter, const Ray& ray)
        {
            float t0, t1;

            mat4 inv_transM44 = glm::inverse (transform.transform);
            mat3 inv_transM33 = mat3 (inv_transM44);
            vec3 rdir = glm::normalize (inv_transM33 * ray.direction);
            vec3 rori = vec3 (inv_transM44 * vec4 (ray.origin, 1.f));
            float b = 2.f * glm::dot (rdir, rori);
            float c = glm::dot (rori, rori) - radius*radius;

            if (!util::solve_quadratic (1, b, c, t0, t1)) return false;

            if (t0 < 0) {
                t0 = t1;
                if (t0 < 0) return false;
            }
            vec3 p = rori + rdir * t0;
            vec3 normal = glm::normalize (p);

            p = vec3 (transform.transform * vec4 (p, 1.f));
            t0 = glm::length (ray.origin - p);
            normal = glm::normalize (glm::transpose (inv_transM33) * normal);

            if (inter.distance == 0 || t0 < inter.distance) {
                inter.distance = t0;
                inter.origin   = ray.origin;
                inter.hitpoint = p;
                inter.incident = ray.direction;
                inter.ouid     = GetUID ();
                inter.normal   = normal;
                inter.material = material;
                return true;
            }
            return false;
        }

        inline void print ()
        {
            Model::print ();
            cout << "radius: "<<radius<<"\n";
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
