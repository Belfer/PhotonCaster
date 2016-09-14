#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace graphics
{

    struct Camera
    {
        glm::vec3 position;
        glm::quat orientation;
        glm::mat4 projection;
        glm::mat4 view;

        inline glm::mat4 combined () const { return projection * view; }
    };

}

#endif // CAMERA_H
