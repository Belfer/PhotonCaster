#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>
#include "material.h"

using namespace glm;

namespace graphics
{

    static const float EPSILON = 0.0005f;

    struct Ray
    {
        vec3 origin;
        vec3 direction;
    };

    struct Intersection
    {
        Intersection () : distance (0), ouid (0) {}
        size_t ouid;
        Material material;
        float distance;
        vec3 origin;
        vec3 hitpoint;
        vec3 normal;
        vec3 incident;
    };

}

#endif // RAY_H
