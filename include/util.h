#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include "mesh_data.h"

using namespace std;
using namespace graphics;

namespace util
{

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

    template<typename T>
    inline static void reverse (T* p_data, T* p_buffer, const size_t& size)
    {
        uint r = size-1;
        for (uint i=0; i<size; ++i) {
            p_buffer[r--] = p_data[i];
        }
    }

    inline static string glsl_version ()
    {
        string glslVersion = string ((char*)glGetString (GL_SHADING_LANGUAGE_VERSION));
        glslVersion.erase (std::find (glslVersion.begin (), glslVersion.end (), '.'));
        //str.erase (std::remove (str.begin (), str.end (), '.'), str.end ());
        if (glslVersion != "120" && glslVersion != "130") {
            glslVersion = "130";
        }

        return glslVersion;
    }

    const static hash<string> str_hashfn;

    inline static void split (const string& s, const char& delim, vector<string>& elems)
    {
        stringstream ss;
        ss.str (s);
        string item;
        while (getline (ss, item, delim)) {
            elems.emplace_back (item);
        }
    }


    inline static vector<string> split (const string& s, const char& delim)
    {
        vector<string> elems;
        split (s, delim, elems);
        return elems;
    }

    inline static std::string splitget (const std::string& src, const char& delim, const size_t& index)
    {
        vector<string> strlist;
        split (src, delim, strlist);
        return strlist[index];
    }
}

#endif // UTIL_H
