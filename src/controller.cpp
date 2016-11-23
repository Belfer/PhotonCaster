#include "controller.h"

namespace util
{

    void CrystalBall::Update (Camera& camera, const float& dt, const vec2& t, const vec2& r)
    {
        if (r.x != 0 || r.y != 0) {
            x_angle += r.y*sensitivity;
            y_angle += r.x*sensitivity;
            quat xrot = glm::angleAxis (x_angle, UP);
            quat yrot = glm::angleAxis (y_angle, RIGHT);
            camera.orientation = yrot * xrot;
            camera.position = -FORWARD * 6.f * glm::conjugate (yrot * xrot);
        }
    }

    void FreeLook::Update (Camera& camera, const float& dt, const vec2& t, const vec2& r)
    {
        if (r.x != 0 || r.y != 0) {
            //quat xrot = glm::angleAxis (glm::radians (r.y*sensitivity), RIGHT);
            //quat yrot = glm::angleAxis (glm::radians (r.x*sensitivity), UP);
            quat xrot = glm::angleAxis (r.y*sensitivity, RIGHT);
            quat yrot = glm::angleAxis (r.x*sensitivity, UP);
            camera.orientation = yrot * xrot;
        }

        if (t.x != 0 || t.y != 0) {
            camera.position += camera.forward () * t.x * dt;
            camera.position += camera.right () * t.y * dt;
        }
    }

}
