#ifndef MODEL_H
#define MODEL_H

#include "object.h"
#include "transform.h"
#include "material.h"
#include "mesh_data.h"
#include "shader.h"
#include "ray.h"
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

        virtual bool Intersect (Intersection& inter, const Ray& ray) = 0;

        /* RTTI */
        virtual const char* GetType () = 0;
        virtual const char* GetParentType (const ushort& i) = 0;
        virtual ushort GetParentCount () = 0;
        virtual size_t GetTypeID () = 0;
        virtual size_t GetParentTypeID (const ushort& i) = 0;
    };

}

#endif // MODEL_H
