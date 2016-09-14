#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace graphics
{

    struct Transform
    {
        Transform ()
            : position (glm::vec3 (0,0,0)),
              rotation (glm::quat ()),
              scale (glm::vec3(1,1,1)) {}

        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        inline glm::mat4 model () const
        {
            glm::mat4 trans_mat = glm::translate (glm::mat4(1.f), position);
            glm::mat4 rot_mat = glm::toMat4 (rotation);
            glm::mat4 scale_mat = glm::scale (glm::mat4(1.f), scale);

            return trans_mat * rot_mat * scale_mat;
        }
    };

}

#endif // TRANSFORM_H
