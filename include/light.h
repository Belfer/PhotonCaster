#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

namespace graphics
{

    enum LightType {
        DIRECTIONAL,
        POINT
    };

    struct Light
    {
        glm::vec4 position;
        glm::vec4 color;
        LightType type;
    };

}

#endif // LIGHT_H
