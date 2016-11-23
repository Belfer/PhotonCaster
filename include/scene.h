#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "light.h"
#include "material.h"
#include "camera.h"
#include "model.h"
#include "primitives.h"

#define MAX_NUM_LIGHTS 8

namespace graphics
{

    struct Scene
    {
        Scene () : width (0), height (0), maxdepth (3), num_lights (0) {}

        uint width, height;
        uint maxdepth;

        vec3 bgcolor;
        vec3 ambient;

        uint num_lights;
        Light lights[MAX_NUM_LIGHTS];

        Camera* p_active_camera;
        vector<Camera*> cameras;
        vector<Model*> models;

        string output;

        inline void print () const
        {
            cout << "width: "<<width<<", height: "<<height<<"\n";
            cout << "max depth: "<<maxdepth<<", output: "<<output<<"\n";
            cout << "bg color: "<<glm::to_string (bgcolor)<<", ambient: "<<glm::to_string (ambient)<<"\n";
            cout << "num lights: "<<num_lights<<"\n";
            for (uint i=0; i<num_lights; ++i) {
                cout << "\tlight "<<i<<"\n";
                cout << "\t\tpos: "<<glm::to_string (lights[i].position)<<"\n";
                cout << "\t\tcolor: "<<glm::to_string (lights[i].color)<<"\n";
                cout << "\t\tintensity: "<<lights[i].intensity<<"\n";
                cout << "\t\ttype: "<<lights[i].type<<"\n";
            }
            cout << "cameras: "<<cameras.size ()<<"\n";
            for (uint i=0; i<cameras.size (); ++i) {
                cout << "\tcamera "<<i<<"\n\t\t";
                cameras[i]->print ();
            }
            cout << "models: "<<models.size ()<<"\n";
            for (uint i=0; i<models.size (); ++i) {
                cout << "\tmodel "<<i<<"\n\t\t";
                models[i]->print ();
            }
        }
    };

}

#endif // SCENE_H
