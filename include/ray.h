#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include "material.h"

using namespace glm;

namespace graphics
{

    struct Ray
    {
        vec3 position;
        vec3 direction;
    };

    struct Intersection
    {
        Intersection () : distance (0) {}
        float distance;
        vec3 normal;
        Material material;
    };

}

#endif // RAY_H
