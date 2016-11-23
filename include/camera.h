#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace glm;
using namespace std;

namespace graphics
{
    const static vec3 UP = vec3 (0,1,0);
    const static vec3 FORWARD = vec3 (0,0,-1);
    const static vec3 RIGHT = vec3 (1,0,0);

    struct Camera
    {
        Camera () : Camera (vec3 (), FORWARD, UP, 70.f) {}

        Camera (const vec3& pos, const vec3& target, const vec3& up, const float fov)
            : position (pos), m_fov (fov)
        {
            orientation = glm::conjugate (glm::toQuat (glm::lookAt (pos, target, up)));
        }

        vec3 position;
        quat orientation;

        inline void Perspective (const float& fov, const float& aspect, const float& znear, const float& zfar)
        {
            m_fov = fov;
            m_aspect = aspect;
            m_projection = glm::perspective (fov, aspect, znear, zfar);
        }

        inline void Update ()
        {
            m_view = glm::lookAt (position, position+forward (), up ());
            m_combined = m_projection * m_view;
        }

        inline float fov () const { return m_fov; }
        inline float aspect () const { return m_aspect; }

        inline vec3 up () const { return orientation * UP; }
        inline vec3 forward () const { return orientation * FORWARD; }
        inline vec3 right () const { return orientation * RIGHT; }

        inline mat4 projection () const { return m_projection; }
        inline mat4 view () const { return m_view; }
        inline mat4 combined () const { return m_combined; }

        inline void print () const
        {
            cout << "pos: "<<glm::to_string (position)<<"\n";
            cout << "ori: fquat(" << orientation.w << ", " << orientation.x << ", "
                      << orientation.y << ", " << orientation.z << ")\n";
        }

    private:
        mat4 m_projection;
        mat4 m_view;
        mat4 m_combined;

        float m_fov;
        float m_aspect;
    };

}

#endif // CAMERA_H
