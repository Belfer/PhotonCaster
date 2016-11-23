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
        Light () : intensity (1), constant (1), linear (0), quadratic (0), type (DIRECTIONAL) {}

        vec3 position;
        vec3 color;
        float intensity;
        float constant;
        float linear;
        float quadratic;
        LightType type;

        inline vec3 dir (const vec3& fragpos) const
        {
            switch (type) {
            case DIRECTIONAL:
                return glm::normalize (-position);
                break;
            case POINT:
                return glm::normalize (fragpos-position);
                break;
            }
        }
    };

}

#endif // LIGHT_H
