#ifndef MESH_H
#define MESH_H

#include "model.h"
#include "transform.h"
#include "material.h"
#include "shader.h"
#include "mesh_data.h"
#include "util.h"

namespace graphics
{

    struct Mesh : public Model
    {
        PolygonMeshData polydata;

        /* RTTI */
        const char* GetType () { return "Mesh"; }
        const char* GetParentType (const ushort& i) { return nullptr; }
        ushort GetParentCount () { return 0; }
        size_t GetTypeID () { static size_t id = util::str_hashfn ("Mesh"); return id; }
        size_t GetParentTypeID (const ushort& i) { return 0; }//return util::str_hashfn (GetParentType (i)); }
    };

}

#endif // MESH_H
