#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <glm/glm.hpp>
#include "camera.h"

using namespace glm;
using namespace graphics;

namespace util
{

    class Controller
    {
    public:
        virtual void Update (Camera& camera, const float& dt, const vec2& t, const vec2& r) = 0;
    };

    class CrystalBall : public Controller
    {
    public:
        CrystalBall (const float& sensitivity)
            : x_angle (0), y_angle (0),
              sensitivity (sensitivity)
        {}

        void Update (Camera& camera, const float& dt, const vec2& t, const vec2& r);

    private:
        float sensitivity;
        float x_angle, y_angle;
    };

    class FreeLook : public Controller
    {
    public:
        FreeLook (const float& sensitivity)
            : sensitivity (sensitivity)
        {}

        void Update (Camera& camera, const float& dt, const vec2& t, const vec2& r);

    private:
        float sensitivity;
    };

}

#endif // CONTROLLER_H
