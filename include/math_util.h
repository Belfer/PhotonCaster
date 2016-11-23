#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <glm/glm.hpp>

#define PI 3.1415

using namespace std;
using namespace glm;

namespace util
{

    inline static vec3 reflect (const vec3& incident, const vec3& normal)
    {
        const float nDoti = glm::dot (normal, incident);
        return incident - 2.f * nDoti * normal;
    }

    inline static vec3 refract (const vec3& incident, const vec3& normal, float eta)
    {
        const float nDoti = glm::dot (normal, incident);
        const float k = eta*eta * (1.f - nDoti*nDoti);
        if (k > 1.f) {
            return vec3 (0.f, 0.f, 0.f);
        }
        const float t = glm::sqrt (1.f - k);

        return eta * incident - (eta * nDoti - t) * normal;
    }

    inline static bool solve_quadratic (const float &a, const float &b, const float &c, float &x0, float &x1)
    {
        float discr = b*b - 4*a*c;
        if (discr < 0) {
            return false;

        } else if (discr == 0) {
            x0 = x1 = -b / 2.f*a;

        } else {
            x0 = (-b + glm::sqrt (discr)) / 2.f*a;
            x1 = (-b - glm::sqrt (discr)) / 2.f*a;
            if (x0 > x1) std::swap (x0, x1);
        }

        return true;
    }

}

#endif // MATHUTIL_H
