#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

using namespace glm;

namespace graphics
{

    struct Material
    {
        vec4 diffuse_color;
        vec4 specular_color;
        vec4 emission_color;
        float shininess;
    };

}

#endif // MATERIAL_H
