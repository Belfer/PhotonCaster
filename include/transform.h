#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

namespace graphics
{

    struct Transform
    {
        Transform ()
            : position (vec3 (0,0,0)),
              rotation (quat ()),
              scale (vec3(1,1,1)) {}

        vec3 position;
        quat rotation;
        vec3 scale;
        mat4 transform;

        inline mat4 model () const
        {
            mat4 trans_mat = glm::translate (mat4(1.f), position);
            mat4 rot_mat = glm::toMat4 (rotation);
            mat4 scale_mat = glm::scale (mat4(1.f), scale);

            return trans_mat * rot_mat * scale_mat;
        }
    };

}

#endif // TRANSFORM_H
