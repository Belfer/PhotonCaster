#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;
using namespace std;

namespace graphics
{

    struct Material
    {
        Material () : shininess (0), reflectI (0), refractI (0), alpha (1) {}

        vec3 ambient;       // Ambient tint
        vec3 emission;      // Color of emission
        vec3 diffuse;       // Color of surface
        vec3 specular;      // Color of highlights
        float shininess;    // Shininess power
        float reflectI;     // Reflect index
        float refractI;     // Refract index
        float alpha;        // Overall alpha of object

        inline void print () const
        {
            cout << "ambient: "<<glm::to_string (ambient)<<"\n";
            cout << "emission: "<<glm::to_string (emission)<<"\n";
            cout << "diffuse: "<<glm::to_string (diffuse)<<"\n";
            cout << "specular: "<<glm::to_string (specular)<<"\n";
            cout << "shininess: "<<shininess<<"\n";
            cout << "reflectI: "<<reflectI<<"\n";
            cout << "refractI: "<<refractI<<"\n";
            cout << "alpha: "<<alpha<<"\n";
        }
    };

}

#endif // MATERIAL_H
