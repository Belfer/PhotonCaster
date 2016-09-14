#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace glm;

namespace graphics
{

    struct Camera
    {
        vec3 position;
        quat orientation;
        mat4 projection;
        mat4 view;

        inline mat4 combined () const { return projection * view; }
    };

}

#endif // CAMERA_H
