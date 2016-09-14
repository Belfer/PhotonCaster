#ifndef MODEL_H
#define MODEL_H

#include "object.h"
#include "transform.h"
#include "material.h"
#include "mesh_data.h"
#include "shader.h"
#include "util.h"

using namespace rtti;

namespace graphics
{

    struct Model : public Object
    {
        Transform transform;
        Material material;
        OpenGLMeshData gldata;
        Shader* p_shader;

        /* RTTI */
        const char* GetType () { return "Model"; }
        const char* GetParentType (const ushort& i) { return nullptr; }
        ushort GetParentCount () { return 0; }
        size_t GetTypeID () { static size_t id = util::str_hashfn ("Model"); return id; }
        size_t GetParentTypeID (const ushort& i) { return 0; }
    };

}

#endif // MODEL_H
