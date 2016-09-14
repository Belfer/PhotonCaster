#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

using namespace glm;

namespace graphics
{

    enum LightType {
        DIRECTIONAL,
        POINT
    };

    struct Light
    {
        vec4 position;
        vec4 color;
        LightType type;
    };

}

#endif // LIGHT_H
