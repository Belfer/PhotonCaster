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
#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace graphics;

namespace util
{
    const static hash<string> str_hashfn; // String hash function

    /* Returns a vec3 given the parameters r,g,b in the range [0,255] */
    inline static vec3 rgb (GLubyte r, GLubyte g, GLubyte b)
    {
        return vec3 (r/255.f, g/255.f, b/255.f);
    }

    /* Returns the current glsl as a string without the . char */
    inline static string glsl_version ()
    {
        string glslVersion = string ((char*)glGetString (GL_SHADING_LANGUAGE_VERSION));
        glslVersion.erase (std::find (glslVersion.begin (), glslVersion.end (), '.'));

        return glslVersion;
    }

    template<typename T>
    inline static void reverse (T* p_data, T* p_buffer, const size_t& size)
    {
        uint r = size-1;
        for (uint i=0; i<size; ++i) {
            p_buffer[r--] = p_data[i];
        }
    }

    inline static void split (const string& s, const string& delims, vector<string>& elems)
    {
        stringstream ss (s);
        string line;
        while (getline (ss, line))
        {
            size_t prev = 0, pos;
            while ((pos = line.find_first_of(delims, prev)) != string::npos)
            {
                if (pos > prev)
                    elems.push_back (line.substr (prev, pos-prev));
                prev = pos+1;
            }
            if (prev < line.length())
                elems.push_back (line.substr (prev, string::npos));
        }
    }


    inline static vector<string> split (const string& s, const string& delims)
    {
        vector<string> elems;
        split (s, delims, elems);
        return elems;
    }

    inline static std::string splitget (const std::string& src, const string& delims, const size_t& index)
    {
        vector<string> strlist;
        split (src, delims, strlist);
        return strlist[index];
    }

}

#endif // UTIL_H
