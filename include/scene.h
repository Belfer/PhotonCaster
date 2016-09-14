#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <glm/glm.hpp>
#include "light.h"
#include "material.h"
#include "camera.h"
#include "model.h"

#define MAX_NUM_LIGHTS 8

namespace graphics
{

    struct Scene
    {
        Scene () : num_lights (0) {}

        glm::vec4 ambient_color;

        int num_lights;
        Light lights[MAX_NUM_LIGHTS];

        Camera* p_active_camera;
        std::vector<Model*> models;
    };

}

#endif // SCENE_H
