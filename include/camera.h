#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

namespace graphics
{
    const static vec3 UP = vec3 (0,1,0);
    const static vec3 FORWARD = vec3 (0,0,1);
    const static vec3 RIGHT = vec3 (1,0,0);

    struct Camera
    {
        vec3 position;
        quat orientation;

        inline void Perspective (const float& fovy, const float& aspect, const float& znear, const float& zfar)
        {
            m_fovy = fovy;
            m_aspect = aspect;
            m_projection = glm::perspective (fovy, aspect, znear, zfar);
        }

        inline void Update ()
        {
            m_view = glm::lookAt (position, position+forward (), up ());
            m_combined = m_projection * m_view;
        }

        inline float fovy () const { return m_fovy; }
        inline float aspect () const { return m_aspect; }

        inline vec3 up () const { return orientation * UP; }
        inline vec3 forward () const { return orientation * FORWARD; }
        inline vec3 right () const { return orientation * RIGHT; }

        inline mat4 projection () const { return m_projection; }
        inline mat4 view () const { return m_view; }
        inline mat4 combined () const { return m_combined; }

    private:
        mat4 m_projection;
        mat4 m_view;
        mat4 m_combined;

        float m_fovy;
        float m_aspect;
    };

}

#endif // CAMERA_H
