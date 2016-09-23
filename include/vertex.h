#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

using namespace glm;

namespace graphics
{

    struct Vertex
    {
        vec3 position;
        vec3 normal;
        vec3 color;
        vec2 texcoord;
    };

}

#endif // VERTEX_H
