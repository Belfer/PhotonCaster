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
            // TODO
            return false;
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
