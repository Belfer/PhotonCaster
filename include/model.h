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

        virtual bool Update (const float& dt) = 0;
        virtual bool Intersect (Intersection& inter, const Ray& ray) = 0;

        virtual inline void print ()
        {
            cout << "type: "<<GetType ()<<"\n";
            cout << "position: "<<glm::to_string (transform.position)<<"\n";
            std::cout << "rotation: fquat(" << transform.rotation.w << ", " << transform.rotation.x << ", "
                      << transform.rotation.y << ", " << transform.rotation.z << ")\n";
            cout << "scale: "<<glm::to_string (transform.scale)<<"\n";
            material.print ();
        }

        /* RTTI */
        virtual const char* GetType () = 0;
        virtual const char* GetParentType (const ushort& i) = 0;
        virtual ushort GetParentCount () = 0;
        virtual size_t GetTypeID () = 0;
        virtual size_t GetParentTypeID (const ushort& i) = 0;
    };

}

#endif // MODEL_H
