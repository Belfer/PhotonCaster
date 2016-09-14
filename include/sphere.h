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

        /* RTTI */
        const char* GetType () { return "Sphere"; }
        const char* GetParentType (const ushort& i) { return nullptr; }
        ushort GetParentCount () { return 0; }
        size_t GetTypeID () { static size_t id = util::str_hashfn ("Sphere"); return id; }
        size_t GetParentTypeID (const ushort& i) { return 0; }
    };

}

#endif // SPHERE_H
