#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include "material.h"

using namespace glm;

namespace graphics
{

    static const float EPSILON = 0.0002f;

    struct Ray
    {
        vec3 origin;
        vec3 direction;
    };

    struct Intersection
    {
        Intersection () : distance (0) {}
        float distance;
        vec3 hitpoint;
        vec3 normal;
        Material material;
    };

}

#endif // RAY_H
