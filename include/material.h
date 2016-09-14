#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

namespace graphics
{

    struct Material
    {
        glm::vec4 diffuse_color;
        glm::vec4 specular_color;
        glm::vec4 emission_color;
        float shininess;
    };

}

#endif // MATERIAL_H
